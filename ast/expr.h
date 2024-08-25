#pragma once

#include "ast.h"
#include "../own_system.h"
#include <vector>

namespace ast {

    struct expr : public statement {};

    class addExpr : public expr {
    private:
        std::vector<expr*> expressions;
    public:
        addExpr(const std::vector<expr*>& expressions): expressions(expressions) {}
        void translate(ITranslator& t) const override;
        ~addExpr();
    };
    class mulExpr : public expr {
    private:
        std::vector<expr*> expressions;
    public:
        mulExpr(const std::vector<expr*>& expressions): expressions(expressions) {}
        void translate(ITranslator& t) const override;
        ~mulExpr();
    };
    class orExpr : public expr {
    private:
        std::vector<expr*> expressions;
    public:
        orExpr(const std::vector<expr*>& expressions): expressions(expressions) {}
        void translate(ITranslator& t) const override;
        ~orExpr();
    };
    class andExpr : public expr {
    private:
        std::vector<expr*> expressions;
    public:
        andExpr(const std::vector<expr*>& expressions): expressions(expressions) {}
        void translate(ITranslator& t) const override;
        ~andExpr();
    };

    class minExpr : public expr {
    private:
        std::vector<expr*> expressions;
    public:
        minExpr(const std::vector<expr*>& expressions): expressions(expressions) {}
        void translate(ITranslator& t) const override;
        ~minExpr();
    };
    class maxExpr : public expr {
    private:
        std::vector<expr*> expressions;
    public:
        maxExpr(const std::vector<expr*>& expressions): expressions(expressions) {}
        void translate(ITranslator& t) const override;
        ~maxExpr();
    };
    class eqExpr : public expr {
    private:
        std::vector<expr*> expressions;
    public:
        eqExpr(const std::vector<expr*>& expressions): expressions(expressions) {}
        void translate(ITranslator& t) const override;
        ~eqExpr();
    };

    class notExpr : public expr {
    private:
        expr* expression;
    public:
        notExpr(expr* expression): expression(expression) {}
        void translate(ITranslator& t) const override;
        ~notExpr();
    };

    class subExpr : public expr {
    private:
        expr* l;
        expr* r;
    public:
        subExpr(expr* l, expr *r): l(l), r(r) {}
        void translate(ITranslator& t) const override;
        ~subExpr();
    };
    class divExpr : public expr {
    private:
        expr* l;
        expr* r;
    public:
        divExpr(expr* l, expr *r): l(l), r(r) {}
        void translate(ITranslator& t) const override;
        ~divExpr();
    };

    class literal: public expr {
    private:
        own_types::value v;
    public:
        literal(own_types::value v): v(v) {}
        void translate(ITranslator& t) const override;
        ~literal() = default;
    };

    struct values : public expr {
        std::vector<expr*> values_;
        values (const std::vector<expr*>& values): values_(values) {}
        void translate(ITranslator& t) const override;
        ~values();
    };
}