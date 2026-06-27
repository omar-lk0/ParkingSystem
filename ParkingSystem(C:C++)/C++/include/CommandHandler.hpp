#pragma once

#include <iosfwd>

class HistoryRepository;

/// Parses and validates CLI commands (association with HistoryRepository).
class CommandHandler {
public:
    explicit CommandHandler(HistoryRepository& repository);

    int execute(int argc, char* argv[]);

    static void showHelp(std::ostream& os);

private:
    HistoryRepository& repository_;

    int handleHelp() const;
    int handleHistory() const;
    int handlePay(int argc, char* argv[]) const;
};
