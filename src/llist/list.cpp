#include "../../include/BeeNum/Brat.h"

#include <string.h>
#include "list.hpp"
#include "helpers.hpp"

using namespace ll;

ListValue::ListValue() {}
ListValue::~ListValue() {}

ListValue::ListValue(ListValue* v):
    word(v->word),
    number(v->number),
    list(v->list) {}

ListValue::ListValue(BeeNum::Brat number) {
    this->number = number;
}

ListValue::ListValue(std::string word) {
    this->word = word;
}

ListValue::ListValue(List* list) {
    this->list = list;
}


ListElement::ListElement():
    next(NULL),
    previous(NULL) {}

ListElement::ListElement(ListValue* v):
    value(v),
    next(NULL),
    previous(NULL) {}

ListElement::ListElement(const ListElement &elem):
    value(elem.value),
    next(elem.next),
    previous(elem.previous) {
}

ListElement* ListElement::getNext() {
    if (this->next == NULL)
        throw std::runtime_error("there is no next element.");

    return this->next;
}

ListElement* ListElement::getPrevious() {
    if (this->next == NULL)
        throw std::runtime_error("there is no previous element.");

    return this->previous;
}

void ListElement::setNext(ListElement* elem) {
    this->next = elem;
}

void ListElement::setPrevious(ListElement* elem) {
    this->previous = elem;
}

ListValue* ListElement::getValue() {
    return &this->value;
}

void ListElement::remove() {
    if (this->next != NULL) {
        this->previous->next = this->next;
        this->next->previous = this->previous;
    } else {
        this->previous->next = NULL;
    }

    free(this);
}


ListElement* List::getFirst() {
    return this->first;
}

ListElement* List::getLast() {
    return this->last;
}

ListElement* List::getTail() {
    return this->first->getNext();
}

ListElement* List::popLast() {
    ListElement* elem = this->last;

    if (this->last != NULL && this->last->getPrevious() != NULL) {
        this->last->getPrevious()->setNext(NULL);
        this->last = this->last->getPrevious();
    }

    if (this->size > 0) {
        this->size--;
    }

    return elem;
}

ListElement* List::popFirst() {
    ListElement* elem = this->first;

    if (this->first != NULL && this->first->getNext() != NULL) {
        this->first->getNext()->setPrevious(NULL);
        this->first = this->first->getNext();
    }

    if (this->size > 0) {
        this->size--;
    }

    return elem;
}

ListElement* List::insert(ListElement* elem) {
    elem = new ListElement(*elem);

    if (this->first == NULL) {
        this->first = elem;

        if (this->last == NULL) {
            this->last = this->first;
            this->size = 0;
        }
    } else {
        this->first->setPrevious(elem);
        elem->setNext(this->first);
        this->first = elem;
    }

    this->size++;
    return elem;
}

ListElement* List::append(ListElement* elem) {
    elem = new ListElement(*elem);

    if (this->last == NULL) {
        this->last = elem;

        if (this->first == NULL) {
            this->first = this->last;
            this->size = 0;
        }

    } else {
        this->last->setNext(elem);
        elem->setPrevious(this->last);
        this->last = elem;
    }

    this->size++;
    return elem;
}

List* List::insertList(List* elem) {
    return helpers::list_insert_list(this, elem->last);
}

List* List::appendList(List* elem) {
    return helpers::list_append_list(this, elem->first);
}


List* ListElement::getTags() {
    return this->tags;
}

bool ListElement::hasTag(const std::string &tag) {
    return this->tags->indexOf(tag) >= 0;
}

void ListElement::addTag(const std::string &tag) {
    this->tags->insert(new ListElement(new ListValue(tag)));
}

List* List::getTags() {
    return helpers::list_add_tag(this, new List(), this->first);
}

bool List::hasTag(const std::string &tag) {
    return helpers::list_has_tag(this->first, tag);
}

int List::indexOf(std::string word) {
    return helpers::list_indexof(this->first, word, 0);
}

int List::indexOf(BeeNum::Brat num) {
    return helpers::list_indexof(this->first, num, 0);
}

bool List::isEqualTo(List* l) {
    throw std::runtime_error("UNIMPLEMENTED LIST::ISEQUALTO");
    return false;
}
