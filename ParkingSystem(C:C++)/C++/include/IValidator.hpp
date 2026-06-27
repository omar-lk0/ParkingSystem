#pragma once

#include <string>

/// Base validator (inheritance): concrete validators share a common interface.
class IValidator {
public:
    virtual ~IValidator() = default;

    virtual bool validate(const std::string& input) const = 0;
    virtual std::string expectedFormat() const = 0;
};
