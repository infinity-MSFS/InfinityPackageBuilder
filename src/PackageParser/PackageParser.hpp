
#pragma once

#include <string>
#include <vector>


#include "ini/ini.h"

namespace InfinityPackageParser {
    namespace PackageTypes {
        struct BaseXML {
            std::string Version;
            std::string Name;
            std::string FolderName;
            std::string PublishingGroupFolderName;
            std::string MetadataFolderName;
            std::string PublishingGroupMetadataFolderName;
            std::vector<std::string> Packages;
        };

        namespace AircraftCFG {
            struct LiveryObject {
                std::string title;
                std::string model;
                std::string panel;
                std::string sound;
                std::string texture;
                std::string kbChecklists;
                std::string kbReference;
                std::string description;
                std::string wipIndicator;
                std::string uiManufacturer;
                std::string uiType;
                std::string uiVariation;
                std::string uiTyperole;
                std::string uiCreatedBy;
                std::string uiThumbnailFile;
                std::string uiCertifiedCeiling;
                std::string uiMaxRange;
                std::string uiAutonomy;
                std::string uiFuelBurnRate;
                std::string atcId;
                std::string icaoAirline;
                std::string atcIdEnable;
                std::string atcAirline;
                std::string atcFlightNumber;
                std::string atcHeavy;
                std::string atcIdColor;
                std::string atcIdFont;
                std::string isAirTraffic;
                std::string isUserSelectable;
                std::string isFlyable;
                std::string canBeUsedByAiTraffic;
                std::string effects;
                std::string atcParkingTypes;
                std::string atcParkingCodes;
                std::string soundAi;
                std::string shortDescription;
            };
        } // namespace AircraftCFG

        namespace PackageDefinitions {
            struct PackageDefinitionType {
                struct AssetPackage {
                    std::string Version;

                    struct ItemSettings {
                        std::string ContentType;
                        std::string Title;
                        std::string Manufacturer;
                        std::string Creator;
                    };

                    ItemSettings item_settings;

                    struct Flags {
                        std::string VisibleInStore;
                        std::string CanBeReferenced;
                    };

                    Flags flags;

                    struct AssetGroups {
                        struct AssetGroup {
                            std::string Name;
                            std::string Type;

                            struct AssetGroupFlags {
                                std::string FSXCompatibility;
                            };

                            AssetGroupFlags flags;
                            std::string AssetDir;
                            std::string OutputDir;
                        };

                        std::vector<AssetGroup> asset_group;
                    };

                    AssetGroups asset_groups;
                };

                AssetPackage asset_package;
            };
        } // namespace PackageDefinitions
    } // namespace PackageTypes

    class PackageParser {
    public:
        PackageParser() = default;

    private:
        void ParseMainXML() const;

        void ParsePackageDefinitionsXML() const;

        void ParseAircraftCFG();

    private:
        /*
         * <Project Version="2" Name="C-17" FolderName="Packages" PublishingGroupFolderName="PublishingGroupsContent"
         * MetadataFolderName="PackagesMetadata" PublishingGroupMetadataFolderName="PublishingGroupsMetadata">
         *    <OutputDirectory>.</OutputDirectory>
         *    <TemporaryOutputDirectory>_PackageInt</TemporaryOutputDirectory>
         *    <PublishingGroupTemporaryOutputDirectory>_PublishingGroupInt</PublishingGroupTemporaryOutputDirectory>
         *    <Packages>
         *        <Package>PackageDefinitions\deltasimulations-c17.xml</Package>
         *    </Packages>
         *    <PublishingGroups/>
         * </Project>
         */
        PackageTypes::BaseXML m_BaseXMLData;
        PackageTypes::PackageDefinitions::PackageDefinitionType m_PackageDefinitionsXml;
        std::vector<PackageTypes::AircraftCFG::LiveryObject> m_AircraftLiveries;
        std::string m_XMLFilePath;
        std::string m_PackageDefinitionsXMLPath;
        std::string m_AircraftCfgPath;
        std::string m_Version;
    };
} // namespace InfinityPackageParser
