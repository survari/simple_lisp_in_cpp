#include <iostream>

#include "runtime.hpp"
#include "../builtin/builtin.hpp"
#include "../expression/expression.hpp"
#include "../variable/scope.hpp"

void ll::Runtime::init() {
    this->addBuiltin(ll::Builtin(std::string("eval"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("let"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 2) {
            throw std::runtime_error("too many/few arguments. let expects (only) 2 arguments: "+root->getValue().toErrorMessage());
            exit(1);
        }

        std::vector<SExpression> args = *arguments->getList();

        if (args[0].getType() != ExpressionType::ET_Word) {
            throw std::runtime_error("let: first argument has to be a word: "+args[0].getValue().toErrorMessage());
            exit(1);
        }

        parent->getScope()->setVariable(args[0].getValue().toString(), &args[1]);
        return args[1];
    }));

    this->addBuiltin(ll::Builtin(std::string("debug"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string(">"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("<"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("="), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("!="), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("<="), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string(">="), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("/"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("*"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("+"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("-"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("%"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("concat"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("length"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("printc"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("print"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("println"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("tag"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("tags"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("each"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("wordstr"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("if"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("range"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("head"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("tail"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("reverse"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("indexof"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("load"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("of"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));
}

const std::vector<ll::Builtin> &ll::Runtime::getBuiltins() const {
    return this->builtins;
}

void ll::Runtime::addBuiltin(const ll::Builtin &b) {
    this->builtins.push_back(b);
}


bool ll::Runtime::existsBuiltin(const std::string &name) const {
    for (const ll::Builtin &b : this->builtins) {
        if (b.getName() == name) {
            return true;
        }
    }

    return false;
}

const ll::Builtin* ll::Runtime::getBuiltin(const std::string &name) const {
    for (const ll::Builtin &b : this->builtins) {
        if (b.getName() == name) {
            return &b;
        }
    }

    return NULL;
}