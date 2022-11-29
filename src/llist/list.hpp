#ifndef LIST_HPP
#define LIST_HPP

#include <string>
#include <cstdlib>

#include "../../include/BeeNum/Brat.h"
#include "../../include/ll_std.h"

// linked list implementation - why? because learning purposes.
namespace ll {
    class List;

    enum ValueType {
        VT_Number,
        VT_Word,
        VT_Identifier,
        VT_Builtin,
        VT_Tag
    };

    class ListValue {
    private:
        std::string word;
        BeeNum::Brat number;
        List* list;

    public:
        ListValue();
        ListValue(ListValue* v);
        ListValue(BeeNum::Brat number);
        ListValue(std::string word);
        ListValue(List* list);
        ~ListValue();
    };

    class ListElement {
        private:
            List* list;
            List* tags;
            ListValue value;
            ListElement* next;
            ListElement* previous;

        public:
            ListElement();
            ListElement(ListValue* v);
            ListElement(const ListElement &elem);

            ListElement* getNext();
            ListElement* getPrevious();
            ListValue* getValue();
            List* getTags();
            List* getTag();
            bool hasTag(const std::string &s);

            void setNext(ListElement* elem);
            void setPrevious(ListElement* elem);
            void addTag(const std::string &tag);
            void removeTag(const std::string &tag);
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
        ListElement* getNth(int i);
        usize getLength();
        int indexOf(std::string word);
        int indexOf(BeeNum::Brat num);
        bool isEqualTo(List* l);

        List* getTags();
        bool hasTag(const std::string &tag);

        ListElement* getTail();
        ListElement* popLast();
        ListElement* popFirst();

        ListElement* insert(ListElement* elem); // add element to front
        ListElement* append(ListElement* elem); // add element to end
        List* insertList(List* elem);           // add list to front
        List* appendList(List* elem);           // add lsit to end
    };
}

#endif /* LIST_HPP */