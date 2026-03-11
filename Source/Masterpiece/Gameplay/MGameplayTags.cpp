#include "Gameplay/MGameplayTags.h"

namespace MGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_PrimaryAttack, "Input.Ability.PrimaryAttack");
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_Skill_Q, "Input.Ability.Skill.Q");
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_Skill_W, "Input.Ability.Skill.W");
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_Skill_E, "Input.Ability.Skill.E");
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_Skill_R, "Input.Ability.Skill.R");
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_Dodge, "Input.Ability.Dodge");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Interaction, "Input.Action.Interaction");

	UE_DEFINE_GAMEPLAY_TAG(State_Attacking, "State.Attacking");
	UE_DEFINE_GAMEPLAY_TAG(State_Casting, "State.Casting");
	UE_DEFINE_GAMEPLAY_TAG(State_Dodging, "State.Dodging");
	UE_DEFINE_GAMEPLAY_TAG(State_Interacting, "State.Interacting");
	UE_DEFINE_GAMEPLAY_TAG(State_Stunned, "State.Stunned");
	UE_DEFINE_GAMEPLAY_TAG(State_Dead, "State.Dead");
	UE_DEFINE_GAMEPLAY_TAG(State_ComboWindow_Open, "State.ComboWindow.Open");

	UE_DEFINE_GAMEPLAY_TAG(Ability_Attack, "Ability.Attack");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Attack_Primary, "Ability.Attack.Primary");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Attack_Combo, "Ability.Attack.Combo");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Skill, "Ability.Skill");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Skill_Q, "Ability.Skill.Q");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Skill_W, "Ability.Skill.W");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Skill_E, "Ability.Skill.E");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Skill_R, "Ability.Skill.R");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Skill_Targeted, "Ability.Skill.Targeted");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Skill_NonTargeted, "Ability.Skill.NonTargeted");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Skill_Buff, "Ability.Skill.Buff");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Dodge, "Ability.Dodge");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Interaction, "Ability.Interaction");

	UE_DEFINE_GAMEPLAY_TAG(Event_Attack_Hit, "Event.Attack.Hit");
	UE_DEFINE_GAMEPLAY_TAG(Event_Attack_Request, "Event.Attack.Request");
	UE_DEFINE_GAMEPLAY_TAG(Event_Attack_Trace_Start, "Event.Attack.Trace.Start");
	UE_DEFINE_GAMEPLAY_TAG(Event_Attack_Trace_End, "Event.Attack.Trace.End");
	UE_DEFINE_GAMEPLAY_TAG(Event_Combo_Open, "Event.Combo.Open");
	UE_DEFINE_GAMEPLAY_TAG(Event_Combo_Close, "Event.Combo.Close");
	UE_DEFINE_GAMEPLAY_TAG(Event_Skill_Cast_Start, "Event.Skill.Cast.Start");
	UE_DEFINE_GAMEPLAY_TAG(Event_Skill_Cast_End, "Event.Skill.Cast.End");
	UE_DEFINE_GAMEPLAY_TAG(Event_Skill_Projectile_Spawn, "Event.Skill.Projectile.Spawn");
	UE_DEFINE_GAMEPLAY_TAG(Event_Buff_Apply, "Event.Buff.Apply");

	UE_DEFINE_GAMEPLAY_TAG(Block_Ability, "Block.Ability");
	UE_DEFINE_GAMEPLAY_TAG(Block_Attack, "Block.Attack");
	UE_DEFINE_GAMEPLAY_TAG(Block_Skill, "Block.Skill");
	UE_DEFINE_GAMEPLAY_TAG(Block_Movement, "Block.Movement");
	UE_DEFINE_GAMEPLAY_TAG(Block_Interaction, "Block.Interaction");
}
