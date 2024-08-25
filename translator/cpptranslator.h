#pragma once

#include "translator.h"
#include <iostream>

class CPPtranslator : public ITranslator {
private:
    std::ostream& out;
    std::string filenane;
public:
    CPPtranslator(std::ostream& out, const std::string& filenane);

    void value(int val) override;
    void value(bool val) override;
    void value(own_types::CELL_VALUE val) override;
    void value(own_types::value val) override;

    void ADD(const std::vector<translatable*>& expressions) override;
    void MUL(const std::vector<translatable*>& expressions) override;
    void OR(const std::vector<translatable*>& expressions) override;
    void AND(const std::vector<translatable*>& expressions) override;

    void MAX(const std::vector<translatable*>& expressions) override;
    void MIN(const std::vector<translatable*>& expressions) override;

    void EQ(const std::vector<translatable*>& expressions) override;

    void SUB(const translatable* left, const translatable* right) override;
    void DIV(const translatable* left, const translatable* right) override;

    void NOT(const translatable* expr) override;

    void declareVariable(own_types::Types t, const std::string& name, const translatable* expr = nullptr) override;
    void declareArrayVariable(own_types::Types t, const std::vector<translatable*>& dimentions, const std::string& name, const translatable* expr = nullptr) override;
    
    void getVariable(const std::string& name) override;
    void getArray(const std::string& name, const std::vector<translatable*>& indexes) override;
    void assign() override;

    void cycle(const translatable* condition, const translatable* body) override;
    void if_elses(const std::vector<std::pair<translatable*, translatable*>>& branches) override;
    void declareFunction(const std::string& name, const translatable* body) override;
    void callFunction(const std::string& name) override;

    void Left(const translatable* expr) override;
    void Right(const translatable* expr) override;
    void Up(const translatable* expr) override;
    void Down(const translatable* expr) override;

    void Senddrons(const translatable* expr) override;
    void Getdronscount(const std::string& name) override;

    void ENDCMD() override;

    void print(const translatable* var) override;

    ~CPPtranslator() = default;
};
