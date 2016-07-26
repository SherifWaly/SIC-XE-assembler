#include "FreeFormatReader.h"
#include "Includes.h"
#include "OpTable.h"
#include "DirectivseTable.h"

/**
  * free foramt:
  *   - fields may occur anywhere on a line
  *   - fields are separated by delimiting characters (e.g., space
  *     character, colon, semi colon, back slash, question mark and
  *     other characters that are not likely to be used in a program).
  *
  * we assume that free format line:
  *   - does not contain any inline comments only seprated comments lines starts with '.'
  */

const regex FREEFORMAT_REGEX0("((\\w+)\\s+)?([\\+]?[a-zA-Z]+)\\s*", regex_constants::ECMAScript);
const regex FREEFORMAT_REGEX1("((\\w+)\\s+)?([\\+]?[a-zA-Z]+)\\s([\\w\\W]*)", regex_constants::ECMAScript);

bool FreeFormatReader::hasNextLine() {
    string line;
    bool isTrue;
    do {
        isTrue = ((bool) getline(in, line));
        line = autalities::removeLeadingSpaces(line);
    } while (isTrue && line.empty());
    initVariables();
    currentLine = line;
    if (isTrue) {
        if (line[0] == '.') {
            isComment = true;
        } else {
            parse(line);
        }
    }
    return isTrue;
}

void FreeFormatReader::parse(string line) {
    autalities::dos2unix(line);
    smatch sm;
    if (regex_match(line, sm, FREEFORMAT_REGEX0)) {
        label = sm[2];
        operation = sm[3];
    } else if (regex_match(line, sm, FREEFORMAT_REGEX1)) {
        label = sm[2];
        operation = sm[3];
        operand = sm[4];
        autalities::removeTrailingSpaces(operand);
        operand = autalities::removeLeadingSpaces(operand);
    } else {
        valid = false;
        addToErrorMessage("can not match line with any valid format");
    }
    OpTable *opTable = new OpTable();
    DirectivseTable *dir = new DirectivseTable();
    label = autalities::tolow(label);
    if (opTable->hasOperation(label) || dir->contains(label)) {
        if (operand.empty()) {
            operand = operation;
        } else if (!operation.empty()) {
            operand = operation + " " + operand;
        }
        operation = label;
        label = "";
    }
    validateLabel();
    validateOperation();
    args = OperandValidator::getOperands(operand);
    cout << "\"" << label << "\", \"" << operation << "\", \"" << operand << "\"" << endl;
}
