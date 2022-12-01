#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "../include/BeeNum/Brat.h"
#include "../include/BeeNum/Math.h"

#include "tokenizer/tokenizer.hpp"
#include "parser/parser.hpp"
#include "runtime/runtime.hpp"

std::string read_file(const std::string &filename) {
    std::ifstream t(filename);
    std::stringstream buffer;

    buffer << t.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[]) {
    BeeNum::Brat a(34534534, "9");
    a *= 2;
    a = BeeNum::Math::pow(a, 10);
    std::cout << a << std::endl;

    std::string filename = argv[argc-1];
    std::string source = read_file(filename);

    ll::Runtime runt = ll::Runtime();
    runt.init();

    std::vector<ll::Token> tokens = ll::Tokenizer::tokenize(runt, filename, source);
    ll::SExpression root = ll::Parser::parse(tokens);
    
    for (ll::SExpression ex : *root.getList()) {
        std::cout << "EVAL ------------------------" << std::endl;
        ex.visualize();
        std::cout << std::endl;
        ex.eval();
    }
}