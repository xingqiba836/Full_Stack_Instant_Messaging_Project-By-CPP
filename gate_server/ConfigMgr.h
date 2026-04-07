#ifndef CONFIG_MGR_H
#define CONFIG_MGR_H

#include <map>
#include <string>

struct SectionInfo {
    std::map<std::string, std::string> section_data;

    std::string operator[](const std::string &key) const
    {
        auto iter = section_data.find(key);
        if (iter == section_data.end()) {
            return "";
        }
        return iter->second;
    }
};

class ConfigMgr {
public:
    ConfigMgr();
    SectionInfo operator[](const std::string &section) const;

private:
    std::map<std::string, SectionInfo> config_map_;
};

extern ConfigMgr gConfigMgr;

#endif // CONFIG_MGR_H
