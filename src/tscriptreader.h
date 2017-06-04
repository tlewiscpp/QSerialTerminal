/***********************************************************************
*    tscriptreader.h:                                                  *
*    TScriptReader, script reader for TStream objects                  *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/tlewiscpp/tjlutils                         *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of a TScripReader class that     *
*    reads and parses script files intended for use alongside the      *
*    TScriptExecutor to script commands for TStream objects            *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef TJLUTILS_TSCRIPTREADER_H
#define TJLUTILS_TSCRIPTREADER_H

#include <string>
#include <memory>
#include <vector>

#include "tstream.h"

class TScriptReader
{
public:
    TScriptReader(const std::string &scriptFilePath);
    std::string scriptFilePath() const;
    std::shared_ptr<std::vector<TStreamCommand>> commands() const;
private:
    std::string m_scriptFilePath;
    std::shared_ptr<std::vector<TStreamCommand>> m_commands;

};

#endif //TJLUTILS_TSCRIPTREADER_H
