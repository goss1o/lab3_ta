#include "ast.h"

void ast::statementGrp::translate(ITranslator &t) const {
    for (auto& i : statements) {
        i->translate(t);
        t.ENDCMD();
    }
}

ast::statementGrp::~statementGrp()
{
    for (auto& i : statements)
        delete i;
}