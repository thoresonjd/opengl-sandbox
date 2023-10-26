/**
 * @file logger.cpp
 * @brief Custom logger that handles basic log output
 * @date October 2023
 */

#include <opengl-sandbox/logger.h>

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

std::tm Logger::getTimestamp() {
	std::time_t now = std::time(0);
	std::tm timestamp;
	localtime_s(&timestamp, &now);
	return timestamp;
}

std::string Logger::timestampToString(const std::tm& timestamp) {
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

std::string Logger::trimString(const std::string& text) {
	std::size_t end = text.length();
	while (std::isspace(text[--end]));
	return text.substr(0, end + 1);
}

bool Logger::requiresConsole(const Output& type) {
	return type == Output::CONSOLE || type == Output::CONSOLE_AND_FILE;
}

bool Logger::requiresFile(const Output& type) {
	return type == Output::FILE || type == Output::CONSOLE_AND_FILE;
}

std::string Logger::outputTypeToString(const Output& type) {
	if (type == Output::CONSOLE)
		return "console";
	if (type == Output::FILE)
		return "file";
	return "console and file";
}