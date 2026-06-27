#pragma once

#include "ParkingRecord.hpp"

#include <string>
#include <vector>

/// Reads and appends parking history using history.txt (file I/O + STL vector).
class HistoryRepository {
public:
    explicit HistoryRepository(std::string filepath = "history.txt");

    const std::string& filepath() const { return filepath_; }

    /// Loads all records from disk into a vector.
    std::vector<ParkingRecord> loadAll() const;

    /// Appends one record to the history file.
    bool append(const ParkingRecord& record);

    /// Prints all records with selective ANSI colors.
    void displayAll(std::ostream& os) const;

private:
    std::string filepath_;
};
