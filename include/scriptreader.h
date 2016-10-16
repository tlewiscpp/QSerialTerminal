#ifndef QSERIALTERMINAL_SCRIPTREADER_H
#define QSERIALTERMINAL_SCRIPTREADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <memory>
#include <algorithm>

#include <fileutilities.h>
#include <generalutilities.h>

#include "serialcommand.h"
#include "qserialterminalstrings.h"

class ScriptReader
{
public:
    ScriptReader(const std::string &scriptFilePath);
    std::shared_ptr<std::vector<SerialCommand>> commands() const;

private:
    std::string m_scriptFilePath;
    std::shared_ptr<std::vector<SerialCommand>> m_commands;

};

#endif //QSERIALTERMINAL_CONFIGURATIONFILEREADER_H
