#ifndef LIST_HPP
#define LIST_HPP

#include <string>
#include <cstdlib>
#include "../../include/bignum/bignum.h"

// linked list implementation - why? because learning purposes.
namespace ll {
    class List;

    union ListValue {
        std::string word;
        bignum number;
        List* list;

        ListValue();
        ListValue(ListValue* v);
        ~ListValue();
    };

    class ListElement {
        private:
            List* list;
            ListValue value;
            ListElement* next;
            ListElement* previous;

        public:
            ListElement();
            ListElement(ListValue v);

            ListElement* getNext();
            ListElement* getPrevious();
            ListValue* getValue();

            void setNext(ListElement* elem);
            void setPrevious(ListElement* elem);
            void remove();
    };

    class List {
    private:
        ListElement* first;
        ListElement* last;
        unsigned long long size;

    public:
        List(): first(NULL), last(NULL) {};

        ListElement* getFirst();
        ListElement* getLast();

        ListElement* getTail();
        ListElement* popLast();
        ListElement* popFirst();

        ListElement* insert(ListElement* elem);
        ListElement* append(ListElement* elem);
    };
}

#endif /* LIST_HPP */