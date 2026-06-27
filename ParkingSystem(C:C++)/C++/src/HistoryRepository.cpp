#include "HistoryRepository.hpp"

#include "AnsiColors.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <utility>

HistoryRepository::HistoryRepository(std::string filepath)
    : filepath_(std::move(filepath)) {}

std::vector<ParkingRecord> HistoryRepository::loadAll() const {
    std::vector<ParkingRecord> records;
    std::ifstream file(filepath_);
    if (!file.is_open()) {
        return records;
    }

    std::string line;
    while (std::getline(file, line)) {
        ParkingRecord record;
        if (ParkingRecord::tryParseLine(line, record)) {
            records.push_back(record);
        }
    }
    return records;
}

bool HistoryRepository::append(const ParkingRecord& record) {
    std::ofstream file(filepath_, std::ios::app);
    if (!file.is_open()) {
        return false;
    }
    file << record.toFileLine() << '\n';
    return file.good();
}

void HistoryRepository::displayAll(std::ostream& os) const {
    const std::vector<ParkingRecord> records = loadAll();
    if (records.empty()) {
        std::ifstream probe(filepath_);
        if (!probe.is_open()) {
            os << ansi::red << "Error opening file!\n" << ansi::reset;
            return;
        }
    }

    os << "Parking History:\n";
    os << "---------------------------------------------\n";
    for (const ParkingRecord& record : records) {
        os << record << '\n';
    }
}
