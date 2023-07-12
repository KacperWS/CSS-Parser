#pragma once
#include "MyString.h"

#define ELEMENTS 8

struct AtrList {
    MyString attribute;
    MyString value;
    AtrList* next = nullptr;
};

struct SelList {
    MyString selector;
    SelList* next = nullptr;
};

struct MergedList {
    SelList* startS = nullptr;
    int countS = 0;
    AtrList* startA = nullptr;
    int countA = 0;
    bool used = false;
    int number = 0;
};

struct List {
    List* next = nullptr, * prev = nullptr;
    MergedList block[ELEMENTS];
    int count = 0;
    int countFree = 0;
};

void Resect(List** temp);
bool DeleteSection(List* temp, int i);
bool DeleteAttribute(List* temp, int i, MyString& atr);
void DeleteSelList(SelList* start);
void DeleteAtrList(AtrList* start);
void DeleteList(List* temp);
void ExtendList(List* temp);
//MyString DeleteWhite(MyString& input);
int ConvertToNumber(MyString& str, int start, int end);
bool isNumber(MyString& str, int start, int end);
int CountChar(MyString& input, char c);
int SearchForLetter(MyString& str, char letter);
void RecountBlockNumbers(List* temp, int i);
bool OutOfRange(int i);
int ReturnBlock(List** temp, int number);
int FindBlockNumber(List* start);
void RemoveDuplicates(List* temp);
int RemoveDuplicatesAtr(AtrList *startA);
int RemoveDuplicatesSel(SelList *startS);
void ReverseAtr(AtrList** startA);
void ReverseSel(SelList** startS);