using System;
using System.IO;
using UnrealBuildTool;

namespace UnrealBuildTool.Rules
{
    public class LibOGDF : ModuleRules
    {

        private string ModulePath
        {
            get {
                return ModuleDirectory;
            }
        }

        private string ThirdPartyPath
        {
            get { return Path.GetFullPath(Path.Combine(ModulePath, "../ThirdParty/")); }
        }

        public LibOGDF(ReadOnlyTargetRules Target):base(Target)
        {

            PublicIncludePaths.AddRange(new string[] {
                Path.Combine(ThirdPartyPath, "OGDF", "Include")
                });

            PrivateIncludePaths.AddRange(new string[] {
                    "LibOGDF/Private",
                });

            PublicDependencyModuleNames.AddRange(new string[] {
                    "Core",
                });

            PrivateDependencyModuleNames.AddRange(new string[] {
                });

            DynamicallyLoadedModuleNames.AddRange(new string[] {
                });

            string ConfigurationName;
            if(Target.Configuration== UnrealTargetConfiguration.Debug||Target.Configuration== UnrealTargetConfiguration.DebugGame) 
            {
                    ConfigurationName = "Debug";
            }else{
                    ConfigurationName = "Release";
            }
            

            string LibPath = Path.Combine(ThirdPartyPath, "OGDF", "Lib", Target.Platform.ToString());
            if (Target.Platform == UnrealTargetPlatform.Win64)
            {

                PublicAdditionalLibraries.Add("psapi.lib");
                PublicAdditionalLibraries.Add(Path.Combine(LibPath, ConfigurationName, "ogdf.lib"));
                PublicAdditionalLibraries.Add(Path.Combine(LibPath, ConfigurationName, "coin.lib"));  //Ìí¼Ó´ËÐÐ
            } else if (Target.Platform == UnrealTargetPlatform.Mac) {

                PublicAdditionalLibraries.Add(Path.Combine(LibPath, "libOGDF.a"));
            }
            
        }
    }
}
