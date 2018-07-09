using UnrealBuildTool;
using System.Collections.Generic;

public class DGameTarget : TargetRules {
    public DGameTarget(TargetInfo Target) : base(Target) {
        Type = TargetType.Game;

        ExtraModuleNames.AddRange(new string[] { "DGame" });
    }
}