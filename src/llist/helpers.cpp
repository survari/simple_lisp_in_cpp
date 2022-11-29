#include "helpers.hpp"
#include "list.hpp"

using namespace ll;

List* ll::helpers::list_add_tag(List* l, List* result, ListElement* el) {
    if (l != NULL && result != NULL && el != NULL) {
        l->appendList(el->getTags());
        ll::helpers::list_add_tag(l, result, el->getNext());
    }

    return result;
}

bool ll::helpers::list_has_tag(ListElement* el, const std::string &tag) {
    if (el == NULL) {
        return false;

    } else if (el->hasTag(tag)) {
        return true;

    } else {
        return ll::helpers::list_has_tag(el->getNext(), tag);
    }
}

isize ll::helpers::list_indexof(ListElement* el, std::string value, isize index) {
    return -1;
}

isize ll::helpers::list_indexof(ListElement* el, BeeNum::Brat value, isize index) {
    return -1;
}

List* helpers::list_insert_list(List* list, ListElement* elem) {
    if (elem == NULL || list == NULL) {
        return list;

    } else {
        list->insert(elem);
        return list_insert_list(list, elem->getPrevious());
    }
}

List* helpers::list_append_list(List* list, ListElement* elem) {
    if (elem == NULL || list == NULL) {
        return list;

    } else {
        list->append(elem);
        return list_insert_list(list, elem->getNext());
    }
}