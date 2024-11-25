
#pragma once

#include <string>
#include <vector>

#include "msgpack.hpp"
namespace Infinity {
    namespace SerializationTypes {
        struct Package {
            bool isGithub{};
            std::string link;

            MSGPACK_DEFINE(isGithub, link);
        };

        namespace Outgoing {
            struct Project {
                std::string name;
                std::string description;
                std::string overview;
                std::string changelogLink;
                std::string background;
                std::string pageBackground;
                std::vector<std::string> variants;
                std::vector<Package> packages;

                MSGPACK_DEFINE(name, description, overview, changelogLink, background, pageBackground, variants, packages);
            };

            struct Palette {
                std::string primary;
                std::string secondary;
                std::string circle1;
                std::string circle2;
                std::string circle3;
                std::string circle4;
                std::string circle5;

                MSGPACK_DEFINE(primary, secondary, circle1, circle2, circle3, circle4, circle5);
            };

            struct GroupData {
                std::string name;
                std::vector<Project> projects;
                std::string logo;
                std::string betaBackground;
                Palette palette;

                MSGPACK_DEFINE(name, projects, logo, betaBackground, palette);
            };

            struct OutgoingBinary {
                GroupData groupData;
                std::string key;

                MSGPACK_DEFINE(groupData, key);
            };

            struct IncomingRawBinary {
                std::vector<unsigned int> data;
            };
        } // namespace Outgoing

        namespace Incoming {
            struct Project {
                std::string name;
                std::string description;
                std::string overview;
                std::string changelog;
                std::string background;
                std::string pageBackground;
                std::vector<std::string> variants;
                std::vector<Package> packages;

                MSGPACK_DEFINE(name, description, overview, changelog, background, pageBackground, variants, packages);
            };

            struct Palette {
                std::string primary;
                std::string secondary;
                std::string circle1;
                std::string circle2;
                std::string circle3;
                std::string circle4;
                std::string circle5;

                MSGPACK_DEFINE(primary, secondary, circle1, circle2, circle3, circle4, circle5);
            };

            struct GroupData {
                std::string name;
                std::vector<Project> projects;
                std::string logo;
                std::string betaBackground;
                Palette palette;
                bool isHidden{};

                MSGPACK_DEFINE(name, projects, logo, betaBackground, palette, isHidden);
            };

            struct IncomingBinary {
                std::map<std::string, GroupData> groupData;

                MSGPACK_DEFINE(groupData);
            };
        } // namespace Incoming
    } // namespace SerializationTypes
} // namespace Infinity
