#include "CommandHandler.hpp"

#include "AnsiColors.hpp"
#include "HistoryRepository.hpp"
#include "ParkingRecord.hpp"
#include "ParkingZone.hpp"
#include "Validators.hpp"

#include <iostream>
#include <string>

CommandHandler::CommandHandler(HistoryRepository& repository) : repository_(repository) {}

void CommandHandler::showHelp(std::ostream& os) {
    os << "Available commands:\n";
    os << "  help                                  - Show commands\n";
    os << "  history                               - View history\n";
    os << "  pay <car> <zone> <hours> <street>     - Add payment\n";
}

int CommandHandler::handleHelp() const {
    showHelp(std::cout);
    return 0;
}

int CommandHandler::handleHistory() const {
    repository_.displayAll(std::cout);
    return 0;
}

int CommandHandler::handlePay(int argc, char* argv[]) const {
    if (argc < 6) {
        std::cout << ansi::red << "Invalid usage!\n" << ansi::reset;
        std::cout << "Expected: pay <car> <zone> <hours> <street>\n";
        std::cout << "Example: pay TM99XYZ red 3 \"Main Street\"\n";
        return 1;
    }

    const std::string car = argv[2];
    const std::string zone = argv[3];
    const std::string hoursStr = argv[4];
    const std::string street = argv[5];

    const CarPlateValidator carValidator;
    if (!carValidator.validate(car)) {
        std::cout << ansi::red << "Invalid car license number!\n" << ansi::reset;
        std::cout << "Expected: " << carValidator.expectedFormat() << '\n';
        return 1;
    }

    const ZoneValidator zoneValidator;
    if (!zoneValidator.validate(zone)) {
        std::cout << ansi::red << "Invalid zone!\n" << ansi::reset;
        std::cout << "Expected: " << zoneValidator.expectedFormat() << '\n';
        return 1;
    }

    const HoursValidator hoursValidator;
    if (!hoursValidator.validate(hoursStr)) {
        std::cout << ansi::red << "Invalid hours!\n" << ansi::reset;
        std::cout << "Expected: " << hoursValidator.expectedFormat() << '\n';
        return 1;
    }

    if (street.empty()) {
        std::cout << ansi::red << "Invalid street!\n" << ansi::reset;
        std::cout << "Expected: non-empty street name\n";
        return 1;
    }

    const int hours = std::stoi(hoursStr);
    const float price = ParkingZone::ratePerHour(zone) * static_cast<float>(hours);

    const ParkingRecord record(car, street, ParkingZone::displayName(zone), hours, price);

    if (!repository_.append(record)) {
        std::cout << ansi::red << "Error writing file!\n" << ansi::reset;
        return 1;
    }

    std::cout << ansi::green << "Payment saved successfully!\n" << ansi::reset;
    return 0;
}

int CommandHandler::execute(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << ansi::red << "Missing option!\n" << ansi::reset;
        std::cout << "Use 'help' to see available commands.\n";
        return 1;
    }

    const std::string command = argv[1];
    if (command == "help") {
        return handleHelp();
    }
    if (command == "history") {
        return handleHistory();
    }
    if (command == "pay") {
        return handlePay(argc, argv);
    }

    std::cout << ansi::red << "Missing option!\n" << ansi::reset;
    std::cout << "Use 'help' to see available commands.\n";
    return 1;
}
