#pragma once

#include <string>
#include "ast.h"
#include "expr.h"

namespace ast {
    class Left : public statement {
        expr *distance;
    public:
        Left(expr *distance): distance(distance) {}
        void translate(ITranslator& t) const override {
            t.Left(distance);
        }
        ~Left() {delete distance;}
    };
    class Right : public statement {
        expr *distance;
    public:
        Right(expr *distance): distance(distance) {}
        void translate(ITranslator& t) const override {
            t.Right(distance);
        }
        ~Right() {delete distance;}
    };
    class Up : public statement {
        expr *distance;
    public:
        Up(expr *distance): distance(distance) {}
        void translate(ITranslator& t) const override {
            t.Up(distance);
        }
        ~Up() {delete distance;}
    };
    class Down : public statement {
        expr *distance;
    public:
        Down(expr *distance): distance(distance) {}
        void translate(ITranslator& t) const override {
            t.Down(distance);
        }
        ~Down() {delete distance;}
    };

    class Senddrons : public expr  {
        expr *count;
    public:
        Senddrons(expr *count) :count(count) {}
        void translate(ITranslator& t) const override {
            t.Senddrons(count);
        }
        ~Senddrons() {delete count;}
    };
    class Getdronscount : public statement  {
        std::string name;
    public:
        Getdronscount(const std::string& name) :name(name) {}
        void translate(ITranslator& t) const override {
            t.Getdronscount(name);
        }
    };
} // namespace ast
