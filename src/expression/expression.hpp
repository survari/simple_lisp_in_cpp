#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <vector>
#include "../tokenizer/tokenizer.hpp"

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
        // List* list;
        Token value;
        std::vector<SExpression> list;
        std::vector<std::string> tags;
        ExpressionType type;

    public:
        SExpression();
        SExpression(const Token &value);
        SExpression(ExpressionType type);

        SExpression* addSExpression(SExpression v);
        const std::vector<std::string> &getTags() const;
        std::vector<std::string> getTagsRecursive() const;

        bool isEvaluable() const;
        std::vector<SExpression>* getList();
        SExpression eval();

        void visualize() const;
    };
}

#endif /* EXPRESSION_HPP */