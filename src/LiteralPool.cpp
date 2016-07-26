//
// Created by Tarek on 5/6/2016.
//

#include "LiteralPool.h"

void LiteralPool::insert(string literal, string locator) {
    literalPool[literal] = locator;
}

void LiteralPool::insert(string literal) {
    if(!hasLiteral(literal)){
        unassignedLiterals.push_back(literal);
        literalPool[literal] = "";
    }
}

string LiteralPool::getLocator(string literal) {
    return literalPool[literal];
}

bool LiteralPool::hasLiteral(string literal) {
    return !(literalPool.find(literal) == literalPool.end());
}
