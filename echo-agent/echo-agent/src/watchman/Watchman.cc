#include "Watchman.h"
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <memory>
#include <drogon/HttpAppFramework.h>
#include <drogon/HttpClient.h>

namespace echo {

Watchman::Watchman(const std::string& containerName)
    : containerName_(containerName), isRunning_(false) {
    // 1. Initialize the regular expression to scan for "ERROR" or "Database Connection Error"
    // We'll use a case-insensitive search for flexibility
    errorRegex_ = std::regex(".*ERROR.*|.*Database Connection Error.*", std::regex_constants::icase);

    // 2. Initialize the Drogon HTTP client to communicate with the Docker Socket
    // We'll use the Unix domain socket at /var/run/docker.sock
    dockerClient_ = drogon::HttpClient::newHttpClient("unix:///var/run/docker.sock");
}

void Watchman::start() {
    if (isRunning_) {
        std::cout << "[Watchman] Monitoring for " << containerName_ << " is already running." << std::endl;
        return;
    }

    isRunning_ = true;
    std::cout << "[Watchman] Starting log monitoring for " << containerName_ << "..." << std::endl;
    connectAndStream();
}

void Watchman::stop() {
    isRunning_ = false;
    std::cout << "[Watchman] Stopping log monitoring for " << containerName_ << "..." << std::endl;
}

void Watchman::connectAndStream() {
    if (!isRunning_) return;

    // 3. Construct the Docker Engine API request for container logs
    // GET /containers/{id}/logs?follow=1&stdout=1&stderr=1
    std::string path = "/containers/" + containerName_ + "/logs?follow=1&stdout=1&stderr=1";
    auto req = drogon::HttpRequest::newHttpRequest();
    req->setMethod(drogon::Get);
    req->setPath(path);

    // 4. Send the request asynchronously and process chunks of data as they arrive
    // Note: This is a simplified version for the prototype.
    // In a production app, we would handle potential connection issues and reconnections.
    dockerClient_->sendRequest(req, [this](drogon::ReqResult result, const drogon::HttpResponsePtr& resp) {
        if (result != drogon::ReqResult::Ok) {
            std::cerr << "[Watchman] Failed to connect to Docker Socket for container: " << containerName_ << std::endl;
            return;
        }

        if (resp->statusCode() != drogon::k200OK) {
            std::cerr << "[Watchman] Docker API returned error: " << resp->statusCode() << " for container: " << containerName_ << std::endl;
            return;
        }

        // The response body contains the log stream.
        // We'll process it in chunks.
        processLogChunk(std::string(resp->getBody()));
    });
}

void Watchman::processLogChunk(const std::string& chunk) {
    // 5. Append the new chunk to our buffer
    buffer_ += chunk;

    // 6. Process the buffer to extract and analyze individual log lines
    // Docker's log streaming format uses an 8-byte header for each message:
    // [stream_type (1 byte)][unused (3 bytes)][payload_size (4 bytes)]
    // For simplicity in this prototype, we'll assume the logs are plain text
    // and split by newline characters. This works for many containers,
    // but a robust implementation would parse the 8-byte header.

    size_t pos;
    while ((pos = buffer_.find('\n')) != std::string::npos) {
        std::string line = buffer_.substr(0, pos);
        analyzeLine(line);
        buffer_.erase(0, pos + 1);
    }
}

void Watchman::analyzeLine(const std::string& line) {
    // 7. Use the regular expression to scan the log line for error patterns
    if (std::regex_search(line, errorRegex_)) {
        std::cout << "[Watchman] ERROR DETECTED in " << containerName_ << ": " << line << std::endl;
        
        // 8. Prepare for Phase 2: This is where we would call the LLM API
        // to explain the root cause and suggest a fix.
        // For now, we'll just print a placeholder message.
        std::cout << "[Watchman] [PHASE 2] AI will analyze this error..." << std::endl;
    }
}

} // namespace echo
