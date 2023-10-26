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
	enum class Output {
		CONSOLE,
		FILE,
		CONSOLE_AND_FILE
	};

private:
	Output type;
	std::ostream* outs[2] = { nullptr, nullptr };
	std::string baseFilePath;

	/**
	 * Retrieves the current time
	 */
	static std::tm getTimestamp();
	
	/**
	 * Converts a timestamp to a string
	 * @param timestamp - A reference to a timestamp
	 * @return The string representation of the timestamp
	 */
	static std::string timestampToString(const std::tm& timestamp);

	/**
	 * Removes trailing whitespace from the end of a string
	 * @param text - The string to trim
	 * @return A substring without trailing whitespace
	 */
	static std::string trimString(const std::string& text);

	/**
	 * Determines if console output is required by an output type
	 */
	static bool requiresConsole(const Output& type);

	/**
	 * Determines if file output is required by an output type
	 */
	static bool requiresFile(const Output& type);

	/**
	 * Converts an output type to a string
	 * @param tyoe - An output type
	 * @return The string representation of the output type
	 */
	static std::string outputTypeToString(const Output& type);

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