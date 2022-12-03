#include <iostream>

#include "runtime.hpp"
#include "../builtin/builtin.hpp"
#include "../expression/expression.hpp"
#include "../variable/scope.hpp"
#include "parser.hpp"

void ll::Runtime::init() {
    this->addBuiltin(ll::Builtin(std::string("eval"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
        exit(1);

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("let"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 2) {
            throw std::runtime_error("too many/few arguments. 'let' expects (only) 2 arguments: "+root->getValue().toErrorMessage());
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
        for (const SExpression &a : *arguments->getList()) {
            a.visualize();
        }

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
        if (arguments->getList()->size() != 1) {
            throw std::runtime_error("too many/few arguments. 'printc' expects (only) 1 argument: " + root->getValue().toErrorMessage());
            exit(1);
        }

        std::cout << (char) std::stoi((*arguments->getList())[0].getValue().getNumValue());
        return (*arguments->getList())[0];
    }));

//    this->addBuiltin(ll::Builtin(std::string("print"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
//        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
//        exit(1);
//
//        return ll::SExpression();
//    }));
//
//    this->addBuiltin(ll::Builtin(std::string("println"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
//        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
//        exit(1);
//
//        return ll::SExpression();
//    }));

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
        if (arguments->getList()->size() != 2) {
            throw std::runtime_error("too many/few arguments. 'each' expects (only) 2 arguments: " + root->getValue().toErrorMessage());
            exit(1);
        }

        std::vector<SExpression> list = *(*arguments->getList())[0].getList();
        SExpression lambda = (*arguments->getList())[1];
        SExpression ret;

        for (SExpression e : list) {
            ret = lambda.runLambda({ e }, runt, root);
        }

        return ret;
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
        if (arguments->getList()->size() != 1) {
            throw std::runtime_error("too many/few arguments. 'load' expects (only) 1 argument: " + root->getValue().toErrorMessage());
            exit(1);
        }

        std::string filename = "";
        SExpression filename_str = (*arguments->getList())[0];

        if (filename_str.getType() == ExpressionType::ET_List && filename_str.includesTagRecursive("str")) {
            for (SExpression s : *filename_str.getList()) {
                filename.push_back((char) std::stoi(s.getValue().getNumValue().toString()));
            }

        } else {
            throw std::runtime_error("'load' expects a string as the first argument: " + root->getValue().toErrorMessage());
            exit(1);
        }

        std::string source = read_file(filename);
        std::vector<Token> tokens = Tokenizer::tokenize(runt, filename, source);
        SExpression r = Parser::parse(tokens, runt->getRoot());
        r.setScope(runt->getRoot());

        SExpression res;

        for (ll::SExpression ex : *r.getList()) {
            res = ex.eval(runt, &r);
        }

        return res;
    }));

    this->addBuiltin(ll::Builtin(std::string("of"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 2) {
            throw std::runtime_error("too many/few arguments. 'of' expects (only) 2 arguments: " + root->getValue().toErrorMessage());
            exit(1);
        }

        SExpression expression = SExpression((*arguments->getList())[0]);
        std::string tag = (*arguments->getList())[1].getValue().toString();

        if (!expression.hasTaggedValue(tag)) {
            throw std::runtime_error("cannot get tagged value '" +tag+ "': does not exist " + root->getValue().toErrorMessage());
            exit(1);
        }

        return expression.getTaggedValue(tag);
    }));

    this->addBuiltin(ll::Builtin(std::string("fn"), [](Runtime* runt, SExpression* root, SExpression* parent, SExpression* arguments) {
        if (arguments->getList()->size() < 2) {
            throw std::runtime_error("too many/few arguments. 'fn' expects at least 2 arguments: " + root->getValue().toErrorMessage());
            exit(1);
        }

        for (const SExpression &a : *(*arguments->getList())[0].getList()) {
            if (a.getType() != ExpressionType::ET_Word) {
                throw std::runtime_error("fn expects a list of words as the first argument: " + root->getValue().toErrorMessage());
            }
        }

        SExpression lambda = *arguments;
        lambda.setType(ExpressionType::ET_Lambda);

        return lambda;
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

void ll::Runtime::setRoot(Scope* s) {
    this->root = s;
}

ll::Scope* ll::Runtime::getRoot() {
    return this->root;
}
