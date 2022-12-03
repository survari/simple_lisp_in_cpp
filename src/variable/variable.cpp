#include "variable.hpp"

using namespace ll;

Variable::Variable(const std::string &name, SExpression value) {
    this->name = name;
    this->value = new SExpression(value);
}

std::string Variable::getName() const {
    return this->name;
}

SExpression* Variable::getValue(Runtime* runtime, SExpression* parent) {
    if (!this->evaluated) {
        this->value = new SExpression(this->value->eval(runtime, parent));
        this->evaluated = true;
    }

    return this->value;
}

void Variable::setValue(SExpression* value) {
    this->value = new SExpression(*value);
}