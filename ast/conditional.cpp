#include "conditional.h"

void ast::cycle::translate(ITranslator &t) const {
    t.cycle(b.condition, b.body);
}

ast::cycle::~cycle() {
    delete b.body;
    delete b.condition;
}

void ast::switcher::translate(ITranslator &t) const {
    std::vector<std::pair<translatable*, translatable*>> c(branches.size());
    for (size_t i = 0; i < branches.size(); i++) {
        c[i] = {branches[i].condition, branches[i].body};
    }
    t.if_elses(c);
}

ast::switcher::~switcher() {
    for (auto &i : branches) {
        delete i.body;
        delete i.condition;
    }
}