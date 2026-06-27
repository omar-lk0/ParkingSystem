#pragma once

#include <string>

/// Validates Romanian standard registration plates (1992+ format).
///
/// County: CC + 2 digits + 3 letters (e.g. TM99XYZ)
/// Bucharest: B + 2 or 3 digits + 3 letters (e.g. B12ABC, B123ABC)
class RomanianPlate {
public:
    static bool isValid(const std::string& plate);
    static std::string expectedFormat();
    static std::string normalize(const std::string& plate);

private:
    static bool isValidCountyCode(const std::string& code);
    static bool isValidNumberGroup(const std::string& digits);
    static bool isValidLetterGroup(const std::string& letters);
};
