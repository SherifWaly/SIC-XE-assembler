#include "DirectivseTable.h"

DirectivseTable::DirectivseTable(string fileName) {
    stream.open(fileName);
    string token = "";
    while(stream >> token) {
        directiveMap[token] = true;
    }
    notSupportedMap["equ"] = true;
    notSupportedMap["ltorg"] = true;
    notSupportedMap["org"] = true;
}

bool DirectivseTable::contains(string dir) {
    return directiveMap.find(dir) != directiveMap.end();
}

int getNumberOfParameters(string directive) {
    return 0;
}

bool DirectivseTable::notSupported(string dir) {
    return notSupportedMap.find(dir) != notSupportedMap.end();
}
