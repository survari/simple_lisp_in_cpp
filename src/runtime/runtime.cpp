#include <iostream>

#include "runtime.hpp"
#include "../builtin/builtin.hpp"
#include "../expression/expression.hpp"

void ll::Runtime::init() {
    this->addBuiltin(ll::Builtin(std::string("debug"), [](Runtime* runt, SExpression* root, SExpression* arguments) {
        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string(">"), [](Runtime* runt, SExpression* root, SExpression* arguments) {
        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("<"), [](Runtime* runt, SExpression* root, SExpression* arguments) {
        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("="), [](Runtime* runt, SExpression* root, SExpression* arguments) {
        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("!="), [](Runtime* runt, SExpression* root, SExpression* arguments) {
        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("<="), [](Runtime* runt, SExpression* root, SExpression* arguments) {
        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string(">="), [](Runtime* runt, SExpression* root, SExpression* arguments) {
        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("/"), [](Runtime* runt, SExpression* root, SExpression* arguments) {
        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("*"), [](Runtime* runt, SExpression* root, SExpression* arguments) {
        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("+"), [](Runtime* runt, SExpression* root, SExpression* arguments) {
        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("-"), [](Runtime* runt, SExpression* root, SExpression* arguments) {
        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("%"), [](Runtime* runt, SExpression* root, SExpression* arguments) {
        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("concat"), [](Runtime* runt, SExpression* root, SExpression* arguments) {
        return ll::SExpression();
    }));
}

const std::vector<ll::Builtin> &ll::Runtime::getBuiltins() const {
    return this->builtins;
}

void ll::Runtime::addBuiltin(const ll::Builtin &b) {
    this->builtins.push_back(b);
}


bool ll::Runtime::existsBuiltin(const std::string &name) const {
    for (const ll::Builtin &b : this->builtins) {
        if (b.getName() == name) {
            return true;
        }
    }

    return false;
}

const ll::Builtin* ll::Runtime::getBuiltin(const std::string &name) const {
    for (const ll::Builtin &b : this->builtins) {
        if (b.getName() == name) {
            return &b;
        }
    }

    return NULL;
}