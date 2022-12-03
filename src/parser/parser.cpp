#include "parser.hpp"
#include "../variable/scope.hpp"
#include "../expression/expression.hpp"
#include "../../include/ll_std.h"
#include "../../include/BeeNum/Brat.h"

using namespace ll;

SExpression build_variable_call(usize* index, const std::vector<std::string> names, Token root, SExpression *parent) {
    if (names.size() == 1) {
        return SExpression(root, ExpressionType::ET_Identifier, {}, parent->getScope());
    }

    if (*index < names.size() && (*index)+2 < names.size()) {
        Token copy = root;
        copy.setType(TokenType::TT_Builtin);
        copy.setValue(std::string("of"));

        SExpression e = SExpression(copy, ExpressionType::ET_Instruction, {}, parent->getScope());
        e.addSExpression(SExpression(copy, ExpressionType::ET_Identifier, {}, parent->getScope()));
        copy.setValue(names[names.size() - (*index) - 1]);

        (*index)++;
        e.addSExpression(build_variable_call(index, names, root, &e));

        copy.setType(TokenType::TT_Word);
        e.addSExpression(SExpression(copy, ExpressionType::ET_Word, {}, parent->getScope()));
        return e;
    }

    Token copy = root;
    copy.setType(TokenType::TT_Builtin);
    copy.setValue(std::string("of"));

    SExpression e = SExpression(copy, ExpressionType::ET_Instruction, {}, parent->getScope());
    e.addSExpression(SExpression(copy, ExpressionType::ET_Identifier, {}, parent->getScope()));

    copy.setType(TokenType::TT_Word);

    copy.setValue(names[names.size() - (*index) - 2]);
    e.addSExpression(SExpression(copy, ExpressionType::ET_Identifier, {}, parent->getScope()));

    copy.setValue(names[names.size() - (*index) - 1]);
    e.addSExpression(SExpression(copy, ExpressionType::ET_Word, {}, parent->getScope()));

    return e;
}

SExpression parse_expression(const std::vector<Token> &tokens,
    usize* index,
    usize indentation_level,
    std::vector<std::string> expression_tags,
    ExpressionType type,
    Scope* parent) {

    SExpression expression;
    std::vector<std::string> current_tags;

    if (*index < tokens.size()) {
        expression = SExpression(tokens[*index], ET_Unknown, {}, parent);
    }

    expression.setType(type);
    expression.addTags(expression_tags);
    expression.getScope()->setParent(parent);

    bool is_unevaluable = false;

    for (; (*index) < tokens.size(); (*index)++) {
        if (is_unevaluable) { // check for apostroph to make things unevaluable
            if (tokens[*index].getType() == TokenType::TT_ParenthesesOpen) {
                type = ET_List;

            } else if (tokens[*index].getType() == TokenType::TT_Word) { // 'word is actually a word, not an identifier!
                type = ET_Word;

                if (expression.getList()->size() == 0 && expression.getType() == ExpressionType::ET_Instruction) {
                    expression.setType(ET_List);
                }

            } else {
                throw std::runtime_error("error: could not make symbol unevaluable: "+tokens[*index].toErrorMessage());
                exit(1);
            }

            is_unevaluable = false;
        }

        if (tokens[*index].getType() == TokenType::TT_Unevaluable) {
            is_unevaluable = true;
            continue;
        }

        if (tokens[*index].getType() == TokenType::TT_Tag) {
            if (expression.getList()->size() == 0 && expression.getType() == ExpressionType::ET_Instruction) {
                expression.setType(ET_List);
            }

            current_tags.push_back(tokens[*index].getStrValue());
            continue;
        }

        if (tokens[*index].getType() == TokenType::TT_ParenthesesOpen) {
            (*index)++;

            if (type == ExpressionType::ET_Unknown) {
                type = ExpressionType::ET_Instruction;
            }

            expression.addSExpression(parse_expression(tokens,
               index,
               indentation_level+1,
               current_tags,
               type,
               expression.getScope()));

            current_tags.clear();
            type = ExpressionType::ET_Unknown;

        } else if (tokens[*index].getType() == TokenType::TT_ParenthesesClose) {
            if (expression.getType() == ExpressionType::ET_Instruction && expression.getList()->size() == 0) {
                expression.setType(ExpressionType::ET_List);
            }

            return expression;

        } else if (tokens[*index].getType() == TokenType::TT_String) {
            if (expression.getList()->size() == 0 && expression.getType() == ExpressionType::ET_Instruction) {
                expression.setType(ET_List);
            }

            SExpression list = SExpression(tokens[*index], ExpressionType::ET_List, current_tags, parent);
            list.addTag("str");

            for (usize c : tokens[*index].toString()) {
                Token t = tokens[*index];

                t.setValue(BeeNum::Brat(c));
                t.setType(TT_Number);

                list.addSExpression(SExpression(t, ExpressionType::ET_Number, {}, parent));
            }

            expression.addSExpression(list);
            current_tags.clear();
            type = ExpressionType::ET_Unknown;


        } else if (tokens[*index].getType() == TokenType::TT_Word) {
            if (type == ExpressionType::ET_Unknown) {
                // convert a.b.c.d (EXPRESSION) => (of (of (of a 'b) 'c) 'd) (EXPRESSION)
                // go backwards => d c b a, add parentheses and of => (of (of (of a 'b) 'c) 'd)
                usize i = 0;

                std::string tmp;
                std::vector<std::string> names;

                for (char c : tokens[*index].getStrValue()) {
                    if (c == '.') {
                        names.push_back(tmp);
                        tmp = "";
                        continue;
                    }

                    tmp += c;
                }

                names.push_back(tmp);

                SExpression ex = build_variable_call(&i, names, tokens[*index], &expression);
                ex.addTags(current_tags);
                expression.addSExpression(ex);

            } else {
                expression.addSExpression(SExpression(tokens[*index], type, current_tags, parent));
            }

            current_tags.clear();
            type = ExpressionType::ET_Unknown;

        } else if (tokens[*index].getType() == TokenType::TT_Number) {
            if (expression.getList()->size() == 0 && expression.getType() == ExpressionType::ET_Instruction) {
                expression.setType(ET_List);
            }

            type = ExpressionType::ET_Number;
            expression.addSExpression(SExpression(tokens[*index], type, current_tags, parent));
            current_tags.clear();
            type = ExpressionType::ET_Unknown;

        }  else if (tokens[*index].getType() == TokenType::TT_Builtin) {
            type = ExpressionType::ET_Identifier;
            expression.addSExpression(SExpression(tokens[*index], type, current_tags, parent));
            current_tags.clear();
            type = ExpressionType::ET_Unknown;

        } else {
            throw std::runtime_error("error: could not parse symbol: "+tokens[*index].toErrorMessage());
            exit(1);
        }
    }

    if (indentation_level == 0)
        return expression;

    throw std::runtime_error(std::string("parse_expression: error: too few parantheses after ") +
        ((*index >= tokens.size())
            ? tokens[tokens.size()-1].toErrorMessage()
            : tokens[*index].toErrorMessage()));

    exit(1);
}

SExpression Parser::parse(std::vector<Token> tokens, Scope* root_scope) {
    usize index = 0;
    SExpression e = parse_expression(tokens, &index, 0, { "root" }, ExpressionType::ET_Unknown, root_scope);

    if (index < tokens.size()) {
        throw std::runtime_error(std::string("parse_expression: error: too many parantheses after ") +
                tokens[index].toErrorMessage());
    }

    return e;
}