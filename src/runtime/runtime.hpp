#ifndef RUNTIME_HPP
#define RUNTIME_HPP

#include <vector>
#include <string>

#include "../builtin/builtin.hpp"

namespace ll {
    class Runtime {
        std::vector<ll::Builtin> builtins;

    public:
        void init();

        const std::vector<ll::Builtin> &getBuiltins() const;
        void addBuiltin(const ll::Builtin &b);
        bool existsBuiltin(const std::string &name) const;
        const ll::Builtin* getBuiltin(const std::string &name) const;
    };
}

#endif /* RUNTIME_HPP */