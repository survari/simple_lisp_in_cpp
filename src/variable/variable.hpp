#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include <string>
#include <map>
#include "../expression/expression.hpp"

namespace ll {
    std::vector<std::string> arguments_to_signature(std::vector<SExpression> arguments);
    bool match_signatures(std::vector<SExpression> evaluated_args, std::vector<std::string> signature);

    class Variable {
        private:
            bool evaluated = false; // lazy evaluation

            std::string name;
            SExpression* value;
//            std::map<std::vector<std::string>, SExpression*> value;

        public:
            Variable(const std::string &name, SExpression value);

            std::string getName() const;
//            SExpression* getValue(const std::vector<std::string> &signature, Runtime* runtime, Scope* scope);
//            void setValue(const std::vector<std::string> &signature, SExpression* value);
            SExpression* getValue(Runtime* runtime, Scope* scope);
            void setValue(SExpression* value);
    };
}

#endif /* VARIABLE_HPP */