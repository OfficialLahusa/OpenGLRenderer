#pragma once

#include <chrono>
#include <thread>


class Clock {

private:
	std::chrono::system_clock systemClock;
	std::chrono::time_point<std::chrono::system_clock> m_LastReset;

public:

	Clock();
	~Clock();

	float reset();
	float getElapsedTime();
	std::chrono::time_point<std::chrono::system_clock>  getSystemTime();

};