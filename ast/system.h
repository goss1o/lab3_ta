#pragma once

#include "ast.h"
#include "expr.h"

namespace sys {
    struct print : public ast::statement {
        ast::expr *to_print;
        print(ast::expr *to_print): to_print(to_print) {}
        void translate(ITranslator& t) const override {
            t.print(to_print);
        }
        ~print() {delete to_print;}
    };
} // namespace sys
