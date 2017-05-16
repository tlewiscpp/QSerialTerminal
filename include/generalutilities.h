/***********************************************************************
*    generalutilities.h:                                               *
*    Namespace GeneralUtilities, for general use functions             *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.com/tlewiscpp/tjlutils                            *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of a GeneralUtilities namespace  *
*    The namespace contains several general use functions, including   *
*    STL algorithm fuinctions, delay functions, char to int functions, *
*    logging functions, and mathematical rounding functions            *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/


#ifndef TJLUTILS_GENERALUTILITIES_H
#define TJLUTILS_GENERALUTILITIES_H

#if defined(_WIN32) && defined(_MSC_VER)
    #if _MSC_VER < 1700
        #error This library needs at least a C++11 compliant compiler
    #endif
#else
    #if __cplusplus <= 199711L
        #error This library needs at least a C++11 compliant compiler
    #endif
#endif

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <random>
#include <cstdlib>
#include <memory>
#include <chrono>
#include <map>
#include <thread>
#include <algorithm>
#include <unordered_map>
#include <exception>
#include <iterator>
#include <functional>
#include <typeinfo>
#include <list>

#if defined(__ANDROID__)
namespace std {
    /*
    int stoi(const std::string &str)
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

    double stof(const std::string &str)
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

    double stod(const std::string &str)
    {
        return stof(str);
    }

    long int stol(const std::string &str)
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

    long long int stoll(const std::string &str)
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
    */
}
#endif


    namespace GeneralUtilities 
    {
        const size_t generalnpos{std::string::npos};
        const int constexpr ASCII_WHITESPACE_MAXIMUM_VALUE{32};
        const unsigned int constexpr ASCII_HIGH_LIMIT{128};
        const unsigned int constexpr ASCII_LOW_LIMIT{0};
        const long long int constexpr MERSENNE_TWISTER_DISCARD_THRESHOLD{700000};
        const long long int constexpr NANOSECONDS_PER_MICROSECOND{1000};
        const long long int constexpr NANOSECONDS_PER_MILLISECOND{1000000};
        const long long int constexpr NANOSECONDS_PER_SECOND{1000000000};
        const long long int constexpr NANOSECONDS_PER_MINUTE{60000000000};
        const long long int constexpr NANOSECONDS_PER_HOUR{3600000000000};
        const long long int constexpr NANOSECONDS_PER_DAY{86400000000000};
        const long long int constexpr MICROSECONDS_PER_MILLISECOND{1000};
        const long long int constexpr MICROSECONDS_PER_SECOND{1000000};
        const long long int constexpr MICROSECONDS_PER_MINUTE{60000000};
        const long long int constexpr MICROSECONDS_PER_HOUR{3600000000};
        const long long int constexpr MICROSECONDS_PER_DAY{86400000000};
        const long long int constexpr MILLISECONDS_PER_SECOND{1000};
        const long long int constexpr MILLISECONDS_PER_MINUTE{60000};
        const long long int constexpr MILLISECONDS_PER_HOUR{3600000};
        const long long int constexpr MILLISECONDS_PER_DAY{86400000};
        const long long int constexpr SECONDS_PER_MINUTE{60};
        const long long int constexpr SECONDS_PER_HOUR{3600};
        const long long int constexpr MINUTES_PER_HOUR{60};
        const long long int constexpr MINUTES_PER_DAY{1440};
        const long long int constexpr HOURS_PER_DAY{24};

        template<typename Container>
        bool isSwitch(const std::string &switchToCheck, const Container &switches) {
            std::string copyString{switchToCheck};
            std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
            for (auto &it : switches) {
                if ((copyString == static_cast<std::string>(it)) &&
                    (copyString.length() == static_cast<std::string>(it).length()) &&
                    (copyString.find(static_cast<std::string>(it)) == 0)) {
                    return true;
                }
            }
            return false;
        }

        template<typename Container>
        bool isSwitch(const char *switchToCheck, const Container &switches) {
            return isSwitch(static_cast<std::string>(switchToCheck), switches);
        }

        template<typename Container>
        bool isEqualsSwitch(const std::string &switchToCheck, const Container &switches) {
            std::string copyString{switchToCheck};
            std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
            for (auto &it : switches) {
                std::string copySwitch{static_cast<std::string>(it) + "="};
                if ((copyString.find(static_cast<std::string>(it) + "=") == 0)) {
                    return true;
                }
            }
            return false;
        }

        template<typename Container>
        bool isEqualsSwitch(const char *switchToCheck, const Container &switches) {
            return isEqualsSwitch(static_cast<std::string>(switchToCheck), switches);
        }

        bool isNonAsciiChar(char c);

        std::string stripNonAsciiCharacters(const std::string &str);

        std::string stripLineEndings(const std::string &str);

        std::string toLowercase(const std::string &str);

        std::string toUppercase(const std::string &str);

        std::string trimWhitespaceFromBeginning(const std::string &str);

        std::string trimWhitespaceFromEnd(const std::string &str);

        std::string trimWhitespace(const std::string &str);

        std::vector<std::string> trimWhitespaceFromBeginning(const std::vector<std::string> &vec);

        std::vector<std::string> trimWhitespaceFromEnd(const std::vector<std::string> &vec);

        std::vector<std::string> trimWhitespace(const std::vector<std::string> &vec);

        bool isWhitespace(const std::string &stringToCheck);

        bool isWhitespace(char charToCheck);

        std::string getBetween(const std::string &beginning, const std::string &ending,
                               const std::string &findString);

        std::vector<std::string> getAllBetween(const std::string &beginning,
                                               const std::string &ending,
                                               const std::string &findString);

        std::string stripBetween(const std::string &beginning, const std::string &ending,
                                 const std::string &findString);

        std::string stripAllBetween(const std::string &beginning, const std::string &ending,
                                    const std::string &findString);

        std::string stripFromString(const std::string &stringToStrip,
                                    const std::string &whatToStrip);

        std::string stripFromString(const std::string &stringToStrip, char whatToStrip);

        std::string stripAllFromString(const std::string &stringToStrip,
                                       const std::string &whatToStrip);

        std::string stripAllFromString(const std::string &stringToStrip, char whatToStrip);

        std::string tWhitespace(int howMuch);

        int charToInt(char charToConvert);

        char intToChar(int intToConvert);

        std::pair<std::string, std::string> splitFileName(const std::string &fullPath);

        void delayHours(unsigned long long howLong);
        void delayMinutes(unsigned long long howLong);
        void delaySeconds(unsigned long long howLong);
        void delayMilliseconds(unsigned long long howLong);
        void delayMicroseconds(unsigned long long howLong);
        void delayNanoseconds(unsigned long long howLong);
 
        bool isDigit(char charToCheck);

        bool endsWith(const std::string &stringToCheck, const std::string &matchString);

        bool endsWith(const std::string &stringToCheck, char matchChar);

        bool endsWithNotIncludingWhitespace(const std::string &stringToCheck,
                                            const std::string &matchString);

        bool endsWithNotIncludingWhitespace(const std::string &stringToCheck, char matchChar);

        bool startsWith(const std::string &stringToCheck, const std::string &matchString);

        bool startsWith(const std::string &stringToCheck, char matchChar);

        bool startsWith(const std::string &str, const char *compare);

        bool startsWithNotIncludingWhitespace(const std::string &stringToCheck,
                                              const std::string &matchString);

        bool startsWithNotIncludingWhitespace(const std::string &stringToCheck, char matchChar);

        bool startsWithNotIncludingWhitespace(const std::string &str, const char *compare);

        void logString(const std::string &stringToLog);

        void logString(const std::string &stringToLog, std::string fileName);

        std::string toString(const std::string &convert);

        std::string toString(const char *convert);

        std::string tQuoted(const std::string &convert);

        std::string tQuoted(const char *convert);

        unsigned int hexStringToUInt(const std::string &str);

        unsigned char hexStringToUChar(const std::string &str);

        std::string toFixedWidth(const std::string &inputString, int fixedWidth);

        std::string toFixedWidth(const char *inputString, int fixedWidth);

        std::string toBinaryString(int number);

        std::string toDecString(int number);

        std::string toHexString(int number);

        int decStringToInt(const std::string &str);

        long decStringToLong(const std::string &str);

        long long int decStringToLongLong(const std::string &str);

        float decStringToFloat(const std::string &str);

        double decStringToDouble(const std::string &str);

        std::string toBinaryString(uint32_t number);

        std::string toDecString(uint32_t number);

        std::string toHexString(uint32_t number);

        std::string toBinaryString(uint8_t number);

        std::string toDecString(uint8_t number);

        std::string toHexString(uint8_t number);

        std::string toBinaryString(bool number);

        std::string toDecString(bool number);

        std::string toHexString(bool number);

        std::string tEndl();

        std::string tParenthesis(const std::string &convert);

        std::string tParenthesis(const char *convert);

        template<typename Function, typename...FunctionArgs>
        void delaySecondsWithCallback(int howLong, const Function &function,
                                      FunctionArgs... functionArgs) {
            howLong = (howLong >= 0 ? howLong : -1 * howLong);
            auto microsecondsToDelay = (howLong * MICROSECONDS_PER_SECOND);
            auto startTime = std::chrono::high_resolution_clock::now();
            auto endTime = std::chrono::high_resolution_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(
                    endTime - startTime).count();
            do {
                (void) function(functionArgs...);
                endTime = std::chrono::high_resolution_clock::now();
                elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(
                        endTime - startTime).count();
            } while (elapsedTime <= microsecondsToDelay);
        }

        template<typename Function, typename...FunctionArgs>
        void delayMillisecondsWithCallback(int howLong, const Function &function,
                                           FunctionArgs... functionArgs) {
            howLong = (howLong >= 0 ? howLong : -1 * howLong);
            auto microsecondsToDelay = (howLong * MICROSECONDS_PER_SECOND);
            auto startTime = std::chrono::high_resolution_clock::now();
            auto endTime = std::chrono::high_resolution_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(
                    endTime - startTime).count();
            do {
                (void) function(functionArgs...);
                endTime = std::chrono::high_resolution_clock::now();
                elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(
                        endTime - startTime).count();
            } while (elapsedTime <= microsecondsToDelay);
        }

        template<typename Function, typename...FunctionArgs>
        void delayMicrosecondsWithCallback(int howLong, const Function &function,
                                           FunctionArgs... functionArgs) {
            howLong = (howLong >= 0 ? howLong : -1 * howLong);
            auto microsecondsToDelay = (howLong * MICROSECONDS_PER_SECOND);
            auto startTime = std::chrono::high_resolution_clock::now();
            auto endTime = std::chrono::high_resolution_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(
                    endTime - startTime).count();
            do {
                (void) function(functionArgs...);
                endTime = std::chrono::high_resolution_clock::now();
                elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(
                        endTime - startTime).count();
            } while (elapsedTime <= microsecondsToDelay);
        }


        template<typename Function>
        std::string stripFromStringIf(const std::string &stringToStrip, Function function) {
            bool substitutionMade{false};
            std::string returnString{""};
            for (auto iter = stringToStrip.begin(); iter != stringToStrip.end(); iter++) {
                if (function(*iter) && !substitutionMade) {
                    substitutionMade = true;
                    continue;
                }
                returnString += *iter;
            }
            return returnString;
        }

        template<typename Function>
        std::string stripAllFromStringIf(const std::string &stringToStrip, Function function) {
            std::string returnString{""};
            for (auto iter = stringToStrip.begin(); iter != stringToStrip.end(); iter++) {
                if (!function(*iter)) {
                    returnString += *iter;
                }
            }
            return returnString;
        }

        /*Log a numerical value. On file opening failure, the value is output to stdout*/
        template<typename T>
        void logDataMember(const T &valueToLog, const std::string &nameOfVariable) {
            std::cout << nameOfVariable << " = " << valueToLog << std::endl;
        }

        template<typename T>
        void logDataMember(const T &valueToLog, const std::string &nameOfVariable,
                           const std::string &fileName) {
            std::ofstream writeToFile;
            writeToFile.open(fileName, std::ios::app);
            if (writeToFile.is_open()) {
                writeToFile << nameOfVariable << " = " << valueToLog << std::endl;
                writeToFile.close();
            }
            else {
                std::cout << "Error opening file " << tQuoted(fileName) <<
                " for writing, writing to stdout instead:" << std::endl;
                std::cout << nameOfVariable << " = " << valueToLog << std::endl;
            }
        }

        template<typename T>
        std::string toString(const T &convert) {
            std::string returnString{""};
            std::stringstream transfer;
            transfer << convert;
            transfer >> returnString;
            return returnString;
        }

        template<typename T>
        bool vectorContains(const std::vector<T> &vectorToCheck, const T &compareObject) {
            for (typename std::vector<T>::const_iterator iter = vectorToCheck.begin();
                 iter != vectorToCheck.end(); iter++) {
                if ((*iter) == (compareObject)) {
                    return true;
                }
            }
            return false;
        }

        template<typename TKey, typename TValue>
        bool unorderedMapContainsKey(const std::unordered_map<TKey, TValue> &tMap,
                                     const TKey &tKey) {
            return (tMap.find(tKey) != tMap.end());
        }

        template<typename TKey, typename TValue>
        bool mapContainsKey(const std::map<TKey, TValue> &tMap, const TKey &tKey) {
            return (tMap.find(tKey) != tMap.end());
        }

        template<typename T>
        struct isCallableImplicit {
        private:
            typedef char(&yes)[1];
            typedef char(&no)[2];

            struct Fallback {
                void operator()();
            };

            struct Derived : T, Fallback {
            };

            template<typename U, U>
            struct Check;

            template<typename>
            yes test(...);

            template<typename C>
            no test(Check<void (Fallback::*)(), &C::operator()> *);

        public:
            const bool value = sizeof(test<Derived>(0)) == sizeof(yes);
        };

        template<typename T>
        struct isCallable
                : std::conditional<std::is_class<T>::value, isCallableImplicit<T>, std::false_type>::type {
        };

        template<typename BeginningIterator, typename EndingIterator, typename T>
        bool itemExists(BeginningIterator &&bit, EndingIterator &&eit, const T &searchTerm) {
            for (auto iter = bit; iter != eit; iter++) {
                if (*iter == searchTerm) {
                    return true;
                }
            }
            return false;
        }

        template<typename Container, typename T>
        bool itemExists(const Container &container, const T &searchTerm) {
            for (auto iter = std::begin(container); iter != std::end(container); iter++) {
                if (*iter == searchTerm) {
                    return true;
                }
            }
            return false;
        }

        template<typename T>
        bool itemExistsInList(const std::list<T> &container, const T &searchTerm) {
            for (auto &it : container) {
                if (searchTerm == it) {
                    return true;
                }
            }
            return false;
        }

        template<typename BeginningIterator, typename EndingIterator, typename T>
        bool itemPartiallyExists(BeginningIterator &&bit, EndingIterator &&eit,
                                 const T &searchTerm) {
            for (auto iter = bit; iter != eit; iter++) {
                if (iter->find(searchTerm) != generalnpos) {
                    return true;
                }
            }
            return false;
        }

        template<typename Container, typename T>
        bool itemPartiallyExists(const Container &container, const T &searchTerm) {
            for (auto iter = std::begin(container); iter != std::end(container); iter++) {
                if (iter->find(searchTerm) != generalnpos) {
                    return true;
                }
            }
            return false;
        }


        template<typename Container, typename T>
        Container safeEraseExactMatch(const Container &container, const T &searchTerm) {
            Container returnContainer{container};
            while (itemExists(returnContainer, searchTerm)) {
                for (auto iter = std::begin(returnContainer);
                     iter != std::end(returnContainer); iter++) {
                    if (*iter == searchTerm) {
                        returnContainer.erase(iter);
                        break;
                    }
                }
            }
            return returnContainer;
        }

        template<typename Container, typename T>
        Container safeErasePartialMatch(const Container &container, const T &searchTerm) {
            Container returnContainer{container};
            while (itemPartiallyExists(returnContainer, searchTerm)) {
                for (auto iter = std::begin(returnContainer);
                     iter != std::end(returnContainer); iter++) {
                    if (iter->find(searchTerm) != generalnpos) {
                        returnContainer.erase(iter);
                        break;
                    }
                }
            }
            return returnContainer;
        }

        template <typename Container, typename InputIter>
        Container parseToContainer(InputIter first, InputIter last, typename std::remove_reference<decltype(*first)>::type delimiter)
        {
            Container returnContainer;
            InputIter it;
            do {
                it = std::find(first, last, delimiter);
                typename Container::value_type tempContainer;
                std::copy(first, it, std::inserter(tempContainer, tempContainer.end()));
                if (!tempContainer.empty()) {
                    returnContainer.insert(returnContainer.end(), tempContainer);
                }
                first = it+1;
            } while (it != last);
            return returnContainer;
        }

        template <typename Container, typename InputIter>
        Container parseToContainerAdapter(InputIter first, InputIter last, typename std::remove_reference<decltype(*first)>::type delimiter)
        {
            Container returnContainer;
            InputIter it;
            do {
                it = std::find(first, last, delimiter);
                typename Container::value_type tempContainer;
                std::copy(first, it, std::inserter(tempContainer, tempContainer.end()));
                if (!tempContainer.empty()) {
                    returnContainer.push(tempContainer);
                }
                first = it+1;
            } while (it != last);
            return returnContainer;
        }
        
        template<typename T>
        std::string tQuoted(const T &convert) {
            return ("\"" + toString(convert) + "\"");
        }

        template<typename T>
        std::string tParenthesis(const T &convert) {
            return ("(" + toString(convert) + ")");
        }

        template<typename Container, typename T, typename Function>
        void addFormattedThing(Container &container, const T &thingToAdd, Function function) {
            static_assert(std::is_same<typename std::decay<decltype(*(std::begin(
                    container)))>::type, typename std::decay<T>::type>::value,
                          "The object to add must dereference to the same type as the container contents");
            container.push_back(function(thingToAdd));
        }

        template<typename T, typename Base>
        std::string toFixedWidth(const T &toPrint, int fixedWidth, Base base = 0) {
            std::string copyString{toString(toPrint)};
            size_t copyLength{copyString.length()};
            if (static_cast<int>(copyLength) < fixedWidth) {
                copyLength = (fixedWidth - copyLength);
                while (copyLength--) {
                    copyString = toString(base) + copyString;
                }
            }
            return copyString;
        }


        template<typename T, typename TLow, typename THigh>
        T doUserEnterNumericParameter(const std::string &name,
                                      const std::function<bool(T)> &validator,
                                      TLow lowLimit,
                                      THigh highLimit) {
            std::cout << std::endl;
            std::string userOption{""};
            while (true) {
                userOption = "";
                std::cout << "Enter a number to use for " << tQuoted(name) <<
                " between (inclusive) " << lowLimit << " and " << highLimit <<
                ", or press CTRL+C to quit: ";
                std::getline(std::cin, userOption);
                if (userOption == "") {
                    continue;
                }
                int userEntry{0};
                try {
                    userEntry = GeneralUtilities::decStringToInt(userOption);
                    if (userEntry < lowLimit) {
                        std::cout << tQuoted(userEntry) << " is less than the minimum value for " <<
                        name << " (" << name << " < " << lowLimit << std::endl << std::endl;
                        continue;
                    } else if (userEntry > highLimit) {
                        std::cout << tQuoted(userEntry) <<
                        " is greater than the maximum value for " << name << " (" << name <<
                        " > " << highLimit << std::endl << std::endl;
                        continue;
                    } else if (!validator(userEntry)) {
                        std::cout << tQuoted(userEntry) << " is an invalid " << name << std::endl <<
                        std::endl;
                        continue;
                    }
                    return userEntry;
                } catch (std::exception &e) {
                    std::cout << tQuoted(userOption) << " is not a number" << std::endl <<
                    std::endl;
                }
            }
        }

        template<typename T>
        std::string doUserEnterStringParameter(const T &name,
                                               const std::function<bool(std::string)> &validator) {
            std::cout << std::endl;
            std::string userOption{""};
            while (true) {
                userOption = "";
                std::cout << "Please enter a string to use for " << tQuoted(name) <<
                ", or press CTRL+C to quit: ";
                std::getline(std::cin, userOption);
                if (userOption == "") {
                    continue;
                }
                try {
                    if (!validator(userOption)) {
                        std::cout << tQuoted(userOption) << " is an invalid " << name <<
                        std::endl << std::endl;
                        continue;
                    }
                    return userOption;
                } catch (std::exception &e) {
                    std::cout << tQuoted(userOption) << " is an invalid " << name << std::endl <<
                    std::endl;
                }
            }
        }

        template<typename T, typename TOps>
        T doUserSelectParameter(const std::string &name,
                                const std::function<T(const std::string &)> &func,
                                const std::vector<TOps> &availableOptions,
                                const char *defaultOption) {
            std::cout << std::endl;
            if (availableOptions.size() == 0) {
                throw std::runtime_error("No " + name + " are available");
            } else if (availableOptions.size() == 1) {
                return func(availableOptions.at(0));
            }
            unsigned int quitOption{0};
            for (unsigned int selectionIndex = 1;
                 selectionIndex <= availableOptions.size(); selectionIndex++) {
                std::cout << selectionIndex << ".) " << availableOptions.at(selectionIndex - 1);
                if (static_cast<std::string>(availableOptions.at(selectionIndex - 1)) ==
                    static_cast<std::string>(defaultOption)) {
                    std::cout << "    <----DEFAULT" << std::endl;
                } else {
                    std::cout << std::endl;
                }
                quitOption = selectionIndex + 1;
            }
            std::cout << quitOption << ".) Quit" << std::endl << std::endl;
            std::string userOption{""};
            while (true) {
                userOption = "";
                std::cout << "Select a " << name <<
                " from the above options, or press CTRL+C to quit: ";
                std::getline(std::cin, userOption);
                if (userOption == "") {
                    return func(defaultOption);
                }
                unsigned int userOptionIndex{0};
                bool userSelectedQuit{false};
                try {
                    userOptionIndex = GeneralUtilities::decStringToInt(userOption);
                    if (userOptionIndex > availableOptions.size() + 1) {
                        std::cout << tQuoted(userOption) <<
                        " wasn't one of the selections, please a number between (inclusive) 1 and " <<
                        quitOption << ", or press CTRL+C to quit" << std::endl << std::endl;
                        continue;
                    }
                    if (userOptionIndex == quitOption) {
                        userSelectedQuit = true;
                        throw std::invalid_argument(
                                "In GeneralUtilities::doUserSelectParameter(): User selected quit option");
                    }
                    return func(availableOptions.at(userOptionIndex - 1));
                } catch (std::exception &e) {
                    if (userSelectedQuit) {
                        throw e;
                    }
                    std::cout << tQuoted(userOption) <<
                    " wasn't one of the selections, enter a number between (inclusive) 1 and " <<
                    quitOption << ", or press CTRL+C to quit" << std::endl << std::endl;
                }
            }
        }

        template <typename StringType>
        inline constexpr bool isLineEnding(const StringType &stringType)
        {
            return ((toString(stringType) == "\r") || (toString(stringType) == "\n") || (toString(stringType) == "\r\n") || (toString(stringType) == "\n\r"));
        }


    }

#if (__cplusplus < 201402L)
#if ((defined(_MSC_VER) && (_MSC_VER < 1700)) || (!defined(_MSC_VER)) || defined(__ANDROID__))
    namespace std {
        template<typename T, typename ...Args>
        unique_ptr<T> make_unique(Args &&...args) {
            return unique_ptr<T>(new T(forward<Args>(args)...));
        }
    }
#endif
#endif

#if defined(__ANDROID__)
    namespace std {
        template<typename T>
        inline string to_string(const T &convert) { return GeneralUtilities::toString(convert); }

        template<typename T>
        inline string quoted(const T &toQuote) { return GeneralUtilities::tQuoted(toQuote); }
    }

#endif


#endif // TJLUTILS_GENERALUTILITIES_H

