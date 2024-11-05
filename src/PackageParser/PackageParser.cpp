
#include "PackageParser.hpp"
#include "MicroXML/MicroXML.hpp"


namespace InfinityPackageParser {

    void PackageParser::ParseMainXML() const {
        NodeXML root = MicroXML::parse(m_XMLFilePath);
        NodeSet project = root("Project");

        PackageTypes::BaseXML parsed_xml;

        parsed_xml.Version = project[0]["Version"];
        parsed_xml.Name = project[0]["Name"];
        parsed_xml.FolderName = project[0]["FolderName"];
        parsed_xml.PublishingGroupFolderName = project[0]["PublishingGroupFolderName"];
        parsed_xml.MetadataFolderName = project[0]["MetadataFolderName"];
        parsed_xml.PublishingGroupMetadataFolderName = project[0]["PublishingGroupMetadataFolderName"];

        for (auto package: root("Project")("Packages")("Package")) {
            parsed_xml.Packages.push_back(package->asString());
        }
    }

    void PackageParser::ParsePackageDefinitionsXML() const {
        NodeXML root = MicroXML::parse(m_PackageDefinitionsXMLPath);
        NodeSet asset_package = root("AssetPackage");

        PackageTypes::PackageDefinitions::PackageDefinitionType package_definition_parsed;

        package_definition_parsed.asset_package.Version = asset_package[0]["Version"];

        package_definition_parsed.asset_package.item_settings.ContentType = asset_package("ItemSettings")("ContentType")[0].asString();
        package_definition_parsed.asset_package.item_settings.Title = asset_package("ItemSettings")("Title")[0].asString();
        package_definition_parsed.asset_package.item_settings.Manufacturer = asset_package("ItemSettings")("Manufacturer")[0].asString();
        package_definition_parsed.asset_package.item_settings.Creator = asset_package("ItemSettings")("Creator")[0].asString();

        package_definition_parsed.asset_package.flags.VisibleInStore = asset_package("Flags")("VisibleInStore")[0].asString();
        package_definition_parsed.asset_package.flags.CanBeReferenced = asset_package("Flags")("CanBeReferenced")[0].asString();

        for (auto asset_node: asset_package("AssetGroups")("AssetGroup")) {
            PackageTypes::PackageDefinitions::PackageDefinitionType::AssetPackage::AssetGroups::AssetGroup asset_group;
            asset_group.Name = asset_node["Name"];
            asset_group.Type = asset_node("Type")[0].asString();
            asset_group.flags.FSXCompatibility = asset_node("Flags/FSXCompatibility")[0].asString();
            asset_group.AssetDir = asset_node("AssetDir")[0].asString();
            asset_group.OutputDir = asset_node("OutputDir")[0].asString();

            package_definition_parsed.asset_package.asset_groups.asset_group.push_back(asset_group);
        }
    }

    void PackageParser::ParseAircraftCFG() {
        const mINI::INIFile file(R"(C:\Users\Taco\Documents\Github\parserTester\aircraft.cfg)"); // needs a full path, we will build with
                                                                                                 // a helper function

        mINI::INIStructure ini;
        file.read(ini);

        for (int i = 0; i < 100; i++) {
            if (const std::string name = "FLTSIM." + std::to_string(i); ini.has(name)) {
                auto object = ini[name];

                PackageTypes::AircraftCFG::LiveryObject new_object;

                new_object.title = object["Title"];
                new_object.model = object["Model"];
                new_object.panel = object["Panel"];
                new_object.sound = object["Sound"];
                new_object.texture = object["Texture"];
                new_object.kbChecklists = object["KB_Checklists"];
                new_object.kbReference = object["KB_Reference"];
                new_object.description = object["description"];
                new_object.wipIndicator = object["wip_indicator"];
                new_object.uiManufacturer = object["ui_manufacturer"];
                new_object.uiType = object["ui_type"];
                new_object.uiVariation = object["ui_variation"];
                new_object.uiTyperole = object["ui_typerole"];
                new_object.uiCreatedBy = object["ui_createdby"];
                new_object.uiThumbnailFile = object["ui_thumbnailfile"];
                new_object.uiCertifiedCeiling = object["ui_certified_ceiling"];
                new_object.uiMaxRange = object["ui_max_range"];
                new_object.uiAutonomy = object["ui_autonomy"];
                new_object.uiFuelBurnRate = object["ui_fuel_burn_rate"];
                new_object.atcId = object["atc_id"];
                new_object.icaoAirline = object["icao_airline"];
                new_object.atcIdEnable = object["atc_id_enable"];
                new_object.atcAirline = object["atc_airline"];
                new_object.atcAirline = object["atc_flight_number"];
                new_object.atcHeavy = object["atc_heavy"];
                new_object.atcIdColor = object["atc_id_color"];
                new_object.atcIdFont = object["atc_id_font"];
                new_object.isAirTraffic = object["isAirTraffic"];
                new_object.isUserSelectable = object["isUserSelectable"];
                new_object.isFlyable = object["isFlyable"];
                new_object.canBeUsedByAiTraffic = object["canBeUsedByAITraffic"];
                new_object.effects = object["Effects"];
                new_object.atcParkingTypes = object["atc_parking_types"];
                new_object.atcParkingCodes = object["atc_parking_codes"];
                new_object.soundAi = object["soundai"];
                new_object.shortDescription = object["short_description"];

                m_AircraftLiveries.push_back(new_object);
            } else {
                i = 100;
            }
        }
    }
} // namespace InfinityPackageParser
