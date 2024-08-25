#include "cpptranslator.h"
#include <algorithm>

CPPtranslator::CPPtranslator(std::ostream &out, const std::string &filenane): out(out), filenane(filenane) {
    out << "#include <iostream>\n";
    out << "#include \"own_system.h\"\n";
    out << "#include \"robot/labyrinth.h\"\n";

    out << "namespace labyrinth {Labyrinth robot(\""<<filenane<<"\");};\n";
};


void CPPtranslator::value(int val)
{
    out << val;
}
void CPPtranslator::value(bool val) {
    out << val;
}
void CPPtranslator::value(own_types::CELL_VALUE val) {
    switch (val)
    {
    case own_types::CELL_VALUE::EMPTY:
        out << "own_types::CELL_VALUE::EMPTY";
        break;
    case own_types::CELL_VALUE::WALL:
        out << "own_types::CELL_VALUE::WALL";
        break;
    case own_types::CELL_VALUE::EXIT:
        out << "own_types::CELL_VALUE::EXIT";
        break;
    case own_types::CELL_VALUE::UNDEFINED:
        out << "own_types::CELL_VALUE::UNDEFINED";
        break;
    default:
        break;
    }
}

void CPPtranslator::value(own_types::value val) {
    switch (val.t)
    {
    case own_types::Types::INT:
        value(val.v.Ival);
        break;
    case own_types::Types::BOOL:
        value(val.v.Bval);
        break;
    case own_types::Types::CELL:
        value(val.v.Cval);
        break;
    default:
        break;
    }
}

void CPPtranslator::ADD(const std::vector<translatable *> &expressions)
{
    out << "(";
    for (size_t i = 0; i < expressions.size(); ++i) {
        expressions[i]->translate(*this);
        if (i != expressions.size() -1)
            out << "+";
    }
    out << ")";
}

void CPPtranslator::MUL(const std::vector<translatable *> &expressions) {
    out << "(";
    for (size_t i = 0; i < expressions.size(); ++i) {
        expressions[i]->translate(*this);
        if (i != expressions.size() -1)
            out << "*";
    }
    out << ")";
}

void CPPtranslator::OR(const std::vector<translatable *> &expressions) {
    out << "(";
    for (size_t i = 0; i < expressions.size(); ++i) {
        expressions[i]->translate(*this);
        if (i != expressions.size() -1)
            out << "||";
    }
    out << ")";
}

void CPPtranslator::AND(const std::vector<translatable *> &expressions) {
    out << "(";
    for (size_t i = 0; i < expressions.size(); ++i) {
        expressions[i]->translate(*this);
        if (i != expressions.size() -1)
            out << "&&";
    }
    out << ")";
}

void CPPtranslator::MAX(const std::vector<translatable *> &expressions) {
    out << "std::max({";
    for (size_t i = 0; i < expressions.size(); ++i) {
        expressions[i]->translate(*this);
        if (i != expressions.size() -1)
            out << ",";
    }
    out << "})";
}
void CPPtranslator::MIN(const std::vector<translatable *> &expressions) {
    out << "std::min({";
    for (size_t i = 0; i < expressions.size(); ++i) {
        expressions[i]->translate(*this);
        if (i != expressions.size() -1)
            out << ",";
    }
    out << "})";
}

void CPPtranslator::EQ(const std::vector<translatable *> &expressions) {
    out << "(";
    for (size_t i = 0; i < expressions.size(); ++i) {

        expressions[0]->translate(*this);
        out << "==";
        expressions[i]->translate(*this);
        
        if (i != expressions.size() -1)
            out << "&&";
    }
    out << ")";
}

void CPPtranslator::SUB(const translatable *left, const translatable *right) {
    out << "(";
    left->translate(*this);
    out << "-";
    right->translate(*this);
    out << ")";
}
void CPPtranslator::DIV(const translatable *left, const translatable *right) {
    out << "(";
    left->translate(*this);
    out << "/";
    right->translate(*this);
    out << ")";
}

void CPPtranslator::NOT(const translatable *expr) {
    out << "(";
    out << "!";
    expr->translate(*this);
    out << ")";
}


