#pragma once

#include "CommandHandler.hpp"
#include "HistoryRepository.hpp"

#include <memory>

/// Top-level application (composition: owns repository and command handler).
class Application {
public:
    Application();
    ~Application() = default;

    int run(int argc, char* argv[]);

private:
    std::unique_ptr<HistoryRepository> repository_;
    std::unique_ptr<CommandHandler> handler_;
};
