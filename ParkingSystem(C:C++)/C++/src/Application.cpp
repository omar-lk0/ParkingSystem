#include "Application.hpp"

#include <iostream>

Application::Application()
    : repository_(std::make_unique<HistoryRepository>()),
      handler_(std::make_unique<CommandHandler>(*repository_)) {}

int Application::run(int argc, char* argv[]) {
    if (argc == 1) {
        CommandHandler::showHelp(std::cout);
        return 0;
    }
    return handler_->execute(argc, argv);
}
