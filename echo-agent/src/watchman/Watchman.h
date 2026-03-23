#pragma once

#include <string>
#include <vector>
#include <regex>
#include <memory>
#include <drogon/HttpAppFramework.h>
#include <drogon/HttpClient.h>

namespace echo {

/**
 * @brief The Watchman class is responsible for streaming logs from a Docker container
 * via the Docker Socket and detecting errors using regular expressions.
 */
class Watchman : public std::enable_shared_from_this<Watchman> {
public:
    /**
     * @param containerName The name or ID of the Docker container to monitor.
     */
    explicit Watchman(const std::string& containerName);

    /**
     * @brief Starts the log streaming and monitoring process.
     */
    void start();

    /**
     * @brief Stops the log streaming process.
     */
    void stop();

private:
    /**
     * @brief Establishes the connection to the Docker Socket and initiates the log request.
     */
    void connectAndStream();

    /**
     * @brief Processes incoming chunks of log data.
     * @param chunk The raw data received from the Docker Socket.
     */
    void processLogChunk(const std::string& chunk);

    /**
     * @brief Scans a single log line for error patterns.
     * @param line The log line to analyze.
     */
    void analyzeLine(const std::string& line);

    std::string containerName_;
    drogon::HttpClientPtr dockerClient_;
    std::string buffer_;
    std::regex errorRegex_;
    bool isRunning_;
};

} // namespace echo
