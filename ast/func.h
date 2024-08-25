#pragma once

#include "ast.h"
#include <string>

namespace ast{
    class funcDecl : public statement {
        std::string name;
        statementGrp *body;
    public:
        funcDecl(const std::string& name, statementGrp* body) :name(name), body(body) {}
        void translate(ITranslator& t) const override;
        ~funcDecl();
    };
   
    struct funcCall : public statement{
        std::string name;
        funcCall(const std::string& name) :name(name) {}
        void translate(ITranslator& t) const override;
    };
    
} // namespace ast

