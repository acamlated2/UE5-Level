// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE5Level : ModuleRules
{
	public UE5Level(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
