#include "objModel.hpp"
#include <sstream>

void scanComment(strIter &it, const strIter &end) {
    while (it != end && *it != '\n')
        it++;
    it++; //Newline
}

void scanSpaces(strIter &it, const strIter &end) {
    while (it != end && (*it == ' ' || *it == '\t' || *it == '\r'))
        it++;
}


bool isDigit(char ch) {
    return '0' <= ch && ch <= '9';
}

int scanWhole(strIter &it, const strIter &end) {
    int res = 0;
    while (it != end && isDigit(*it)) {
        res = res * 10 + (*it - '0');
        it++;
    }
    return res;
}

float scanFractPart(strIter &it, const strIter &end) {
    float res = 0.f;
    while (it != end && isDigit(*it)) {
        res = res * 0.1f + (*it - '0');
        it++;
    }
    return res * 0.1f;
}

int scanInt(strIter &it, const strIter &end) {
    int sign = 1;
    if (*it == '-')
        sign = -1;
    if (*it == '-' || *it == '+')
        it++;
    return sign * scanWhole(it, end);
}

float scanNumber(strIter &it, const strIter &end) {
    // ['+' | '-'] {0...9} ['.' {0...9} [('e' | 'E') [+ | -] {0...9}]]
    float sign = 1.f;
    if (*it == '-')
        sign = -1.f;
    if (*it == '-' || *it == '+')
        it++;
    float whole = scanWhole(it, end);
    float fract = 0.f;
    float exp = 1.f;
    bool hasExp = false;
    if (*it == '.') {
        it++;
        fract = scanFractPart(it, end);
    }
    if (*it == 'e' || *it == 'E') {
        hasExp = true;
        it++;
        float esign = 1.f;
        if (*it == '-')
            esign = -1.f;
        if (*it == '-' || *it == '+')
            it++;
        exp = esign * scanWhole(it, end);
    }
    float res = sign * (whole + fract);
    if (hasExp)
        powf(res, exp);
    return res;
}


bool checkWord(strIter &it, const strIter &end, const std::string &word) {
    strIter t = it;
    for (auto&& e : word) {
        if (t == end || *t == '\n' || *t != e)
            return false;
        t++;
    }
    return true;
}

bool isAlphabetic(char ch) {
    return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
}

bool isSpecSymb(char ch) {
    return ch == '_' || ch == '.' || ch == '/' || ch == '\\';
}

std::string scanIdent(strIter &it, const strIter &end) {
    if (it != end && !(isAlphabetic(*it) || isSpecSymb(*it)))
        return "";
    std::ostringstream ss;
    while (it != end && (isAlphabetic(*it) || isDigit(*it) || isSpecSymb(*it))) {
        ss << *it;
        it++;
    }
    return ss.str();
}