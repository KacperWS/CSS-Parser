#include "MyString.h"
#include "Utillities.h"

void InputAttributes(List* temp) { //Wczytuje atrybuty
    MyString input;
    cin >> input;
    if (input[0] == '{') { //Jesli poczatek bloku wczytaj atrybut
        input = "";
        cin >> input;
    }
    input.removeSpecialChars();
    temp->block[temp->countFree].startA = new AtrList;
    AtrList* AtrTemp = temp->block[temp->countFree].startA;
    while (input[0] != '}') { //Dopoki nie jest koniec bloku atrybutow
        MyString tmp;
        int count = 0;
        for (int i = 0; i < input.size() - 1; i++) {
            if (input[i] != ':') {
                tmp += input[i]; //wczytanie nazwy atrybutu
                count++;
            }
            else
                break;
        }
        tmp.removeSpecialChars(); tmp.removeLeadLastSpace();
        AtrTemp->attribute = tmp; //dodac atrybut
        tmp = "";
        for (++count; count < input.size() - 1; count++) { //Petla zaczyna sie tam gdzie jest wartosc atrybutu
            if (input[count] != ';')
                tmp += input[count]; //wczytanie wartosci
            else
                break;
        }
        tmp.removeSpecialChars(); tmp.removeLeadLastSpace();
        AtrTemp->value = tmp; //dodac wartos atrybutu
        input = "";
        temp->block[temp->countFree].countA++;
        cin >> input;
        input.removeSpecialChars();
        if (input[0] != '}') {
            AtrTemp->next = new AtrList;
            AtrTemp = AtrTemp->next;
        }
    }
    AtrTemp = temp->block[temp->countFree].startA; //Cofa sie na poczatek listy zeby wskaznik nie zaginal
    RemoveDuplicates(temp);
}

void AtributeFromLine(List* temp, MyString& input, int len) { //Specjalna funkcja dla atrybutow ktore sa w tej samej lini co selektory
    temp->block[temp->countFree].startA = new AtrList;
    AtrList* AtrTemp = temp->block[temp->countFree].startA;
    MyString tmp;
    int count = 0;
    for (int i = len; i < input.size() - 1; i++) {
        if (input[i] != ':') {
            tmp += input[i];
            count++;
        }
        else
            break;
    }
    tmp.removeSpecialChars();
    AtrTemp->attribute = tmp;
    //dodac atrybut
    tmp = ""; count += len;
    for (++count; count < input.size() - 1; count++) {
        if (input[count] != ';')
            tmp += input[count];
        else
            break;
    }
    tmp.removeSpecialChars(); tmp.removeLeadLastSpace();
    AtrTemp->value = tmp;
    temp->block[temp->countFree].countA++;
}

void InputSection(List* temp, MyString& input) { //Wczytuje sekcje
    if (input[0] == '{') //Globalna sekcja
        InputAttributes(temp);
    else {
        temp->block[temp->countFree].startS = new SelList;
        int countTemp = 0; SelList* SelTemp = temp->block[temp->countFree].startS;
        while (true) { 
            MyString tmp;
            int count = 0;
            for (int i = 0; i < input.size(); i++) {
                if (input[i] == ',' || input[i] == '{' || input[i] == '\0') { //Sprawdza czy to koniec nazwy selektora
                    for (int j = count; j < i; j++) {
                        tmp += input[j]; //wczytuje selektor
                        count++;
                    }
                    count++;
                    tmp.removeSpecialChars(); tmp.removeLeadLastSpace();
                    SelTemp->selector = tmp;
                    temp->block[temp->countFree].countS++;
                    tmp = "";
                    if ((input[i+1] != '{' && input[i+1] != '\0') && input.size()+1>i) { //Tworzy kolejny element listy selektorow jesli sa dalej selektory
                        SelTemp->next = new SelList;
                        SelTemp = SelTemp->next;
                    }
                    else if (input[i + 1] == '{' || input[i + 1] == '\0') {
                        count = i + 1;
                        break;
                    }
                }
            }
            //cout << input << " 1 " << input[input.size() - 2] << " "<<input.size() << " "<<count << endl;
            countTemp = count;
            if (input[input.size() - 2] != '{') {
                SelTemp->next = new SelList;
                SelTemp = SelTemp->next;
                input = ""; cin >> input;
                input.removeSpecialChars(); input.push_back('\0');
            }
            else break;
        }
        SelTemp = temp->block[temp->countFree].startS;
        if (countTemp + 2 < input.size()) //Jesli selektory sa w jednej lini z atrybutami
            AtributeFromLine(temp, input, countTemp);
        else
            InputAttributes(temp); //Wczytaj atrybuty
    }
}

void Input(MyString& input, List* start) { //UStawia liste na najblizszy wolny blok i wczytuje sekcje
    List* tmp = start;
    ExtendList(tmp);
    //char a = input[input.size() - 2];
    while (tmp->next != nullptr)
        tmp = tmp->next;
    tmp->block[tmp->countFree].number = FindBlockNumber(start);
    tmp->block[tmp->countFree].used = true;
    input.removeSpecialChars(); input.push_back('\0');
    InputSection(tmp, input);
    tmp->count++;
    tmp->countFree++;
}

bool WriteSection(List** start) { //Wczytuje sekcje do zapisania, jesli sa puste linie wczytuje ponownie
    MyString input;
    List* temp = *start;
    while (cin >> input) {
        //input = Sanitize(input);
        while (input == "") {
            input = "";
            cin >> input;
        }
        if (strcmp(input.c_str(), "????") == 0)
            return false;
        Input(input, temp);
        input = "";
    }
    return true;
}