// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class GDC2016 : ModuleRules
	{
		public GDC2016(ReadOnlyTargetRules Target) : base(Target)
		{
			PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

			PrivateDependencyModuleNames.AddRange(
				new string[] {
					"Core",
					"CoreUObject",
					"Engine",
					"MessagingCommon",
					"MessagingRpc",
				});

			PrivateIncludePathModuleNames.AddRange(
				new string[] {
					"Messaging",
				});

			PrivateIncludePaths.AddRange(
				new string[] {
					"GDC2016/Private",
				});
		}
	}
}
