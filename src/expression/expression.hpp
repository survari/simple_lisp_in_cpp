#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "../llist/list.hpp"

namespace ll {
    enum ExpressionType {
        // Unevaluable
        ET_Word,
        ET_List,
        ET_Lambda,

        // Evaluable
        ET_Identifier,
        ET_Instruction
    };

    class SExpression {
        List list;
        ExpressionType type;

    public:
        SExpression();

        bool isEvaluable() const;
        List* getList();
        SExpression eval();
    };
}

#endif /* EXPRESSION_HPP */