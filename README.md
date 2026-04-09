# Masterpiece

`Masterpiece`는 Unreal Engine 5.6으로 제작 중인 싱글플레이 액션 RPG 프로토타입입니다.

이 저장소의 현재 초점은 방대한 콘텐츠를 빠르게 쌓는 것이 아니라, 아래 4가지를 안정적으로 굳히는 데 있습니다.

- 데이터 주도형 콘텐츠 정의 구조
- Gameplay Ability System 중심 전투/입력 파이프라인
- 플레이어 기능의 컴포넌트 분리
- Gameplay Tag 기반 UI 레이어/위젯 관리

이 문서는 2026년 4월 9일 기준 저장소 상태를 기준으로 작성했습니다. 초기 기획 문서가 아니라, 현재 코드와 에셋에 실제로 들어와 있는 진행 상황을 정리한 README입니다.

## 기술 스택

- Unreal Engine 5.6
- C++
- Gameplay Ability System
- Enhanced Input
- Gameplay Tags
- UMG
- Behavior Tree / AIController

## 현재 진행 상황

### 구현된 축

- `AMGameplayGameMode`, `AMGameplayPlayerController`, `AMGameplayPlayerState` 기본 연결
- 플레이어 `PlayerState` 소유 ASC + 전투/플레이어 AttributeSet 구성
- 클릭 지점 이동과 적 클릭 시 자동 공격 요청 파이프라인
- 아이템/스킬 공용 `Definition -> Instance` 런타임 구조
- `DefinitionRegistry` / `DefinitionSubsystem` 기반 데이터 로딩
- 인벤토리 아이템 생성, 스택, 장비 장착/해제, 소비 아이템 슬롯 상태 관리
- 스킬 컬렉션 비동기 로딩과 런타임 스킬 인스턴스 생성
- HUD / 메뉴 레이어를 분리한 `UIManagerSubsystem`
- 인벤토리 UI, 스킬 트리 UI, 스킬 퀵슬롯 HUD 위젯
- 공용 슬롯 드래그 앤 드롭 + 툴팁 기반 UI
- 적 캐릭터, AI 컨트롤러, BT/BB 에셋 연결

### 부분 구현 상태

- 스킬 정의에는 선행 스킬, 요구 레벨, 랭크 비용이 들어가 있지만 실제 포인트 소비/언락 로직은 아직 연결되지 않았습니다.
- 스킬을 Q/W/E/R 슬롯에 장착하는 구조는 있으나 실제 스킬 슬롯 입력 실행은 미완성입니다.
- 장비/소비 아이템은 상태 관리가 들어가 있지만 실제 효과 적용 경로는 더 다듬어야 합니다.
- 애니메이션 노티파이 기반 이벤트/공격 트레이스 훅은 있으나 실제 데미지 판정과 체력 처리 코드는 비어 있습니다.

### 아직 미구현이거나 뼈대만 있는 영역

- 퀘스트, 세이브, NPC 상호작용, 월드 진행
- 회피, 상호작용, 소비 아이템 사용 입력
- 데미지, 힐링, 사망 처리
- 소비 아이템 전용 HUD 퀵슬롯 UI
- 로비, 매치메이킹, 파티, 세션 계층

## 코어 설계

### 1. 데이터 주도형 정의 시스템

프로젝트의 핵심은 아이템과 스킬을 같은 패턴으로 다루는 공용 정의 시스템입니다.

`DeveloperSettings -> DefinitionRegistry -> DefinitionCollection -> DefinitionObject -> DefinitionInstance`

- `UMDefinitionRegistry`
  - 컬렉션 태그와 실제 DataAsset을 연결하는 최상위 레지스트리입니다.
- `UMDefinitionCollection`
  - 같은 카테고리의 정의 집합입니다.
- `UMDefinitionObject`
  - 에디터에서 작성하는 정적 정의 데이터입니다.
- `UMDefinitionInstance`
  - 런타임 상태를 갖는 인스턴스입니다.

이 구조 위에 다음이 올라가 있습니다.

- 아이템: `UMItemDefinition`, `UMItemInstance`
- 스킬: `UMSkillDefinition`, `UMSkillInstance`

