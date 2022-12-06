#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include <string>
#include <map>
#include "../expression/expression.hpp"

namespace ll {
    class Variable {
        private:
            bool evaluated = false; // lazy evaluation
            bool is_lambda = false;

            std::string name;
//            SExpression* value;
            std::map<std::string, SExpression*> value;

        public:
            Variable(Runtime* runt, Scope* scope, const std::string &name, SExpression value);

            std::string getName() const;
//            SExpression* getValue(const std::vector<std::string> &signature, Runtime* runtime, Scope* scope);
//            void setValue(const std::vector<std::string> &signature, SExpression* value);
            SExpression* getValue(Runtime* runtime, Scope* scope);
            SExpression* getLambda(std::vector<SExpression> args);
            void setValue(Runtime* runtime, Scope* scope, SExpression* value);

        std::string getSignatures(Runtime* runtime, Scope* scope);
    };
}

#endif /* VARIABLE_HPP */