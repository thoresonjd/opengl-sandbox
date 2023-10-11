/**
 * @file logger.h
 * @brief Custom logger that handles basic log output
 * @date October 2023
 */

#ifndef LOGGER_H
#define LOGGER_H
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

/**
 * @class Logger - Handles console and file logging
 */
class Logger {
public:

	/**
	 * Represents output types for the logger to consider
	 */
	enum class Output;

private:
	Output type;
	std::ostream* outs[2] = { nullptr, nullptr };
	std::string baseFilePath;

	/**
	 * Retrieves the current time
	 */
	std::tm getTimestamp() const;
	
	/**
	 * Converts a timestamp to a string
	 * @param timestamp - A reference to a timestamp
	 * @return The string representation of the timestamp
	 */
	std::string timestampToString(const std::tm& timestamp) const;

	/**
	 * Removes trailing whitespace from the end of a string
	 * @param text - The string to trim
	 * @return A substring without trailing whitespace
	 */
	std::string trimString(const std::string& text) const;

	/**
	 * Determines if console output is required by an output type
	 */
	bool requiresConsole(const Output& type) const;

	/**
	 * Determines if file output is required by an output type
	 */
	bool requiresFile(const Output& type) const;

	/**
	 * Converts an output type to a string
	 * @param tyoe - An output type
	 * @return The string representation of the output type
	 */
	std::string outputTypeToString(const Output& type) const;

public:

	/**
	 * Default constructor
	 * @param type - The type of output to log to
	 * @param baseFilePath - The base path to log file output
	 */
	Logger(const Output& type, const std::string& baseFilePath = "");
	Logger(const Logger& other) = delete;
	Logger(Logger&& other) = delete;
	Logger& operator=(const Logger& other) = delete;
	Logger& operator=(Logger&& other) = delete;
	~Logger();

	/**
	 * Logs messages to the output stream(s)
	 * @param message - The text to log
	 */
	void log(const std::string& message) const;
};

#endif

enum class Logger::Output {
	CONSOLE,
	FILE,
	CONSOLE_AND_FILE
};

Logger::Logger(const Output& type, const std::string& baseFilePath)
	: type(type), baseFilePath(baseFilePath) {
	if (requiresConsole(type))
		outs[0] = &std::cout;
	if (requiresFile(type)) {
		std::string timestamp = timestampToString(getTimestamp());
		std::string fileName = baseFilePath + timestamp;
		outs[1] = new std::ofstream(fileName);
	}
	log("Logging to " + outputTypeToString(type));
}

Logger::~Logger() {
	if (outs[1]) 
		delete outs[1];
	outs[0] = nullptr;
	outs[1] = nullptr;
}

void Logger::log(const std::string& message) const {
	std::string timestamp = timestampToString(getTimestamp());
	std::string trimmedMessage = trimString(message);
	std::string entry = "[" + timestamp + "] " + trimmedMessage;
	if (outs[0])
		*outs[0] << entry << std::endl;
	if (outs[1])
		*outs[1] << entry << std::endl;
}

std::tm Logger::getTimestamp() const {
	std::time_t now = std::time(0);
	std::tm timestamp;
	localtime_s(&timestamp, &now);
	return timestamp;
}

std::string Logger::timestampToString(const std::tm& timestamp) const {
	std::string year = std::to_string(timestamp.tm_year + 1900);
	std::string month = std::to_string(timestamp.tm_mon + 1);
	std::string day = std::to_string(timestamp.tm_mday);
	std::string hour = std::to_string(timestamp.tm_hour);
	std::string minute = std::to_string(timestamp.tm_min);
	std::string second = std::to_string(timestamp.tm_sec);
	month = timestamp.tm_mon < 9 ? "0" + month : month;
	day = timestamp.tm_mday < 10 ? "0" + day : day;
	hour = timestamp.tm_hour < 10 ? "0" + hour : hour;
	minute = timestamp.tm_min < 10 ? "0" + minute : minute;
	second = timestamp.tm_sec < 10 ? "0" + second : second;
	return year + month + day + "-" + hour + minute + second;
}

std::string Logger::trimString(const std::string& text) const {
	std::size_t end = text.length();
	while (std::isspace(text[--end]));
	return text.substr(0, end + 1);
}

bool Logger::requiresConsole(const Output& type) const {
	return type == Output::CONSOLE || type == Output::CONSOLE_AND_FILE;
}

bool Logger::requiresFile(const Output& type) const {
	return type == Output::FILE || type == Output::CONSOLE_AND_FILE;
}

std::string Logger::outputTypeToString(const Output& type) const {
	if (type == Output::CONSOLE)
		return "console";
	if (type == Output::FILE)
		return "file";
	return "console and file";
}