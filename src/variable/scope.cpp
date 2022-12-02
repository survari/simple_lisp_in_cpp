#include "scope.hpp"
#include "variable.hpp"

using namespace ll;

Scope::Scope() {
    this->parent = NULL;
}

Scope::Scope(Scope* parent) {
    this->parent = parent;
}

Scope* Scope::getParent() {
    return this->parent;
}

Variable* Scope::getVariable(const std::string &name) {
    for (Variable &v : this->variables) {
        if (v.getName() == name) {
            return &v;
        }
    }

    if (this->parent == NULL) {
        return NULL;
    }

    return this->parent->getVariable(name);
}

bool Scope::hasVariable(const std::string &name) {
    for (Variable &v : this->variables) {
        if (v.getName() == name) {
            return true;
        }
    }

    if (this->parent == NULL) {
        return false;
    }

    return this->parent->hasVariable(name);
}

bool Scope::localHasVariable(const std::string &name) {
    for (Variable &v : this->variables) {
        if (v.getName() == name) {
            return true;
        }
    }

    return false;
}

void Scope::addVariable(const std::string &name, Variable v, Runtime* runtime, SExpression* expression) {
    if (!localHasVariable(name)) {
        this->variables.push_back(v);
    } else {
        setVariable(name, new SExpression(*v.getValue(runtime, expression)));
    }
}

void Scope::setVariable(const std::string &name, SExpression* value) {
    for (Variable &v : this->variables) {
        if (v.getName() == name) {
            v.setValue(new SExpression(*value));
        }
    }

    if (this->parent == NULL) {
        return;
    }

    return this->parent->setVariable(name, new SExpression(*value));
}