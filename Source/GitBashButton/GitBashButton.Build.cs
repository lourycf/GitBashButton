using UnrealBuildTool;

public class GitBashButton : ModuleRules
{
    public GitBashButton(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "Slate",
            "SlateCore",
            "LevelEditor",
            "ToolMenus",
            "InputCore",
            "UnrealEd",
            "Projects"
        });
    }
}