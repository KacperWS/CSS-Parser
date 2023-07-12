#pragma once
#include "MyString.h"
#include "Utillities.h"

bool WriteSection(List** start);
void InputAttributes(List* temp);
void AtributeFromLine(List* temp, MyString& input, int len);
void InputSection(List* temp, MyString& input);
void Input(MyString& input, List* start);