#include "ParkingZone.hpp"

#include <algorithm>
#include <cctype>

namespace {

std::string toLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
    return value;
}

}  // namespace

bool ParkingZone::isValid(const std::string& zone) {
    const std::string lower = toLower(zone);
    return lower == "red" || lower == "yellow" || lower == "green";
}

float ParkingZone::ratePerHour(const std::string& zone) {
    const std::string lower = toLower(zone);
    if (lower == "yellow") {
        return 1.0f;
    }
    if (lower == "red") {
        return 1.5f;
    }
    if (lower == "green") {
        return 2.0f;
    }
    return -1.0f;
}

std::string ParkingZone::displayName(const std::string& zone) {
    const std::string lower = toLower(zone);
    if (lower == "red") {
        return "Red";
    }
    if (lower == "yellow") {
        return "Yellow";
    }
    if (lower == "green") {
        return "Green";
    }
    return zone;
}
