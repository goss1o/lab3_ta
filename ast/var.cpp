#include "var.h"

ast::varDecl::varDecl(const std::string &name, own_types::Types type, values *start_val, bool is_const)
    : name(name), is_const(is_const), type(type), start_val(start_val) {}

void ast::varDecl::translate(ITranslator &t) const {
    t.declareVariable(type, name, start_val);
}

ast::varDecl::~varDecl() {
    delete start_val;
}

ast::arrayDecl::arrayDecl(const std::string &name, own_types::Types type, const std::vector<expr*> &dimentions, values *start_val, bool is_const)
    : varDecl(name, type, start_val, is_const), dimentions(dimentions) {}

void ast::arrayDecl::translate(ITranslator &t) const {
    auto dim = std::vector<translatable*>(dimentions.size());
    for (size_t i = 0; i < dim.size(); i++) {
        dim[i] = dimentions[i];
    }
    
    t.declareArrayVariable(type, dim, name, start_val);
}

ast::arrayDecl::~arrayDecl() {
    for (auto& i:dimentions)
        delete i;
}

void ast::varGet::translate(ITranslator &t) const {
    t.getVariable(name);
}

void ast::arrGet::translate(ITranslator &t) const {
    auto ind = std::vector<translatable*>(indexes.size());
    for (size_t i = 0; i < ind.size(); i++) {
        ind[i] = indexes[i];
    }
    t.getArray(name, ind);
}

ast::arrGet::~arrGet(){
    for (auto& i:indexes)
        delete i;
}

// ast::varDeclGrp::varDeclGrp(const std::vector<statement *> &statements) {
    
// }

// ast::varDeclGrp::~varDeclGrp()
// {
//     for (auto& i: statements)
//         delete i;
// }

ast::assigning::assigning(expr *value, const std::vector<statement *> &to): value(value) {
    this->to.reserve(to.size());
    for (auto&i : to) {
        this->to.push_back(dynamic_cast<varGet*>(i));
    }
}

void ast::assigning::translate(ITranslator &t) const
{
    for (size_t i = 0; i < to.size(); i++) {
        to[i]->translate(t);
        t.assign();
    }
    value->translate(t);
}

ast::assigning::~assigning() {
    delete value;
    for (auto& i: to)
        delete i;
}
