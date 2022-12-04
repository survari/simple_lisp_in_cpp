#include <iostream>
#include <algorithm>

#include "runtime.hpp"
#include "../builtin/builtin.hpp"
#include "../expression/expression.hpp"
#include "../variable/scope.hpp"
#include "parser.hpp"

int compare_num(int mode, BeeNum::Brat a, BeeNum::Brat b) {
    switch (mode) {
        case 1: return a > b;
        case 2: return a < b;
        case 3: return a >= b;
        case 4: return a <= b;
        case 5: return ((bool) std::stoll(a.point(0))) && ((bool) std::stoll(b.point(0)));
        case 6: return ((bool) std::stoll(a.point(0))) || ((bool) std::stoll(b.point(0)));
        case 7: return !((bool) std::stoll(a.point(0)));
    }

    return a != b;
}

BeeNum::Brat expression_to_comparation_value(ll::SExpression e) {
    switch (e.getType()) {
        case ll::ExpressionType::ET_List:
            return e.getList()->size();

        case ll::ExpressionType::ET_Number:
            return e.getValue().getNumValue();

        default:
            return e.getValue().toString().size();
    }
}

ll::SExpression comparation_result_to_expression(int result, ll::Token parent) {
    parent.setType(ll::TokenType::TT_Number);
    parent.setValue(BeeNum::Brat(result));

    return ll::SExpression(parent, ll::ExpressionType::ET_Number, {});
}

ll::SExpression num_to_expr(BeeNum::Brat result, ll::Token parent) {
    parent.setType(ll::TokenType::TT_Number);
    parent.setValue(BeeNum::Brat(result));

    return ll::SExpression(parent, ll::ExpressionType::ET_Number, {});
}

