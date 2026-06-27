#include "RomanianPlate.hpp"

#include <cctype>
#include <unordered_set>

namespace {

const std::unordered_set<std::string> kCountyCodes = {
    "AB", "AG", "AR", "BC", "BH", "BN", "BR", "BT", "BV", "BZ", "CJ", "CL", "CS",
    "CT", "CV", "DB", "DJ", "GJ", "GL", "GR", "HD", "HR", "IF", "IL", "IS", "MH",
    "MM", "MS", "NT", "OT", "PH", "SB", "SJ", "SM", "SV", "TL", "TM", "TR", "VL",
    "VN", "VS",
};

bool isAsciiLetter(char ch) {
    return (ch >= 'A' && ch <= 'Z');
}

bool isAsciiDigit(char ch) {
    return ch >= '0' && ch <= '9';
}

}  // namespace

std::string RomanianPlate::normalize(const std::string& plate) {
    std::string normalized;
    normalized.reserve(plate.size());
    for (char ch : plate) {
        if (std::isspace(static_cast<unsigned char>(ch)) || ch == '-') {
            continue;
        }
        normalized.push_back(
            static_cast<char>(std::toupper(static_cast<unsigned char>(ch))));
    }
    return normalized;
}

bool RomanianPlate::isValidCountyCode(const std::string& code) {
    return kCountyCodes.count(code) > 0;
}

bool RomanianPlate::isValidNumberGroup(const std::string& digits) {
    if (digits.size() != 2 && digits.size() != 3) {
        return false;
    }
    for (char ch : digits) {
        if (!isAsciiDigit(ch)) {
            return false;
        }
    }
    if (digits == "00" || digits == "000") {
        return false;
    }
    return true;
}

bool RomanianPlate::isValidLetterGroup(const std::string& letters) {
    if (letters.size() != 3) {
        return false;
    }
    for (char ch : letters) {
        if (!isAsciiLetter(ch) || ch == 'Q') {
            return false;
        }
    }
    if (letters[0] == 'I' || letters[0] == 'O') {
        return false;
    }
    if (letters == "III" || letters == "OOO") {
        return false;
    }
    return true;
}

bool RomanianPlate::isValid(const std::string& plate) {
    const std::string normalized = normalize(plate);
    if (normalized.empty()) {
        return false;
    }

    for (char ch : normalized) {
        if (!isAsciiLetter(ch) && !isAsciiDigit(ch)) {
            return false;
        }
    }

    // County format: CC + 2 digits + 3 letters (7 characters).
    if (normalized.size() == 7) {
        const std::string county = normalized.substr(0, 2);
        if (isValidCountyCode(county) && isAsciiDigit(normalized[2])) {
            const std::string digits = normalized.substr(2, 2);
            const std::string letters = normalized.substr(4, 3);
            return isValidNumberGroup(digits) && isValidLetterGroup(letters);
        }
    }

    // Bucharest: B + 2 digits + 3 letters (6 characters).
    if (normalized.size() == 6 && normalized[0] == 'B' && isAsciiDigit(normalized[1])) {
        const std::string digits = normalized.substr(1, 2);
        const std::string letters = normalized.substr(3, 3);
        return isValidNumberGroup(digits) && isValidLetterGroup(letters);
    }

    // Bucharest: B + 3 digits + 3 letters (7 characters, only when not a county code).
    if (normalized.size() == 7 && normalized[0] == 'B' && isAsciiDigit(normalized[1]) &&
        !isValidCountyCode(normalized.substr(0, 2))) {
        const std::string digits = normalized.substr(1, 3);
        const std::string letters = normalized.substr(4, 3);
        return isValidNumberGroup(digits) && isValidLetterGroup(letters);
    }

    return false;
}

std::string RomanianPlate::expectedFormat() {
    return "Romanian plate: county CC-NN-LLL (e.g. TM99XYZ) or Bucharest B-NN-LLL / "
           "B-NNN-LLL (e.g. B12ABC, B123ABC); letters cannot use Q or start with I/O";
}
