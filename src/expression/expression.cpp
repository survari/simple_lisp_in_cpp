#include <iostream>

#include "expression.hpp"

using namespace ll;

SExpression::SExpression() {

}

bool SExpression::isEvaluable() const {
    switch (this->type) {
        case ExpressionType::ET_Identifier:
            return true;

        case ExpressionType::ET_Instruction:
            return true;

        default:
            return false;
    }
}

List* SExpression::getList() {
    return this->list;
}

SExpression SExpression::eval() {
    if (this->isEvaluable()) {
        switch (this->type) {
            case ExpressionType::ET_Identifier: // Variable: a=13
                std::cout << "UNIMPLEMENTED: EVAL VARIABLE" << std::endl;
                break;

            case ExpressionType::ET_Instruction: // Instruction (a 1 2 3) = return value of a(1, 2, 3)
                std::cout << "UNIMPLEMENTED: EVAL VARIABLE" << std::endl;
                break;

            default:
                break;
        }

    } else {

    }

    return SExpression();
}
