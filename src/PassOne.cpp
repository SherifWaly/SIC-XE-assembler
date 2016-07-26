#include "PassOne.h"

/** @brief (return symtable)
  */
SymTable *PassOne::getSymTable() {
    return symTab;
}

/** @brief (parse input file validate it and fill symtable)
  * print output file with:
  *  1. location counter beside each statements
  *  2. errors or warnings below each statement if any
  *  3. symtable if successfully assembeled
  */
void PassOne::pass() {
    int lineNumber = 1;
    string msg = "";
    bool started = false, noStart = true, noEnd = true;

    /// loop till end statement or no line remains
    while (input->hasNextLine()) {
        /// print line
        outStream << lineNumber << "\t" << autalities::toUp(locator) << "\t";
        outStream << input->getLine() << "\n";
        msg = "";
        // lineNumber++;
        if (noEnd && !input->isCommentLine()) { // not a comment line
            /// handel start statement
            string operation = input->getOperation();
            string operand = input->getOperand();
            auto args = input->getArgs(); // operand subfields

            if (operation == "start") {
                if (!started) {
                    started = true;
                    noStart = false;
                    handelStart(args, msg);
                } else {
                    addErrorMessage(msg, "duplicated start statement");
                }
            } else {
                if (noStart) {
                    noStart = false;
                    started = true;
                    //addErrorMessage(msg, "messing start statement");
                }
                /// handel label and add it to symtable
                string label = input->getLabel();
                if (!label.empty()) {
                    if (symTab->hasLabel(label)) { //duplicate symbol
                        addErrorMessage(msg, "Symbol \'" + label + "\' already defined\n");
                    } else {
                        symTab->insert(label, locator);
                    }
                }
                /// handel operation and operand field
                if (args.empty() && !operand.empty()) { // args empty mean error to match it with any operand types
                    addErrorMessage(msg, "wrong operand field");
                }
                if (!operation.empty()) {
                    if (opTab->hasOperation(operation)) { // valid operation
                        handelOperation(args, msg, operation);
                    } else if (input->isFormatFour()) { // directive with format 4
                        addErrorMessage(msg, "invalid format 4 with directives");
                    } else if (operation == "word") {
                        handelWord(args, msg);
                    } else if (operation == "resw" || operation == "resb") {
                        handelRes(args, msg, operation);
                    } else if (operation == "byte") {
                        handelByte(args, msg);
                    } else if (operation == "end") {
                        noEnd = false;
                    } else if (operation == "org") {
                        handelOrg(args, msg);
                    } else if (operation == "equ") {
                        handelEqu(args, label, msg);
                    } else if (operation == "ltorg") {
                        handelLtorg(msg,lineNumber);
                    } else if (dirTab->contains(operation) && dirTab->notSupported(operation)) {
                        addWarningMessage(msg, "not supported directive");
                    } else if (!dirTab->contains(operation)) {
                        addErrorMessage(msg, "invalid operation code");
                    }
                    if(args.size() > 0 && args[0].isLiteral()){
                        string value = args[0].toHex();
                        literalPool->insert(value);
                    }

                } else {
                    addErrorMessage(msg, "operation field is messing");
                }
            }
            if (!input->isValid())
                addToMessage(msg, input->getErrorMessage());
            outStream << msg;
        }
        lineNumber++;
    }
    if (noEnd) {
        addErrorMessage(msg, "no end found");
        outStream << msg;
    }
    handelLtorg(msg,lineNumber);
    outStream << "****\t**********End of pass 1***********\n";
    if (errorCounter > 0) {
        outStream << ">>> incomplete assembely with " << errorCounter << " errors\n";
    } else {
        printSymTable();
    }
    outStream.close();
    input->close();
}

void PassOne::handelStart(vector<OperandValidator::Operand> args, string &msg) {
    if (args.empty() || !(args[0].isHex() && args[0].isPlain())) {
        addErrorMessage(msg, "start must take hex argument");
    } else {
        locator = args[0].operand;
        startingAdress = locator;
        while (startingAdress.size() < 6) startingAdress = '0' + startingAdress;
        locator = startingAdress;
    }
}

/** @brief (validate operation field and its operands and update location counter)
 */
void PassOne::handelOperation(vector<OperandValidator::Operand> args, string &msg, string &operation) {
    int format = opTab->getFormat(operation);
    if (format == 3 && input->isFormatFour()) format = 4;
    else if (input->isFormatFour()) {
        addErrorMessage(msg, "invalid conversion to format 4");
    }
    locator = addToLocator(locator, format);
    int numberOfOperand = opTab->getNumberOfOperands(operation);
    string operandsType = opTab->getOperandsType(operation);

    if (args.size() < numberOfOperand) {
        addErrorMessage(msg, "messing operands");
        return;
    } else if (args.size() > numberOfOperand) {
        addWarningMessage(msg, "args more than required for " + operation);
    }
    for (int i = 0; i < numberOfOperand; ++i) {
        if (!args[i].ofType(operandsType[i])) {
            addErrorMessage(msg, operation + " does not support operand type");
            break;
        }
    }
}

/** @brief (handel WORD directive and its operands and update location counter)
 */
void PassOne::handelWord(vector<OperandValidator::Operand> args, string &msg) {
    if (args.size() < 1) {
        addErrorMessage(msg, "messing valid operand");
        return;
    }
    int i = 0;
    for (auto arg : args) {
        if (!(arg.isNumber() && arg.isPlain())) {
            addErrorMessage(msg, "word supports decimal numeric values only");
            break;
        }
        i++;
    }
    locator = addToLocator(locator, i * 3);
}

