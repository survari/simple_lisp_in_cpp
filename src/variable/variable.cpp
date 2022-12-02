#include "variable.hpp"

using namespace ll;

Variable::Variable(const std::string &name, SExpression value) {
    this->name = name;
    this->value = new SExpression(value);
}

std::string Variable::getName() {
    return this->name;
}

SExpression* Variable::getValue() {
    return this->value;
}

void Variable::setValue(SExpression* value) {
    this->value = new SExpression(*value);
}