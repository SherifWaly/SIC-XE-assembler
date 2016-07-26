//
// Created by Tarek on 5/6/2016.
//

#ifndef ASSEMBLER_LITERALPOOL_H
#define ASSEMBLER_LITERALPOOL_H

#include "Includes.h"

class LiteralPool {
public:
    void insert(string literal, string locator);

    void insert(string literal);

    string getLocator(string literal);

    bool hasLiteral(string literal);

    vector<string> getUnassignedLiterals() {
        return unassignedLiterals;
    }

    void clear() {
        unassignedLiterals.clear();
    }
private:
    unordered_map<string, string> literalPool;
    vector<string>unassignedLiterals;
};


#endif //ASSEMBLER_LITERALPOOL_H