/** @brief (handel BYTE directive and its operands and update location counter)
 */
void PassOne::handelByte(vector<OperandValidator::Operand> args, string &msg) {
    if (args.size() < 1) {
        addErrorMessage(msg, "messing operand");
        return;
    } else if (args.size() > 1) {
        addWarningMessage(msg, "args more than required for byte");
    }
    if (args[0].type == OperandValidator::OperandType::CBYTES) {
        int arrSize = args[0].operand.size();
        locator = addToLocator(locator, arrSize);
    } else if (args[0].type == OperandValidator::OperandType::XBYTES) {
        int arrSize = args[0].operand.size();
        if (arrSize % 2)
            addErrorMessage(msg, "odd length for hex string");
        else
            locator = addToLocator(locator, arrSize / 2);
    } else {
        addErrorMessage(msg, "invalid operand for byte");
    }
}

/** @brief (handel RESW,RESB directive and its operands and update location counter)
 */
void PassOne::handelRes(vector<OperandValidator::Operand> args, string &msg, string &operation) {
    if (args.size() < 1) {
        addErrorMessage(msg, "messing operand");
        return;
    } else if (args.size() > 1) {
        addWarningMessage(msg, "args more than required for " + operation);
    }
    if (!args[0].isPosNumber()) {
        addErrorMessage(msg, "operand must be positive numerical value");
    } else {
        int arrSize = autalities::toInteger(args[0].operand);
        int elementSize = operation == "resw" ? 3 : 1;
        locator = addToLocator(locator, arrSize * elementSize);
    }
}

void PassOne::handelOrg(vector<OperandValidator::Operand> args, string &msg) {
    if(args.size() == 0 && !tmpLocator.empty()) {
        locator = tmpLocator;
        tmpLocator = "";
    } else if (args.size() == 1) {
        Sympol evaluated = OperandValidator::evaluateExpression(args[0], locator, symTab);
        if (evaluated.value.empty()) {
            addErrorMessage(msg, "not valid expression");
        } else if (evaluated.isAbs) {
            addErrorMessage(msg, "org must have a realocatable address");
        } else {
            tmpLocator = locator;
            locator = evaluated.value;
        }
    } else {
        addErrorMessage(msg, "org must take zero or one operand");
    }
}

void PassOne::handelEqu(vector<OperandValidator::Operand> args, string label, string &msg) {
    if (args.size() == 1) {
        Sympol evaluated = OperandValidator::evaluateExpression(args[0], locator, symTab);
        if (evaluated.value.empty()) {
            addErrorMessage(msg, "not valid expression");
        } else if (!label.empty()) {
            symTab->insert(label, evaluated);
        }
    } else {
        addErrorMessage(msg, "equ must take one operand");
    }
}

void PassOne::handelLtorg(string &msg , int lineNumber) {
    vector<string>unassigned = literalPool->getUnassignedLiterals();
    for(auto s : unassigned){
        literalPool->insert(s,locator);
        OperandValidator::Operand temp;
        temp.operand = s;
        temp.type = OperandValidator::OperandType::XBYTES;
        vector<OperandValidator::Operand>operands {temp};
        outStream << lineNumber << "\t" << autalities::toUp(locator) << "\t";
        outStream << "byte \t" << "X\'" << s << "\'" << "\n";
        handelByte(operands,msg);
    }
    literalPool->clear();
}

/** @brief (print symtable contents to output stream after success assemble)
 */
void PassOne::printSymTable() {
    ///Symbol table
    for (auto x : symTab->getSymtab()) {
        char line[500];
        sprintf(line, "****\t\t%-20s%-10s%-15s\n", x.first.c_str(), x.second.value.c_str(), (x.second.isAbs ? "absolute" : "realocatable"));
        outStream << line;
        cout << line;
    }
}

/** @brief (add message with "warning")
 */
void PassOne::addWarningMessage(string &msg, string toBeAdded) {
    // warningCounter++;
    addToMessage(msg, "warning: " + toBeAdded);
}

/** @brief (add message with "error" and increament errorCounter)
 */
void PassOne::addErrorMessage(string &msg, string toBeAdded) {
    errorCounter++;
    addToMessage(msg, "error: " + toBeAdded);
}

/** @brief (add new line to message)
 */
void PassOne::addToMessage(string &msg, string toBeAdded) {
    if (!toBeAdded.empty()) {
        msg += "****\t\t" + toBeAdded + "\n";
    }
}

void PassOne::appendToFile(string line) {
    //write to file
    outStream << ((line) + '\n');
}

/** @brief (update location counter)
 */
string PassOne::addToLocator(string number, int delta) {
    int x = autalities::hexToInteger(number);
    return autalities::intToWord(x + delta); // of size 6
}

string PassOne::getprogrammLength() {
    return autalities::intToHex(autalities::subtractHex(locator, startingAdress));
}

int PassOne::getErrorsCounter() {
    return errorCounter;
}

/** @brief (constructor initialze variables)
  */

PassOne::PassOne(InputReader *reader, string outputFile) {
    input = reader;
    opTab = new OpTable();
    symTab = new SymTable();
    dirTab = new DirectivseTable();
    literalPool = new LiteralPool();
    this->outputFile = outputFile;
    outStream.open(outputFile, ios_base::out);
}

PassOne::PassOne(string fileName, string outputFile) : PassOne::PassOne(new FreeFormatReader(fileName), outputFile) { }
