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

Variable* Scope::getVariable(const std::string &name, Scope* alt_parent) {
    for (Variable &v : this->variables) {
        if (v.getName() == name) {
            return &v;
        }
    }

    if (this->parent != NULL && this->parent->hasVariable(name)) {
        return this->parent->getVariable(name);
    }

    if (alt_parent != NULL && alt_parent->hasVariable(name)) {
        return alt_parent->getVariable(name);
    }

    return this->parent->getVariable(name);
}

bool Scope::hasVariable(const std::string &name, Scope* alt_parent) {
    for (Variable &v : this->variables) {
        if (v.getName() == name) {
            return true;
        }
    }

    if (this->parent != NULL && this->parent->hasVariable(name)) {
        return true;
    }

    if (alt_parent != NULL && alt_parent->hasVariable(name)) {
        return true;
    }

    return false;
}

bool Scope::localHasVariable(const std::string &name) {
    for (Variable &v : this->variables) {
        if (v.getName() == name) {
            return true;
        }
    }

    return false;
}

//void Scope::addVariable(const std::string &name, Variable v, Runtime* runtime, SExpression* expression) {
//    if (!localHasVariable(name)) {
//        this->variables.push_back(v);
//    } else {
//        setVariable(name, new SExpression(*v.getValue(runtime, expression)));
//    }
//}

void Scope::setVariable(const std::string &name, SExpression* value) {
    if (!hasVariable(name)) {
        this->variables.push_back(Variable(name, *value));
        return;

    } else if (!localHasVariable(name)) {
        if (this->parent == NULL) {
            return;
        }

        this->parent->setVariable(name, new SExpression(*value));
        return;
    }

    for (Variable &v : this->variables) {
        if (v.getName() == name) {
            v.setValue(new SExpression(*value));
        }
    }
}

std::vector<std::string> Scope::getVariableNames() {
    std::vector<std::string> variables;

    if (this->getParent() != NULL) {
        for (const std::string &s : this->parent->getVariableNames()) {
            variables.push_back("p-"+s);
        }
    }

    for (const Variable &v : this->variables) {
        variables.push_back(v.getName());
    }

    return variables;
}

std::vector<Variable>* Scope::getVariables() {
    return &this->variables;
}

void Scope::setParent(Scope* parent) {
    this->parent = parent;
}
