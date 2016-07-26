#ifndef SYMTABLE_H
#define SYMTABLE_H

#ifndef INCLUDES_H
#include "Includes.h"
#endif // INCLUDES_H

#ifndef ASSEMBLERUTALITIES_H
#include "AssemblerUtalities.h"
#endif // ASSEMBLERUTALITIES_H

class Sympol {

public:
    Sympol() {};
    Sympol(string val, bool isAbsl) : value{val}, isAbs{isAbsl} {}
    string value = "";
    bool isAbs = false;

    Sympol operator += (const Sympol b) {
        value = autalities::addHex(value, b.value);
        isAbs = (isAbs and b.isAbs);
        return *(this);
    }

    Sympol operator -= (const Sympol b) {
        value = autalities::intToHex(autalities::subtractHex(value, b.value));
        isAbs = ((isAbs and b.isAbs) or (!isAbs and !b.isAbs));
        return *(this);
    }

    friend Sympol operator + (Sympol a, const Sympol b) {
        a += b;
        return a;
    }

    friend Sympol operator - (Sympol a, const Sympol b) {
        a -= b;
        return a;
    }

    Sympol operator *=(const Sympol b) {
        int intValue = autalities::hexToInteger(value);
        string temp = b.value;
        intValue *= autalities::hexToInteger(temp);
        this->isAbs = true;
        this->value = autalities::intToHex(intValue);
        return *(this);
    }

    Sympol operator /=(const Sympol b) {
        int intValue = autalities::hexToInteger(value);
        string temp = b.value;
        intValue /= autalities::hexToInteger(temp);
        this->isAbs = true;
        this->value = autalities::intToHex(intValue);
        return *(this);
    }
};

class SymTable {
public:
    SymTable();

    void insert(string label, string locator, bool isAbs = false);

    void insert(string label, Sympol symp);

    // return -1 if not found
    string getLocator(string label);

    bool isAbsolute(string label);

    bool hasLabel(string label);

    Sympol getSympol(string label);

    unordered_map<string, Sympol> getSymtab() {
        return symTab;
    }

    string getRegister(string r);

private:
    unordered_map<string, Sympol> symTab;
    unordered_map<string, string> registers;
};

#endif // SYMTABLE_H
