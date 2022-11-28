#include <iostream>
#include <regex>

#include "tokenizer.hpp"
#include "../runtime/runtime.hpp"

using namespace ll;

Token::Token(const std::string &filename,
    usize line,
    usize column,
    TokenType type,
    const std::string &value) :
        line(line),
        column(column),
        filename(filename),
        value(value),
        type(type) {}

usize Token::getLine() const {
    return this->line;
}

usize Token::getColumn() const {
    return this->column;
}

const std::string &Token::getFilename() const {
    return this->filename;
}

const std::string &Token::getValue() const {
    return this->value;
}

const TokenType &Token::getType() const {
    return this->type;
}

void Token::setType(TokenType t) {
    this->type = t;
}

std::string Token::toErrorMessage() {
    return std::string(this->getFilename() + ":"
        + std::to_string(this->getLine()) + ":"
        + std::to_string(this->getColumn()) + ": "
        + this->getValue());
}

#define TOKEN(type, value) Token(filename, line, start_column, type, value);
#define PUSH(t, v) if (v != "" && v != " " && v != "\n" && v != "\t") { \
    if (!push(runt, &tokens, Token(filename, line, start_column, t, v))) { \
        std::cout << "error tokenizing: unknown identifier: " << Token(filename, line, start_column, t, v).toErrorMessage() << std::endl; exit(1); \
    } else { \
        value = ""; \
        type = TokenType::TT_Unknown; \
        start_column = column; \
    } \
}

bool is_number(const std::string &value) {
    return std::regex_match(value, std::regex("^(0x[0-9abcdef]+|0b[0-1]+|([1-9]+[0-9]*|0)(\\.[0-9]+|)([eE][1-9]+[0-9]*|))$"));
}

bool is_word(const std::string &value) {
    return std::regex_match(value, std::regex("^[a-zA-Z]+[a-zA-Z0-9-_]*$"));
}

bool is_builtin(const ll::Runtime &runt, const std::string &value) {
    return runt.existsBuiltin(value);
}

bool push(const ll::Runtime &runt, std::vector<Token> *tokens, Token t) {
    if (t.getValue() == "") {
        return true;
    }

    if (t.getType() == TokenType::TT_Unknown) {
        if (is_number(t.getValue())) {
            t.setType(TokenType::TT_Number);

        } else if (is_builtin(runt, t.getValue())) {
            t.setType(TokenType::TT_Builtin);

        } else if (is_word(t.getValue())) {
            t.setType(TokenType::TT_Word);

        } else {
            return false;
        }
    }

    tokens->push_back(t);
    return true;
}

std::vector<Token> Tokenizer::tokenize(const Runtime &runt, const std::string &filename, const std::string &source) {
    std::vector<Token> tokens;

    usize line = 1;
    usize column = 0;
    usize start_column = 0;

    TokenType type = TokenType::TT_Unknown;
    std::string value = "";

    bool comment = false;
    bool long_comment = false;
    bool string = false;

    for (usize i = 0; i < source.size(); i++) {
        column++;

        // handle whitespaces
        while (i < source.size() && (source[i] == ' ' || source[i] == '\n' || source[i] == '\t') && !string) {
            PUSH(type, value);

            if (source[i] == '\n') {
                column = 0;
                line++;
            }

            i++;
            column++;
            start_column = column;
            continue;
        }

        // activate comment
        if (!string && source[i] == '#') {
            PUSH(type, value);

            if (comment) {
                long_comment = true;

            } else {
                comment = true;
            }

            column++;
            continue;
        }

        // handle comments
        if (!string && comment) {
            while (comment && i < source.size()) {
                if (source[i] == '\n') {
                    line++;
                    column = 0;
                    start_column = column;

                    if (!long_comment) {
                        comment = false;
                    }

                } else if (long_comment && source[i] == '#') {
                    if (i+1 < source.size() && source[i+1] == '#') {
                        comment = false;
                        long_comment = false;

                        i++;
                        column++;
                    }
                }

                i++;
                column++;
            }

            i--;
            column--;
            continue;
        }

        // handle strings
        if (source[i] == '\"') {
            if (!string) {
                string = true;
                type = TokenType::TT_String;
            } else {
                string = false;
            }

            continue;
        }

        // handle escape in strings
        if (string && source[i] == '\\' && i+1 < source.size()) {
            value += source[i+1];
            i++;
            column++;
            continue;
        }

        if (!string && source[i] == '(') {
            PUSH(type, value);
            PUSH(TokenType::TT_ParenthesesOpen, std::string("("));
            start_column++;
            continue;
        }

        if (!string && source[i] == ')') {
            PUSH(type, value);
            start_column = column;
            PUSH(TokenType::TT_ParenthesesClose, std::string(")"));
            start_column++;
            continue;
        }

        if (!string && source[i] == '\'') {
            PUSH(type, value);
            start_column = column;
            PUSH(TokenType::TT_Unevaluable, std::string("'"));
            start_column++;
            continue;
        }

        value += source[i];
    }

    if (!comment && !long_comment) {
        PUSH(type, value);
    }

    return tokens;
}

void visualize(const std::vector<Token> &tokens) {
    int indentation = 0;
    bool begin = false;

    for (long long unsigned int i = 0; i < tokens.size(); i++) {
        const ll::Token &t = tokens[i];

        if (begin && t.getType() != ll::TokenType::TT_ParenthesesOpen) {
            begin = false;
        }

        std::cout << (t.getType() == ll::TokenType::TT_String ? "\"" : "") << t.getValue() << (t.getType() == ll::TokenType::TT_String ? "\"" : "");

        if (t.getType() == ll::TokenType::TT_ParenthesesOpen) {
            indentation++;
            begin = true;
            continue;

        } else if (t.getType() == ll::TokenType::TT_ParenthesesClose) {
            if (i+1 < tokens.size() && tokens[i+1].getType() != ll::TokenType::TT_ParenthesesClose) {
                std::cout << std::endl;

                for (int j = 1; !begin && j < indentation; j++)
                    std::cout << "    ";
            }

            indentation--;

            if (indentation == 0)
                std::cout << std::endl;

        } else if (t.getType() != ll::TokenType::TT_Unevaluable
            && (i+1 < tokens.size()
            && tokens[i+1].getType() != ll::TokenType::TT_ParenthesesClose)) {
            std::cout << " ";
        }
    }
}