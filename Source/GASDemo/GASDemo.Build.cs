// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GASDemo : ModuleRules
{
	public GASDemo(ReadOnlyTargetRules Target) : base(Target)
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
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
        });

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"GASDemo",
			"GASDemo/Variant_Platforming",
			"GASDemo/Variant_Platforming/Animation",
			"GASDemo/Variant_Combat",
			"GASDemo/Variant_Combat/AI",
			"GASDemo/Variant_Combat/Animation",
			"GASDemo/Variant_Combat/Gameplay",
			"GASDemo/Variant_Combat/Interfaces",
			"GASDemo/Variant_Combat/UI",
			"GASDemo/Variant_SideScrolling",
			"GASDemo/Variant_SideScrolling/AI",
			"GASDemo/Variant_SideScrolling/Gameplay",
			"GASDemo/Variant_SideScrolling/Interfaces",
			"GASDemo/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
