#ifndef CPPSERIALPORT_BYTEARRAY_H
#define CPPSERIALPORT_BYTEARRAY_H

#include <string>
#include <vector>
#include <algorithm>

namespace CppSerialPort {

class ByteArray {
public:
    explicit ByteArray() = default;
    explicit ByteArray(char c);
    explicit ByteArray(const char *cStr);
    explicit ByteArray(const std::string &str);
    explicit ByteArray(char *buffer, size_t length);
    explicit ByteArray(const std::vector<char> &byteArray);
    ByteArray &operator=(const ByteArray &rhs);
    ByteArray &operator=(const std::vector<char> &rhs);
    ByteArray &operator=(const std::string &rhs);
    ByteArray &operator=(char c);
    ByteArray &operator=(ByteArray &&rhs);
    ByteArray &operator=(std::vector<char> &&rhs);
    ByteArray(const ByteArray &other) = default;
    ByteArray(ByteArray &&other) = default;
    template <typename InputIterator> explicit ByteArray(InputIterator begin, InputIterator end) :
            m_buffer{begin, end} { }

    ByteArray &append(char c);
    ByteArray &append(const ByteArray &rhs);
    ByteArray &append(const std::string &rhs);
    ByteArray &append(const std::vector<char> &rhs);
    ByteArray &operator+=(char c);
    ByteArray &operator+=(const ByteArray &rhs);
    ByteArray &operator+=(const std::string &rhs);
    ByteArray &operator+=(const std::vector<char> &rhs);
    ByteArray &operator+(char c);
    ByteArray &operator+(const ByteArray &rhs);
    ByteArray &operator+(const std::string &rhs);
    ByteArray &operator+(const std::vector<char> &rhs);

    size_t find(const ByteArray &toFind);
    size_t find(char c);

    friend ByteArray operator+(char c, const ByteArray &rhs);
    friend ByteArray operator+(const ByteArray &lhs, const ByteArray &rhs);
    friend ByteArray operator+(const std::string &lhs, const ByteArray &rhs);
    friend ByteArray operator+(const std::vector<char> &lhs, const ByteArray &rhs);
    friend ByteArray operator+(const ByteArray &lhs, char c);
    friend ByteArray operator+(const ByteArray &lhs, const std::string &rhs);
    friend ByteArray operator+(const ByteArray &lhs, const std::vector<char> &rhs);

    std::vector<char>::iterator begin();
    const std::vector<char>::const_iterator cbegin() const;
    std::vector<char>::reverse_iterator rbegin();
    const std::vector<char>::const_reverse_iterator crbegin() const;
    std::vector<char>::iterator end();
    const std::vector<char>::const_iterator cend() const;
    std::vector<char>::reverse_iterator rend();
    const std::vector<char>::const_reverse_iterator crend() const;

    ByteArray &clear();
    size_t size() const;
    size_t length() const;
    bool empty() const;
    const char *data() const;
    char *data();

    ByteArray &popBack();
    ByteArray &popFront();

    char &operator[](size_t index);
    const char &operator[](size_t index) const;
    char &at(size_t index);
    const char &at(size_t index) const;

    ByteArray subsequence(size_t index, size_t length = 0) const;

    friend bool operator==(const ByteArray &lhs, const ByteArray &rhs) { return std::equal(lhs.m_buffer.begin(), lhs.m_buffer.end(), rhs.m_buffer.begin()); }
    explicit operator std::string() const;
    std::string toString() const;

    bool endsWith(char *buffer, size_t length) const;
    bool endsWith(const char *cStr) const;
    bool endsWith(const std::string &str) const;
    bool endsWith(const ByteArray &byteArray) const;

    bool startsWith(const ByteArray &byteArray) const;
    bool startsWith(char *buffer, size_t length) const;
    bool startsWith(const char *cStr) const;
    bool startsWith(const std::string &str) const;
private:
    std::vector<char> m_buffer;

    bool endsWith(const std::vector<char> &ending) const;
    bool startsWith(const std::vector<char> &start) const;
};

}


#endif //CPPSERIALPORT_BYTEARRAY_H
