#pragma once

#include "ast.h"
#include "expr.h"
#include <vector>

namespace ast {
    struct branch {
        expr *condition = nullptr;
        statementGrp *body = nullptr;
    };
    
    class cycle : public statement {
    private:
        branch b;
    public:
        cycle(expr *c, statementGrp* b): b{c,b} {}
        void translate(ITranslator& t) const override;
        ~cycle();
    };
    class switcher : public statement {
    private:
        std::vector<branch> branches;
    public:
        switcher(const std::vector<branch>& branches): branches(branches) {}
        void translate(ITranslator& t) const override;
        ~switcher();
    };

} // namespace ast


