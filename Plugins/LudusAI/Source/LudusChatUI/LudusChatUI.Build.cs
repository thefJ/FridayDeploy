using UnrealBuildTool;

public class LudusChatUI : ModuleRules
{
	public LudusChatUI(ReadOnlyTargetRules Target) : base(Target)
	{
		bUsePrecompiled = true;
		PrecompileForTargets = PrecompileTargetsType.Any;
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp17;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"Slate",
				"SlateCore",
				"UMG",
				"LudusClient",
				"Projects",
				"UnrealEd",
				"LudusMarkdown",
				"Documentation",
				"GraphEditor",
				"BlueprintGraph",
				"MessageLog",
				"ApplicationCore"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"LudusCore",
				"WorkspaceMenuStructure",
				"ToolMenus",
				"LudusMarkdown",
				"AppFramework",
				"EditorFramework",
				"Kismet",
				"PlacementMode",
				"Settings",
				"PropertyEditor",
				"DesktopPlatform",
				"AssetTools",
				"SourceCodeAccess",
				"ContentBrowser",
				"LevelEditor",
				"AssetRegistry",
				"Analytics",
				"GameProjectGeneration",
			}
		);

		PrivateIncludePathModuleNames.AddRange(
			new string[] {
				"MainFrame",
				"TargetPlatform",
				"TargetDeviceServices",
				"LauncherServices"
			}
		);

		DynamicallyLoadedModuleNames.AddRange(
			new string[] {
				"MainFrame",
				"LauncherServices"
			}
		);
	}
}