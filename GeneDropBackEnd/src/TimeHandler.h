#pragma once
#include <ctime>
#include <mutex>
#include <string>


class TimeHandler {
public:
	TimeHandler();
	~TimeHandler();

	std::time_t getCurrentTime();
	std::string makeTimeStamp(const std::time_t time) const;
	std::string getCurrentTimeStamp();

private:
	static std::mutex localTimeMutex;
};

