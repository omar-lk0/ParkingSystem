#pragma once

/// Generic inclusive-range check used for hours and similar numeric validation.
template <typename T>
bool inInclusiveRange(const T& value, const T& minValue, const T& maxValue) {
    return value >= minValue && value <= maxValue;
}
