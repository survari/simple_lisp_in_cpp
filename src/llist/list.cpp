#include "list.hpp"

ll::ListValue::ListValue() {}
ll::ListValue::~ListValue() {}

ll::ListValue::ListValue(ll::ListValue* v) {
}


ll::ListElement::ListElement():
    next(NULL),
    previous(NULL) {}

ll::ListElement::ListElement(ll::ListValue v):
    value(&v),
    next(NULL),
    previous(NULL) {}

ll::ListElement* ll::ListElement::getNext() {
    if (this->next == NULL)
        throw std::runtime_error("there is no next element.");

    return this->next;
}

ll::ListElement* ll::ListElement::getPrevious() {
    if (this->next == NULL)
        throw std::runtime_error("there is no previous element.");

    return this->previous;
}

void ll::ListElement::setNext(ListElement* elem) {
    this->next = elem;
}

void ll::ListElement::setPrevious(ListElement* elem) {
    this->previous = elem;
}

ll::ListValue* ll::ListElement::getValue() {
    return &this->value;
}

void ll::ListElement::remove() {
    if (this->next != NULL) {
        this->previous->next = this->next;
        this->next->previous = this->previous;
    } else {
        this->previous->next = NULL;
    }

    free(this);
}


ll::ListElement* ll::List::getFirst() {
    return this->first;
}

ll::ListElement* ll::List::getLast() {
    return this->last;
}

ll::ListElement* ll::List::getTail() {
    return this->first->getNext();
}

ll::ListElement* ll::List::popLast() {
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

ll::ListElement* ll::List::popFirst() {
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

ll::ListElement* ll::List::insert(ListElement* elem) {
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

ll::ListElement* ll::List::append(ListElement* elem) {
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