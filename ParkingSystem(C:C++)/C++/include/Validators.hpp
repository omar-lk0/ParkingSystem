#pragma once

#include "IValidator.hpp"

#include <string>

/// Validates parking zone names (red, yellow, green).
class ZoneValidator : public IValidator {
public:
    bool validate(const std::string& input) const override;
    std::string expectedFormat() const override;
};

/// Validates parking duration in hours (1–24).
class HoursValidator : public IValidator {
public:
    bool validate(const std::string& input) const override;
    std::string expectedFormat() const override;
};

/// Validates Romanian car registration plates (county and Bucharest formats).
class CarPlateValidator : public IValidator {
public:
    bool validate(const std::string& input) const override;
    std::string expectedFormat() const override;
};
