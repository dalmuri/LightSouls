// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LightSouls : ModuleRules
{
	public LightSouls(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "HairStrandsCore", "Niagara", "NavigationSystem", "AIModule", "GameplayTasks" });
	}
}
