#include "FixedFormatReader.h"

/** @brief (parse input file and split it to label, operation and operand)
  *
  * The source program to be assembled must be in fixed format as follows:
  *  1. bytes 1-8 label
  *  2. 9 blank
  *  3. 10-15 operation code
  *  4. 16-17 blank
  *  5. 18-35 operand
  *  6. 36-66 comment
  */

const int LABEL_START_INDEX = 0, LABLE_FIELD_LENGTH = 9;
const int OPERATION_START_INDEX = 9, OPERATION_FIELD_LENGTH = 8;
const int OPERAND_START_INDEX = 17, OPERAND_FIELD_LENGTH = 18;
const int MAX_LINE_LENGTH = 66;

bool FixedFormatReader::hasNextLine() {
    string line;
    bool isTrue = ((bool)getline(in, line)); // there exist a line
    initVariables();
    autalities::dos2unix(line);
    currentLine = line;
    if(isTrue) {
        if(line[0] == '.') { // line is a comment
            isComment = true;
        } else {
            int lineLen = line.length();
            if (lineLen >= LABEL_START_INDEX + 1) { // there is label field
                // label subtring with space if any
                label = line.substr(LABEL_START_INDEX, min(LABLE_FIELD_LENGTH, lineLen - LABEL_START_INDEX));
                if((label.length() == LABLE_FIELD_LENGTH && trailSpaces(label, 1)) || label.length() < LABLE_FIELD_LENGTH) {
                    validateLabel();
                } else {
                    label = "";
                    valid = false;
                    addToErrorMessage("col 9 must be blank");
                }
            }
            if (lineLen >= OPERATION_START_INDEX + 1) {// there is operation field
                // operation subtring with space if any
                operation = line.substr(OPERATION_START_INDEX, min(OPERATION_FIELD_LENGTH, lineLen - OPERATION_START_INDEX));
                if((operation.length() == OPERATION_FIELD_LENGTH && trailSpaces(operation, 2))
                        || operation.length() < OPERATION_FIELD_LENGTH) {
                    validateOperation();
                } else {
                    operation = "";
                    valid = false;
                    addToErrorMessage("col 16 and 17 must be blank");
                }
            }
            if (lineLen >= OPERAND_START_INDEX + 1) {// there is operand field
                // operand subtring with space if any
                operand = line.substr(OPERAND_START_INDEX, min(OPERAND_FIELD_LENGTH, lineLen - OPERAND_START_INDEX));
                autalities::removeTrailingSpaces(operand);
            }
            args = OperandValidator::getOperands(operand);
            if(lineLen > MAX_LINE_LENGTH) { // if line length is too long
                valid = false;
                addToErrorMessage("line is too long [max=66]");
            }
            cout << "\"" << label << "\", \"" << operation << "\", \"" << operand << "\"" << endl;
        }
    }
    return isTrue;
}


/** @brief (count space chars at end of string and compare it with givien number)
  * @return true if number of spaces at the end of string is greater than or equal givien number
  */
bool FixedFormatReader::trailSpaces(string &s, int n) {
    int cnt = 0;
    for(int i = s.length() - 1; i >= 0; --i) {
        if(cnt >= n)
            return true;
        if (s[i] == ' ')
            ++cnt;
        else
            break;
    }
    return (cnt >= n);
}
