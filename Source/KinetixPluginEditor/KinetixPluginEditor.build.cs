using UnrealBuildTool;

public class KinetixPluginEditor : ModuleRules
{
	public KinetixPluginEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"EditorFramework",
				"UnrealEd",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"HTTP",
				"Json",
				"JsonUtilities",

				"IKRig",
				"IKRigEditor",
				"AnimationCore",
				"MeshDescription",
				"SkeletalMeshDescription",
				"EditorSubsystem", 
				"Blutility",
				"ToolMenus",
				"Projects",
				"UMGEditor",
                "DeveloperSettings"
			}
		);
	}
}
