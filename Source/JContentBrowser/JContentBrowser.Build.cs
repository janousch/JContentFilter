// Copyright 2020 Ramon Janousch

using UnrealBuildTool;

public class JContentBrowser : ModuleRules
{
	public JContentBrowser(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnforceIWYU = true;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"AssetRegistry",
				"Core",
				"CoreUObject",
				"Engine",
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Slate",
				"SlateCore",
				"ContentBrowser",
				"AssetTools",
				"UnrealEd"
            }
		);
	}
}
