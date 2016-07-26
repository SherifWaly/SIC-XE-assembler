#include "AssemblerUtalities.h"

/**
 * namespace contains sic assembeler utalities functions
 */

namespace autalities {


/**
 * @param s
 * @brief (convert given string to lowercase)
 * @return string converted to lowercase
 */
string tolow(string s) {
    for (int i = 0; i < s.size(); i++) {
        s[i] = tolower(s[i]);
    }
    return s;
}

/**
 * @param s
 * @brief (convert given string to uppercase)
 * @return string converted to uppercase
 */
string toUp(string s) {
    for (int i = 0; i < s.size(); i++) {
        s[i] = toupper(s[i]);
    }
    return s;
}

string removeLeadingSpaces(string &s) {
    int ls = 0;
    for(; ls < s.size(); ++ls) {
        if (s[ls] != ' ' && s[ls] != '\t')
            break;
    }
    return s.substr(ls);
}

/**
 * @param &s
 *          refernce to string
 * @brief (remove trialing spaces from end of givien string)
 */

void removeTrailingSpaces(string &s) {
    while (s.back() == ' ' || s.back() == '\t') {
        s.pop_back();
    }
}

/**
 * @param &s
 *          refernce to string
 * @brief (remove \r character at end of string if exists)
 *
 * if string is read from windows file the end of line char will be \r\n
 * and if read in linux it will read \r char which may cause unexpected behavior
 * so we remove it
 */
void dos2unix(string &s) {
    while (s.back() == '\r') {
        s.pop_back();
    }
}

string repeat(const string &word, int times) {
   string result;
   result.reserve(times * word.length()); // avoid repeated reallocation
   for ( int a = 0 ; a < times ; a++)
      result += word ;
   return result;
}

/**
 * @param txt
 *            string of digit characters
 * @return int converted to number
 * @brief (convert given +/- string to decimal integer)
 */

int toInteger(string &txt) {
    int x = 0;
    int i = txt.empty() || (txt[0] != '-' && txt[0] != '+') ? 0 : 1;
    int sign = 1;
    if (!txt.empty() && txt[0] == '-') {
        sign = -1;
    }
    for (; i < txt.size(); i++) {
        x = x * 10 + txt[i] - '0';
    }
    return x * sign;
}

/**
 * @param c
 *          hex char
 * @return int converted to decimal number
 * @brief (convert given hex char(0:9 a:f) to decimal integer(0:15))
 */

int hexToInteger(char c) {
    int digit = tolower(c) - '0';
    return digit + (digit > 9 ? (10 + '0' - 'a') : 0);
}

/**
 * @param txt
 *          string hex chars
 * @return int converted to decimal number
 * @brief (convert given +/- hex string to decimal integer +/-)
 */

int hexToInteger(string &txt) {
    if(txt.size() == 6 && tolower(txt[0]) == 'f') { // word and negative
        txt = "ff" + txt;
    }
    int x = 0;
    for (int i = 0; i < txt.size(); i++) {
        x <<= 4;
        x |= hexToInteger(txt[i]);
    }
    return x;
}

/**
 * @param d
 *          int between 0 to 15
 * @return hex char
 * @brief (convert given int(0:15) to char of hex value(0:9-a:f))
 */

char intToHexChar(int d) {
    return (d > 9 ? ('a' - 10) : '0') + d;
}

/**
 * @param number
 * @return hex string
 * @brief (convert given int +/- to string of hex chars of length 6)
 * use it if you don't care about the size of the string
 */

string intToHex(int number) {
    int msk = 0b1111;
    string hex = "";
    for(int i = 0; i < 6; ++i) {
        hex += intToHexChar(number & msk);
        number >>= 4;
    }
    reverse(hex.begin(), hex.end());
    return hex;
}

/**
 * @param number
 * @param sz
 *           the required size of string
 * @return hex string
 * @brief (convert given int +/- to string of hex chars of length sz)
 */

string intToHex(int number, int sz) {
    string c = number < 0 ? "f" : "0";
    string hex = intToHex(number);
    return normalize(hex, sz, c);
}

/**
 * @param number
 * @return binary string
 * @brief (convert given + int to string of binaries)
 * use it if you don't care about the size of string
 */
string intToBin(int number) {
    string ans = "";
    while (number > 0) {
        ans += (number % 2) ? "1" : "0";
        number /= 2;
    }
    reverse(ans.begin(), ans.end());
    return ans;
}

/**
 * @param number
 * @param sz
 *           the required size of string
 * @return binary string
 * @brief (convert given + int to string of binaries of size sz)
 */

string intToBin(int number, int sz) {
    string c = number < 0 ? "1" : "0";
    string bin = intToBin(number);
    return normalize(bin, sz, c);
}

/**
 * @param a
 *          binary string
 * @return decimal integer
 * @brief (convert given binary string to integer)
 */
int binToInteger(string a) {
    int answer = 0;
    for (int i = 0; i < a.length(); ++i) {
        answer <<= 1;
        answer |= (a[i] - '0');
    }
    return answer;
}

/**
 * @param a
 *          hex string
 * @param b
 *          hex string
 * @return decimal integer
 * @brief (return the result of (a-b) as decimal integer)
 */

int subtractHex(string a, string b) {
    return (hexToInteger(a) - hexToInteger(b));
}


/**
 * @param a
 *          hex string
 * @param b
 *          hex string
 * @return hex string
 * @brief (return the result of (a+b) as hex string)
 */

string addHex(string a, string b) {
    return intToHex(hexToInteger(a) + hexToInteger(b));
}


/**
 * @param decimal
 * @return hex string
 * @brief (convert given int to hex string of length 2)
 */

string intToByte(int decimal) {
    return intToHex(decimal, 2);
}


/**
 * @param string decimal
 * @return hex string
 * @brief (convert given string decimal to hex string of length 2)
 */

string intToByte(string decimal) {
    return intToByte(toInteger(decimal));
}

/**
 * @param decimal
 * @return hex string
 * @brief (convert given int to hex string of length 6)
 */

string intToWord(int decimal) {
    return intToHex(decimal, 6);
}


/**
 * @param decimal string
 * @return hex string
 * @brief (convert given decimal string to hex string of length 6)
 */
string intToWord(string decimal) {
    return intToWord(toInteger(decimal));
}

/**
 * @param s
 *         numerical string
 * @param sz
 *         required size
 * @return string
 * @brief (normalize the given num string to size sz)
 * if the size of string > sz returns last sz chars
 * else if add "0" to the begining of the string
 */

string normalize(string s, int sz) {
    return normalize(s, sz, "0");
}

/**
 * @param s
 *         numerical string
 * @param sz
 *         required size
 * @param c
 *         the string to be added to begin of string if size < sz
 * @return string
 * @brief (normalize the given num string to size sz)
 * if the size of string > sz returns last sz chars
 * else if add string c to the begin of the string
 */

string normalize(string s, int sz, string c) {
    while(s.size() < sz) {
        s = c + s;
    }
    return s.substr(s.size() - sz);
}

bool checkLocator(string &text) {

}

}
