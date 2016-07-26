#ifndef INPUTREADER_H
#define INPUTREADER_H

#ifndef INCLUDES_H
#include "Includes.h"
#endif // INCLUDES_H

#ifndef ASSEMBLERUTALITIES_H
#include "AssemblerUtalities.h"
#endif // ASSEMBLERUTALITIES_H

#ifndef OPERANDVALIDATOR_H
#include "OperandValidator.h"
#endif // OPERANDVALIDATOR_H

const regex LABEL_REGEX("([a-zA-Z][a-zA-Z0-9]*)?\\s*", regex_constants::ECMAScript);
const int LABEL_MATCH_INDEX = 1;

const regex OPERATION_REGEX("(\\+)?([a-zA-Z]+)\\s*", regex_constants::ECMAScript);
const int OPERATION_MATCH_INDEX = 2;

class InputReader {

public:
    InputReader(string fileName);
    //virtual ~InputReader();
    // Read a line return if there is more lines in the file
    virtual bool hasNextLine() = 0;
    // return empty string if there is not
    string getLabel();
    // return empty string if there is not
    string getOperation();
    // return empty string if there is not
    string getOperand();

    vector<OperandValidator::Operand> getArgs();

    // return true if line is a Comment
    bool isCommentLine();
    // return if this instruction is a format 4 or not
    bool isFormatFour();
    //return the full current line
    string getLine();

    bool isValid();

    string getErrorMessage();

    void close();

protected:
    bool validateLabel();
    bool validateOperation();

    void initVariables();
    void addToErrorMessage(string msg);

    ifstream in;
    string label, operand, operation;
    vector<OperandValidator::Operand> args;
    bool isComment;
    bool isFour;
    bool valid = true;
    string currentLine;
    string errorMessage;
};

#endif // INPUTREADER_H
