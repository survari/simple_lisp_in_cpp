#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <iostream>
#include <vector>

#include "../../include/ll_std.h"
#include "../../include/BeeNum/Brat.h"
#include "../runtime/runtime.hpp"

namespace ll {
    enum TokenType {
        TT_Word,
        TT_Number,
        TT_Builtin,
        TT_String,
        TT_ParenthesesOpen,
        TT_ParenthesesClose,
        TT_Tag,
        TT_Unknown,
        TT_Unevaluable
    };

    class Token {
    private:
        usize line;
        usize column;

        std::string filename;
        std::string str_value;
        BeeNum::Brat num_value;

        TokenType type;

    public:
        Token();

        Token(const std::string &filename,
            usize line,
            usize column,
            TokenType type,
            const std::string &value);

        Token(const std::string &filename,
            usize line,
            usize column,
            TokenType type,
            const BeeNum::Brat &value);

        void setType(TokenType t);
        void setColumn(usize column);
        void setLine(usize line);
        void setFilename(std::string filename);
        void setValue(std::string value);
        void setValue(BeeNum::Brat value);

        usize getLine() const;
        usize getColumn() const;
        const std::string &getFilename() const;
        const std::string &getStrValue() const;
        std::string toString() const;
        const BeeNum::Brat &getNumValue() const;
        const TokenType &getType() const;
        std::string toErrorMessage() const;
    };

    class Tokenizer {
    public:
        static std::vector<Token> tokenize(const Runtime &runt, const std::string &filename, const std::string &source);
        void visualize(const std::vector<Token> &tokens);
    };
}

#endif /* TOKENIZER_HPP */