핵심 포인트는 다음과 같습니다.

- 모든 식별은 `GameplayTag`를 기준으로 이뤄집니다.
- 정의 데이터는 DataAsset/인스턴스드 오브젝트로 유지하고, UI는 런타임 인스턴스를 직접 구독합니다.
- `DefinitionSubsystem`이 레지스트리를 한 번 초기화한 뒤 컬렉션과 정의를 태그 기준으로 조회합니다.
- 컬렉션 로딩은 동기/비동기 둘 다 지원합니다.

### 2. 플레이어 런타임 모델

플레이어는 `PlayerState`와 `Character`의 책임을 분리해 둔 구조입니다.

- `AMGameplayPlayerState`
  - `UMAbilitySystemComponent`
  - `UMCombatAttributeSet`
  - `UMPlayerAttributeSet`
- `AMPlayerCharacterBase`
  - 카메라, 이동, 전투, 스킬, 인벤토리 관련 컴포넌트
  - 실제 아바타와 입력 반응

이 분리는 이후 멀티플레이나 빙의/리스폰을 고려할 때 유리합니다. 현재 프로젝트는 사실상 싱글플레이 프로토타입이지만, 능력/속성의 영속 상태는 `PlayerState`에 두고 아바타 행동은 `Character` 쪽에 배치하는 방향이 이미 반영되어 있습니다.

반대로 적은 일회성 AI 액터 성격이 강하므로 `AMEnemyCharacterBase`가 ASC와 전투 속성을 직접 소유합니다.

### 3. 입력 -> 능력 -> 전투 흐름

현재 전투 입력 파이프라인은 아래 흐름을 따릅니다.

`Enhanced Input -> PlayerComponent -> Gameplay Event -> GAS Ability -> Character/Component`

구체적으로는 다음과 같습니다.

- `UMPlayerComponent`가 입력 액션을 바인딩합니다.
- 이동 명령 입력 시 커서 히트를 검사합니다.
- 적을 클릭하면 `Event.AutoAttack.Request`를 보냅니다.
- 바닥을 클릭하면 `Event.Move.Request`를 보냅니다.
- `UMPlayerAbility_Movement`, `UMPlayerAbility_AutoAttack`가 해당 이벤트를 받아 동작합니다.
- `UMCombatComponent`는 1차 공격 요청을 Gameplay Event로 넘깁니다.
- 애니메이션 노티파이와 노티파이 스테이트가 추가 이벤트/공격 트레이스 창을 열 수 있게 설계돼 있습니다.

즉, 입력 로직이 직접 전투를 처리하지 않고, 태그 기반 이벤트를 통해 GAS로 넘긴 뒤 캐릭터/컴포넌트가 실제 행동을 수행하는 방식입니다.

### 4. UI 레이어 구조

UI는 `ULocalPlayerSubsystem` 기반의 매니저가 전체를 관리합니다.

- `UMUIManagerSubsystem`
  - 위젯 생성
  - 레이어별 스택 관리
  - Escape 처리
  - 태그 기준 토글
- `UMUIWidgetRegistry`
  - `UI.Widget.*` 태그와 실제 위젯 클래스를 연결
- `UMTaggedWidget`
  - 위젯 자체가 태그를 소유
- `UMActivatableWidget`
  - 메뉴형 위젯의 입력 정책과 활성/비활성 처리

현재 레이어는 다음 구분을 사용합니다.

- `UI.Widget.GameHUD`
- `UI.Widget.GameMenu`
- `UI.Widget.Notify`
- `UI.Widget.Modal`

이 구조 덕분에 PlayerController나 개별 위젯이 서로를 하드 참조하지 않고, 태그만으로 열고 닫을 수 있습니다.

### 5. 공용 슬롯 UI 패턴

인벤토리와 스킬 UI는 공용 슬롯 기반으로 통일돼 있습니다.

- `UMSlotWidgetBase`
  - `DefinitionInstance`를 바인딩
  - 툴팁 생성
  - 드래그 앤 드롭 처리
- 인벤토리 슬롯
  - `UMInventoryItemSlotWidget`
  - `UMInventoryEquipmentSlotWidget`
