#include <drogon/drogon.h>
#include <watchman/Watchman.h>
#include <iostream>
#include <memory>

int main() {
    // 1. Load Drogon configuration
    drogon::app().loadConfigFile("./config.json");

    // 2. Initialize the Watchman
    // For this prototype, we'll hardcode the dummy container name
    // In a real app, this could be dynamic via Docker API container listing
    auto watchman = std::make_shared<echo::Watchman>("dummy-app");
    
    // 3. Start monitoring in the background using Drogon's event loop
    drogon::app().getLoop()->runInLoop([watchman]() {
        std::cout << "[Echo Agent] Starting Watchman for dummy-app..." << std::endl;
        watchman->start();
    });

    // 4. Run the Drogon application
    std::cout << "[Echo Agent] Application is running on port 8080" << std::endl;
    drogon::app().run();

    return 0;
}
