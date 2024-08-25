#pragma once

#include <vector>

#include "../translator/translator.h"

namespace ast {
    struct statement : public translatable {
        virtual ~statement() = default;
    };

    class statementGrp : public statement {
    private:
        std::vector<statement*> statements;
    public:
        statementGrp(const std::vector<statement*>& statements): statements(statements) {}
        void translate(ITranslator& t) const override;
        ~statementGrp();
    };
    
} // namespace ast
