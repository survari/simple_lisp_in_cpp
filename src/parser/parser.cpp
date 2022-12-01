#include "parser.hpp"
#include "../expression/expression.hpp"
#include "../../include/ll_std.h"
#include "../../include/BeeNum/Brat.h"

using namespace ll;

SExpression parse_expression(const std::vector<Token> &tokens,
    usize* index,
    usize indentation_level,
    std::vector<std::string> expression_tags,
    ExpressionType type) {

    SExpression expression;
    std::vector<std::string> current_tags;

    expression.setType(type);
    expression.addTags(expression_tags);

    bool is_unevaluable;

    /* TODO:
        - :tags - DONE
        - 'unevaluable ('() => list, 'a => word)
        - "strings" - DONE
        - identifiers (a => (:b 1 :c 2), a.b => 1)
    */

    for (; (*index) < tokens.size(); (*index)++) {
        if (is_unevaluable) {
            // TODO: check for parentheses => list / word => word
            if (tokens[*index].getType() == TokenType::TT_ParenthesesOpen) {
                type = ET_List;

            } else if (tokens[*index].getType() == TokenType::TT_Word) { // 'word is actually a word, not an identifier!
                type = ET_Word;
                
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
            current_tags.push_back(tokens[*index].getStrValue());
            continue;
        }

        if (tokens[*index].getType() == TokenType::TT_ParenthesesOpen) {
            (*index)++;

            if (type == ExpressionType::ET_Unknown) {
                type = ExpressionType::ET_Instruction;
            }

            expression.addSExpression(parse_expression(tokens, index, indentation_level+1, current_tags, type));
            current_tags.clear();
            type = ExpressionType::ET_Unknown;

        } else if (tokens[*index].getType() == TokenType::TT_ParenthesesClose) {
            return expression;

        } else if (tokens[*index].getType() == TokenType::TT_String) {
            SExpression list = SExpression(ExpressionType::ET_List);
            list.addTag("str");

            for (usize c : tokens[*index].toString()) {
                Token t = tokens[*index];

                t.setValue(BeeNum::Brat(c));
                t.setType(TT_Number);

                list.addSExpression(SExpression(t, ExpressionType::ET_Number));
            }

            expression.addSExpression(list);
            current_tags.clear();
            type = ExpressionType::ET_Unknown;


        } else if (tokens[*index].getType() == TokenType::TT_Word) {
            if (type == ExpressionType::ET_Unknown) {
                type = ExpressionType::ET_Identifier;
            }

            expression.addSExpression(SExpression(tokens[*index], type));
            current_tags.clear();
            type = ExpressionType::ET_Unknown;

        } else if (tokens[*index].getType() == TokenType::TT_Number) {
            type = ExpressionType::ET_Number;
            expression.addSExpression(SExpression(tokens[*index], type));
            current_tags.clear();
            type = ExpressionType::ET_Unknown;

        }  else if (tokens[*index].getType() == TokenType::TT_Builtin) {
            type = ExpressionType::ET_Identifier;
            expression.addSExpression(SExpression(tokens[*index], type));
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

SExpression Parser::parse(std::vector<Token> tokens) {
    usize index = 0;
    SExpression e = parse_expression(tokens, &index, 0, { "root" }, ExpressionType::ET_List);

    if (index < tokens.size()) {
        throw std::runtime_error(std::string("parse_expression: error: too many parantheses after ") +
                tokens[index].toErrorMessage());
    }

    return e;
}