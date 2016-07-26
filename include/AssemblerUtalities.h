#ifndef ASSEMBLERUTALITIES_H
#define ASSEMBLERUTALITIES_H

#ifndef INCLUDES_H

#include "Includes.h"

#endif // INCLUDES_H

using namespace std;

namespace autalities {

string tolow(string s);

string toUp(string s);

string removeLeadingSpaces(string &s);

void removeTrailingSpaces(string &s);

void dos2unix(string &s);

string repeat(const string &word, int times);

int toInteger(string &txt);

int hexToInteger(char c);

int hexToInteger(string &txt);

char intToHexChar(int d);

string intToHex(int number);

string intToHex(int number, int sz);

string intToBin(int number);

string intToBin(int number, int sz);

int binToInteger(string a);

int subtractHex(string a, string b);

string addHex(string a, string b);

string intToByte(int decimal);

string intToByte(string decimal);

string intToWord(int decimal);

string intToWord(string decimal);

string normalize(string s, int sz);

string normalize(string s, int sz, string c);

bool checkLocator(string &text);

}
#endif // ASSEMBLERUTALITIES_H
