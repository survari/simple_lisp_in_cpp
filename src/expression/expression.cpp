#include <iostream>

#include "../llist/list.hpp"
#include "expression.hpp"
#include "../tokenizer/tokenizer.hpp"

using namespace ll;

SExpression::SExpression() {

}

SExpression::SExpression(const Token &value, ExpressionType type) {
    this->value.setType(value.getType());
    this->value.setColumn(value.getColumn());
    this->value.setLine(value.getLine());
    this->value.setFilename(value.getFilename());
    this->value.setValue(value.getStrValue());
    this->value.setValue(value.getNumValue());

    this->type = type;
}

SExpression::SExpression(ExpressionType type) {
    this->type = type;
}

bool SExpression::isEvaluable() const {
    switch (this->type) {
        case ExpressionType::ET_Identifier:
            return true;

        case ExpressionType::ET_List:
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

SExpression* SExpression::addTag(std::string tag) {
    if (!hasTag(tag)) {
        this->tags.push_back(tag);
    }

    return this;
}

SExpression* SExpression::addTags(const std::vector<std::string> &tags) {
    for (const std::string &tag : tags) {
        if (!hasTag(tag)) {
            this->tags.push_back(tag);
        }
    }

    return this;
}

void SExpression::setType(ExpressionType type) {
    this->type = type;
}

bool SExpression::hasTag(const std::string &tag) const {
    for (const std::string &s : this->getTags()) {
        if (s == tag) {
            return true;
        }
    }

    return false;
}

bool SExpression::includesTagRecursive(const std::string &tag) const {
    for (const std::string &s : this->getTagsRecursive()) {
        if (s == tag) {
            return true;
        }
    }

    return false;
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

void SExpression::visualize(int indentation) const {
    for (const std::string &t : this->tags) {
        std::cout << ":" << t << " ";
    }

    if (this->list.size() > 0) {
        if (indentation > 0 && this->type != ExpressionType::ET_List) {
            std::cout << std::endl;

            for (int i = 0; i < indentation; i++)
                std::cout << "    ";
        }

        std::cout << "(";

        for (const SExpression &e : this->list) {
            e.visualize(indentation+1);
            std::cout << " ";
        }

        std::cout << "\b)";

    } else {
        if (this->value.getType() == TokenType::TT_String)
            std::cout << "\"" << this->value.getStrValue() << "\"";
        else if (this->value.getType() == TokenType::TT_Number)
            std::cout << this->value.getNumValue();
        else
            std::cout << this->value.getStrValue();
    }

    if (indentation == 0) {
        std::cout << std::endl;
    }
}