// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Terroria : ModuleRules
{
	public Terroria(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"GameplayTasks",
			"GameplayTags",
			"GameplayAbilities",
			"Niagara",
			"NavigationSystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Terroria",
			"Terroria/Variant_Horror",
			"Terroria/Variant_Horror/UI",
			"Terroria/Variant_Shooter",
			"Terroria/Variant_Shooter/AI",
			"Terroria/Variant_Shooter/UI",
			"Terroria/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
