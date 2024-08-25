#include "expr.h"

void ast::addExpr::translate(ITranslator &t) const {
    std::vector<translatable*>temp;
    temp.reserve(expressions.size());
    for (auto &i : expressions)
        temp.push_back(static_cast<translatable*>(i));
    t.ADD(temp);
}
ast::addExpr::~addExpr() {
    for (auto& i: expressions)
        delete i;
}

void ast::mulExpr::translate(ITranslator &t) const
{
    std::vector<translatable*>temp;
    temp.reserve(expressions.size());
    for (auto &i : expressions)
        temp.push_back(static_cast<translatable*>(i));
    t.MUL(temp);
}
ast::mulExpr::~mulExpr() {
    for (auto& i: expressions)
        delete i;
}
void ast::orExpr::translate(ITranslator &t) const
{
    std::vector<translatable*>temp;
    temp.reserve(expressions.size());
    for (auto &i : expressions)
        temp.push_back(static_cast<translatable*>(i));
    t.OR(temp);
}
ast::orExpr::~orExpr() {
    for (auto& i: expressions)
        delete i;
}
void ast::andExpr::translate(ITranslator &t) const
{
    std::vector<translatable*>temp;
    temp.reserve(expressions.size());
    for (auto &i : expressions)
        temp.push_back(static_cast<translatable*>(i));
    t.AND(temp);
}
ast::andExpr::~andExpr() {
    for (auto& i: expressions)
        delete i;
}
void ast::minExpr::translate(ITranslator &t) const
{
    std::vector<translatable*>temp;
    temp.reserve(expressions.size());
    for (auto &i : expressions)
        temp.push_back(static_cast<translatable*>(i));
    t.MIN(temp);
}
ast::minExpr::~minExpr() {
    for (auto& i: expressions)
        delete i;
}
void ast::maxExpr::translate(ITranslator &t) const
{
    std::vector<translatable*>temp;
    temp.reserve(expressions.size());
    for (auto &i : expressions)
        temp.push_back(static_cast<translatable*>(i));
    t.MAX(temp);
}
ast::maxExpr::~maxExpr() {
    for (auto& i: expressions)
        delete i;
}
void ast::eqExpr::translate(ITranslator &t) const
{
    std::vector<translatable*>temp;
    temp.reserve(expressions.size());
    for (auto &i : expressions)
        temp.push_back(static_cast<translatable*>(i));
    t.EQ(temp);
}

ast::eqExpr::~eqExpr() {
    for (auto& i: expressions)
        delete i;
}

void ast::notExpr::translate(ITranslator &t) const {
    t.NOT(expression);
}

ast::notExpr::~notExpr() {
    delete expression;
}

void ast::subExpr::translate(ITranslator &t) const {
    t.SUB(l, r);
}

ast::subExpr::~subExpr() {
    delete l;
    delete r;
}

void ast::divExpr::translate(ITranslator &t) const {
    t.DIV(l, r);
}

ast::divExpr::~divExpr() {
    delete l;
    delete r;
}

void ast::literal::translate(ITranslator &t) const {
    t.value(v);
}

void ast::values::translate(ITranslator &t) const {
    for (auto &i : values_)
        i->translate(t);
}

ast::values::~values() {
    for (auto &i : values_)
        delete i;
}
