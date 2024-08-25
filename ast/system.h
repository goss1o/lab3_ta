#pragma once

#include "ast.h"
#include "var.h"

namespace sys {
    struct print : public ast::statement {
        ast::varGet *to_print;
        print(ast::varGet *to_print): to_print(to_print) {}
        void translate(ITranslator& t) const override {
            t.print(to_print);
        }
        ~print() {delete to_print;}
    };
} // namespace sys
