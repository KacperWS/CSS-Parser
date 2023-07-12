#pragma once
#include <iostream>
using namespace std;
class MyString {
private:
    char* data;
    size_t length;
    size_t capacity;
public:
    MyString();
    MyString(const char* str);
    MyString(const MyString& other);
    ~MyString();
    size_t size() const;
    const char* c_str() const;
    MyString& operator=(const MyString& other);
    MyString operator+(const MyString& other) const;
    MyString& operator+=(const MyString& other);
    MyString& operator+=(char c);
    char& operator[](size_t index);
    const char& operator[](size_t index) const;
    void push_back(char c);
    void removeSpecialChars();
    void removeLeadLastSpace();
    void DeleteWhite();
    bool operator==(const MyString& other) const;
    bool operator!=(const MyString& other) const;
    bool operator<(const MyString& other) const;
    bool operator>(const MyString& other) const;
    bool operator<=(const MyString& other) const;
    bool operator>=(const MyString& other) const;
    friend ostream& operator<<(ostream& os, const MyString& str);
    friend istream& operator>>(istream& is, MyString& str);
};