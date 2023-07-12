#include "MyString.h"
#include "Utillities.h"

int NumberOfSections(List* temp) { //Zlicza ilosc sekcji
    int count = 0;
    while (temp != nullptr) {
        count += temp->count; //zmienna count informuje ile jest sekcjiw bloku
        temp = temp->next;
    }
    return count;
}

void NumberOfSelectorsInSection(List* temp, MyString& input, int len) { //Liczba selektorow w danej sekcji
    int number = ConvertToNumber(input, 0, len - 5); 
    number = ReturnBlock(&temp, number);
    if (OutOfRange(number))
        return;
    if (!((temp->block[number].used == true && temp->block[number].startA == nullptr) || temp->block[number].used == false)) //Sprawdza czy dany blok nie jest pusty
        cout << input << " == " << temp->block[number].countS << endl;
}

void NumberOfAttributesInSection(List* temp, MyString& input, int len) { //Liczba atrybutow w sekcji
    int number = ConvertToNumber(input, 0, len - 5);
    number = ReturnBlock(&temp, number);
    if (OutOfRange(number))
        return;
    if (!((temp->block[number].used == true && temp->block[number].startA == nullptr) || temp->block[number].used == false)) //Sprawdza czy dany blok nie jest pusty
        cout << input << " == " << temp->block[number].countA << endl;
}

void JSelectorInIBlock(List* temp, MyString& input, int len) { //Wypisuje selektor o numerze J w bloku o numerku I
    int count = 0;
    int number = ConvertToNumber(input, 0, len - 5);
    number = ReturnBlock(&temp, number);
    if (OutOfRange(number))
        return;
    int letterS = SearchForLetter(input, 'S'); //Miejsce litery S w inpucie
    int selectorNumber = ConvertToNumber(input, letterS + 1, len); //Numer selektora do wypisania
    if (!((temp->block[number].used == true && temp->block[number].startA == nullptr) || temp->block[number].countS == 0)) { //Sprawdza czy dany blok nie jest pusty
        SelList* tmp = temp->block[number].startS;
        count++; //Zlicza który to selektor w bloku
        while (tmp != nullptr) {
            if (count == selectorNumber) 
                break;
            else
                count++;
            tmp = tmp->next;
        }
        if (tmp != nullptr) //Sprawdza czy tmp jest nullem czyli czy znalaz³o selektor
            cout << input << " == " << tmp->selector << endl;
    }
}

void NAttributeForSectionI(List* temp, MyString& input, int len) {
    int number = ConvertToNumber(input, 0, len - 5);
    number = ReturnBlock(&temp, number);
    if (OutOfRange(number))
        return;
    if (!(temp->block[number].used == true && temp->block[number].startA == nullptr)) { //Sprawdza czy dany blok nie jest pusty
        AtrList* tmpA = temp->block[number].startA;
        MyString a, value;
        for (int i = SearchForLetter(input, 'A') + 2; i < len; i++) {
            a += input[i]; //Wczytuje atrybut do wyszukania
        }
        a.removeSpecialChars();
        while (tmpA != nullptr) {
            if (tmpA->attribute == a) 
                value = tmpA->value; //Jeœli znaleziono atrybut zbieramy jego wartosc
            tmpA = tmpA->next; //Pêtla leci dalej ¿eby pominac duplikaty
        }
        if (value.size() > 0) //Jeœli nie znaleziono nic nie wypisuj
            cout << input << " == " << value << endl;
    }
}

void NumberOfNAttribute(List* temp, MyString& input) { //Zlicza ³¹czn¹ liczbê selektorów danej nazwy
    MyString a;
    int count = 0;
    for (int i = 0; i < SearchForLetter(input, 'A') - 1; i++) {
        a += input[i]; //Wczytuje nazwe selektora do szukania
    }
    while (temp != nullptr) {
        for (int i = 0; i < ELEMENTS; i++) { //Przelatuje kazdy blok w liscie po kolei
            AtrList* tmpA = temp->block[i].startA;
            if (tmpA != nullptr) {
                do {
                    if (tmpA->attribute == a) {
                        count++;
                        break; //Pomija duplikaty
                    }
                    tmpA = tmpA->next;
                    //if (tmpA == nullptr)
                        //break; //Inaczej warunek ni¿ej mo¿e dostaæ ACCESS VIOLATION
                } while (tmpA != nullptr);
            }
        }
        temp = temp->next;
    }
    cout << input << " == " << count << endl;
}

