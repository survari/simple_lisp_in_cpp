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
            Variable* getVariable(const std::string &name, Scope* alt_parent=NULL);
            bool hasVariable(const std::string &name, Scope* alt_parent=NULL);
            bool localHasVariable(const std::string &name);

//        void addVariable(const std::string &name, Variable v, Runtime* runtime, SExpression* expression);
//        void setVariable(const std::string &signature, const std::string &name, SExpression *value);

            std::vector<std::string> getVariableNames();
            std::vector<Variable>* getVariables();

            void setParent(Scope* parent);
            void setLocalVariable(Runtime* runtime, const std::string &name, SExpression* value);
            void setVariable(Runtime* runtime, const std::string &name, SExpression *value);
    };
}

#endif /* SCOPE_HPP */