#include <string>
#include <iostream>
#include <regex>
#include <fstream>

#include "../runtime/runtime.hpp"
#include "../../include/BeeNum/Brat.h"
#include "../../include/BeeNum/Math.h"

#include "tokenizer.hpp"

using namespace ll;

Token::Token():
    line(0),
    column(0),
    filename("<unknown_file>"),
    str_value("<unknown_value>"),
    type(TT_Unknown) {
}

Token::Token(const std::string &filename,
    usize line,
    usize column,
    TokenType type,
    const std::string &value) :
        line(line),
        column(column),
        filename(filename),
        str_value(value),
        type(type) {}

Token::Token(const std::string &filename,
    usize line,
    usize column,
    TokenType type,
    const BeeNum::Brat &value) :
        line(line),
        column(column),
        filename(filename),
        num_value(value),
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

const std::string &Token::getStrValue() const {
    return this->str_value;
}


std::string Token::toString() const {
    if (this->type == TT_Number) {
        return this->num_value.lisp_format_string();
    }

    return this->str_value;
}

const BeeNum::Brat &Token::getNumValue() const {
    return this->num_value;
}

const TokenType &Token::getType() const {
    return this->type;
}

void Token::setType(TokenType t) {
    this->type = t;
}

void Token::setColumn(usize column) {
    this->column = column;
}

void Token::setLine(usize line) {
    this->line = line;
}

void Token::setFilename(std::string filename) {
    this->filename = filename;
}

void Token::setValue(std::string value) {
    this->str_value = value;
}

void Token::setValue(BeeNum::Brat value) {
    this->num_value = value;
}

std::string Token::toErrorMessage() const {
    std::string chars;

    for (char c : this->toString()) {
        chars += std::to_string(c) + " ";
    }

    return std::string(this->getFilename() + ":"
        + std::to_string(this->getLine()) + ":"
        + std::to_string(this->getColumn()) + ": "
        + this->toString() + " [ " + chars + "]");
}

#define TOKEN(type, value) Token(filename, line, start_column, type, value);
#define PUSH(t, v) if (!string && v != "" && v != " " && v != "\n" && v != "\r" && v != "\t") { \
    if (!push(*runt, &tokens, Token(filename, line, start_column, t, v))) { \
        std::cout << "error tokenizing: unknown identifier: " << Token(filename, line, start_column, t, v).toErrorMessage() << std::endl; exit(1); \
    } else { \
        value = ""; \
        type = TokenType::TT_Unknown; \
        start_column = column; \
    } \
}

bool is_int_number(const std::string &value) {
    return std::regex_match(value, std::regex("^([+-]|)(0x[0-9abcdef]+|0b[0-1]+|([1-9]+[0-9]*|0))$"));
}

bool is_number(const std::string &value) {
    return std::regex_match(value, std::regex("^([+-]|)(0x[0-9abcdef]+|0b[0-1]+|([1-9]+[0-9]*|0)(\\.[0-9]+|)([eE](-|)[1-9]+[0-9]*|))$"));
}

bool is_word(const std::string &value) {
    return std::regex_match(value, std::regex("^[a-zA-Z]+[a-zA-Z0-9-_]*(\\.[a-zA-Z]+[a-zA-Z0-9-_]*)*$"));
}

bool is_builtin(const ll::Runtime &runt, const std::string &value) {
    return runt.existsBuiltin(value);
}

bool is_tag(const std::string &value) {
    return std::regex_match(value, std::regex("^:[a-zA-Z]+[a-zA-Z0-9-_]*$"));
}

BeeNum::Brat ll::parse_number(const std::string &value) {
    // 1234.5678e9
    std::string base = "";      // 1234             m=0
    std::string decimal = "";   // 5678             m=1
    int exponent = 0;           // 9                m=2

    std::string tmp = "";
    int mode = 0; // mode = m

    for (char c : value) {
        if (c == '.') {
            base = tmp;
            tmp = "";

            mode = 1; // decimal is next
            continue;
        }

        if (c == 'e' || c == 'E') {
            if (mode == 0) {
                base = tmp;
                tmp = "";

            } else if (mode == 1) {
                decimal = tmp;
                tmp = "";
            }

            mode = 2;
            continue;
        }

        tmp += c;
    }

    if (tmp != "") {
        if (mode == 1) {
            decimal = tmp;
        } else if (mode == 2) {
            exponent = std::stoi(tmp);
        } else {
            base = tmp;
        }
    }

    if (exponent < 0) {
        return (BeeNum::Brat(base+decimal)) / BeeNum::Math::pow(BeeNum::Brat(10), decimal.size()) / BeeNum::Math::pow(BeeNum::Brat(10), abs(exponent));
    } else {
        return (BeeNum::Brat(base+decimal)) / BeeNum::Math::pow(BeeNum::Brat(10), decimal.size()) * BeeNum::Math::pow(BeeNum::Brat(10), exponent);
    }
}

bool push(const ll::Runtime &runt, std::vector<Token> *tokens, Token t) {
    if (t.getStrValue() == "" && t.getType() != TokenType::TT_String) {
        return true;
    }

    if (t.getType() == TokenType::TT_Unknown) {
        if (is_number(t.toString())) {
            t.setType(TokenType::TT_Number);
            t.setValue(ll::parse_number(t.getStrValue()));

        } else if (is_builtin(runt, t.toString())) {
            t.setType(TokenType::TT_Builtin);

        } else if (is_word(t.toString())) {
            t.setType(TokenType::TT_Word);

        } else if (is_tag(t.toString())) {
            t.setValue(t.toString().substr(1, t.toString().size()));
            t.setType(TokenType::TT_Tag);

        } else {
            return false;
        }
    }

    tokens->push_back(t);
    return true;
}

std::vector<Token> Tokenizer::tokenize(Runtime *runt, const std::string &filename, const std::string &source) {
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
        while (!string && i < source.size() && (source[i] == ' ' || source[i] == '\n' || source[i] == '\r' || source[i] == '\t')) {
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
                tokens.push_back(Token(filename, line, start_column, TokenType::TT_String, value));
                value = "";
                type = TokenType::TT_Unknown;
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

        std::cout << (t.getType() == ll::TokenType::TT_String ? "\"" : "") << t.toString() << (t.getType() == ll::TokenType::TT_String ? "\"" : "");

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

std::string ll::read_file(const std::string &filename) {
    std::ifstream t(filename);
    std::stringstream buffer;

    if (!t.is_open()) {
        throw std::runtime_error("file not found: "+filename);
    }

    buffer << t.rdbuf();
    return buffer.str();
}