#include <iostream>

#include "expression.hpp"
#include "../tokenizer/tokenizer.hpp"
#include "../variable/scope.hpp"

using namespace ll;

SExpression::SExpression() {
    this->scope = new Scope(NULL);
}

SExpression::SExpression(const Token &value, ExpressionType type, const std::vector<std::string> &tags, Scope* parent) {
    this->value.setType(value.getType());
    this->value.setColumn(value.getColumn());
    this->value.setLine(value.getLine());
    this->value.setFilename(value.getFilename());
    this->value.setValue(value.getStrValue());
    this->value.setValue(value.getNumValue());

    this->type = type;
    addTags(tags);

    this->scope = new Scope(parent);
}

SExpression::SExpression(ExpressionType type) {
    this->type = type;
    this->scope = new Scope(NULL);
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

SExpression SExpression::eval(Runtime* runtime, SExpression* parent) {
    if (this->isEvaluable()) {
        switch (this->type) {
            case ExpressionType::ET_Identifier: // Variable or Builtin: a=13
                if (!this->getScope()->hasVariable(this->value.getStrValue(), parent->getScope())) {
                    throw std::runtime_error("unknown variable: "+this->getValue().toErrorMessage());
                    exit(1);
                }

                return SExpression(*this->getScope()->getVariable(this->getValue().getStrValue(), parent->getScope())->getValue(runtime, parent));

            case ExpressionType::ET_Instruction: { // Instruction (a 1 2 3) = return value of a(1, 2, 3)
                // Builtin
                switch (this->value.getType())
                {
                    case TokenType::TT_Builtin: {
                        std::string name = this->list[0].getValue().toString();
                        SExpression* arguments = new SExpression(ExpressionType::ET_List);

                        for (usize i = 1; i < this->list.size(); i++) {
                            if ((name == "let" || name == "fn") && i > 1) {
                                arguments->addSExpression(this->list[i]); // do not evaluate expression! lazy evaluation etc.
                            } else {
                                arguments->addSExpression(this->list[i].eval(runtime, this));
                            }
                        }

                        return runtime->getBuiltin(name)->getLambda()(runtime, this, parent, arguments);
                    }

                    case TokenType::TT_Word: {
                        if (!this->getScope()->hasVariable(this->value.toString())) {
                            throw std::runtime_error("variable does not exist " + this->value.toErrorMessage());
                            exit(1);
                        }

                        SExpression lambda = *this->getScope()->getVariable(this->value.toString())->getValue(runtime, parent);

                        std::cout << "SELF: "; this->visualize();
                        std::cout << this->getList()->size() << std::endl;
                        std::cout << "LAMBDA: "; lambda.visualize();

                        return lambda.runLambda(*this->getList(), runtime, this);
                    }

                    default:
                        throw std::runtime_error("there is something crazy going on here! "+this->value.toErrorMessage()+" / "+std::to_string(this->value.getType()));
                        break;
                }

                // Function
                break;
            }

            case ExpressionType::ET_List: {
                SExpression evaluated = *this;
                evaluated.list.clear();

                for (SExpression &ex : *this->getList()) {
                    evaluated.addSExpression(ex.eval(runtime, this));
                }

                return evaluated;
            }

            default:
                break;
        }

    } else {
        return SExpression(*this);
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
    std::vector<std::string> tags = this->tags;

    if (this->list.size() > 0) {
        for (const SExpression &l: this->list) {
            for (const std::string &t: l.getTagsRecursive()) {
                tags.push_back(t);
            }
        }
    }

    return tags;
}

ExpressionType SExpression::getType() const {
    return this->type;
}

const Token &SExpression::getValue() const {
    return this->value;
}

Scope* SExpression::getScope() {
    return this->scope;
}

void SExpression::visualize(int indentation) const {
    for (const std::string &t : this->tags) {
        std::cout << ":" << t << " ";
    }

    if (this->list.size() > 0 || this->type == ExpressionType::ET_List) {
        if (indentation > 0 && this->type != ExpressionType::ET_List) {
            std::cout << std::endl;

            for (int i = 0; i < indentation; i++)
                std::cout << "    ";

        } else if (this->type == ExpressionType::ET_List) {
            std::cout << "'";
        }

        std::cout << "(";

        for (const SExpression &e : this->list) {
            e.visualize(indentation+1);
            std::cout << " ";
        }

        if (this->list.size() > 0) {
            std::cout << "\b";
        }

        std::cout << ")";

    } else {
        if (this->value.getType() == TokenType::TT_String)
            std::cout << "\"" << this->value.getStrValue() << "\"";

        else if (this->value.getType() == TokenType::TT_Number)
            std::cout << this->value.getNumValue();

        else if (this->type == ExpressionType::ET_Word)
            std::cout << "'" << this->value.getStrValue();

        else
            std::cout << this->value.getStrValue();
    }

    if (indentation == 0) {
        std::cout << std::endl;
    }
}

SExpression SExpression::getTaggedValue(std::string tag) {
    SExpression expression;

    for (const SExpression &e : this->list) {
        if (e.hasTag(tag)) {
            return e;
        }
    }

    return expression;
}

bool SExpression::hasTaggedValue(std::string tag) {
    for (const SExpression &e : this->list) {
        if (e.hasTag(tag)) {
            return true;
        }
    }

    return false;
}

void SExpression::setScope(Scope* s) {
    this->scope = s;
}

SExpression SExpression::runLambda(std::vector<SExpression> arguments, Runtime* runt, SExpression* parent) {
    if (this->getType() != ExpressionType::ET_Lambda) {
        throw std::runtime_error("function call is no lambda " + arguments[0].getValue().toErrorMessage());
        exit(1);
    }

    std::vector<SExpression>* instructions = this->getList();
    std::vector<SExpression>* parameters = (*instructions)[0].getList();

    if (parameters->size() != arguments.size()) {
        throw std::runtime_error("function call: given parameters ("+
                                 std::to_string(arguments.size())+") does not match parameter length ("+
                                 std::to_string(parameters->size())+"): " + arguments[0].getValue().toErrorMessage());

        exit(1);
    }

    for (int i = 0; i < parameters->size(); i++) {
        parent->getScope()->setVariable((*parameters)[i].value.toString(), &arguments[i]);
        this->getScope()->setParent(parent->getScope());
        this->getScope()->setVariable((*parameters)[i].value.toString(), &arguments[i]);
    }

    SExpression ret;
    for (int i = 1; i < instructions->size(); i++) {
        (*instructions)[i].getScope()->setParent(this->getScope());
        ret = (*instructions)[i].eval(runt, this);
    }

    return ret;
}
