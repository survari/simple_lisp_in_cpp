#ifndef BUILTIN_HPP
#define BUILTIN_HPP

#include <functional>
#include <string>

namespace ll {
    class SExpression;
    class Runtime;

    class Builtin {
    private:
        std::function<SExpression(Runtime* runtime, SExpression* root, SExpression* parent, SExpression* arguments)> lambda;
        std::string name;

    public:
        Builtin(const std::string &name, std::function<SExpression(Runtime* runtime, SExpression* root, SExpression* parent, SExpression* arguments)> lambda);

        const std::string &getName() const;
        const std::function<SExpression(Runtime* runtime, SExpression* root, SExpression* parent, SExpression* arguments)> &getLambda() const;
    };
}

#endif /* BUILTIN_HPP */