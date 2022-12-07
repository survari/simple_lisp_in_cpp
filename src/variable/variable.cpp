#include "variable.hpp"

using namespace ll;

Variable::Variable(Runtime* runt, Scope* scope, const std::string &name, SExpression value) {
    this->name = name;

    if (value.getType() == ExpressionType::ET_Lambda) {
        this->value[value.getLambdaSignature(runt, scope)] = new SExpression(value);
        this->is_lambda = true;

    } else {
        this->value["<`>"] = new SExpression(value);
    }
}

std::string Variable::getName() const {
    return this->name;
}

//SExpression* Variable::getValue(const std::vector<std::string> &signature, Runtime* runtime, Scope* scope) {
SExpression* Variable::getValue(Runtime* runtime, Scope* scope) {
//    SExpression* v = NULL;
//
//    if (this->value.count(signature)) {
//        v = this->value[signature];
//    } else {
//        v = this->value.begin()->second;
//    }
//
//    if (!this->evaluated) {
//        *v = SExpression(v->eval(runtime, scope));
//        this->evaluated = true;
//    }
//
//    return v;

    if (this->value["<`>"] == NULL) {
        std::map<std::string, SExpression*>::iterator it;

        for (it = this->value.begin(); it != this->value.end(); it++) {
            if (it->second != NULL) {
                return it->second;
            }
        }
    }

    if (!this->evaluated) {
        this->value["<`>"] = new SExpression(this->value["<`>"]->eval(runtime, scope));
        this->evaluated = true;
    }

    return this->value["<`>"];
}

//void Variable::setValue(const std::vector<std::string> &signature, SExpression* value) {
void Variable::setValue(Runtime* runt, Scope* scope, SExpression* value) {
//    SExpression* v = NULL;
//
//    if (this->value.count(signature)) {
//        v = this->value[signature];
//
//    } else {
//        v = this->value.begin()->second;
//    }
//
//    *v = *value;
    if (this->is_lambda && value->getType() == ExpressionType::ET_Lambda) {
        this->value[value->getLambdaSignature(runt, scope)] = new SExpression(*value);
        return;
    }

    this->value.clear();
    this->value["<`>"] = new SExpression(*value);
    this->is_lambda = false;
    this->evaluated = false;
}

SExpression* Variable::getLambda(std::vector<SExpression> args) {
    std::map<std::string, SExpression*>::iterator it;
    int current_option_variables = -1;
    SExpression* current_option_result = NULL;

    for (it = this->value.begin(); it != this->value.end(); it++) {
        std::vector<std::string> names;
        std::string tmp;

        for (usize i = 0; i < it->first.size(); i++) {
            if (it->first[i] == '`') {
                names.push_back(tmp);
                tmp = "";

            } else {
                tmp += it->first[i];
            }
        }

        if (names.size() == 0 || names[0] == "" || names.size() != args.size()) {
            continue;
        }

        int variable_count = 0;
        int i = 0;
        for (; i < names.size(); i++) {
            if (names[i] == "@") {
                variable_count++;
                continue;

            } else if (names[i] != args[i].toString()) {
                i = names.size() + 1;
                break;
            }
        }

        if (i >= names.size() + 1) {
            continue;
        }

        if (variable_count < current_option_variables || current_option_variables == -1) {
            current_option_result = it->second;
            current_option_variables = variable_count;
        }
    }

    return current_option_result;
}

std::string Variable::getSignatures(Runtime* runtime, Scope* scope) {
    std::string s;
    std::map<std::string, SExpression*>::iterator it;

    for (it = this->value.begin(); it != this->value.end(); it++) {
        if (it->second != NULL) {
            s += "  - " + it->second->getLambdaSignature(runtime, scope) + "\n";
        }
    }

    return s;
}
