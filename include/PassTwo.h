//
// Created by Tarek on 5/6/2016.
//

#ifndef ASSEMBLER_PASSTWO_H
#define ASSEMBLER_PASSTWO_H

#include "Includes.h"
#include "OpTable.h"
#include "DirectivseTable.h"
#include "SymTable.h"
#include "InputReader.h"
#include "IntermediateReader.h"
#include "LiteralPool.h"
#include "ObjectWriter.h"

class PassTwo {
public:
    PassTwo(string fileName, SymTable *symtabel, LiteralPool *literalPool, string length, string output="op.txt");

    void pass();

private:
    IntermediateReader *input;
    OpTable *opTab;
    SymTable *symTab;
    ObjectWriter *opwriter;
    LiteralPool *literalPool;
    string startingAdress = "000000";
    string locator = "000000";
    string length;
    DirectivseTable *dirTab;
    int errorCounter = 0;
    int lineCounter = 0;
    string base = "";

    string intermediateFile, objectFile;

    void handelStart(vector<OperandValidator::Operand> args, string label, string &msg);

    void handelOperation(vector<OperandValidator::Operand> args, string &msg, string &operation, bool isFormatFour);

    void handelWord(vector<OperandValidator::Operand> args, string &msg);

    void handelByte(vector<OperandValidator::Operand> args, string &msg);

    void handelResw(vector<OperandValidator::Operand> args, string &msg);

    void handelResb(vector<OperandValidator::Operand> args, string &msg);

    void handelBase(vector<OperandValidator::Operand> args, string &msg);

    void addToMessage(string &msg, string toBeAdded);

    void addErrorMessage(string &msg, string toBeAdded);

    string addToLocator(string number, int delta);

    Sympol evaluateOperand(OperandValidator::Operand &operand, string &msg);

};


#endif //ASSEMBLER_PASSTWO_H
