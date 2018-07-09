using UnrealBuildTool;
using System.Collections.Generic;

public class DGameEditorTarget : TargetRules {
    public DGameEditorTarget(TargetInfo Target) : base(Target) {
        Type = TargetType.Editor;

        ExtraModuleNames.AddRange(new string[] { "DGame" });
    }
}