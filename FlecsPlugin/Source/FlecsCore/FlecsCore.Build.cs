using UnrealBuildTool;

public class FlecsCore : ModuleRules
{
    private bool RegisterMeta = true;
    public FlecsCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.NoPCHs;
        CStandard = CStandardVersion.C99;
        bUseUnity = false;
        bWarningsAsErrors = false;
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "FlecsLib", "Engine"
            }
        );
        
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore", 
            }
        );
        
        // PublicIncludePaths.AddRange(
        //     new string[]
        //     {
        //         "FlecsCore/FlecsRaw/include",
        //     }
        // );
        PublicIncludePaths.Add($"{ModuleDirectory}/FlecsRaw/include");
        PrivateDefinitions.Add("flecs_EXPORTS");
        
        if(Target.Type==TargetRules.TargetType.Editor)
        {
            OptimizeCode = CodeOptimization.Never;
            if(RegisterMeta)
                PrivateDefinitions.Add("REGISTER_META=1");
        }
    }
}