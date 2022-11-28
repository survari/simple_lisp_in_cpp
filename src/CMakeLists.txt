include_directories(../include/)

file(GLOB slicpp_SRC
    "*/*.cpp"
    "*.cpp"
)

add_executable(slicpp

    ../include/BeeNum/Bint.cpp
    ../include/BeeNum/Bint/arithmetics.cpp
    ../include/BeeNum/Bint/bitoperations.cpp
    ../include/BeeNum/Bint/comparison.cpp
    ../include/BeeNum/Bint/io.cpp
    ../include/BeeNum/Bint/shifts.cpp

    ../include/BeeNum/Brat.cpp
    ../include/BeeNum/Brat/arithmetics.cpp
    ../include/BeeNum/Brat/comparison.cpp
    ../include/BeeNum/Brat/io.cpp

    ../include/BeeNum/Math.cpp

    ${slicpp_SRC})

# add_executable(slicpp
#     main.cpp

#     tokenizer/tokenizer.cpp
#     runtime/runtime.cpp
#     expression/expression.cpp
#     runtime/runtime.cpp
#     builtin/builtin.cpp

#     ../include/BeeNum/Bint.cpp
#     ../include/BeeNum/Bint/arithmetics.cpp
#     ../include/BeeNum/Bint/bitoperations.cpp
#     ../include/BeeNum/Bint/comparison.cpp
#     ../include/BeeNum/Bint/io.cpp
#     ../include/BeeNum/Bint/shifts.cpp

#     ../include/BeeNum/Brat.cpp
#     ../include/BeeNum/Brat/arithmetics.cpp
#     ../include/BeeNum/Brat/comparison.cpp
#     ../include/BeeNum/Brat/io.cpp

#     ../include/BeeNum/Math.cpp)

if(MSVC)
    target_compile_options(slicpp PRIVATE /W4 /WX)
else()
    target_compile_options(slicpp PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-unused-variable -Wno-unused-parameter)
endif()