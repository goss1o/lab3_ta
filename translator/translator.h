#pragma once

#include <string>
#include <vector>
#include "../own_system.h"

struct translatable;

struct ITranslator {
    virtual void value(int val) = 0;
    virtual void value(bool val) = 0;
    virtual void value(own_types::CELL_VALUE val) = 0;
    virtual void value(own_types::value val) = 0;

    virtual void ADD(const std::vector<translatable*>& expressions) = 0;
    virtual void MUL(const std::vector<translatable*>& expressions) = 0;
    virtual void OR(const std::vector<translatable*>& expressions) = 0;
    virtual void AND(const std::vector<translatable*>& expressions) = 0;

    virtual void MAX(const std::vector<translatable*>& expressions) = 0;
    virtual void MIN(const std::vector<translatable*>& expressions) = 0;

    virtual void EQ(const std::vector<translatable*>& expressions) = 0;

    virtual void SUB(const translatable* left, const translatable* right) = 0;
    virtual void DIV(const translatable* left, const translatable* right) = 0;

    virtual void NOT(const translatable* expr) = 0;

    virtual void declareVariable(own_types::Types t, const std::string& name, const translatable* expr = nullptr) = 0;
    virtual void declareArrayVariable(own_types::Types t, const std::vector<translatable*>& dimentions, const std::string& name, const translatable* expr = nullptr) = 0;
    
    virtual void getVariable(const std::string& name) = 0;
    virtual void getArray(const std::string& name, const std::vector<translatable*>& indexes) = 0;
    virtual void assign() = 0;

    virtual void cycle(const translatable* condition, const translatable* body) = 0;
    virtual void if_elses(const std::vector<std::pair<translatable*, translatable*>>& branches) = 0;
    virtual void declareFunction(const std::string& name, const translatable* body) = 0;
    virtual void callFunction(const std::string& name) = 0;

    virtual void Left(const translatable* expr) = 0;
    virtual void Right(const translatable* expr) = 0;
    virtual void Up(const translatable* expr) = 0;
    virtual void Down(const translatable* expr) = 0;

    virtual void Senddrons(const translatable* expr) = 0;
    virtual void Getdronscount(const std::string& name) = 0;
    
    virtual void ENDCMD() = 0;

    virtual void print(const translatable* var) = 0;
    
    virtual ~ITranslator() = default;
};

struct translatable {
    virtual void translate(ITranslator&) const = 0;
    virtual ~translatable() = default;
};
