#include <iostream>

#include "expression.hpp"
#include "../tokenizer/tokenizer.hpp"
#include "../variable/scope.hpp"
#include "../runtime/runtime.hpp"

using namespace ll;

SExpression::SExpression() {
    this->type = ExpressionType::ET_Unknown;
}

SExpression::SExpression(ExpressionType type) {
    this->type = type;
}

SExpression::SExpression(const Token &value, ExpressionType type, const std::vector<std::string> &tags) {
    this->value.setType(value.getType());
    this->value.setColumn(value.getColumn());
    this->value.setLine(value.getLine());
    this->value.setFilename(value.getFilename());
    this->value.setValue(value.getStrValue());
    this->value.setValue(value.getNumValue());

    this->type = type;
    addTags(tags);
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

SExpression SExpression::eval(Runtime* runtime, Scope* parent_scope) {
    // each expression evaluation has its own scope
    Scope* scope = new Scope();
    scope->setParent(parent_scope);

    if (this->isEvaluable()) {
        switch (this->type) {
            case ExpressionType::ET_Identifier: { // Variable or Builtin: a=13
                if (!scope->hasVariable(this->value.getStrValue(), scope)) {
                    throw std::runtime_error("unknown variable: " + this->getValue().toErrorMessage());
                    exit(1);
                }

                SExpression expression = SExpression(*scope->getVariable(this->getValue().getStrValue(), scope)->getValue(runtime, scope));
                return expression;
            }

            case ExpressionType::ET_Instruction: { // Instruction (a 1 2 3) = return value of a(1, 2, 3)
                // Builtin
                switch (this->value.getType())
                {
                    case TokenType::TT_Builtin: {
                        std::string name = this->list[0].getValue().toString();
                        SExpression* arguments = new SExpression(ExpressionType::ET_List);

                        for (usize i = 1; i < this->list.size(); i++) {
                            if ((name == "fn" && i > 1) || name == "if") {
                                arguments->addSExpression(this->list[i]); // do not evaluate expression! lazy evaluation etc.
                            } else {
                                arguments->addSExpression(this->list[i].eval(runtime, scope));
                            }
                        }

                        return runtime->getBuiltin(name)->getLambda()(runtime, this, parent_scope, arguments);
                    }

                    case TokenType::TT_Word: {
                        if (!scope->hasVariable(this->value.toString())) {
                            throw std::runtime_error("variable does not exist " + this->value.toErrorMessage());
                            exit(1);
                        }

                        SExpression a;
                        SExpression b;
                        std::vector<SExpression> arguments;

                        for (int i = 1; i < this->getList()->size(); i++) {
                            SExpression e = this->getList()->at(i).eval(runtime, parent_scope);
                            arguments.push_back(e);
                            b.addSExpression(e);
                        }

                        a.addSExpression(b);
                        Variable* v = scope->getVariable(this->value.toString());
                        SExpression* v_value = v->getValue(runtime, scope);
                        SExpression* lambda = v->getLambda(arguments);

                        if (v_value->getType() != ExpressionType::ET_Lambda) {
                            throw std::runtime_error("variable is no lambda: " + this->value.toErrorMessage());
                            exit(1);
                        }

                        if (lambda == NULL) {
                            throw std::runtime_error("lambda has no matching signature for: [ " +
                                a.getLambdaSignature(runtime, scope) +
                                " ]\n      at:  " + this->value.toErrorMessage() +
                                "\n\nhas following signatures:\n" + v->getSignatures(runtime, scope));
                            exit(1);
                        }

                        return lambda->runLambda(arguments, runtime, this, scope);
                    }

                    case TokenType::TT_ParenthesesOpen: {
                        SExpression lambda = (*this->getList())[0].eval(runtime, scope);
                        std::vector<SExpression> arguments;

                        if (lambda.getType() != ExpressionType::ET_Lambda) {
                            throw std::runtime_error("variable is no lambda " + this->value.toErrorMessage());
                            exit(1);
                        }

                        for (int i = 1; i < this->getList()->size(); i++) {
                            arguments.push_back((*this->getList())[i]);
                        }

                        return lambda.runLambda(arguments, runtime, this, scope);
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
                    evaluated.addSExpression(ex.eval(runtime, scope));
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

SExpression SExpression::runLambda(std::vector<SExpression> arguments, Runtime* runt, SExpression* parent, Scope* parent_scope) {
    Scope* scope = new Scope();
    scope->setParent(parent_scope);

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

    for (int i = 0; i < arguments.size(); i++) {
        arguments[i] = arguments[i].eval(runt, scope);
    }

    for (int i = 0; i < parameters->size(); i++) {
//        if (parameters->at(i).getType() == ET_Identifier) {
//            std::cout << parameters->at(i).value.toString() << " <=> " << &arguments[i] << std::endl;
            scope->setLocalVariable(runt, parameters->at(i).value.toString(), &arguments[i]);
//        }
    }

    SExpression ret;
    for (int i = 1; i < instructions->size(); i++) {
        ret = instructions->at(i).eval(runt, scope);
    }

    return ret;
}

std::string SExpression::toString(int indentation) const {
    std::string s;

    for (const std::string &t : this->tags) {
        s += ":" + t + " ";
    }

    if (this->list.size() > 0 || this->type == ExpressionType::ET_List) {
        if (indentation > 0 && this->type != ExpressionType::ET_List) {
//            s += "\n";

            for (int i = 0; i < indentation; i++)
                s += "    ";

        } else if (this->type == ExpressionType::ET_List) {
            s += "'";
        }

        s += "(";

        for (const SExpression &e : this->list) {
            s += e.toString(indentation+1) + " ";
        }

        if (this->list.size() > 0) {
            s.pop_back();
        }

        s += ")";

    } else {
        if (this->value.getType() == TokenType::TT_String)
            s += "\"" + this->value.getStrValue() + "\"";

        else if (this->value.getType() == TokenType::TT_Number)
            s += this->value.getNumValue();

        else if (this->type == ExpressionType::ET_Word)
            s += "'" + this->value.getStrValue();

        else
            s += this->value.getStrValue();
    }

//    if (indentation == 0) {
//        s += "\n";
//    }

    return s;
}

std::string SExpression::getLambdaSignature(Runtime* runt, Scope* parent_scope) {
    std::string signature = "";
    std::vector<SExpression> parameters = *this->getList()->at(0).getList();

    for (SExpression s : parameters) {
        if (s.getType() != ExpressionType::ET_Word) {
            signature += s.eval(runt, parent_scope).toString() + "`";
        } else {
            signature += "@`";
        }
    }

    return signature;
}