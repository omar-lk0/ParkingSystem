#pragma once

#include <string>

/// Zone pricing rules used when creating a parking payment record.
class ParkingZone {
public:
    static bool isValid(const std::string& zone);
    static float ratePerHour(const std::string& zone);
    static std::string displayName(const std::string& zone);
};
