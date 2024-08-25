
#pragma once

#include "ast.h"
#include "expr.h"
#include "../own_system.h"

#include <string>

class mytype;

namespace ast {     
    struct varDecl : public statement {
        std::string name;
        bool is_const;
        own_types::Types type;
        values* start_val;
        varDecl(const std::string& name, own_types::Types type, values* start_val = nullptr, bool is_const = false);
        void translate(ITranslator& t) const override;
        ~varDecl();
    };
    struct arrayDecl: public varDecl {
        std::vector<expr*> dimentions;
        arrayDecl(const std::string& name, own_types::Types type, const std::vector<expr*>& dimentions, values* start_val = nullptr, bool is_const = false);
        void translate(ITranslator& t) const override;
        ~arrayDecl();
    };

    // class varDeclGrp : public statement {
    // private:
    //     std::vector<varDecl*> statements;
    // public:
    //     varDeclGrp(const std::vector<statement*>& statements);
    //     ~varDeclGrp();
    // };


    class varGet: public expr {
    protected:
        std::string name;
    public:
        varGet(const std::string& name): name(name) {}
        void translate(ITranslator& t) const override;
    };

    class arrGet: public varGet {
        std::vector<expr*> indexes;
    public:
        arrGet(const std::string& name, const std::vector<expr*>& indexes): varGet(name), indexes(indexes) {}
        void translate(ITranslator& t) const override;
        ~arrGet();
    };

    class assigning : public statement{
        expr *value;
        std::vector<varGet*> to;
    public:
        assigning(expr *value, const std::vector<statement*>& to);
        void translate(ITranslator& t) const override;
        ~assigning();
    };

} // namespace ast