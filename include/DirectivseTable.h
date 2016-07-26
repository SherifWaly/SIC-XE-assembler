#ifndef DIRECTIVSETABLE_H
#define DIRECTIVSETABLE_H

#ifndef INCLUDES_H
#include "Includes.h"
#endif // INCLUDES_H

class DirectivseTable {
public:
    DirectivseTable(string fileName = "directives.txt");
    /*
    static DirectivseTable *getInstance(){
        if(innstance == NULL){
            innstance = new DirectivseTable();
        }
        return innstance;
    }
    */
    bool contains(string dir);
    int getNumberOfParameters(string directive);
    bool notSupported(string dir);

private:
    //instead of bool it would be an executer object
    unordered_map<string, bool> directiveMap, notSupportedMap;
    ifstream stream;
    //static DirectivseTable *innstance ;
};
//DirectivseTable *DirectivseTable::innstance = 0;
#endif // DIRECTIVSETABLE_H
