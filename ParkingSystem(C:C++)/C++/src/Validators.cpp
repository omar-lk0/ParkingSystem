#include "Validators.hpp"

#include "RomanianPlate.hpp"
#include "Utils.hpp"

#include <cctype>
#include <sstream>

bool ZoneValidator::validate(const std::string& input) const {
    return input == "red" || input == "yellow" || input == "green";
}

std::string ZoneValidator::expectedFormat() const {
    return "red | yellow | green";
}

bool HoursValidator::validate(const std::string& input) const {
    if (input.empty()) {
        return false;
    }
    for (char ch : input) {
        if (!std::isdigit(static_cast<unsigned char>(ch))) {
            return false;
        }
    }
    std::istringstream stream(input);
    int hours = 0;
    stream >> hours;
    return !stream.fail() && inInclusiveRange(hours, 1, 24);
}

std::string HoursValidator::expectedFormat() const {
    return "1 - 24 hours (integer)";
}

bool CarPlateValidator::validate(const std::string& input) const {
    return RomanianPlate::isValid(input);
}

std::string CarPlateValidator::expectedFormat() const {
    return RomanianPlate::expectedFormat();
}
