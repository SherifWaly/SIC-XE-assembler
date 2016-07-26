#ifndef OPERANDVALIDATOR_H
#define OPERANDVALIDATOR_H

#ifndef INCLUDES_H
#include "Includes.h"
#endif // INCLUDES_H

#include "SymTable.h"

#ifndef ASSEMBLERUTALITIES_H
#include "AssemblerUtalities.h"
#endif // ASSEMBLERUTALITIES_H

namespace OperandValidator {

const regex RIGESTER_REGEX("[aAbBsStTlLfFxX]", regex_constants::ECMAScript);

const regex XBYTES_REGEX("[xX]\'([a-fA-F0-9]+)\'", regex_constants::ECMAScript);
const regex CBYTES_REGEX("[cC]\'([\\w\\W]+\)'", regex_constants::ECMAScript);

const regex CLITERAL_REGEX("=[Cc]\'([\\w\\W]+)\'", regex_constants::ECMAScript);
const regex XLITERAL_REGEX("=[Xx]\'([a-fA-F0-9]+)\'", regex_constants::ECMAScript);
const regex DLITERAL_REGEX("=([0-9]+)", regex_constants::ECMAScript);

const regex LABEL_REGEX("([#@]?)([a-zA-Z][a-zA-Z0-9]*)", regex_constants::ECMAScript);
const regex NUM_REGEX("([#@]?)([\\+\\-]?[0-9]+)", regex_constants::ECMAScript);

const regex HEX_REGEX("[0-9a-fA-F]+", regex_constants::ECMAScript);

const regex EXPERSION_REGEX("([\\-\\+]?\\w+|\\*)(\\s*([\\+\\*/\\-])\\s*([\\-\\+]?\\w+|\\*))?", regex_constants::ECMAScript);

const regex COMMA_REGEX("(\\s*[^,^\\s]+\\s*,)*\\s*[^,^\\s]+\\s*", regex_constants::ECMAScript);

enum class OperandType {
    NO_MATCH, NUMBER,
    CLITERAL, XLITERAL, DLITERAL,
    CBYTES, XBYTES,
    EXPRESION, LABEL,
    HEX, REGESTER
};

enum OperandTypeGroup : char {
    MEMORY = 'm', LITERAL = 'l',
    REGESTER = 'r', BYTES = 'b',
    HEX = 'h', NUMBER = 'n',
    POSNUMBER = 'p'
};

class Operand {

public:
    OperandType type = OperandType::NO_MATCH;
    string operand;
    bool isImmediate = false, isInDirect = false, isIndexed = false;

    bool ofType(char c);
    bool isPlain();
    bool isNumber();
    bool isPosNumber();
    bool isHex();
    bool isByte();
    bool isMemory();
    bool isLiteral();

    string toHex();

    string stringType();
};

vector<Operand> getOperands(string field);

vector<Operand> getOperands(vector<string> args);

Operand toOperand(string arg);

Sympol toSympol(string operand, string locator, SymTable *symTab);

Sympol evaluateExpression(Operand expression, string locator, SymTable *symTab);

}
#endif // OPERANDVALIDATOR_H
