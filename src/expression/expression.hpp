#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <vector>
#include "../tokenizer/tokenizer.hpp"

namespace ll {
    class Scope;
    class SExpression;

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

    public:
        SExpression();
        SExpression(const Token &value, ExpressionType type, const std::vector<std::string> &tags);
        SExpression(ExpressionType type);

        SExpression* addSExpression(SExpression v);
        SExpression* addTag(std::string tag);
        SExpression* addTags(const std::vector<std::string> &tag);
        void setType(ExpressionType type);

        bool hasTag(const std::string &tag) const;
        bool includesTagRecursive(const std::string &tag) const;
        const std::vector<std::string> &getTags() const;
        std::vector<std::string> getTagsRecursive() const;
        bool hasTaggedValue(std::string tag);
        SExpression getTaggedValue(std::string tag);

        ExpressionType getType() const;
        const Token &getValue() const;

        bool isEvaluable() const;
        std::vector<SExpression> *getList();
        SExpression eval(Runtime* runtime, Scope* scope);

        void visualize(int indentation=0) const;
        std::string toString(int indentation = 0) const;

        SExpression runLambda(std::vector<SExpression> arguments, Runtime* runt, SExpression* parent, Scope* parent_scope);
    };
}

#endif /* EXPRESSION_HPP */