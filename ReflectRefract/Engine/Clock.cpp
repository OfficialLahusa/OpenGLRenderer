#include "Clock.h"

Clock::Clock() {
	m_LastReset = systemClock.now();
}

Clock::~Clock() {

}

float Clock::reset() {
	std::chrono::duration<float> elapsed = systemClock.now() - m_LastReset;
	m_LastReset = systemClock.now();
	return elapsed.count();
}

float Clock::getElapsedTime() {
	std::chrono::duration<float> elapsed = systemClock.now() - m_LastReset;
	return elapsed.count();
}

std::chrono::time_point<std::chrono::system_clock> Clock::getSystemTime() {
	return systemClock.now();
}
