#include "ConfigMgr.h"

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <filesystem>
#include <iostream>

ConfigMgr gConfigMgr;

ConfigMgr::ConfigMgr()
{
    try {
        std::filesystem::path current_path = std::filesystem::current_path();
        std::filesystem::path config_path = current_path / "config.ini";
        std::cout << "Config path: " << config_path.string() << std::endl;

        boost::property_tree::ptree property_tree;
        boost::property_tree::read_ini(config_path.string(), property_tree);

        for (const auto &section_pair : property_tree) {
            SectionInfo section_info;
            for (const auto &key_value_pair : section_pair.second) {
                section_info.section_data[key_value_pair.first] = key_value_pair.second.get_value<std::string>();
            }
            config_map_[section_pair.first] = section_info;
        }
    } catch (const std::exception &exception) {
        std::cerr << "Load config.ini failed: " << exception.what() << std::endl;
    }
}

SectionInfo ConfigMgr::operator[](const std::string &section) const
{
    auto iter = config_map_.find(section);
    if (iter == config_map_.end()) {
        return SectionInfo{};
    }
    return iter->second;
}
