/***********************************************************************
*    generalutilities.cpp:                                             *
*    Namespace GeneralUtilities, for general use functions             *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.com/tlewiscpp/tjlutils                            *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of a GeneralUtilities namespace*
*    The namespace contains several general use functions, including   *
*    STL algorithm fuinctions, delay functions, char to int functions, *
*    logging functions, and mathematical rounding functions            *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "generalutilities.h"

namespace GeneralUtilities
{
    float decStringToFloat(const std::string &str)
    {
        std::string copyString{""};
        for (auto &it : str) {
            if ((it != '0') && (it != '.')) {
                copyString += it;
            }
        }
        if (copyString.length() == 0) {
            return 0;
        }
        auto returnValue = atof(str.c_str());
        if (returnValue == 0) {
            throw std::invalid_argument("stof: invalid string " + str);
        }
        return returnValue;
    }

    double decStringToDouble(const std::string &str)
    {
        return GeneralUtilities::decStringToFloat(str);
    }

    int decStringToInt(const std::string &str)
    {
        std::string copyString{""};
        for (auto &it : str) {
            if (it != '0') {
                copyString += it;
            }
        }
        if (copyString.length() == 0) {
            return 0;
        }
        auto returnValue = atoi(str.c_str());
        if (returnValue == 0) {
            throw std::invalid_argument("stoi: invalid string " + str);
        }
        return returnValue;
    }

    long int decStringToLong(const std::string &str)
    {
        std::string copyString{""};
        for (auto &it : str) {
            if (it != '0') {
                copyString += it;
            }
        }
        if (copyString.length() == 0) {
            return 0;
        }
        auto returnValue = atoi(str.c_str());
        if (returnValue == 0) {
            throw std::invalid_argument("stol: invalid string " + str);
        }
        return returnValue;
    }

    long long int decStringToLongLong(const std::string &str)
    {
        std::string copyString{""};
        for (auto &it : str) {
            if (it != '0') {
                copyString += it;
            }
        }
        if (copyString.length() == 0) {
            return 0;
        }
        auto returnValue = atoi(str.c_str());
        if (returnValue == 0) {
            throw std::invalid_argument("stoll: invalid string " + str);
        }
        return returnValue;
    }

    std::string toBinaryString(int number)
    {
        if (number == 0) {
            return "0";
        }
        int digitCount = 0;
        int copy = number;
        do {
            digitCount++;
        } while (copy /= 2);
        copy = number;
        std::string returnString{""};
        for (int i = 0; i < digitCount; i++) {
            copy = number % 2;
            if (copy == 0) returnString.insert(returnString.begin(), '0');
            else if ((copy % 1) == 0) returnString.insert(returnString.begin(), '1');
            number /= 2;
        }
        return returnString;
    }

    std::string toDecString(int number)
    {
        if (number == 0) {
            return "0";
        }
        int digitCount = 0;
        int copy = number;
        while (copy > 0) {
            copy /= 10;
            digitCount++;
        }
        copy = number;
        std::string returnString{""};
        for (int i = 0; i < digitCount; i++) {
            copy = number % 10;
            if (copy == 0) returnString.insert(returnString.begin(), '0');
            else if ((copy % 9 ) == 0) returnString.insert(returnString.begin(), '9');
            else if ((copy % 8) == 0) returnString.insert(returnString.begin(), '8');
            else if ((copy % 7) == 0) returnString.insert(returnString.begin(), '7');
            else if ((copy % 6) == 0) returnString.insert(returnString.begin(), '6');
            else if ((copy % 5) == 0) returnString.insert(returnString.begin(), '5');
            else if ((copy % 4) == 0) returnString.insert(returnString.begin(), '4');
            else if ((copy % 3) == 0) returnString.insert(returnString.begin(), '3');
            else if ((copy % 2) == 0) returnString.insert(returnString.begin(), '2');
            else if ((copy % 1) == 0) returnString.insert(returnString.begin(), '1');
            number /= 10;
        }
        return returnString;
    }

    std::string toHexString(int number)
    {
        if (number == 0) {
            return "0";
        }
        int digitCount = 0;
        int copy = number;
        do {
            digitCount++;
        } while (copy /= 16);
        copy = number;
        std::string returnString{""};
        for (int i = 0; i < digitCount; i++) {
            copy = number % 16;
            if (copy == 0) returnString.insert(returnString.begin(), '0');
            else if ((copy % 15) == 0) returnString.insert(returnString.begin(), 'f');
            else if ((copy % 14) == 0) returnString.insert(returnString.begin(), 'e');
            else if ((copy % 13) == 0) returnString.insert(returnString.begin(), 'd');
            else if ((copy % 12) == 0) returnString.insert(returnString.begin(), 'c');
            else if ((copy % 11) == 0) returnString.insert(returnString.begin(), 'b');
            else if ((copy % 10) == 0) returnString.insert(returnString.begin(), 'a');                        
            else if ((copy % 9) == 0) returnString.insert(returnString.begin(), '9');
            else if ((copy % 8) == 0) returnString.insert(returnString.begin(), '8');
            else if ((copy % 7) == 0) returnString.insert(returnString.begin(), '7');
            else if ((copy % 6) == 0) returnString.insert(returnString.begin(), '6');
            else if ((copy % 5) == 0) returnString.insert(returnString.begin(), '5');
            else if ((copy % 4) == 0) returnString.insert(returnString.begin(), '4');
            else if ((copy % 3) == 0) returnString.insert(returnString.begin(), '3');
            else if ((copy % 2) == 0) returnString.insert(returnString.begin(), '2');
            else if ((copy % 1) == 0) returnString.insert(returnString.begin(), '1');
            number /= 16;
        }
        return returnString;
    }

    std::string toBinaryString(bool number) { return (number ? toBinaryString(1) : toBinaryString(0)); }
    std::string toDecString(bool number) { return (number ? toDecString(1) : toDecString(0)); }
    std::string toHexString(bool number) { return (number ? toHexString(1) : toHexString(0)); }
    
    std::string toBinaryString(uint32_t number)
    {
        return toBinaryString(static_cast<int>(number));
    }

    std::string toDecString(uint32_t number)
    {
        return toDecString(static_cast<int>(number));
    }


    std::string toHexString(uint32_t number)
    {
        return toHexString(static_cast<int>(number));
    }

    std::string toBinaryString(uint8_t number)
    {
        return toBinaryString(static_cast<int>(number));
    }

    std::string toDecString(uint8_t number)
    {
        return toHexString(static_cast<int>(number));
    }    
    
    std::string toHexString(uint8_t number)
    {
        return toHexString(static_cast<int>(number));
    }

    std::string stripLineEndings(const std::string &str)
    {
        std::string copyString{str};
        copyString = stripAllFromString(copyString, "\r");
        copyString = stripAllFromString(copyString, "\n");
        return copyString;
    }

    bool isNonAsciiChar (char c)
    {  
        return !isprint( static_cast<unsigned char>( c ) );
    } 

    std::string stripNonAsciiCharacters(const std::string &str) 
    { 
        std::string copyString{str};
        copyString.erase(remove_if(copyString.begin(), copyString.end(), isNonAsciiChar), copyString.end());  
        return copyString;
    }

    std::string toLowercase(const std::string &str)
    {
        std::string returnString{str};
        std::transform(returnString.begin(), returnString.end(), returnString.begin(), ::tolower);
        return returnString;
    }

    std::string toUppercase(const std::string &str)
    {
        std::string returnString{str};
        std::transform(returnString.begin(), returnString.end(), returnString.begin(), ::toupper);
        return returnString;
    }

    std::vector<std::string> trimWhitespaceFromBeginning (const std::vector<std::string> &vec)
    {
        std::vector<std::string> returnVec{vec};
        for (auto &it : returnVec) {
            it = trimWhitespaceFromBeginning(it);
        }
        return returnVec;
    }

    std::vector<std::string> trimWhitespaceFromEnd (const std::vector<std::string> &vec)
    {
        std::vector<std::string> returnVec{vec};
        for (auto &it : returnVec) {
            it = trimWhitespaceFromEnd(it);
        }
        return returnVec;
    }

    std::vector<std::string> trimWhitespace (const std::vector<std::string> &vec)
    {
        std::vector<std::string> returnVec{vec};
        for (auto &it : returnVec) {
            it = trimWhitespace(it);
        }
        return returnVec;
    }

    std::string trimWhitespace(const std::string &str)
    {
        if (str.length() == 0) {
            return str;
        }
        std::string copyString{str};
        copyString = trimWhitespaceFromBeginning(copyString);
        copyString = trimWhitespaceFromEnd(copyString);
        return copyString;
    }
    
    std::string trimWhitespaceFromBeginning(const std::string &str)
    {
        if (str.length() == 0) {
            return str;
        }
        std::string copyString{str};
        while ((copyString.length() != 0) && (copyString[0] == ' ')) {
            copyString = copyString.substr(1);
        }
        return copyString;
    }
    
    std::string trimWhitespaceFromEnd(const std::string &str)
    {
        if (str.length() == 0) {
            return str;
        }
        std::string copyString{str};
        while ((copyString.length() != 0) && (copyString[copyString.length()-1] == ' ')) {
            copyString = copyString.substr(0, copyString.length()-1);
        }
        return copyString;
    }
    
    bool isWhitespace(const std::string &stringToCheck)
    {
        for (std::string::const_iterator iter = stringToCheck.begin(); iter != stringToCheck.end(); iter++) {
            if (static_cast<unsigned>(*iter) > ASCII_WHITESPACE_MAXIMUM_VALUE) {
                return false;
            } else if (((*iter) != '\r') || ((*iter) != '\n')) {
                return false;
            }
        }
        return true;
    }

    bool isWhitespace(char charToCheck)
    {
        return isWhitespace(std::string(1, charToCheck));
    }
    
    std::string getBetween(const std::string &beginning, const std::string &ending, const std::string &findString)
    {
        size_t foundPosition{0};
        size_t foundEndPosition{0};
        if (beginning == ending) {
            size_t tempFound{findString.find(beginning)};
            foundPosition = findString.find(beginning);
            foundEndPosition = foundPosition + (findString.substr(tempFound+1).find(ending));
        } else {    
            foundPosition = findString.find(beginning);
            foundEndPosition = findString.find(ending);
        }
        if (foundPosition == std::string::npos) {
            if (ending.length() == 0) {
                return findString;
            } else {
                return "";
            }
        } else if (foundEndPosition == std::string::npos) {
            if (ending.length() == 0) {
                return findString.substr(foundPosition+1);
            } else {
                return "";
            }
        } else {
            return findString.substr(foundPosition+1, foundEndPosition-foundPosition-1);
        }
    }

    std::vector<std::string> getAllBetween(const std::string &beginning, const std::string &ending, const std::string &findString)
    {
        std::vector<std::string> returnVector{};
        std::string copyString{findString};
        do {
            std::string gotString{getBetween(beginning, ending, copyString)};
            if (gotString.length() != 0) {
                returnVector.push_back(gotString);
            }
            copyString = copyString.substr(copyString.find(ending)+1);
        } while (copyString != "");
        return returnVector;
    }

    std::string stripBetween(const std::string &beginning, const std::string &ending, const std::string &findString)
    {
        std::string removeString{getBetween(beginning, ending, findString)};
        if (removeString == "") {
            return "";
        }
        size_t foundPosition{findString.find(removeString)};
        long long unsigned int offsetPosition{foundPosition + (removeString.length())};
        return (findString.substr(0, foundPosition) + findString.substr(offsetPosition));
    }

    std::string stripAllBetween(const std::string &beginning, const std::string &ending, const std::string &findString)
    {
        std::string returnString{""};
        std::string copyString{findString};
        while (getBetween(beginning, ending, copyString).length() != 0) {
            std::string gotString{beginning + getBetween(beginning, ending, copyString) + ending};
            returnString += (beginning + ending);
            copyString = stripFromString(copyString, gotString);
        }
        return returnString;
    }
    
    std::string stripFromString(const std::string &stringToStrip, const std::string &whatToStrip)
    {
        std::string returnString{stringToStrip};
        if (returnString.find(whatToStrip) == std::string::npos) {
            return returnString;
        }
        size_t foundPosition{stringToStrip.find(whatToStrip)};
        if (foundPosition == 0) {
            returnString = returnString.substr(whatToStrip.length());
        } else if (foundPosition == (returnString.length() - whatToStrip.length())) {
            returnString = returnString.substr(0, foundPosition);
        } else {
            returnString = returnString.substr(0, foundPosition) + returnString.substr(foundPosition+whatToStrip.length());
        }
        return returnString;
    }

    std::string stripFromString(const std::string &stringToStrip, char whatToStrip)
    {
        return stripFromString(stringToStrip, std::string(1, whatToStrip));
    }

    std::string stripAllFromString(const std::string &stringToStrip, const std::string &whatToStrip)
    {
        std::string returnString = stringToStrip;
        if (returnString.find(whatToStrip) == std::string::npos) {
            return returnString;
        }
        while (returnString.find(whatToStrip) != std::string::npos) {
            returnString = stripFromString(returnString, whatToStrip);
        }
        return returnString;
    }

    std::string stripAllFromString(const std::string &stringToStrip, char whatToStrip)
    {
        return stripAllFromString(stringToStrip, std::string(1, whatToStrip));
    }

    std::string tWhitespace(int howMuch) 
    {
        std::string returnString{""};
        for (int i = 0; i < howMuch; i++) {
            returnString += " ";
        }
        return returnString;
    }

    std::string toFixedWidth(const std::string &inputString, int fixedWidth)
    {
        std::string copyString{inputString};
        size_t copyLength{copyString.length()};
        if (static_cast<int>(copyLength) < fixedWidth) {
            copyLength = (fixedWidth - copyLength);
            while (copyLength--) {
                copyString = "0" + copyString;
            }
        }
        return copyString;
    }

    std::string toFixedWidth(const char *inputString, int fixedWidth)
    {
        return toFixedWidth(static_cast<std::string>(inputString), fixedWidth);
    }

    int charToInt(char charToConvert)
    {
        switch(charToConvert) {
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        default: return 0;
        }
    }

    char intToChar (int intToConvert)
    {
        switch(intToConvert) {
        case 0: return '0';
        case 1: return '1';
        case 2: return '2';
        case 3: return '3';
        case 4: return '4';
        case 5: return '5';
        case 6: return '6';
        case 7: return '7';
        case 8: return '8';
        case 9: return '9';
        default: return '0';
        }
    }

    void delayHours(unsigned long long howLong)
    {
        std::this_thread::sleep_for(std::chrono::hours(howLong));
    }

    void delayMinutes(unsigned long long howLong)
    {
        std::this_thread::sleep_for(std::chrono::minutes(howLong));
    }

    void delaySeconds(unsigned long long howLong)
    {
        std::this_thread::sleep_for(std::chrono::seconds(howLong));
    }

    void delayMilliseconds(unsigned long long howLong)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(howLong));
    }

    void delayMicroseconds(unsigned long long howLong)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(howLong));
    }

    void delayNanoseconds(unsigned long long howLong)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(howLong));
    }

    std::pair<std::string, std::string> splitFileName(const std::string &fullPath)
    {
        std::string filePath{""};
        std::string fileName{""};
        std::pair<std::string, std::string> returnPair;
        std::size_t lastFoundSlash{fullPath.find_last_of("/\\")};
        if (lastFoundSlash != std::string::npos) {
            filePath = fullPath.substr(0, lastFoundSlash);
            fileName = fullPath.substr(lastFoundSlash+1);
            returnPair = {filePath, fileName};
            return returnPair;
        }
        else {
            returnPair = {"", ""};
            return returnPair;

        }
    }

    bool isDigit(char charToCheck)
    {
        return ((charToCheck == '0') || (charToCheck == '1') || (charToCheck == '2') || (charToCheck == '3') || (charToCheck == '4') || (charToCheck == '5') || (charToCheck == '6') || (charToCheck == '7') || (charToCheck == '8') || (charToCheck == '9'));
    }

    unsigned int hexStringToUInt(const std::string &str)
    {
        std::string copyString{str};
        if (startsWith(copyString, "0x")) {
            copyString = copyString.substr(2);
        }
        unsigned int ret{0};   
        std::stringstream ss;
        ss << std::hex << copyString;
        ss >> ret;
        return ret;
    }

    unsigned char hexStringToUChar(const std::string &str)
    {
        std::string copyString{str};
        if (startsWith(copyString, "0x")) {
            copyString = copyString.substr(2);
        }
        unsigned int ret{0};   
        std::stringstream ss;
        ss << std::hex << copyString;
        ss >> ret;
        return ret;
    }

    bool endsWith(const std::string &stringToCheck, const std::string &matchString)
    {
        if (matchString.size() > stringToCheck.size()) {
            return false;
        }
        return std::equal(matchString.rbegin(), matchString.rend(), stringToCheck.rbegin());
    }

    bool endsWith(const std::string &stringToCheck, char matchChar)
    {
        return endsWith(stringToCheck, std::string(1, matchChar));
    }

    bool endsWithNotIncludingWhitespace(const std::string &stringToCheck, const std::string &matchString)
    {
        (void)stringToCheck;
        (void)matchString;
        return false;
    }


    bool endsWithNotIncludingWhitespace(const std::string &stringToCheck, char matchChar)
    {
        return endsWithNotIncludingWhitespace(stringToCheck, std::string(1, matchChar));
    }

    bool startsWith(const std::string &stringToCheck, const std::string &matchString)
    {
       return (stringToCheck.find(matchString) == 0);
    }

    bool startsWith(const std::string &stringToCheck, char matchChar)
    {
       return startsWith(stringToCheck, std::string{1, matchChar});
    } 

    bool startsWith(const std::string &str, const char *compare)
    {
        return startsWith(str, static_cast<std::string>(compare));
    }

    bool startsWithNotIncludingWhitespace(const std::string &stringToCheck, const std::string &matchString)
    {
        (void)stringToCheck;
        (void)matchString;
        return false;
    }

    bool startsWithNotIncludingWhitespace(const std::string &stringToCheck, char matchChar)
    {
        return startsWithNotIncludingWhitespace(stringToCheck, std::string{1, matchChar});
    }

    bool startsWithNotIncludingWhitespace(const std::string &stringToCheck, const char *compare)
    {
        return startsWithNotIncludingWhitespace(stringToCheck, static_cast<std::string>(compare));
    }

    /*Logging simple strings to stdout or to a file. On file opening failure, the string will be output to stdout*/
    void logString(const std::string &stringToLog) { std::cout << stringToLog << std::endl; }
    void logString(const std::string &stringToLog, std::string fileName)
    {
        std::ofstream writeToFile;
        writeToFile.open(stringToLog);
        if (writeToFile.is_open()) {
            writeToFile << stringToLog << std::endl;
            writeToFile.close();
        }
        else {
            std::cout << "Error opening file " << tQuoted(fileName) << " for writing, writing to stdout instead:" << std::endl;
            std::cout << "    " << stringToLog << std::endl;
        }
    }

    std::string toString(const std::string &convert)
    {
        return convert;
    }

    std::string toString(const char *convert)
    {
        return static_cast<std::string>(convert);
    }

    std::string tQuoted(const std::string &convert)
    {
        return ("\"" + convert + "\"");
    }

    std::string tQuoted(const char *convert)
    {
        return ("\"" + static_cast<std::string>(convert) + "\"");
    }
    std::string tEndl()
    {
        #if (defined(_WIN32) || defined(_MSC_VER))
            return "\r\n";
        #else
            return "\n";
        #endif
    }

    std::string tParenthesis(const std::string &convert)
    {
        return ("(" + convert + ")");
    }

    std::string tParenthesis(const char *convert)
    {
        return ("(" + static_cast<std::string>(convert) + ")");
    }
}