std::string cpptypename(own_types::Types t) {
    switch (t)
    {
    case own_types::Types::INT:
        return "int";
        break;
    case own_types::Types::BOOL:
        return "bool";
        break;
    case own_types::Types::CELL:
        return "own_types::CELL_VALUE";
        break;
    default:
        break;
    }
}
std::string defaultvalue(own_types::Types t) {
    switch (t)
    {
    case own_types::Types::INT:
        return "0";
        break;
    case own_types::Types::BOOL:
        return "false";
        break;
    case own_types::Types::CELL:
        return "own_types::CELL_VALUE::UNDEFINED";
        break;
    default:
        break;
    }
}
void CPPtranslator::declareVariable(own_types::Types t, const std::string &name, const translatable *expr) {
    out << cpptypename(t);
    out << " " << name;

    out << "=";
    if (expr) {
        expr->translate(*this);
    } else {
        out << defaultvalue(t);
    }
}

void CPPtranslator::declareArrayVariable(own_types::Types t, const std::vector<translatable*> &dimentions, const std::string &name, const translatable *expr) {
    if (dimentions.size() == 0)
        throw std::invalid_argument("Bad dimentions");
    
    out << "own_types::Array<";
    for (size_t i = 1; i < dimentions.size(); i++) {
        out << "own_types::subarray<";
    }
    out << cpptypename(t);
    for (size_t i = 0; i < dimentions.size(); i++) out << ">";
    out << " " << name;

    out << "=";
    if (expr) {
        expr->translate(*this);
    } else {
        out << "own_types::Array(";
        dimentions[0]->translate(*this);
        out << ",";
        for (size_t i = 1; i < dimentions.size(); i++) {
            out << "own_types::subarray(";
            dimentions[i]->translate(*this);
            out << ",";
        }
        out << defaultvalue(t);
        for (size_t i = 0; i < dimentions.size(); i++) out << ")";
    }
}

void CPPtranslator::getVariable(const std::string &name) {
    out << name;
}

void CPPtranslator::getArray(const std::string &name, const std::vector<translatable *> &indexes) {
    out << name;
    for (auto &i : indexes) {
        out << "[";
        i->translate(*this);
        out << "]";
    }
}

void CPPtranslator::assign() {
    out << "=";
}

void CPPtranslator::cycle(const translatable *condition, const translatable *body) {
    out << "while";
    out << "("; condition->translate(*this); out << ")";
    out << "{\n"; body->translate(*this); out << "}";
}

void CPPtranslator::if_elses(const std::vector<std::pair<translatable *, translatable *>> &branches) {
    for (size_t i = 0; i < branches.size(); i++) {
        if (i > 0)
            out << " else ";
        out << "if";
        out << "("; branches[i].first->translate(*this); out << ")";
        out << "{\n"; branches[i].second->translate(*this); out << "}";
    }
    
}

void CPPtranslator::declareFunction(const std::string &name, const translatable *body) {
    out << "int " <<name <<"()";
    out << "{\n"; body->translate(*this); out << "}";
}

void CPPtranslator::callFunction(const std::string &name) {
    out << name << "()";
}

void CPPtranslator::Left(const translatable *expr) {
    out << "labyrinth::robot.moveRobotLeft(";
    expr->translate(*this);
    out << ")";
}
void CPPtranslator::Right(const translatable *expr) {
    out << "labyrinth::robot.moveRobotRight(";
    expr->translate(*this);
    out << ")";
}
void CPPtranslator::Up(const translatable *expr) {
    out << "labyrinth::robot.moveRobotUp(";
    expr->translate(*this);
    out << ")";
}
void CPPtranslator::Down(const translatable *expr) {
    out << "labyrinth::robot.moveRobotDown(";
    expr->translate(*this);
    out << ")";
}
void CPPtranslator::Senddrons(const translatable *expr) {
    out << "labyrinth::robot.sendDrones(";
    expr->translate(*this);
    out << ")";
}

void CPPtranslator::Getdronscount(const std::string &name) {
    out << name << "=" << "labyrinth::robot.getDronesCount()";
}

void CPPtranslator::ENDCMD()
{
    out << ";" << std::endl;;
}

void CPPtranslator::print(const translatable *var) {
    out << "std::cout<<";
    var->translate(*this);
    out << "<<std::endl";
}
