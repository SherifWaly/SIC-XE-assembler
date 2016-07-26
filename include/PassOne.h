#ifndef PASSONE_H
#define PASSONE_H

#ifndef INCLUDES_H

#include "Includes.h"

#endif // INCLUDES_H

#ifndef INPUTREADER_H

#include "InputReader.h"

#endif // INPUTREADER_H

#ifndef READERS_H

#include "FreeFormatReader.h"
#include "FixedFormatReader.h"

#endif // READERS_H

#ifndef TABELS

#include "OpTable.h"
#include "DirectivseTable.h"
#include "SymTable.h"
#include "LiteralPool.h"

#endif // TABELS

#ifndef OPERANDVALIDATOR_H
#include "OperandValidator.h"
#endif // OPERANDVALIDATOR_H

class PassOne {
public:
    PassOne(string fileName, string outputFile);

    PassOne(InputReader *reader, string outputFile);

    //virtual ~PassOne();
    void pass();

    SymTable *getSymTable();

    string getprogrammLength();

    int getErrorsCounter();

    LiteralPool *getLiteralPool(){
        return literalPool;
    }

private:
    InputReader *input;
    OpTable *opTab;
    SymTable *symTab;
    LiteralPool *literalPool;
    string startingAdress = "000000";
    string locator = "000000", tmpLocator = "";
    string outputFile;
    ofstream outStream;
    DirectivseTable *dirTab;
    int errorCounter = 0;

    void handelStart(vector<OperandValidator::Operand> args, string &msg);

    void handelOperation(vector<OperandValidator::Operand> args, string &msg, string &operation);

    void handelWord(vector<OperandValidator::Operand> args, string &msg);

    void handelByte(vector<OperandValidator::Operand> args, string &msg);

    void handelRes(vector<OperandValidator::Operand> args, string &msg, string &operation);

    void handelOrg(vector<OperandValidator::Operand> args, string &msg);

    void handelEqu(vector<OperandValidator::Operand> args, string label, string &msg);

    void handelLtorg(string &msg ,int lineNumber);

    void addToMessage(string &msg, string toBeAdded);

    void addErrorMessage(string &msg, string toBeAdded);

    void addWarningMessage(string &msg, string toBeAdded);

    void printSymTable();

    string addToLocator(string number, int delta);

    void appendToFile(string line);
};

#endif // PASSONE_H
