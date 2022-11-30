#include <iostream>

#include "../llist/list.hpp"
#include "expression.hpp"
#include "../tokenizer/tokenizer.hpp"

using namespace ll;

SExpression::SExpression() {

}

SExpression::SExpression(const Token &value) {
    this->value.setType(value.getType());
    this->value.setColumn(value.getColumn());
    this->value.setLine(value.getLine());
    this->value.setFilename(value.getFilename());
    this->value.setValue(value.getValue());
}

SExpression::SExpression(ExpressionType type) {
    this->type = type;
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

std::vector<SExpression> *SExpression::getList() {
    return &this->list;
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

SExpression* SExpression::addSExpression(SExpression v) {
    this->list.push_back(v);
    return this;
}

const std::vector<std::string> &SExpression::getTags() const {
    return this->tags;
}

std::vector<std::string> SExpression::getTagsRecursive() const {
    std::vector<std::string> tags;

    if (this->list.size() > 0) {
        for (const SExpression &l : this->list) {
            for (const std::string &t : l.getTagsRecursive()) {
                tags.push_back(t);
            }
        }

        return tags;
    } else {
        return this->tags;
    }
}

void SExpression::visualize() const {
    for (const std::string &t : this->tags) {
        std::cout << ":" << t << " ";
    }

    if (this->list.size() > 0) {
        std::cout << "(";

        for (const SExpression &e : this->list) {
            e.visualize();
            std::cout << " ";
        }

        std::cout << "\b)";

    } else {
        if (this->value.getType() == TokenType::TT_String)
            std::cout << "\"" << this->value.getValue() << "\"";
        else
            std::cout << this->value.getValue();
    }
}