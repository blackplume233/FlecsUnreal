using UnrealBuildTool;

public class FlecsTest : ModuleRules
{
    bool bEnableFlecsTestCode = false;
    public FlecsTest(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
                "FlecsCore",
                
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

        if (bEnableFlecsTestCode)
        {
            PrivateDefinitions.Add("EnableFlecsTestCode=1");
        }
    }
}