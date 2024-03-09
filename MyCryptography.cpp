//
// Created by Wahares on 18.05.2023.
//

#include "MyCryptography.h"
#include "StringExpansion.h"
#include <vector>
#include <ctime>
#include <random>

using std::string;
using std::vector;


/**
 * code text in base16 system but shifts every char by 33 to avoid unwanted and problematic chars
 * @param text - text to encode
 * @return text encoded in base16 system
 */
string MyCryptography::base16encode(const string &text) {
    string coded;
    for (char c: text) {
        unsigned char cod = c;
        cod >>= 4;
        cod += 33;
        coded += (char) cod;
        cod = c;
        cod <<= 4;
        cod >>= 4;
        cod += 33;
        coded += (char) cod;
    }
    return coded;

}

/**
 * decode text from base16 system including previous shifting by 33
 * @param text - text to decode
 * @return text decoded in base16 system
 */
string MyCryptography::base16decode(const string &text) {
    string coded;
    for (int i = 0; i < text.length(); i += 2) {
        auto cod = (unsigned char) (text[i] - 33);
        cod <<= 4;
        cod |= (unsigned char) (text[i + 1] - 33);
        coded += (char) cod;
    }
    return coded;
}

/**
 *
 * @param k - key used in encoding
 * @param t - text to ecode
 * @return encoded text
 */
string MyCryptography::encode(const string &k, const string &t) {
    int values[k.length()];
    int mainValue = 0;

    //decoding key to number values and main value
    for (int i = 0; i < k.length(); ++i) {
        values[i] = (int) (unsigned char) k[i];
        mainValue += values[i];
    }

    string result;

    for (int i = 0; i < t.length(); ++i) {
        int currKeyVal = values[i % k.length()] % 8;
        auto b = (unsigned char) t[i];
        unsigned char tmp = b;
        b >>= currKeyVal;
        b |= tmp << (8 - currKeyVal);
        b = b ^ currKeyVal;
        if (currKeyVal % 2 == 0)
            b = ~b;
        b = b ^ mainValue;
        result += (char) b;
    }
    return result;
}

/**
 *
 * @param k - key used in decoding
 * @param t - text to decode
 * @return decoded text
 */
string MyCryptography::decode(const string &k, const string &t) {
    int values[k.length()];
    int mainValue = 0;
    //decoding key to number values and main value
    for (int i = 0; i < k.length(); ++i) {
        values[i] = (int) (unsigned char) k[i];
        mainValue += values[i];
    }

    string result;

    for (int i = 0; i < t.length(); ++i) {
        int currKeyVal = values[i % k.length()] % 8;
        auto b = (unsigned char) t[i];
        b = b ^ mainValue;

        if (currKeyVal % 2 == 0)
            b = ~b;
        b = b ^ currKeyVal;
        unsigned char tmp = b;
        b <<= currKeyVal;
        b |= tmp >> (8 - currKeyVal);

        result += (char) b;
    }
    return result;
}

bool MyCryptography::validateInput(const string &text) {
    return base16decode(text).ends_with("[success123!@#]");
}

bool MyCryptography::validateDecodedInput(const string &text) {
    return text.starts_with("[success123!@#]");
}

string MyCryptography::packData(const std::vector<PassAccount> &accounts, const std::string &key) {

    string result = "[success123!@#]\n";
    for (const PassAccount &acc: accounts) {
        string tmp;
        tmp += "[ACC]";
        tmp += base16encode(acc.name);
        tmp += "[PAS]";
        tmp += base16encode(acc.password);
        tmp += "[CAT]";
        tmp += base16encode(acc.category);
        tmp += "[LOG]";
        tmp += base16encode(acc.login);
        tmp += "[WWW]";
        tmp += base16encode(acc.www);
        tmp += "[END]";
        result += tmp + '\n';
    }
    string s = encode(key, result);
    return s;
}

void MyCryptography::unPackData(const string &data, vector<PassAccount> &vec) {

    vec.clear();

    for (const string &s: StringExpansion::splitString(data, "\n")) {
        if (s.empty())
            continue;
        auto tmp = PassAccount(base16decode(StringExpansion::dataBetween(s, "[ACC]", "[PAS]")),
                               base16decode(StringExpansion::dataBetween(s, "[PAS]", "[CAT]")),
                               base16decode(StringExpansion::dataBetween(s, "[CAT]", "[LOG]")),
                               base16decode(StringExpansion::dataBetween(s, "[LOG]", "[WWW]")),
                               base16decode(StringExpansion::dataBetween(s, "[WWW]", "[END]")));
        vec.push_back(tmp);
    }
}

string MyCryptography::generatePassword(int charCount, bool hasLetters, bool hasSpecial) {
    string tmp;
    string dict = "1234567890";
    if (hasLetters)
        dict += "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
    if (hasSpecial)
        dict += "!@#$%^&*()";
    std::time_t seed = std::time(nullptr);
    std::srand(seed);
    for (int i = 0; i < charCount; ++i) {
        tmp += dict[rand() % dict.length()];
    }
    return tmp;
}

/**
 * calculates strength of password considering length, count of distinct characters and kinds of chars
 * @param password
 * @return strength in scale 0-3
 */

int MyCryptography::calcPasswordStrength(const string &password) {
    int strength = 0;

    //points based on length
    int tmp = (int) password.length();
    if (tmp < 6)
        return 0;
    if (tmp > 9)
        strength++;
    if (tmp > 12)
        strength++;

    //points based on number of distinct chars
    tmp = StringExpansion::NumOfDistinctChars(password);
    if (tmp < 3)
        return 0;
    if (tmp > password.length() / 2)
        strength++;
    if (tmp == password.length())
        strength++;

    //calculate points given if password has 1.upper AND lower chars 2.digits 3.non-alphanumeric chars
    int kinds = 0;
    if (std::count_if(password.begin(), password.end(), [](char c) -> bool { return isupper(c); }) != 0
        && std::count_if(password.begin(), password.end(), [](char c) -> bool { return islower(c); }) != 0)
        kinds++;
    if (std::count_if(password.begin(), password.end(), [](char c) -> bool { return isdigit(c); }) != 0)
        kinds++;
    if (std::count_if(password.begin(), password.end(), [](char c) -> bool { return !isalnum(c); }) != 0)
        kinds++;

    //clamp 0-3
    tmp = strength + kinds / 2;
    if (tmp > 3)
        tmp = 3;
    return tmp;
}



