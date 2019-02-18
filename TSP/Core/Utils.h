#pragma once

#include "Constants.h"

#include <cassert>
#include <string>
#include <mutex>

bool isEqual(float first, float second, float epsilion = Constants::EPSILION);

std::string_view trimStart(std::string_view str);
std::string_view trimEnd(std::string_view str);
std::string_view trim(std::string_view str);

std::string trimAndLowerCopy(const std::string& str);
bool trimAndLowerEqual(std::string_view first, std::string_view second);

template <class OutputIt>
void splitString(const std::string& str, const std::string& delim, OutputIt it)
{
	assert(!str.empty());
	assert(!delim.empty());
	size_t prev = 0, next = str.find_first_of(delim);
	while (next != std::string::npos)
	{
		*it = str.substr(prev, next - prev);
		++it;
		prev = next + 1;
		// this is safe even if prev > str.size(), it will just return npos
		next = str.find_first_of(delim, prev);
	}
	// append any leftover suffix.
	if (prev < str.size())
	{
		*it = str.substr(prev, str.size() - prev);
		++it;
	}
}

// Simple RAII timer
class ScopeTimer
{
public:
	explicit ScopeTimer(const std::string& name);
	explicit ScopeTimer(std::string&& name);
	~ScopeTimer();

	ScopeTimer(const ScopeTimer&) = delete;
	ScopeTimer& operator = (const ScopeTimer&) = delete;

private:
	std::string m_name;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};

// a very(!) basic thread safe logger
class ThreadSafeLogger
{
public:
	static ThreadSafeLogger& instance();

	template<class... Args>
	void log(Args&&... args)
	{
		std::lock_guard<std::mutex> l(m_mutex);
		(std::cout << ... << args) << std::endl;
	}

private:
	ThreadSafeLogger() = default;

	ThreadSafeLogger(const ThreadSafeLogger&) = delete;
	ThreadSafeLogger& operator = (const ThreadSafeLogger&) = delete;

private:
	std::mutex m_mutex;
};
