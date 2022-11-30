#include "parser.hpp"
#include "../expression/expression.hpp"
#include "../../include/ll_std.h"

using namespace ll;

SExpression parse_expression(const std::vector<Token> &tokens, usize* index, usize indentation_level) {
    SExpression expression;

    /* TODO:
        - :tags
        - 'unevaluable
        - "strings"
    */

    for (; (*index) < tokens.size(); (*index)++) {
        if (tokens[*index].getType() == TokenType::TT_ParenthesesOpen) {
            (*index)++;
            expression.addSExpression(parse_expression(tokens, index, indentation_level+1));

        } else if (tokens[*index].getType() == TokenType::TT_ParenthesesClose) {
            return expression;

        } else {
            expression.addSExpression(SExpression(tokens[*index]));
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
    SExpression e = parse_expression(tokens, &index, 0);

    if (index < tokens.size()) {
        throw std::runtime_error(std::string("parse_expression: error: too many parantheses after ") +
                tokens[index].toErrorMessage());
    }

    return e;
}