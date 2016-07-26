#ifndef OPTABLE_H
#define OPTABLE_H

#ifndef INCLUDES_H
#include "Includes.h"
#endif // INCLUDES_H

struct Instruction {
    int format, numOfOperation;
    string opCode, operandsType;
    //  Instruction(int format, int numOfOperation, string opCode)
    //  :format(format), numOfOperation(numOfOperation), opCode(opCode){}
};

class OpTable {
public:
    OpTable(string fileName = "OpTable.txt");
    /*
    static OpTable *getInstance(){
        if(instance == NULL){
            instance = new OpTable();
        }
        return instance;
    }
    */
    bool hasOperation(string operation);
    int getFormat(string operation);
    string getOpcode(string operation);
    int getNumberOfOperands(string operation);
    string getOperandsType(string operation);
private:
    // map operation -> format , opcode , numberOfOperands
    unordered_map<string, Instruction> opTab;
    //  static OpTable *instance ;
};
//OpTable *OpTable::instance=0;
#endif // OPTABLE_H
