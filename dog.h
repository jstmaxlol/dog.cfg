// dog.cfg (dog.h)
// version: 0400

#pragma once

#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <type_traits>

namespace pup{
    using Section = std::unordered_map<std::string, std::string>;
    using Config = std::unordered_map<std::string, Section>;

    /// @brief Creates a new (or clears existing) configuration file.
    /// @param filename The path to the config file to create.
    /// @return True if the file was successfully created.
    inline bool Create(const std::string& filename) {
        std::ofstream file(filename);
        return file.good();
    }

    /// @brief Loads a config file from disk into memory.
    /// @param filename The path to the config file.
    /// @return A Config object representing all sections and key-value pairs.
    inline Config Load(const std::string& filename) {
        std::ifstream file(filename);
        Config config;
        std::string line, current_section;

        while(std::getline(file, line)) {
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);
            if (line.empty() || line[0] == '#' || line[0] == ';') continue;

            if (line.front() == '[' && line.back() == ']') {
                current_section = line.substr(1, line.size() - 2);
            } else {
                auto sep = line.find('=');
                if (sep != std::string::npos) {
                    std::string key = line.substr(0, sep);
                    std::string val = line.substr(sep + 1);
                    auto trim = [](std::string& s) {
                        s.erase(0, s.find_first_not_of(" \t\r\n"));
                        s.erase(s.find_last_not_of(" \t\r\n") + 1);
                    };
                    trim(key);
                    trim(val);
                    config[current_section][key] = val;
                }
            }
        }
        return config;
    }

    /// @brief Saves the given Config object to a file.
    /// @param filename The path to write the config to.
    /// @param config The in-memory Config object to save.
    /// @return True if saving was successful.
    inline bool Save(const std::string& filename, const Config& config) {
        std::ofstream file(filename);
        if (!file.good()) return false;
        for (const auto& [section, entries] : config) {
            if (!section.empty())
                file << "[" << section << "]";
            for (const auto& [key, val] : entries) {
                file << key << "=" << val << "\n";
            }
            file << "\n";
        }
        return true;
    }

    /// @brief Converts a Config object into a human-readable string.
    /// @param config The config object to stringify.
    /// @return A formatted string representing the config file contents.
    inline std::string View(const Config& config) {
        std::stringstream ss;
        for (const auto& [section, entries] : config) {
            if (!section.empty())
                ss << "[" << section << "]\n";
            for (const auto& [k, v] : entries)
                ss << k << " = " << v << "\n";
            ss << "\n";
        }
        return ss.str();
    }

    /// @brief Edits (or inserts) a key-value pair in a specific section.
    /// @param config The config object to modify.
    /// @param section The section name.
    /// @param key The key name.
    /// @param value The new value to assign.
    inline void Edit(Config& config, const std::string& section, const std::string& key, const std::string& value) {
        config[section][key] = value;
    }

    /// @brief Removes a key from a section in the config.
    /// @param config The config object to modify.
    /// @param section The section name.
    /// @param key The key to remove.
    inline void Remove(Config& config, const std::string& section, const std::string& key) {
        config[section].erase(key);
    }

    /// @brief Retrieves a value from the config and attempts to convert it to the specified type.
    /// @tparam T The expected return type (e.g., int, bool, std::string).
    /// @param config The config object to search.
    /// @param section The section name.
    /// @param key The key name.
    /// @param fallback A default value returned if the key or section is missing or conversion fails.
    /// @return The value as type T, or the fallback if unavailable.
    template<typename T>
    inline T Get(const Config& config, const std::string& section, const std::string& key, T fallback = T()) {
        auto sec_it = config.find(section);
        if (sec_it == config.end()) return fallback;
        auto key_it = sec_it->second.find(key);
        if (key_it == sec_it->second.end()) return fallback;

        std::istringstream iss(key_it->second);
        T value;
        if (!(iss >> value)) return fallback;
        return value;
    }

    /// @brief Specialization of get() for string values (avoids parsing).
    /// @param config The config object to search.
    /// @param section The section name.
    /// @param key The key name.
    /// @param fallback A fallback string if the key is not found.
    /// @return The string value of the key, or the fallback.
    template<>
    inline std::string pup::Get<std::string>(const Config& config, const std::string& section, const std::string& key, std::string fallback) {
        auto sec_it = config.find(section);
        if (sec_it == config.end()) return fallback;
        auto key_it = sec_it->second.find(key);
        if (key_it == sec_it->second.end()) return fallback;
        return key_it->second;
    }
}
