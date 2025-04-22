using UnrealBuildTool;

public class LudusClient : ModuleRules
{
	public LudusClient(ReadOnlyTargetRules Target) : base(Target)
	{
		bUsePrecompiled = true;
		PrecompileForTargets = PrecompileTargetsType.Any;
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"LudusCore",
				"HTTP",
				"HTTPServer",
				"Projects",
				"LudusCore",
				"Json",
				"JsonUtilities", 
				"HotReload",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"LudusEditor",
				"LevelEditor",
				"UnrealEd",
			}
		);
	}
}