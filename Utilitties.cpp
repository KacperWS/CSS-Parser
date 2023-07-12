#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Utillities.h"
#include "MyString.h"

using namespace std;

void Resect(List** temp) { //Usuwa puste bloki
    List* newTemp = *temp;
    bool deleted = false;
    while ((*temp)->next != nullptr) {
        if ((*temp)->count == 0 && (*temp)->countFree == ELEMENTS) {
            List* del = (*temp);
            if ((*temp)->next != nullptr)
                (*temp)->next->prev = (*temp)->prev;
            if((*temp)->prev != nullptr)
                (*temp)->prev->next = (*temp)->next;
            (*temp) = (*temp)->next;
            deleted = true;
            delete del;
        }
        else {
            (*temp) = (*temp)->next;
        }
    }
    if(!deleted)
        *temp = newTemp;
} 

bool DeleteSection(List* temp, int i) { //Usuwa sekcje z bloku
    i = ReturnBlock(&temp, i);
    if ((temp->block[i].used == true && temp->block[i].startA == nullptr) || OutOfRange(i))
        return false; //Jesli nie da³o siê usunac zakoncz
    DeleteSelList(temp->block[i].startS);
    DeleteAtrList(temp->block[i].startA);
    temp->block[i].startS = nullptr;
    temp->block[i].startA = nullptr;
    temp->block[i].countS = 0;
    temp->block[i].countA = 0;
    temp->block[i].number = -1;
    temp->count--;
    RecountBlockNumbers(temp, i); //Przelicza numerki blokow za usunietym
    //Resect(temp);
    return true;
}

bool DeleteAttribute(List* temp, int i, MyString& atr) { //usuwa atrybut z sekcji
    i = ReturnBlock(&temp, i);
    if (OutOfRange(i))
        return false;
    int count = 0;
    bool deleted = false;
    AtrList* tempA = temp->block[i].startA, * tempPrev = temp->block[i].startA;
    if (tempA->next == nullptr || tempA->next->attribute.size() < 1) { //Warunek jesli to jedyny atrybut w bloku
        if (tempA->attribute == atr) {
            temp->block[i].countA--;
            deleted = true;
        }
    }
    else {
        while (tempA != nullptr) { //Szuka atrybutu zeby usunac
            if (tempA->attribute == atr) {
                AtrList* del = tempA;
                tempA = tempA->next;
                tempPrev->next = tempA;
                delete del;
                temp->block[i].countA--;
                deleted = true;
                break;
            }
            count++;
            tempPrev = tempA;
            tempA = tempA->next;
        }
    }
    if (count == 0) //Jesli byl to pierwszy atrybut do skasowania cofa wskaznik zeby nie zgubil poczatku
        temp->block[i].startA = tempA;
    if (temp->block[i].countA == 0) //Usuwa pusta sekcje
        DeleteSection(temp, i);
    return deleted; //Zwaraca powodzenia usuniecia
}

void DeleteSelList(SelList* start) { //Usuwa cala liste selektorow
    SelList* temp = start;
    while (start != nullptr) {
        start = start->next;
        delete temp;
        temp = start;
    }
    delete start;
    delete temp;
}

void DeleteAtrList(AtrList* start) { //Usuwa cala liste atrybutow
    AtrList* temp = start;
    while (start != nullptr) {
        start = start->next;
        delete temp;
        temp = start;
    }
    delete start;
    delete temp;
}

void DeleteList(List* temp) { //Usuwa cala liste blokow i sekcji
    while (temp->next != nullptr) {
        List* temp2 = temp;
        temp = temp->next;
        for (int i = 0; i < ELEMENTS; i++) {
            DeleteSelList(temp2->block[i].startS);
            DeleteAtrList(temp2->block[i].startA);
        }
        delete temp2;
    }
    if (temp->next == nullptr) {
        DeleteSelList(temp->block[0].startS);
        DeleteAtrList(temp->block[0].startA);
        delete temp;
    }
}

void ExtendList(List* temp) { //Zwieksza liste jesli jest przepelniona
    if (temp->countFree == ELEMENTS && temp->next == nullptr) {
        List* newElement = new List;
        temp->next = newElement;
        newElement->prev = temp;
    }
    else {
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        if (temp->countFree == ELEMENTS) {
            List* newElement = new List;
            temp->next = newElement;
            newElement->prev = temp;
        }
    }
}

/*MyString DeleteWhite(MyString& input) {
    MyString temp = input;
    int count = 0;
    for (int i = 0; i < input.size() - 1; i++) {
        if (temp[i] == ' ')
            count++;
    }
    for (int i = 0; i < count + 1; i++) {
        if (i != count) {
            for (int j = 0; j < input.size() - 1; j++) {
                if (temp[j] == ' ') {
                    temp[j] = temp[j + 1];
                    temp[j + 1] = ' ';
                }
            }
        }
    }
    return temp;
}*/

