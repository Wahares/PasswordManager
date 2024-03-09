//
// Created by Wahares on 22.05.2023.
//

#include "StringExpansion.h"
#include <iostream>
#include <vector>
#include <unordered_set>

using std::vector;
using std::string;

/**
 * splits string by separator
 * @param input - input string
 * @param separator - separator used in splitting
 * @return vector of splitted strings
 */
vector<string> StringExpansion::splitString(const string &input, const string &separator) {
    vector<string> words;
    size_t startPos =0,endPos;

    while ((endPos = input.find(separator, startPos)) != string::npos) {
        words.push_back(input.substr(startPos, endPos - startPos));
        startPos = endPos + separator.length();
    }

    words.push_back(input.substr(startPos));

    return words;
}
/**
 *
 * @param input - input text
 * @param start - left side
 * @param end - right side
 * @return string between start and end
 */
std::string StringExpansion::dataBetween(const string &input, const string &start, const string &end) {
    string tmp = splitString(input,start)[1];
    tmp = splitString(tmp,end)[0];
    return tmp;
}

/**
 * distinct char counter
 * @param text - text to be counted on
 * @return number of unique chars in given text
 */
int StringExpansion::NumOfDistinctChars(const string &text) {
    std::unordered_set<char> chars;
    for (char c : text)
        chars.insert(c);
    return (int)chars.size();
}

/**
 * int parse
 * @param text - text to be parsed
 * @return text parsed to int
 */
int StringExpansion::parseToInt(const string &text) {
    return std::stoi(text);
}
/**
 * parsable check
 * @param text - text to be changed to int
 * @return bool if text is parsable to int
 */
bool StringExpansion::parsableToInt(const string &text) {
    size_t size;
    try
    {std::stoi(text,&size);return size == text.length();}
    catch(const std::exception& e)
    {return false;}
}


