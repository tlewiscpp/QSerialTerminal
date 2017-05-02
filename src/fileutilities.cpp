/***********************************************************************
*    fileutilities.cpp:                                                *
*    Namespace FileUtilities, for file/argv IO abstraction             *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.com/Pinguinsan/tjlutils                            *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of a FileUtilities namespace   *
*    It is used to abstract file/argv IO into high level functions     *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "fileutilities.h"

namespace FileUtilities
{

    std::vector<std::string> getFileListAsVector(const char *directory, const char *mask, bool caseSensitive)
    {
        if (!directoryExists(directory)) {
            return std::vector<std::string>{};
        }
        std::vector<std::string> returnVector;
        DIR *targetDirectory;
        struct dirent *entity;
        if ((targetDirectory = opendir(directory)) != nullptr) {
            while ((entity = readdir(targetDirectory)) != nullptr) {
                if (!directoryExists(entity->d_name)) {
                    std::string maskCopy{static_cast<std::string>(mask)};
                    std::string nameCopy{static_cast<std::string>(entity->d_name)};
                    if (maskCopy != "") {
                        if (!caseSensitive) {
                            std::transform(maskCopy.begin(), maskCopy.end(), maskCopy.begin(), ::tolower);
                            std::transform(nameCopy.begin(), nameCopy.end(), nameCopy.begin(), ::tolower);
                        }
                        if (nameCopy.find(maskCopy) != std::string::npos) {
                            returnVector.emplace_back(entity->d_name);
                        }
                    } else { 
                        returnVector.emplace_back(entity->d_name);
                    }
                }
            }
            closedir (targetDirectory);
        } else {
            return std::vector<std::string>{};
        }
        return returnVector;
    }

    std::vector<std::string> getDirectoryListAsVector(const char *directory, const char *mask, bool caseSensitive)
    {
        if (!directoryExists(directory)) {
            return std::vector<std::string>{};
        }
        std::vector<std::string> returnVector;
        DIR *targetDirectory;
        struct dirent *entity;
        if ((targetDirectory = opendir(directory)) != nullptr) {
            while ((entity = readdir(targetDirectory)) != nullptr) {
                if (directoryExists(entity->d_name)) {
                    std::string maskCopy{static_cast<std::string>(mask)};
                    std::string nameCopy{static_cast<std::string>(entity->d_name)};
                    if (maskCopy != "") {
                        if (!caseSensitive) {
                            std::transform(maskCopy.begin(), maskCopy.end(), maskCopy.begin(), ::tolower);
                            std::transform(nameCopy.begin(), nameCopy.end(), nameCopy.begin(), ::tolower);
                        }
                        if (nameCopy.find(maskCopy) != std::string::npos) {
                            returnVector.emplace_back(entity->d_name);
                        }
                    } else { 
                        returnVector.emplace_back(entity->d_name);
                    }
                }
            }
            closedir (targetDirectory);
        } else {
            return std::vector<std::string>{};
        }
        return returnVector;
    }

    std::vector<std::string> getPathDirectoriesAsVector()
    {
        std::string temp{static_cast<std::string>(getenv("PATH"))};
        return GeneralUtilities::parseToContainer<std::vector<std::string>>(temp.begin(), temp.end(), ':');
    }

    std::list<std::string> getPathDirectories()
    {
        std::list<std::string> returnList;
        for (auto &it : FileUtilities::getPathDirectoriesAsVector()) {
            returnList.emplace_back(it);
        }
        returnList.sort();
        return returnList;
    }

    std::vector<std::string> getFileListAsVector(const std::string &directory, const std::string &mask, bool caseSensitive)
    {
        return getFileListAsVector(directory.c_str(), mask.c_str(), caseSensitive);
    }

    std::vector<std::string> getFileListAsVector(const char *directory, const std::string &mask, bool caseSensitive)
    {
        return getFileListAsVector(directory, mask.c_str(), caseSensitive);
    }

    std::vector<std::string> getDirectoryListAsVector(const std::string &directory, const std::string &mask, bool caseSensitive)
    {
        return getDirectoryListAsVector(directory.c_str(), mask.c_str(), caseSensitive);
    }

    std::vector<std::string> getDirectoryListAsVector(const char *directory, const std::string &mask, bool caseSensitive)
    {
        return getDirectoryListAsVector(directory, mask.c_str(), caseSensitive);
    }

    std::list<std::string> getFileList(const char *directory, const char *mask, bool caseSensitive)
    {
        std::list<std::string> returnList;
        for (auto &it : FileUtilities::getFileListAsVector(directory, mask, caseSensitive)) {
            returnList.emplace_back(it);
        }
        returnList.sort();
        return returnList;
    }

    std::list<std::string> getDirectoryList(const char *directory, const char *mask, bool caseSensitive)
    {
        std::list<std::string> returnList;
        for (auto &it : FileUtilities::getDirectoryListAsVector(directory, mask, caseSensitive)) {
            returnList.emplace_back(it);
        }
        returnList.sort();
        return returnList;
    }

    std::list<std::string> getFileList(const std::string &directory, const std::string &mask, bool caseSensitive)
    {
        return getFileList(directory.c_str(), mask.c_str(), caseSensitive);
    }

    std::list<std::string> getFileList(const char *directory, const std::string &mask, bool caseSensitive)
    {
        return getFileList(directory, mask.c_str(), caseSensitive);
    }

    std::list<std::string> getDirectoryList(const std::string &directory, const std::string &mask, bool caseSensitive)
    {
        return getDirectoryList(directory.c_str(), mask.c_str(), caseSensitive);
    }

    std::list<std::string> getDirectoryList(const char *directory, const std::string &mask, bool caseSensitive)
    {
        return getDirectoryList(directory, mask.c_str(), caseSensitive);
    }

    std::string getCurrentDirectory()
    {
        #if defined(_WIN32) || defined (__MINGW32__)
            char* buffer;
            buffer = _getcwd(NULL, 0);
            /*Check for null buffer, indicating a failure*/
            if (!buffer) {
                throw std::runtime_error(_CWD_MALLOC_EXCEPTION_STRING);
            } else {
                return std::string(buffer);
            }
        #else
            char *currentWorkingDirectory;
            char filePathBuffer[PATH_MAX + 1];

            currentWorkingDirectory = getcwd(filePathBuffer, PATH_MAX + 1 );
            //Check if cwd is null
            if (currentWorkingDirectory) {
                return std::string(currentWorkingDirectory);
            } else {
                throw std::runtime_error(_CWD_MALLOC_EXCEPTION_STRING);
            }
        #endif
    }

    bool directoryExists(const std::string &directoryToCheck)
    { 
        struct stat info;
        if(stat(directoryToCheck.c_str(), &info) != 0) {
            return false;
        } else if(info.st_mode & S_IFDIR)  { 
            return true;
        } else {
            return false;
        }
        return directoryExists(directoryToCheck.c_str());
    }

    bool directoryExists(const char *directoryToCheck)
    {
        return directoryExists(static_cast<std::string>(directoryToCheck));
    }

    bool fileExists(const std::string &fileToCheck)
    {
        #if defined(_WIN32) && !defined(__CYGWIN__)
            std::ifstream readFromFile;
            readFromFile.open(fileToCheck);
            if (readFromFile.good()) {
                readFromFile.close();
                return true;
            } else {
                return false;
            }
        #else
            return (access(fileToCheck.c_str(),F_OK) != -1);
        #endif
    }

    bool fileExists(const char *fileToCheck)
    {
        return fileExists(static_cast<std::string>(fileToCheck));
    }

    
    unsigned int getInstanceCount(const char *str)
    {
        return getInstanceCount(static_cast<std::string>(str));
    }

    unsigned int getInstanceCount(const std::string &str)
    {
        std::unique_ptr<SystemCommand> systemCommand{new SystemCommand()};
        systemCommand->setCommand("ps aux | grep " + str);
        systemCommand->execute();
        if (systemCommand->hasError()) {
            return 0;
        } else {
            return systemCommand->outputAsVector().size()-2;
        }
    }

    std::vector<std::string> parseArgsToVector(int argcIn,char **argvIn) 
    {
        if (argcIn == 0) {
            throw std::logic_error(_PATL_ARGC_ZERO_EXCEPTION_STRING);
        }
        std::vector<std::string> returnVector;
        //No need to check, c++ standard ensures argv[0..argc-1] is not null
        for (int i = 0; i < argcIn; i++) {
            returnVector.push_back(argvIn[i]);
        }
        return returnVector;
    }

    std::list<std::string> parseArgsToList(int argcIn, char **argvIn)
    {
        if (argcIn == 0) {
            throw std::logic_error(_PATL_ARGC_ZERO_EXCEPTION_STRING);
        }
        std::list<std::string> returnList;
        //No need to check, c++ standard ensures argv[0..argc-1] is not null
        for (int i = 0; i < argcIn; i++) {
            returnList.push_back(argvIn[i]);
        }
        return returnList;
    }
}