void ll::Runtime::init() {
    this->addBuiltin(ll::Builtin(std::string("eval"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() < 1) {
            throw std::runtime_error("too many/few arguments. 'eval' expects at least 1 argument: " + root->getValue().toErrorMessage());
            exit(1);
        }

        SExpression res;

        for (ll::SExpression ex : *arguments->getList()) {
            if (ex.getType() == ExpressionType::ET_List && ex.getValue().getType() == TokenType::TT_String) {
                std::string source = "";

                for (SExpression c : *ex.getList()) {
                    if (c.getValue().getType() == TokenType::TT_Number) {
                        source.push_back((char) std::stoi(c.getValue().getNumValue()));
                    }
                }

                std::vector<Token> tokens = Tokenizer::tokenize(runt, root->getValue().getFilename(), source);
                SExpression r = Parser::parse(tokens);

                for (ll::SExpression e : *r.getList()) {
                    res = e.eval(runt, parent);
                }

            } else {
                res = ex.eval(runt, parent);
            }
        }

        return res;
    }));

    this->addBuiltin(ll::Builtin(std::string("let"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 2) {
            throw std::runtime_error("too many/few arguments. 'let' expects (only) 2 arguments: "+root->getValue().toErrorMessage());
            exit(1);
        }

        std::vector<SExpression> args = *arguments->getList();

        if (args[0].getType() != ExpressionType::ET_Word) {
            throw std::runtime_error("let: first argument has to be a word: "+args[0].getValue().toErrorMessage());
            exit(1);
        }

        parent->setVariable(args[0].getValue().toString(), &args[1]);
        return args[1];
    }));

    this->addBuiltin(ll::Builtin(std::string("debug"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        for (const SExpression &a : *arguments->getList()) {
            a.visualize();
        }

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("="), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 2) {
            throw std::runtime_error("too many/few arguments. '=' expects (only) 2 arguments: " + root->getValue().toErrorMessage());
            exit(1);
        }

        return comparation_result_to_expression(arguments->getList()->at(0).toString() == arguments->getList()->at(1).toString(), root->getValue());
    }));

    this->addBuiltin(ll::Builtin(std::string("!="), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 2) {
            throw std::runtime_error("too many/few arguments. '!=' expects (only) 2 arguments: " + root->getValue().toErrorMessage());
            exit(1);
        }

        return comparation_result_to_expression(arguments->getList()->at(0).toString() != arguments->getList()->at(1).toString(), root->getValue());
    }));

    this->addBuiltin(ll::Builtin(std::string(">"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 2) {
            throw std::runtime_error("too many/few arguments. '>' expects (only) 2 arguments: " + root->getValue().toErrorMessage());
            exit(1);
        }

        return comparation_result_to_expression(compare_num(1,
                                                            expression_to_comparation_value(arguments->getList()->at(0)),
                                                            expression_to_comparation_value(arguments->getList()->at(1))),
                                                root->getValue());
    }));

    this->addBuiltin(ll::Builtin(std::string("<"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 2) {
            throw std::runtime_error("too many/few arguments. '<' expects (only) 2 arguments: " + root->getValue().toErrorMessage());
            exit(1);
        }

        return comparation_result_to_expression(compare_num(2,
                                                            expression_to_comparation_value(arguments->getList()->at(0)),
                                                            expression_to_comparation_value(arguments->getList()->at(1))),
                                                root->getValue());
    }));

    this->addBuiltin(ll::Builtin(std::string(">="), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 2) {
            throw std::runtime_error("too many/few arguments. '>=' expects (only) 2 arguments: " + root->getValue().toErrorMessage());
            exit(1);
        }

        return comparation_result_to_expression(compare_num(3,
                                                            expression_to_comparation_value(arguments->getList()->at(0)),
                                                            expression_to_comparation_value(arguments->getList()->at(1))),
                                                root->getValue());
    }));

    this->addBuiltin(ll::Builtin(std::string("<="), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 2) {
            throw std::runtime_error("too many/few arguments. '<=' expects (only) 2 arguments: " + root->getValue().toErrorMessage());
            exit(1);
        }

        return comparation_result_to_expression(compare_num(4,
                                                            expression_to_comparation_value(arguments->getList()->at(0)),
                                                            expression_to_comparation_value(arguments->getList()->at(1))),
                                                root->getValue());
    }));

    this->addBuiltin(ll::Builtin(std::string("and"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 2) {
            throw std::runtime_error("too many/few arguments. 'and' expects (only) 2 arguments: " + root->getValue().toErrorMessage());
            exit(1);
        }

        return comparation_result_to_expression(compare_num(5,
                                                            expression_to_comparation_value(arguments->getList()->at(0)),
                                                            expression_to_comparation_value(arguments->getList()->at(1))),
                                                root->getValue());
    }));

    this->addBuiltin(ll::Builtin(std::string("or"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 2) {
            throw std::runtime_error("too many/few arguments. 'or' expects (only) 2 arguments: " + root->getValue().toErrorMessage());
            exit(1);
        }

        return comparation_result_to_expression(compare_num(6,
                                                            expression_to_comparation_value(arguments->getList()->at(0)),
                                                            expression_to_comparation_value(arguments->getList()->at(1))),
                                                root->getValue());
    }));

    this->addBuiltin(ll::Builtin(std::string("not"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 1) {
            throw std::runtime_error("too many/few arguments. 'not' expects (only) 1 argument: " + root->getValue().toErrorMessage());
            exit(1);
        }

        return comparation_result_to_expression(compare_num(7,
                                                            expression_to_comparation_value(arguments->getList()->at(0)),
                                                            expression_to_comparation_value(arguments->getList()->at(0))),
                                                root->getValue());
    }));

    this->addBuiltin(ll::Builtin(std::string("/"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 2) {
            throw std::runtime_error("too many/few arguments. '/' expects (only) 2 arguments: " + root->getValue().toErrorMessage());
            exit(1);
        }

        return num_to_expr((expression_to_comparation_value(arguments->getList()->at(0)) /
                            expression_to_comparation_value(arguments->getList()->at(1))),
                           root->getValue());
    }));

    this->addBuiltin(ll::Builtin(std::string("*"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 2) {
            throw std::runtime_error("too many/few arguments. '*' expects (only) 2 arguments: " + root->getValue().toErrorMessage());
            exit(1);
        }

        return num_to_expr((expression_to_comparation_value(arguments->getList()->at(0)) *
                            expression_to_comparation_value(arguments->getList()->at(1))),
                           root->getValue());
    }));

    this->addBuiltin(ll::Builtin(std::string("+"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() <= 1) {
            throw std::runtime_error("too few arguments. '+' expects at least 1 argument: " + root->getValue().toErrorMessage());
            exit(1);
        }

        BeeNum::Brat result = "0";

        for (usize i = 0; i < arguments->getList()->size(); i++) {
            result += expression_to_comparation_value(arguments->getList()->at(i));
        }

        return num_to_expr(result, root->getValue());
    }));

    this->addBuiltin(ll::Builtin(std::string("-"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 2) {
            throw std::runtime_error("too many/few arguments. '-' expects (only) 2 arguments: " + root->getValue().toErrorMessage());
            exit(1);
        }

        return num_to_expr((expression_to_comparation_value(arguments->getList()->at(0)) -
                            expression_to_comparation_value(arguments->getList()->at(1))),
                           root->getValue());
    }));

    this->addBuiltin(ll::Builtin(std::string("%"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 2) {
            throw std::runtime_error("too many/few arguments. '*' expects (only) 2 arguments: " + root->getValue().toErrorMessage());
            exit(1);
        }

        return num_to_expr(std::stoll(expression_to_comparation_value(arguments->getList()->at(0))) %
            std::stoll(expression_to_comparation_value(arguments->getList()->at(1))),
            root->getValue());
    }));

    this->addBuiltin(ll::Builtin(std::string("concat"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() <= 0) {
            throw std::runtime_error("too few arguments. 'concat' expects at least one argument: " + root->getValue().toErrorMessage());
            exit(1);
        }

        Token copy = root->getValue();
        copy.setValue(std::string("("));
        copy.setType(TokenType::TT_ParenthesesOpen);

        SExpression res = SExpression(copy, ExpressionType::ET_List, root->getTags());
        res.getList()->clear();

        for (SExpression &arg : *arguments->getList()) {
            res.addTags(arg.getTags());

            if (arg.getType() == ET_List) {
                for (SExpression &e : *arg.getList()) {
                    res.addSExpression(e);
                }

            } else {
                res.addSExpression(arg);
            }
        }

        return res;
    }));

    this->addBuiltin(ll::Builtin(std::string("length"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 1) {
            throw std::runtime_error("too many/few arguments. 'printc' expects (only) 1 argument: " + root->getValue().toErrorMessage());
            exit(1);
        }

        return num_to_expr(BeeNum::Brat((*arguments->getList())[0].getList()->size()), root->getValue());
    }));

    this->addBuiltin(ll::Builtin(std::string("printc"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 1) {
            throw std::runtime_error("too many/few arguments. 'printc' expects (only) 1 argument: " + root->getValue().toErrorMessage());
            exit(1);
        }

        std::cout << (char) std::stoi((*arguments->getList())[0].getValue().getNumValue());
        return (*arguments->getList())[0];
    }));

