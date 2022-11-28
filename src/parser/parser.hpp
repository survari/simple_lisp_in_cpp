#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>

#include "../tokenizer/tokenizer.hpp"
#include "../expression/expression.hpp"

namespace ll {
    class Parser {
    public:
        static SExpression parse(std::vector<Token> tokens);
    };
}

#endif /* PARSER_HPP */