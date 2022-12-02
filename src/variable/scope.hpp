#ifndef SCOPE_HPP
#define SCOPE_HPP

#include <vector>
#include <string>
#include "variable.hpp"

namespace ll {
    class SExpression;

    class Scope {
        private:
            std::vector<Variable> variables;
            Scope* parent;

        public:
            Scope();
            Scope(Scope* parent);

            Scope* getParent();
            Variable* getVariable(const std::string &name);
            bool hasVariable(const std::string &name);
            bool localHasVariable(const std::string &name);

            void addVariable(const std::string &name, Variable v, Runtime* runtime, SExpression* expression);
            void setVariable(const std::string &name, SExpression *value);
    };
}

#endif /* SCOPE_HPP */