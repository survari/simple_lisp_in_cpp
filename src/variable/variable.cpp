#include "variable.hpp"

using namespace ll;

std::vector<std::string> ll::arguments_to_signature(std::vector<SExpression> arguments) {
    std::vector<std::string> s;

    for (SExpression ex : arguments) {
        if (ex.getType() == ExpressionType::ET_Word) {
            s.push_back("<`>");

        } else {
            s.push_back(ex.toString());
        }
    }

    return s;
}

bool ll::match_signatures(std::vector<SExpression> evaluated_args, std::vector<std::string> signature) {
    if (evaluated_args.size() != signature.size()) {
        return false;
    }

    for (usize i = 0; i < signature.size(); i++) {
        if (signature[i] != "<`>" && signature[i] != evaluated_args[i].toString()) {
            return false;
        }
    }

    return true;
}

Variable::Variable(const std::string &name, SExpression value) {
    this->name = name;
    this->value = new SExpression(value);
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

    if (!this->evaluated) {
        *this->value = SExpression(this->value->eval(runtime, scope));
        this->evaluated = true;
    }

    return this->value;
}

//void Variable::setValue(const std::vector<std::string> &signature, SExpression* value) {
void Variable::setValue(SExpression* value) {
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
    this->value = new SExpression(*value);
}