//    this->addBuiltin(ll::Builtin(std::string("print"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
//        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
//        exit(1);
//
//        return ll::SExpression();
//    }));
//
//    this->addBuiltin(ll::Builtin(std::string("println"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
//        throw std::runtime_error("unimplemented builtin: "+root->getValue().toErrorMessage());
//        exit(1);
//
//        return ll::SExpression();
//    }));

    this->addBuiltin(ll::Builtin(std::string("tag"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 2) {
            throw std::runtime_error("too many/few arguments. 'tag' expects (only) 2 arguments: " + root->getValue().toErrorMessage());
            exit(1);
        }

        SExpression expression = (*arguments->getList())[0];
        expression.addTag((*arguments->getList())[1].getValue().toString());

        return expression;
    }));

    this->addBuiltin(ll::Builtin(std::string("tags"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 1) {
            throw std::runtime_error("too many/few arguments. 'tags' expects 1 arguments: " + root->getValue().toErrorMessage());
            exit(1);
        }

        Token copy = root->getValue();
        copy.setValue(std::string("("));
        copy.setType(TokenType::TT_ParenthesesOpen);

        SExpression result = SExpression(copy, ExpressionType::ET_List, root->getTags());
        SExpression expression = (*arguments->getList())[0];
        std::vector<std::string> tags;

        if (expression.getType() == ExpressionType::ET_List) {
            for (SExpression ex : *expression.getList()) {
                for (const std::string &s : ex.getTags()) {
                    if (std::find(tags.begin(), tags.end(), s) == tags.end()) {
                        tags.push_back(s);
                    }
                }
            }

        } else {
            for (const std::string &s : expression.getTags()) {
                if (std::find(tags.begin(), tags.end(), s) == tags.end()) {
                    tags.push_back(s);
                }
            }
        }

        for (const std::string &s : tags) {
            Token copy = root->getValue();
            copy.setValue(s);
            copy.setType(TokenType::TT_Word);

            result.addSExpression(SExpression(copy, ExpressionType::ET_Word, {}));
        }

        return result;
    }));

    this->addBuiltin(ll::Builtin(std::string("each"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 2) {
            throw std::runtime_error("too many/few arguments. 'each' expects (only) 2 arguments: " + root->getValue().toErrorMessage());
            exit(1);
        }

        std::vector<SExpression> list = *(*arguments->getList())[0].getList();
        SExpression lambda = (*arguments->getList())[1];

        Token copy = root->getValue();
        copy.setValue(std::string("("));
        copy.setType(TokenType::TT_ParenthesesOpen);

        SExpression ret = SExpression(copy, ExpressionType::ET_List, {});

        for (SExpression e : list) {
            ret.addSExpression(lambda.runLambda({ e }, runt, root, parent));
        }

        return ret;
    }));

    this->addBuiltin(ll::Builtin(std::string("wordstr"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 1) {
            throw std::runtime_error("too many/few arguments. 'wordstr' expects (only) 1 argument: " + root->getValue().toErrorMessage());
            exit(1);
        }

        std::string s = (*arguments->getList())[0].getValue().toString();

        Token copy = root->getValue();
        copy.setValue(std::string("("));
        copy.setType(TokenType::TT_ParenthesesOpen);

        SExpression string_expr = SExpression(copy, ExpressionType::ET_List, {"str" });
        copy.setType(TokenType::TT_Number);

        for (char c : s) {
            copy.setValue(BeeNum::Brat(c));
            string_expr.addSExpression(SExpression(copy, ExpressionType::ET_Number, {}));
        }

        return string_expr;
    }));

    this->addBuiltin(ll::Builtin(std::string("if"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        SExpression res;

        if (arguments->getList()->size() == 2) {
            bool value = arguments->getList()->at(0).eval(runt, parent).getValue().getNumValue() > BeeNum::Brat("0");

            if (value) {
                res = arguments->getList()->at(1).eval(runt, parent);
            }

        } else if (arguments->getList()->size() == 3) {
            bool value = arguments->getList()->at(0).eval(runt, parent).getValue().getNumValue() > BeeNum::Brat("0");

            if (value) {
                res = arguments->getList()->at(1).eval(runt, parent);
            } else {
                res = arguments->getList()->at(2).eval(runt, parent);
            }

        } else {
            throw std::runtime_error("too many/few arguments. 'range' expects (only) 2 arguments: " + root->getValue().toErrorMessage());
            exit(1);
        }

        if (res.getType() == ExpressionType::ET_Lambda) {
            return res.runLambda({}, runt, root, parent);
        } else {
            return res;
        }

        return ll::SExpression();
    }));

    this->addBuiltin(ll::Builtin(std::string("range"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 2) {
            throw std::runtime_error("too many/few arguments. 'range' expects (only) 2 arguments: " + root->getValue().toErrorMessage());
            exit(1);
        }

        BeeNum::Brat s = (*arguments->getList())[0].getValue().getNumValue();
        BeeNum::Brat e = (*arguments->getList())[1].getValue().getNumValue();

        Token copy = root->getValue();
        copy.setValue(std::string("("));
        copy.setType(TokenType::TT_ParenthesesOpen);

        SExpression expr = SExpression(copy, ExpressionType::ET_List, {});
        copy.setType(TokenType::TT_Number);

        for (usize i = std::stoi(s.point(0)); i <= std::stoi(e.point(0)); i++) {
            copy.setValue(BeeNum::Brat(i));
            expr.addSExpression(SExpression(copy, ExpressionType::ET_Number, {}));
        }

        return expr;
    }));

    this->addBuiltin(ll::Builtin(std::string("head"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 1) {
            throw std::runtime_error("too many/few arguments. 'head' expects (only) 1 argument: " + root->getValue().toErrorMessage());
            exit(1);
        }

        SExpression expr = arguments->getList()->at(0);

        if (expr.getList()->size() < 1) {
            return expr;
        }

        return expr.getList()->at(0);
    }));

    this->addBuiltin(ll::Builtin(std::string("tail"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 1) {
            throw std::runtime_error("too many/few arguments. 'head' expects (only) 1 argument: " + root->getValue().toErrorMessage());
            exit(1);
        }

        SExpression expr = arguments->getList()->at(0);

        if (expr.getList()->size() < 1) {
            return expr;
        }

        expr.getList()->erase(expr.getList()->begin());
        return expr;
    }));

    this->addBuiltin(ll::Builtin(std::string("reverse"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        SExpression expr = arguments->getList()->at(0);
        expr.getList()->clear();

        for (usize i = arguments->getList()->size(); i > 0; i--) {
            expr.getList()->push_back(arguments->getList()->at(arguments->getList()->size() - i - 1));
        }

        return expr;
    }));

    this->addBuiltin(ll::Builtin(std::string("indexof"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
        if (arguments->getList()->size() != 2) {
            throw std::runtime_error("too many/few arguments. 'indexof' expects (only) 2 arguments: " + root->getValue().toErrorMessage());
            exit(1);
        }

        SExpression list = arguments->getList()->at(0);
        SExpression find_element = arguments->getList()->at(1);

        Token copy = root->getValue();
        copy.setValue(BeeNum::Brat(-1));
        copy.setType(TokenType::TT_Number);

        SExpression result = SExpression(copy, ExpressionType::ET_Number, {});
        result.getList()->clear();

        for (usize i = 0; i < list.getList()->size(); i++) {
            if (list.getList()->at(i).toString() == find_element.toString()) {
                copy.setValue(BeeNum::Brat(i));
                return SExpression(copy, ExpressionType::ET_Number, {});
            }
        }

        return result;
    }));

    this->addBuiltin(ll::Builtin(std::string("load"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
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
        SExpression r = Parser::parse(tokens);

        SExpression res;

        for (ll::SExpression ex : *r.getList()) {
            res = ex.eval(runt, parent);
        }

        return res;
    }));

    this->addBuiltin(ll::Builtin(std::string("of"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
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

    this->addBuiltin(ll::Builtin(std::string("fn"), [](Runtime* runt, SExpression* root, Scope* parent, SExpression* arguments) {
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
