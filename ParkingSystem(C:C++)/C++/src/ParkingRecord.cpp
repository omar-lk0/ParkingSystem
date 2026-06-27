#include "ParkingRecord.hpp"

#include "AnsiColors.hpp"

#include <cstdio>
#include <iomanip>
#include <sstream>

ParkingRecord::ParkingRecord(std::string car, std::string street, std::string zone,
                             int hours, float price)
    : car_(std::move(car)),
      street_(std::move(street)),
      zone_(std::move(zone)),
      hours_(hours),
      price_(price) {}

std::string ParkingRecord::toFileLine() const {
    std::ostringstream stream;
    stream << car_ << '|' << street_ << '|' << zone_ << '|' << hours_ << '|'
           << std::fixed << std::setprecision(2) << price_;
    return stream.str();
}

bool ParkingRecord::tryParseLine(const std::string& line, ParkingRecord& out) {
    if (line.empty()) {
        return false;
    }

    char car[50] = {};
    char street[100] = {};
    char zone[20] = {};
    int hours = 0;
    float price = 0.0f;

    const int matched = std::sscanf(line.c_str(), "%49[^|]|%99[^|]|%19[^|]|%d|%f", car,
                                  street, zone, &hours, &price);
    if (matched != 5) {
        return false;
    }

    out = ParkingRecord(car, street, zone, hours, price);
    return true;
}

std::ostream& operator<<(std::ostream& os, const ParkingRecord& record) {
    os << record.car_ << " - " << record.street_ << " - " << ansi::yellow << record.zone_
       << ansi::reset << " Zone - " << record.hours_ << " hours - " << ansi::green
       << std::fixed << std::setprecision(2) << record.price_ << " RON" << ansi::reset;
    return os;
}
