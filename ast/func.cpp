#include "func.h"

void ast::funcDecl::translate(ITranslator &t) const {
    t.declareFunction(name, body);
}

ast::funcDecl::~funcDecl() {
    delete body;
}

void ast::funcCall::translate(ITranslator &t) const {
    t.callFunction(name);
}
