#pragma once

#include <iosfwd>
#include <string>

/// One parking payment entry (car, street, zone, hours, price).
class ParkingRecord {
public:
    ParkingRecord() = default;
    ParkingRecord(std::string car, std::string street, std::string zone, int hours,
                  float price);

    const std::string& car() const { return car_; }
    const std::string& street() const { return street_; }
    const std::string& zone() const { return zone_; }
    int hours() const { return hours_; }
    float price() const { return price_; }

    /// Serializes to the pipe-delimited file format.
    std::string toFileLine() const;

    /// Parses one line from history.txt; returns empty optional on failure.
    static bool tryParseLine(const std::string& line, ParkingRecord& out);

    /// Colored terminal output (zone and price highlighted only).
    friend std::ostream& operator<<(std::ostream& os, const ParkingRecord& record);

private:
    std::string car_;
    std::string street_;
    std::string zone_;
    int hours_ = 0;
    float price_ = 0.0f;
};
