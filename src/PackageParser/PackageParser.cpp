
#include "PackageParser.hpp"


namespace InfinityPackageParser {
    PackageParser::PackageParser() {
        // TODO: construct data structs
    }


    void PackageParser::ParseMainXML() {
        NodeXML root = MicroXML::parse(m_XMLFilePath);
        NodeSet project = root("Project");

        PackageTypes::BaseXML parsedXML;

        parsedXML.Version = project[0]["Version"];
        parsedXML.Name = project[0]["Name"];
        parsedXML.FolderName = project[0]["FolderName"];
        parsedXML.PublishingGroupFolderName = project[0]["PublishingGroupFolderName"];
        parsedXML.MetadataFolderName = project[0]["MetadataFolderName"];
        parsedXML.PublishingGroupMetadataFolderName = project[0]["PublishingGroupMetadataFolderName"];

        for (auto package: root("Project")("Packages")("Package")) {
            parsedXML.Packages.push_back(package->asString());
        }
    }

    void PackageParser::ParsePackageDefinitionsXML() {
        NodeXML root = MicroXML::parse(m_PackageDefinitionsXMLPath);
        NodeSet assetPackage = root("AssetPackage");

        PackageTypes::PackageDefinitions::PackageDefinitionType package_definition_parsed;

        package_definition_parsed.asset_package.Version = assetPackage[0]["Version"];

        package_definition_parsed.asset_package.item_settings.ContentType = assetPackage("ItemSettings")("ContentType")[0].asString();
        package_definition_parsed.asset_package.item_settings.Title = assetPackage("ItemSettings")("Title")[0].asString();
        package_definition_parsed.asset_package.item_settings.Manufacturer = assetPackage("ItemSettings")("Manufacturer")[0].asString();
        package_definition_parsed.asset_package.item_settings.Creator = assetPackage("ItemSettings")("Creator")[0].asString();

        package_definition_parsed.asset_package.flags.VisibleInStore = assetPackage("Flags")("VisibleInStore")[0].asString();
        package_definition_parsed.asset_package.flags.CanBeReferenced = assetPackage("Flags")("CanBeReferenced")[0].asString();

        for (auto assetGroup: assetPackage("AssetGroups")("AssetGroup")) {
            PackageTypes::PackageDefinitions::PackageDefinitionType::AssetPackage::AssetGroups::AssetGroup asset_group;
            asset_group.Name = assetGroup["Name"];
            asset_group.Type = assetGroup("Type")[0].asString();
            asset_group.flags.FSXCompatibility = assetGroup("Flags/FSXCompatibility")[0].asString();
            asset_group.AssetDir = assetGroup("AssetDir")[0].asString();
            asset_group.OutputDir = assetGroup("OutputDir")[0].asString();

            package_definition_parsed.asset_package.asset_groups.asset_group.push_back(asset_group);
        }
    }

    void PackageParser::ParseAircraftCFG() {
        const mINI::INIFile file(R"(C:\Users\Taco\Documents\Github\parserTester\aircraft.cfg)"); // needs a full path, we will build with a helper function

        mINI::INIStructure ini;
        file.read(ini);


        for (int i = 0; i < 100; i++) {
            std::string name = "FLTSIM." + std::to_string(i);
            if (ini.has(name)) {
                auto object = ini[name];

                PackageTypes::AircraftCFG::LiveryObject newObject;

                newObject.title = object["Title"];
                newObject.model = object["Model"];
                newObject.panel = object["Panel"];
                newObject.sound = object["Sound"];
                newObject.texture = object["Texture"];
                newObject.kbChecklists = object["KB_Checklists"];
                newObject.kbReference = object["KB_Reference"];
                newObject.description = object["description"];
                newObject.wipIndicator = object["wip_indicator"];
                newObject.uiManufacturer = object["ui_manufacturer"];
                newObject.uiType = object["ui_type"];
                newObject.uiVariation = object["ui_variation"];
                newObject.uiTyperole = object["ui_typerole"];
                newObject.uiCreatedBy = object["ui_createdby"];
                newObject.uiThumbnailFile = object["ui_thumbnailfile"];
                newObject.uiCertifiedCeiling = object["ui_certified_ceiling"];
                newObject.uiMaxRange = object["ui_max_range"];
                newObject.uiAutonomy = object["ui_autonomy"];
                newObject.uiFuelBurnRate = object["ui_fuel_burn_rate"];
                newObject.atcId = object["atc_id"];
                newObject.icaoAirline = object["icao_airline"];
                newObject.atcIdEnable = object["atc_id_enable"];
                newObject.atcAirline = object["atc_airline"];
                newObject.atcAirline = object["atc_flight_number"];
                newObject.atcHeavy = object["atc_heavy"];
                newObject.atcIdColor = object["atc_id_color"];
                newObject.atcIdFont = object["atc_id_font"];
                newObject.isAirTraffic = object["isAirTraffic"];
                newObject.isUserSelectable = object["isUserSelectable"];
                newObject.isFlyable = object["isFlyable"];
                newObject.canBeUsedByAiTraffic = object["canBeUsedByAITraffic"];
                newObject.effects = object["Effects"];
                newObject.atcParkingTypes = object["atc_parking_types"];
                newObject.atcParkingCodes = object["atc_parking_codes"];
                newObject.soundAi = object["soundai"];
                newObject.shortDescription = object["short_description"];

                m_AircraftLiveries.push_back(newObject);
            } else {
                i = 100;
            }
        }
    }
}
