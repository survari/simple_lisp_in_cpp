cmake_minimum_required(VERSION 3.21)
project(lisp_like)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(include/BeeNum)
include_directories(src/builtin)
include_directories(src/expression)
include_directories(src/llist)
include_directories(src/parser)
include_directories(src/runtime)
include_directories(src/tokenizer)
include_directories(src/variable)

add_executable(lisp_like
        include/BeeNum/Bint/arithmetics.cpp
        include/BeeNum/Bint/bitoperations.cpp
        include/BeeNum/Bint/comparison.cpp
        include/BeeNum/Bint/io.cpp
        include/BeeNum/Bint/shifts.cpp
        include/BeeNum/Brat/arithmetics.cpp
        include/BeeNum/Brat/comparison.cpp
        include/BeeNum/Brat/io.cpp
        include/BeeNum/Bint.cpp
        include/BeeNum/Bint.h
        include/BeeNum/Brat.cpp
        include/BeeNum/Brat.h
        include/BeeNum/Math.cpp
        include/BeeNum/Math.h
        include/ll_std.h
        src/builtin/builtin.cpp
        src/builtin/builtin.hpp
        src/expression/expression.cpp
        src/expression/expression.hpp
        src/llist/helpers.cpp
        src/llist/helpers.hpp
        src/llist/list.cpp
        src/llist/list.hpp
        src/parser/parser.cpp
        src/parser/parser.hpp
        src/runtime/runtime.cpp
        src/runtime/runtime.hpp
        src/tokenizer/tokenizer.cpp
        src/tokenizer/tokenizer.hpp
        src/variable/scope.cpp
        src/variable/scope.hpp
        src/variable/variable.cpp
        src/variable/variable.hpp
        src/main.cpp)
