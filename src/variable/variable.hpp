#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include <string>
#include "../expression/expression.hpp"

namespace ll {
    class Variable {
        private:
            bool evaluated = false; // lazy evaluation

            std::string name;
            SExpression* value;

        public:
            Variable(const std::string &name, SExpression value);

            std::string getName() const;
            SExpression* getValue(Runtime* runtime, SExpression* parent);

            void setValue(SExpression* value);
    };
}

#endif /* VARIABLE_HPP */