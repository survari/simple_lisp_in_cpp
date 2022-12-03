#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <vector>
#include "../tokenizer/tokenizer.hpp"

namespace ll {
    class Scope;

    enum ExpressionType {
        // Unevaluable
        ET_Word,        // like 'a. is a name, will not be evaluated!
        ET_Lambda,      // (fn (a b c) (debug (a b c))
        ET_Number,      // eval(1) => 1

        // Evaluable
        ET_Identifier,  // is a variable, or a nested call like a.b, person.name, ...,
                        // like if a = (:b 1 :c 2) then: eval(a) => (:b 1 :c 2) BUT eval(a.b) => 1
        ET_Instruction, // means that the first element in list is the name of a lambda-variable/builtin
        ET_List,        // because each element gets evaluated. if a=1, b=2, c=3 then: eval(a b c) => (1 2 3)

        // Unknown Type
        ET_Unknown
    };

    class SExpression {
        // List* list;
        Token value;
        std::vector<SExpression> list;
        std::vector<std::string> tags;
        ExpressionType type;
        Scope* scope;

    public:
        SExpression();
        SExpression(const Token &value, ExpressionType type, const std::vector<std::string> &tags, Scope* parent_scope);
        SExpression(ExpressionType type);

        SExpression* addSExpression(SExpression v);
        SExpression* addTag(std::string tag);
        SExpression* addTags(const std::vector<std::string> &tag);
        void setType(ExpressionType type);

        bool hasTag(const std::string &tag) const;
        bool includesTagRecursive(const std::string &tag) const;
        const std::vector<std::string> &getTags() const;
        std::vector<std::string> getTagsRecursive() const;
        ExpressionType getType() const;
        const Token &getValue() const;
        Scope *getScope();

        bool isEvaluable() const;
        std::vector<SExpression> *getList();
        SExpression eval(Runtime* runtime, SExpression* parent);

        void visualize(int indentation=0) const;

        SExpression getTaggedValue(std::string tag);

        bool hasTaggedValue(std::string tag);

        void setScope(Scope* s);

        SExpression runLambda(std::vector<SExpression> arguments, Runtime* runt, SExpression* parent);
    };
}

#endif /* EXPRESSION_HPP */