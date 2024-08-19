using UnrealBuildTool;

public class FlecsCore : ModuleRules
{
    private bool RegisterMeta = true;
    public FlecsCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.NoPCHs;
        CStandard = CStandardVersion.C11;
        //bUseUnity = false;
        OptimizeCode = CodeOptimization.Never;
        bWarningsAsErrors = false;
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "FlecsLib"
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
        PrivateDefinitions.Add("flecs_EXPORTS");
        
        if(Target.Type==TargetRules.TargetType.Editor)
        {
            if(RegisterMeta)
                PrivateDefinitions.Add("REGISTER_META=1");
        }
    }
}