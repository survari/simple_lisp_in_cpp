#include <string>
#include "../../include/BeeNum/Brat.h"
#include "../../include/ll_std.h"

namespace ll {
    class List;
    class ListElement;

    namespace helpers {
        List* list_add_tag(List* l, List* result, ListElement* el);
        bool list_has_tag(ListElement* el, const std::string &tag);
        isize list_indexof(ListElement* el, std::string value, isize index);
        isize list_indexof(ListElement* el, BeeNum::Brat value, isize index);
        List* list_insert_list(List* l, ListElement* elem);
        List* list_append_list(List* l, ListElement* elem);
    }
}