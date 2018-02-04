#include "TimeHandler.h"
#include <chrono>
#include <sstream>
#include <iomanip>

// Disables unsafe warning for localtime
 #pragma warning(disable: 4996)


std::mutex TimeHandler::localTimeMutex;


TimeHandler::TimeHandler() {}


TimeHandler::~TimeHandler() {}


std::time_t TimeHandler::getCurrentTime() {
	std::lock_guard<std::mutex> lock(TimeHandler::localTimeMutex);

	auto timeNow = std::chrono::system_clock::now();
	auto convertedTime = std::chrono::system_clock::to_time_t(timeNow);

	return convertedTime;
}


std::string TimeHandler::makeTimeStamp(const std::time_t time) const {
	std::stringstream stampStream;
	stampStream << std::put_time(std::localtime(&time), "%H-%M-%S@%d-%m-%Y");
	return stampStream.str();
}


std::string TimeHandler::getCurrentTimeStamp() {
	return makeTimeStamp(getCurrentTime());
}
