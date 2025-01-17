#include <algorithm>
#include "ConfigFile.h"
#include "CommonUtils.h"

ConfigFileReader::ConfigFileReader(const std::string &path)
{
    _path = path;
}

bool ConfigFileReader::exists()
{
    std::ifstream f(_path);
    return f.good();
}

ConfigFileEntry ConfigFileReader::readEntry(const std::string &line)
{
    size_t eqPos = line.find('=');

    if (eqPos == std::string::npos) {
        throw std::string("Invalid syntax");
    }

    std::string leftSide = CommonUtils::trim(line.substr(0, eqPos), ' ');

    std::string rightSide = CommonUtils::trim(line.substr(eqPos + 1), ' ');

    return ConfigFileEntry(leftSide, rightSide);
}

std::map <std::string, ConfigFileEntry> ConfigFileReader::read()
{
    std::vector <std::string> lines;
    std::map <std::string, ConfigFileEntry> entries;

    CommonUtils::readLinesFromStream(_path, lines);

    for (int i = 0; i < lines.size(); i++) {
        ConfigFileEntry e = readEntry(lines[i]);
        std::string k = CommonUtils::toLower(e.key);
        entries[k] = e;
    }

    return entries;
}
