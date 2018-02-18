#ifndef TRMIDOUBLECHECKBOX_PROGRAMOPTION_H
#define TRMIDOUBLECHECKBOX_PROGRAMOPTION_H

#include "ApplicationUtilities.h"

using ApplicationUtilities::TStringFormat;

class ProgramOption {
public:
    inline ProgramOption(char shortOption, const std::string &longOption, int argumentSpecifier, const std::string &description) :
            m_shortOption{'\0'},
            m_longOption{""},
            m_argumentSpecifier{0},
            m_description{""}
    {
        this->setShortOption(shortOption);
        this->setLongOption(longOption);
        this->setArgumentSpecifier(argumentSpecifier);
        this->setDescription(description);
    }

    inline ProgramOption(option gnuOption, const std::string &description) :
            m_shortOption{'\0'},
            m_longOption{""},
            m_argumentSpecifier{0},
            m_description{""}
    {
        this->setShortOption(gnuOption.val);
        this->setLongOption(gnuOption.name);
        this->setArgumentSpecifier(gnuOption.has_arg);
        this->setDescription(description);
    }

    inline int shortOption() const { return this->m_shortOption; }
    inline std::string longOption() const { return this->m_longOption; }
    inline int argumentSpecifier() const { return this->m_argumentSpecifier; }
    inline std::string description() const { return this->m_description; }

    inline void setShortOption(int shortOption) {
        if (shortOption == 0) {
            throw std::runtime_error("ProgramOption::setShortOption(int shortOption): Short option must be a non-zero integer value");
        }
        this->m_shortOption = shortOption;
    }
    inline void setLongOption(const std::string &longOption) {
        if (longOption.length() == 0) {
            throw std::runtime_error("ProgramOption::setLongOption(const std::string &): Long option must be at least 1 characters in length");
        }
        this->m_longOption = longOption;
    }
    inline void setArgumentSpecifier(int argumentSpecifier) {
        if ( (argumentSpecifier != no_argument) && (argumentSpecifier != optional_argument) && (argumentSpecifier != required_argument) ) {
            //WIN32 MINGW workaround
            auto noArgumentCopy = no_argument;
            auto optionalArgumentCopy = optional_argument;
            auto requiredArgumentCopy = required_argument;
            throw std::runtime_error(TStringFormat("ProgramOption::setArgumentSpecifier(int): Argument specifier must be either no_argument, optional_argument, or required_argument ({0}, {1}, {2})", noArgumentCopy, optionalArgumentCopy, requiredArgumentCopy));
        }
        this->m_argumentSpecifier = argumentSpecifier;
    }
    inline void setDescription(const std::string &description) {
        if (description.length() == 0) {
            throw std::runtime_error("ProgramOption::setDescription(const std::string &str): Description must be at least 1 character long");
        }
        this->m_description = description;
    }
    inline option toPosixOption() const {
        if ( (this->m_longOption[0] == '-') && (this->m_longOption[1] == '-') ) {
            return option{this->m_longOption.substr(2).c_str(), this->m_argumentSpecifier, nullptr, this->m_shortOption};
        } else if (this->m_longOption[0] == '-') {
            return option{this->m_longOption.substr(1).c_str(), this->m_argumentSpecifier, nullptr, this->m_shortOption};
        } else {
            return option{this->m_longOption.c_str(), this->m_argumentSpecifier, nullptr, this->m_shortOption};
        }
    };

private:
    int m_shortOption;
    std::string m_longOption;
    int m_argumentSpecifier;
    std::string m_description;
};


#endif //TRMIDOUBLECHECKBOX_PROGRAMOPTION_H
