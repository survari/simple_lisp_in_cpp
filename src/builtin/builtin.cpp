#include "builtin.hpp"

using namespace ll;

Builtin::Builtin(const std::string &name, std::function<SExpression(Runtime* runtime, SExpression* root, SExpression* parent, SExpression* arguments)> lambda)
    : lambda(lambda), name(name) {}

const std::string &Builtin::getName() const {
    return this->name;
}

const std::function<SExpression(Runtime* runtime, SExpression* root, SExpression* parent, SExpression* arguments)> &Builtin::getLambda() const {
    return this->lambda;
}