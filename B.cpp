#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <pthread.h>
#include <dirent.h>
#include <ctime>
#include <bits/stdc++.h>
#include <vector>

std::vector<std::string> anomalies; // To store anomalies
std::unordered_map<std::string, int> patterns; // Pattern and its threshold
std::unordered_map<std::string, int> timezones; // Server and its timezone offset
pthread_mutex_t reportMutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for report file synchronization

// Structure to pass arguments to threads
struct ThreadArgs {
    std::string server;
    std::string logFile;
};

// Function to parse the configuration file
void parseConfig(const std::string& configFile) {
    std::ifstream file(configFile);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error opening config file.\n";
        return;
    }

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string token;

        if (line.find("Pattern") != std::string::npos) {
            std::string pattern;
            int threshold;
            ss >> token >> pattern;
            ss >> token >> threshold;
            patterns[pattern] = threshold;
        } else if (line.find("Server") != std::string::npos) {
            std::string server;
            int timezone;
            ss >> token >> server;
            ss >> token >> timezone;
            timezones[server] = timezone;
        }
    }
}

// Function to process a log file for a specific server
void* processLogFile(void* args) {
    ThreadArgs* threadArgs = (ThreadArgs*)args;
    std::string server = threadArgs->server;
    std::string logFile = threadArgs->logFile;

    std::ifstream file(logFile);
    std::string line;
    std::unordered_map<std::string, int> patternCount; // Stores counts for patterns in a log

    if (!file.is_open()) {
        std::cerr << "Error opening log file: " << logFile << "\n";
        return nullptr;
    }

    while (std::getline(file, line)) {
        for (const auto& pattern : patterns) {
            if (line.find(pattern.first) != std::string::npos) {
                patternCount[pattern.first]++;
            }
        }
    }

    // Identify anomalies
    for (const auto& count : patternCount) {
        if (count.second >= patterns[count.first]) {
            std::string anomaly = logFile.substr(logFile.find_last_of("/") + 1);
            pthread_mutex_lock(&reportMutex);
            anomalies.push_back(anomaly + ", " + server + ", " + count.first + ", " + std::to_string(count.second) + " times");
            pthread_mutex_unlock(&reportMutex);
        }
    }

    delete threadArgs; // Free memory allocated for thread arguments
    return nullptr;
}

// Function to process all logs in a directory (server directory)
void processLogsInServerDir(const std::string& server, const std::string& serverDir) {
    DIR* dir = opendir(serverDir.c_str());
    if (!dir) {
        std::cerr << "Could not open directory: " << serverDir << "\n";
        return;
    }

    struct dirent* entry;
    std::vector<pthread_t> threads;

    while ((entry = readdir(dir)) != nullptr) {
        std::string filename(entry->d_name);
        if (filename.find(".log") != std::string::npos) {
            // For each log file, create a new thread for processing
            std::string logFile = serverDir + "/" + filename;
            ThreadArgs* args = new ThreadArgs{server, logFile};

            pthread_t thread;
            pthread_create(&thread, nullptr, processLogFile, (void*)args);
            threads.push_back(thread);
        }
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        pthread_join(thread, nullptr);
    }

    closedir(dir);
}

// Function to write anomalies to report file
void writeReport(const std::string& reportFile) {
    std::ofstream report(reportFile);

    if (!report.is_open()) {
        std::cerr << "Error opening report file.\n";
        return;
    }

    for (const auto& anomaly : anomalies) {
        report << anomaly << std::endl;
    }

    report.close();
}

// Main function
int main() {
    std::string configFile = "config.txt";
    std::string logsDir = "./logs";  // Directory where server subdirectories are stored
    std::string reportFile = "report.txt";

    // Parse the configuration file
    parseConfig(configFile);

    // Process logs from all servers
    for (const auto& server : timezones) {
        std::string serverDir = logsDir + "/" + server.first;
        processLogsInServerDir(server.first, serverDir);
    }

    // Write the report to file
    writeReport(reportFile);

    std::cout << "Report generated successfully in " << reportFile << std::endl;
    return 0;
}
