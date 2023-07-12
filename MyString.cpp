#include <iostream>
#include "MyString.h"

using namespace std;

MyString::MyString(){
   data = NULL;
   length = 0;
   capacity = 0;
}
MyString::MyString(const char* str) {
    length = strlen(str);
    data = new char[length + 1];
    capacity = length + 1;
    strcpy(data, str);
}
MyString::MyString(const MyString& other) {
        length = other.length;
        data = new char[length + 1];
        capacity = length + 1;
        strcpy(data, other.data);
    }
MyString::~MyString() {
        delete[] data;
    }
size_t MyString::size() const {
    return length;
}
const char* MyString::c_str() const {
    return data;
}
MyString& MyString::operator=(const MyString& other) {
    if (this != &other) {
        delete[] data;
        length = other.length;
        capacity = other.capacity;
        data = new char[length + 1];
        strcpy(data, other.data);
    }
    return *this;
}
MyString MyString::operator+(const MyString& other) const {
    MyString result;
    result.length = length + other.length;
    result.capacity = capacity + other.capacity;
    result.data = new char[result.length];
    strcpy(result.data, data);
    strcat(result.data, other.data);
    return result;
}
MyString& MyString::operator+=(const MyString& other) {
    size_t new_length = length + other.length;
    size_t new_capacity = capacity + other.capacity;
    char* new_data = new char[new_length + 1];
    strcpy(new_data, data);
    strcat(new_data, other.data);
    delete[] data;
    data = new_data;
    length = new_length;
    capacity = new_capacity;
    return *this;
}
MyString& MyString::operator+=(char c) {
    char tmp[2] = { c, '\0' };
    MyString temp(tmp);
    if (length == 0) {
        *this = temp;
    }
    else {
        *this += temp;
    }
    return *this;
}
char& MyString::operator[](size_t index) {
    if (index >= length) {
        std::exit(0);
    }
    else {
        return data[index];
    }
}
const char& MyString::operator[](size_t index) const {
    if (index >= length) {
        std::exit(0);
    }
    return data[index];
}
void MyString::push_back(char c) { //Wczytuje znak do napisu
    if (length + 1 > capacity) {
        size_t new_capacity = (capacity == 0 ? 1 : capacity * 2);
        char* new_data = new char[new_capacity];
        if (data != nullptr) {
            copy(data, data + length, new_data);
            delete[] data;
        }
        data = new_data;
        capacity = new_capacity;
    }
    data[length++] = c;
}
void MyString::removeSpecialChars() { //Usuwa znaki specjalne z napisu
    int i = 0;
    int j = 0;
    while (data[i]) {
        if (data[i] >= 32) {
            data[j++] = data[i];
        }
        i++;
    }
    data[j] = '\0';
    length = j;
    capacity = j + 1;
}
void MyString::removeLeadLastSpace() { //Usuwa koncowe i wiodace spacje z napisu
    size_t firstNonSpace = 0;
    while (data[firstNonSpace] != '\0' && isspace(data[firstNonSpace])) {
        firstNonSpace++;
    }
    if (firstNonSpace > 0) {
        size_t len = length - firstNonSpace;
        memmove(data, data + firstNonSpace, len);
        length = len;
        data[len] = '\0';
    }
    if (data[length - 1] == 32) {
        memmove(data, data, length + 1);
        length = length - 1;
        data[length] = '\0';
    }
}
void MyString::DeleteWhite() { //Usuwa wszystkie spacje z napisu
    int count = 0;
    for (int i = 0; i < length - 1; i++) {
        if (data[i] == ' ')
            count++;
    }
    for (int i = 0; i < count + 1; i++) {
        if (i != count) {
            for (int j = 0; j < length - 1; j++) {
                if (data[j] == ' ') {
                    data[j] = data[j + 1];
                    data[j + 1] = ' ';
                }
            }
        }
    }
}
bool MyString::operator==(const MyString& other) const {
    return strcmp(data, other.data) == 0;
}
bool MyString::operator!=(const MyString& other) const {
    return !(*this == other);
}
bool MyString::operator<(const MyString& other) const {
    return strcmp(data, other.data) < 0;
}
bool MyString::operator>(const MyString& other) const {
    return strcmp(data, other.data) > 0;
}
bool MyString::operator<=(const MyString& other) const {
    return strcmp(data, other.data) <= 0;
}
bool MyString::operator>=(const MyString& other) const {
    return strcmp(data, other.data) >= 0;
}
ostream& operator<<(std::ostream& os, const MyString& str) {
    os << (str.data != nullptr ? str.data : "");
    return os;
}
istream& operator>>(istream& is, MyString& str) {
    char c = is.get();
    while (c != '\n' && !is.eof()) {
        str.push_back(c);
        c = is.get();
    }
    str.push_back('\0');
    return is;
}