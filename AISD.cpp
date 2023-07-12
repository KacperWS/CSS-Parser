#include <iostream>
#include <string.h>
#include <cmath>
#include <ctype.h>
#include <stdio.h>
#include <cstdlib>
#include "MyString.h"
#include "Utillities.h"
#include "Commands.h"
#include "Input.h"

using namespace std;

bool ExecuteCommands(List **start) { //Wykonuj komendy
    MyString input;
    while (cin >> input) {
        List* temp = *start;
        if (input == "") { //Wvzytaj nastepna linie jesli ta jest pusta
            cin >> input;
            for (int i = 0; i < input.size()-2; i++) {
                input[i] = input[i + 1];
            }
            input[input.size() - 2] = '\0';
        }
        else if (input == "????") { //jesli znaki wczytywania komend wczytaj komende
            input = ""; cin >> input;
        }
        if (strcmp(input.c_str(), "****") == 0) // Wyjdz jesli wczytywanie sekcji
            return false;
        if (CountChar(input, ',') > 2) { //Sprawdza czy wejscie nie jest popsute :)
            input = ""; continue;
        }
        int len = int(input.size());
        int letterS = SearchForLetter(input, 'S'), letterA = SearchForLetter(input, 'A');
        if (input[0] == '?') {
            cout << input << " == " << NumberOfSections(temp) << endl;
        }
        else if (input[len - 4] == 'S' && input[len - 2] == '?' && isNumber(input, 0, len - 5)) {
            NumberOfSelectorsInSection(temp, input, len);
        }
        else if (input[len - 4] == 'A' && input[len - 2] == '?' && isNumber(input, 0, len - 5)) {
            NumberOfAttributesInSection(temp, input, len);
        }
        else if (letterS != false && input[len - 2] != '?') {
            JSelectorInIBlock(temp, input, len);
        }
        else if (letterA != false && input[len - 2] != '?' && isNumber(input, 0, letterA - 1)) {
            NAttributeForSectionI(temp, input, len);
        }
        else if (letterA != false && input[len - 2] == '?') {
            NumberOfNAttribute(temp, input);
        }
        else if (letterS != false && input[len - 2] == '?') {
            NumberOfSelectorZ(temp, input);
        }
        else if (SearchForLetter(input, 'E') != false) {
            ValueOfNForSelectorZ(temp, input, len);
        }
        else if (SearchForLetter(input, 'D') != false && input[SearchForLetter(input, 'D')+2] == '*') {
            if (DeleteSection(temp, ConvertToNumber(input, 0, SearchForLetter(input, 'D') - 1))) {
                cout << input << " == " << "deleted" << endl; //Resect(start);
            }
        }
        else if (SearchForLetter(input, 'D') != false) {
            MyString n;
            for (int i = SearchForLetter(input, 'D') + 2; i < len - 1; i++) {
                n += input[i];
            }
            n.removeSpecialChars();
            if (DeleteAttribute(temp, ConvertToNumber(input, 0, SearchForLetter(input, 'D') - 1), n)) {
                cout << input << " == " << "deleted" << endl; //Resect(start);
            }
        }
        input = "";
    }
    DeleteList(*start); //Wykona sie jak przyjdzie EOF
    std::exit(0);
    return false;
}

void Commands(MyString& input, List** start) { ///Funkcja prze³¹cza miêdzy wczytywaniem a komendami
    while (strcmp(input.c_str(), "****") == 0) {
        if (WriteSection(start)) //Jeœli CSS kontynuuj inaczej zmieñ na komendy
            input = "****";
        else
            input = "????";
    }
    while (strcmp(input.c_str(), "????") == 0) {
        if (ExecuteCommands(start))
            input = "????";
        else
            input = "****";
    }
}

int main() {

    List* start = new List;
    MyString input = "****"; //Aby program zacz¹³ od czytania CSS
    while (input[0]!=EOF)
    {
        Commands(input, &start);
    }
    DeleteList(start); //Usun wszytskie zmienne dynamiczne

    return 0;
}