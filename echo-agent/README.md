# Echo Agent: Autonomous DevOps Monitoring Sidecar

Echo is a lightweight, C++ based DevOps monitoring agent designed to run as a "sidecar" container. It monitors logs from other Docker containers in real-time, detects errors using regular expressions, and prepares for AI-driven root cause analysis.

## Project Goal

The primary objective of Echo is to provide an automated way to monitor application logs, identify critical errors, and eventually leverage Large Language Models (LLMs) to explain the root cause and suggest potential fixes. This project is built using the **Drogon C++ framework**, **Docker Socket API**, and **C++ Regular Expressions**.

## Architecture Overview

Echo operates by connecting to the Docker Engine API via the Unix domain socket (`/var/run/docker.sock`). It uses an asynchronous HTTP client to stream logs from target containers. The `Watchman` class processes these logs, applying regex patterns to identify error conditions.

### Project Structure

| Directory/File | Description |
| :--- | :--- |
| `src/watchman/` | Contains the `Watchman` class for log streaming and error detection. |
| `main.cc` | Entry point for the Drogon application. |
| `config.json` | Drogon framework configuration. |
| `docker/` | Dockerfiles for the Echo Agent and a dummy testing application. |
| `docker-compose.yml` | Orchestrates the multi-container setup for local testing. |

## Getting Started

### Prerequisites

- Docker and Docker Compose installed on your system.
- C++17 compatible compiler (if building locally outside of Docker).
- Drogon framework (if building locally).

### Running with Docker Compose

To start the Echo Agent and the dummy testing application, run the following command from the project root:

```bash
docker-compose up --build
```

The Echo Agent will start monitoring the `dummy-app` container. You should see logs in the console indicating when an error is detected:

```text
[Watchman] ERROR DETECTED in dummy-app: ERROR: Database Connection Error - Unable to connect to host 'db:5432'
[Watchman] [PHASE 2] AI will analyze this error...
```

## Phase 2 Roadmap: AI Integration

The next phase of development involves integrating an LLM API (e.g., OpenAI, Gemini) to provide intelligent diagnostics.

1.  **LLM Client:** Extend `Watchman` to call an LLM API asynchronously when an error is detected.
2.  **Contextual Prompts:** Construct prompts that include the error log and surrounding context for better analysis.
3.  **Automated Suggestions:** Parse the LLM's response to present the root cause and suggested fix directly in the logs.

## References

1. [Docker Engine API Reference](https://docs.docker.com/reference/api/engine/version/v1.45/)
2. [Drogon Framework Documentation](https://drogon.org/)
3. [C++ Regular Expressions (<regex>)](https://en.cppreference.com/w/cpp/regex)
