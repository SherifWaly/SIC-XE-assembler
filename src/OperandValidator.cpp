#include "OperandValidator.h"

namespace OperandValidator {

bool Operand::ofType(char c) {
    if(c == OperandTypeGroup::NUMBER) {
        return isNumber() && isPlain();
    } else if (c == OperandTypeGroup::HEX) {
        return isHex() && isPlain();
    } else if (c == OperandTypeGroup::LITERAL) {
        return isLiteral();
    } else if (c == OperandTypeGroup::MEMORY) {
        return isMemory();
    } else if (c == OperandTypeGroup::POSNUMBER) {
        return isPosNumber() && isPlain();
    } else if (c == OperandTypeGroup::REGESTER) {
        return type == OperandType::REGESTER;
    }
    return false;
}

bool Operand::isPlain() {
    return !(isImmediate || isIndexed || isInDirect);
}


bool Operand::isMemory() {
    return isPosNumber() || (isNumber() && isImmediate)
           || (type == OperandType::LABEL) || isLiteral()
           || (type == OperandType::EXPRESION) || isHex();
}

bool Operand::isLiteral() {
    return (type == OperandType::CLITERAL || type == OperandType::XLITERAL
            || type == OperandType::DLITERAL);
}

bool Operand::isNumber() {
    return (type == OperandType::NUMBER);
}

bool Operand::isPosNumber() {
    return (isNumber() && operand[0] != '-');
}

bool Operand::isHex() {
    return (type == OperandType::HEX || isPosNumber());
}

string Operand::stringType() {
    return "";
}

string Operand::toHex() {
    string ret = "";
    if (isNumber() || type == OperandType::DLITERAL) {
        return autalities::intToWord(operand);
    } else if (isHex() || type == OperandType::XBYTES || type == OperandType::XLITERAL) {
        return operand;
    } else if (type == OperandType::CBYTES || type == OperandType::CLITERAL) {
        for(char c : operand) {
            ret += autalities::intToByte((int) c);
        }
    }
    return ret;
}

/*******************************************************/

vector<string> splitAtComma(string field) {
    vector<string> ret;
    if(field.empty())
        return ret;
    bool speratedWithComma = regex_match(field, COMMA_REGEX);
    if(regex_match(field, CBYTES_REGEX) || regex_match(field, CBYTES_REGEX) || !speratedWithComma) {
        ret.push_back(field);
    } else if (speratedWithComma) {
        replace(field.begin(), field.end(), ',', ' ');
        stringstream ss(field);
        string tmp;
        while(ss >> tmp) {
            ret.push_back(tmp);
        }
    }
    return ret;
}

/** @brief (split given expression at operators(+,-))
  * @return
  *        vector of splited strings
  *
  * eg: 12+ alpha-100 + 20 -> {"12", "+", "alpha", "-", "100", "+", "20"}
  */

vector<string> splitExpression(string expression) {
    vector<string> terms;
    int splits = count(expression.begin(), expression.end(), '+')
                 + count(expression.begin(), expression.end(), '-');
    static string termRegex = "\\s*([\\+\\-])\\s*([\\w\\*]+)";
    string regexString = "([\\w\\*]+)" + autalities::repeat(termRegex, splits);
    regex expressionRegex(regexString, regex_constants::ECMAScript);
    smatch sm;
    regex_match(expression, sm, expressionRegex);
    bool flag = false;
    for(string term : sm){
        if(flag)
            terms.push_back(term);
        flag = true;
    }
    return terms;
}

/** @brief (evaluate expression and return its hex value and type (absolute or realocatble))
  * @return
  *        Sympol object, if there is an error then return Sympol with empty string
  */

Sympol evaluateExpression(Operand expression, string locator, SymTable *symTab) {
    Sympol emptySymp, res;
    smatch matcher;
    if(regex_match(expression.operand, matcher, EXPERSION_REGEX)) {
        vector<string> terms = {matcher[1]};
        //expression ex: 100 - 50
        if(matcher.size() >= 4 && matcher[3] != "") {
            terms.push_back(matcher[3]);
            terms.push_back(matcher[4]);
        }
        res = toSympol(terms[0], locator, symTab);
        if (terms.size() > 1 && !(res.value.empty())) {
            Sympol operand2 = toSympol(terms[2], locator, symTab);
            if(operand2.value.empty()){ // error in evaluating this term
                return emptySymp;
            }
            if(terms[1] == "+" && (res.isAbs || operand2.isAbs)) {
                res += operand2;
            } else if (terms[1] == "-" && (!res.isAbs || operand2.isAbs)) {
                res -= operand2;
            } else if (terms[1] == "*" && (operand2.isAbs && res.isAbs)) {
                res *= operand2;
            } else if (terms[1] == "/" && (operand2.isAbs && res.isAbs)) {
                res /= operand2;
            } else { // error not supported operator
                return emptySymp;
            }
        }
    }
    return res;
}

/** @brief (split the given string at commas and match it with different regexes)
  * @return
  *        vector of operands, if there is an error then return empty vector
  */

vector<Operand> getOperands(string field) {
    return getOperands(splitAtComma(field));
}

/** @brief (match each string in args with different regexes)
  * @return
  *        vector of operands, if there is an error then return empty vector
  */
vector<Operand> getOperands(vector<string> args) {
    vector<Operand> operandList;
    for(string arg : args) {
        Operand tmp = toOperand(arg);
        if(tmp.type == OperandType::REGESTER
                && tmp.operand == "x"
                && !operandList.empty() && operandList.back().ofType(OperandTypeGroup::MEMORY)) {

            operandList.back().isIndexed = true;
        } else {
            operandList.push_back(tmp);
        }
    }
    return operandList;
}

/** @brief (match string with different regexes and extract different information about it)
  * @return
  *        object of Operand, if there is an error then type set to be NO_MATCH
  *
  * turn the operand to lower case if it is not a char array
  * set flags if immediate(#), indirect(@) or indexed(,X)
  * set operand type form enum operand type
  */
Operand toOperand(string arg) {
    Operand tmp;
        tmp.operand = autalities::tolow(arg);
        smatch sm;
        if (regex_match(arg, RIGESTER_REGEX)) {
            tmp.type = OperandType::REGESTER;
        } else if(regex_match(arg, sm, XBYTES_REGEX)) {
            tmp.type = OperandType::XBYTES;
            tmp.operand = autalities::tolow(sm[1]);
        } else if(regex_match(arg, sm, CBYTES_REGEX)) {
            tmp.type = OperandType::CBYTES;
            tmp.operand = sm[1];
        } else if(regex_match(arg, sm, XLITERAL_REGEX)) {
            tmp.type = OperandType::XLITERAL;
            tmp.operand = autalities::tolow(sm[1]);
        } else if(regex_match(arg, sm, DLITERAL_REGEX)) {
            tmp.type = OperandType::DLITERAL;
            tmp.operand = sm[1];
        } else if(regex_match(arg, sm, CLITERAL_REGEX)) {
            tmp.type = OperandType::CLITERAL;
            tmp.operand = sm[1];
        } else if(regex_match(arg, sm, LABEL_REGEX)) {
            tmp.type = OperandType::LABEL;
            tmp.operand = autalities::tolow(sm[2]);
            tmp.isImmediate = (sm[1] == "#");
            tmp.isInDirect = (sm[1] == "@");
        } else if(regex_match(arg, sm, NUM_REGEX)) {
            tmp.type = OperandType::NUMBER;
            tmp.operand = sm[2];
            tmp.isImmediate = (sm[1] == "#");
            tmp.isInDirect = (sm[1] == "@");
        } else if(regex_match(arg, HEX_REGEX)) {
            tmp.type = OperandType::HEX;
        } else if(regex_match(arg, EXPERSION_REGEX)) {
            tmp.type = OperandType::EXPRESION;
        }
    return tmp;
}


/** @brief (evaluate hex value of the given string)
  * @return
  *        Sympol, if there is an error then return sympol with empty value
  */
Sympol toSympol(string operand, string locator, SymTable *symTab) {
    Sympol ret;
    Operand tmp = toOperand(operand);
    string value = "";
    if(tmp.type == OperandType::LABEL) {
        if(symTab->hasLabel(tmp.operand)) {
            return symTab->getSympol(tmp.operand);
        } else {
            return ret; // error undefined label
        }
    } else if (tmp.operand == "*") {
        ret.value = locator;
        ret.isAbs = false;
    } else if(tmp.isNumber() && tmp.isPlain()){
        ret.value = tmp.toHex();
        ret.isAbs = true;
    }
    return ret;
}


}
