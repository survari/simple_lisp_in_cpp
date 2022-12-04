#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "../include/BeeNum/Brat.h"
#include "../include/BeeNum/Math.h"

#include "tokenizer/tokenizer.hpp"
#include "parser/parser.hpp"
#include "runtime/runtime.hpp"
#include "variable/scope.hpp"

int main(int argc, char* argv[]) {
    std::string filename = argv[argc-1];
    std::string source = ll::read_file(filename);

    ll::Scope root_scope = ll::Scope();
    ll::Runtime runt = ll::Runtime();
    runt.init();

    std::vector<ll::Token> tokens = ll::Tokenizer::tokenize(&runt, filename, source);
    ll::SExpression root = ll::Parser::parse(tokens);

    for (ll::SExpression ex : *root.getList()) {
//        std::cout << "------------------------ EVAL" << std::endl; ex.visualize();
//        std::cout << "EVAL ------------------------" << std::endl;
        ex.eval(&runt, &root_scope);
    }
}