void NumberOfSelectorZ(List* temp, MyString& input) { //Zlicza liczbe danego selektora globalnie
    MyString a;
    int count = 0;
    for (int i = 0; i < SearchForLetter(input, 'S') - 1; i++) {
        a += input[i]; //Wczytuje nazwe selektora
    }
    a.removeSpecialChars();
    while (temp != nullptr) {
        for (int i = 0; i < ELEMENTS; i++) { //przelatuje kazdy blok po kolei
            SelList* tmpS = temp->block[i].startS;
            if (tmpS != nullptr) {
                while (tmpS->next != nullptr) {
                    if (tmpS->selector == a) {
                        count++;
                        break; //Pomija duplikaty
                    }
                    tmpS = tmpS->next;
                }
                if (tmpS->next == nullptr && tmpS->selector == a) //Warunek jeœli w bloku jest jeden selektor ¿eby go policzylo
                    count++;
            }
        }
        temp = temp->next;
    }
    cout << input << " == " << count << endl;
}

void ValueOfNForSelectorZ(List* temp, MyString& input, int len) { //Wypisuje wartosc atrybutu N dla selektora Z
    MyString z, n, atr;
    for (int i = 0; i < SearchForLetter(input, 'E') - 1; i++) {
        z += input[i]; //Wczytanie selektora
    }
    for (int i = SearchForLetter(input, 'E') + 2; i < len - 1; i++) {
        n += input[i]; //Wczytanie atrubutu
    }
    while (temp != nullptr) {
        for (int i = 0; i < ELEMENTS; i++) { //Kazy blok po kolei przeszukuje
            SelList* tmpS = temp->block[i].startS;
            AtrList* tmpA = temp->block[i].startA;
            if (tmpS != nullptr) {
                do {
                    if (tmpS->selector.size() == 0)
                        break; //Puste sekcje pomija
                    if (tmpS->selector == z) {
                        while (tmpA != nullptr) { //Szuka atrybutu w sekcji z danym selektorem
                            if (tmpA->attribute.size() == 0)
                                break; //Jesli pusty atrybut wyjdz
                            if (tmpA->attribute == n) {
                                atr = tmpA->value;
                                break; //Poprawic bo gubi duplikaty
                            }
                            tmpA = tmpA->next;
                        }
                        break;
                    }
                    tmpS = tmpS->next;
                    if (tmpS == nullptr)
                        break; //Zabezpiecza warunek ni¿ej przed ACCESS VIOLATION
                } while (tmpS->next != nullptr);
            }
        }
        temp = temp->next;
    }
    if (atr.size() > 1) //Jesli znalazlo atrybut wypisz
        cout << input << " == " << atr << endl;
}

/*bool ExecuteCommands(List* temp) {
    MyString input;
    while (cin >> input) {
        if (input == "") {
            cin >> input;
            for (int i = 0; i < input.size() - 2; i++) {
                input[i] = input[i + 1];
            }
            input[input.size() - 2] = '\0';
        }
        else if (input == "????") {
            input = ""; cin >> input;
        }
        if (strcmp(input.c_str(), "****") == 0)
            return false;
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
        else if (SearchForLetter(input, 'D') != false && input[SearchForLetter(input, 'D') + 2] == '*') {
            if (DeleteSection(temp, ConvertToNumber(input, 0, SearchForLetter(input, 'D') - 1)))
                cout << input << " == " << "deleted" << endl;
        }
        else if (SearchForLetter(input, 'D') != false) {
            MyString n;
            for (int i = SearchForLetter(input, 'D') + 2; i < len - 1; i++) {
                n += input[i];
            }
            n.removeSpecialChars();
            if (DeleteAttribute(temp, ConvertToNumber(input, 0, SearchForLetter(input, 'D') - 1), n))
                cout << input << " == " << "deleted" << endl;
        }
        input = "";
    }
    DeleteList(temp);
    std::exit(0);
    return false;
}*/