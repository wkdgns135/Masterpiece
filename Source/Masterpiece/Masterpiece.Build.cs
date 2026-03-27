// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Masterpiece : ModuleRules
{
	public Masterpiece(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "DeveloperSettings", "InputCore", "EnhancedInput", "AIModule", "GameplayAbilities", "GameplayTags", "GameplayTasks", "NavigationSystem", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		PrivateIncludePaths.Add("Masterpiece");

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
