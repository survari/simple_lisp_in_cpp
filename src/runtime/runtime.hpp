#ifndef RUNTIME_HPP
#define RUNTIME_HPP

#include <vector>
#include <string>

#include "../builtin/builtin.hpp"

namespace ll {
    class Scope;

    class Runtime {
        std::vector<ll::Builtin> builtins;
        Scope* root;

    public:
        void init();

        const std::vector<ll::Builtin> &getBuiltins() const;
        void addBuiltin(const ll::Builtin &b);
        bool existsBuiltin(const std::string &name) const;
        const ll::Builtin* getBuiltin(const std::string &name) const;
        void setRoot(Scope* s);

        Scope* getRoot();
    };
}

#endif /* RUNTIME_HPP */