- HUD 스킬 슬롯
  - `UMHUDSkillQuickSlotWidget`
- 스킬 트리 노드
  - `UMSkillTreeSlotWidget`

즉, "정의 인스턴스를 보여주는 슬롯"이라는 공용 개념 위에 각 UI가 올라가 있습니다.

### 6. 데이터 검증 전략

에디터 단계에서 잘못된 데이터가 들어오지 않도록 컬렉션/레지스트리 단위 검증을 넣어 두었습니다.

- 스킬 컬렉션
  - 중복 태그 검사
  - 요구 레벨 규칙 검사
  - 선행 스킬 DAG 검증
- 아이템 컬렉션
  - 중복 태그 검사
  - 스택/가격/타입 태그 검사
- UI 레지스트리
  - 등록 태그와 위젯 CDO 태그 일치 여부 검사
- 정의 레지스트리
  - 레지스트리 키와 컬렉션 태그 일치 여부 검사

프로젝트가 커질수록 런타임 디버깅보다 데이터 검증이 더 중요해지기 때문에, 이 부분은 현재 설계에서 꽤 중요한 축입니다.

## 현재 콘텐츠 스냅샷

저장소에는 다음 프로토타입 에셋이 들어 있습니다.

- 맵
  - `Content/Gameplay/L_TestLevel`
  - `Content/Gameplay/Character/L_Character`
- 플레이어/적
  - `BP_MPlayerCharacterBase`
  - `BP_EnemyCharacter`
  - `BP_DamageableDummy`
- 스킬 샘플
  - Aurora 계열 스킬 컬렉션/어빌리티 에셋
- UI
  - `WBP_MainHUD`
  - `WBP_InventoryWidget`
  - `WBP_SkillTreeWidget`
  - 관련 슬롯/툴팁 위젯
- 레지스트리
  - `DA_SkillCollectionRegistry`
  - `DA_ItemCollectionRegistry`
  - `DA_GameHUDWidgetRegistry`
  - `DA_GameMenuWidgetRegistry`

현재 기본 시작 맵은 `L_TestLevel`이며, 기본 게임 모드는 `BP_GameplayGameMode`로 연결돼 있습니다.

## 현재 입력 상태

코드 기준으로 실제 연결된 입력 상태는 다음과 같습니다.

- 이동 명령 액션
  - 커서 지점 이동 또는 적 클릭 시 자동 공격 요청
- 줌 액션
  - 스프링암 길이 조정
- 스킬 슬롯 액션
  - 입력 바인딩은 있으나 실제 스킬 실행 함수는 아직 비어 있음
- 상호작용 / 회피 액션
  - 입력 바인딩은 있으나 실제 처리 로직은 아직 비어 있음

추가로 인벤토리/스킬 트리 토글은 `PlayerController` API는 준비돼 있지만, 최종 입력 연결은 블루프린트/프로젝트 설정 쪽에서 마무리하는 단계입니다.

## 프로젝트 구조

```text
Source/Masterpiece
├─ Core
│  ├─ Types
│  ├─ Interface
│  ├─ Save
│  └─ Utility
├─ Gameplay
│  ├─ AbilitySystem
│  ├─ Animation
│  ├─ Character
│  │  ├─ Component
│  │  ├─ Enemy
│  │  ├─ NPC
│  │  └─ Player
│  ├─ Definition
│  ├─ GameMode
│  ├─ GameState
│  ├─ Interface
│  ├─ Interaction
│  ├─ Inventory
│  ├─ Item
│  ├─ PlayerController
│  ├─ PlayerState
│  ├─ Quest
│  ├─ Stat
│  ├─ UI
│  └─ World
└─ Lobby
   ├─ CharacterSelect
   ├─ Matchmaking
   ├─ Party
   ├─ Session
   └─ UI
```

현재 실질적으로 가장 많이 진행된 영역은 아래입니다.

- `Gameplay/Definition`
- `Gameplay/Item`
- `Gameplay/Character/Player`
- `Gameplay/UI`
- `Gameplay/AbilitySystem`

반대로 `Lobby`, `Quest`, `Save`, `NPC`, `Interaction`은 아직 설계 예약 영역에 가깝습니다.