int ConvertToNumber(MyString& str, int start, int end) { //Zmienia napis w liczbe, nalezy podac wspolrzedne liczby
    MyString temp;
    str.DeleteWhite();
    for (int i = start; i < end; i++) {
        if (isdigit(str[i]))
            temp += str[i];
    }
    return atoi(temp.c_str());
}

bool isNumber(MyString& str, int start, int end) { //Sprwadza czy dany fragment napisu jest liczba
    for (int i = start; i < end; i++) {
        if (!isdigit(str[i]))
            return false;
    }
    return true;
}

int CountChar(MyString& input, char c) {
    int count = 0;
    for (int i = 0; i < input.size(); i++) {
        if (input[i] == c)
            count++;
    }
    return count;
}

int SearchForLetter(MyString& str, char letter) { //Szuka pozycji litery srodkowej w komendzie stad warunek na przecinku
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] == letter && str[i + 1] == ',')
            return i;
    }
    return false;
}

void RecountBlockNumbers(List* temp, int i) { //Przelicza numery blokow
    int j = i;
    while (temp != nullptr) {
        if (!(temp->block[j].used == true && temp->block[j].startA == nullptr)) {
            temp->block[j].number--;
        }
        if (j == 7) {
            j = -1; //Bêdzie iteracja
            temp = temp->next;
        }
        j++;
    }
}

bool OutOfRange(int i) { //Sprwadza dany blok istnieje do u¿ytku
    if (i > ELEMENTS-1)
        return true;
    else
        return false;
}

int ReturnBlock(List** temp, int number) { //Zwraca dokladne miejsce danej sekcji wraz z przesunieta lista glowna
    bool find = true;
    while (find) {
        for (int i = 0; i < ELEMENTS; i++) {
            if ((*temp)->block[i].number == number) {
                number = i;
                find = false;
                break;
            }
        }
        if ((*temp)->next != nullptr && find) //Zapobiega ucieczce wskaznika na dalsze bloki
            (*temp) = (*temp)->next;
        else break;
    }
    return number;
}

int FindBlockNumber(List* start) { //Zwraca najbliszy wolny numer bloku
    List* temp = start;
    bool find = true;
    int count = 0;
    while (find) {
        for (int i = 0; i < ELEMENTS; i++) {
            if (temp->block[i].used == false) { //Znaleziony pusty blok
                count++;
                find = false;
                break;
            }
            if (!(temp->block[i].used == true && temp->block[i].startA == nullptr))
                count++; //Zlicza zajete bloki
        }
        if (temp->next != nullptr) 
            temp = temp->next;
        else break; //Zapobiega wkazaniu na nulla
    }
    return count;
}

void RemoveDuplicates(List *temp) {
    AtrList* currentA = temp->block[temp->countFree].startA;
    ReverseAtr(&currentA);
    temp->block[temp->countFree].countA-=RemoveDuplicatesAtr(currentA);
    ReverseAtr(&currentA);
    temp->block[temp->countFree].startA = currentA;

    SelList* currentS = temp->block[temp->countFree].startS;
    ReverseSel(&currentS);
    temp->block[temp->countFree].countS -= RemoveDuplicatesSel(currentS);
    ReverseSel(&currentS);
    temp->block[temp->countFree].startS = currentS;
}

int RemoveDuplicatesAtr(AtrList *startA) {
    AtrList* current = startA;
    int count = 0;
    while (current != nullptr) {
        AtrList* inner_current = current;
        while (inner_current->next != nullptr) {
            if (inner_current->next->attribute == current->attribute) {
                AtrList* duplicate = inner_current->next;
                inner_current->next = duplicate->next;
                delete duplicate;
                count++;
            }
            else {
                inner_current = inner_current->next;
            }
        }
        current = current->next;
    }
    return count;
}

int RemoveDuplicatesSel(SelList* startS) {
    SelList* current = startS;
    int count = 0;
    while (current != nullptr) {
        SelList* inner_current = current;
        while (inner_current->next != nullptr) {
            if (inner_current->next->selector == current->selector) {
                SelList* duplicate = inner_current->next;
                inner_current->next = duplicate->next;
                delete duplicate;
                count++;
            }
            else {
                inner_current = inner_current->next;
            }
        }
        current = current->next;
    }
    return count;
}

void ReverseAtr(AtrList** startA) {
    AtrList* prev = nullptr;
    AtrList* current = *startA;
    AtrList* next;
    while (current != nullptr) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *startA = prev;
}

void ReverseSel(SelList** startS) {
    SelList* prev = nullptr;
    SelList* current = *startS;
    SelList* next;
    while (current != nullptr) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *startS = prev;
}