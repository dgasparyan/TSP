#include "Utils.h"

#include "Constants.h"
#include "Coordinates.h"
#include "City.h"

#include <iostream>
#include <chrono>
#include <cmath>
#include <algorithm>

bool isEqual(float first, float second)
{
	return fabs(first - second) < Constants::EPSILION;
}

std::string toLowerCopy(const std::string& str)
{
	std::string result;
	result.reserve(str.size());
	std::transform(str.begin(), str.end(), std::back_inserter(result), ::tolower);
	return result;
}

bool trimAndLowerEqual(std::string_view first, std::string_view second)
{
	// trim both
	first.remove_prefix(std::min(first.find_first_not_of(" "), first.size()));
	first.remove_suffix(std::min(first.find_last_not_of(" "), first.size()));

	second.remove_prefix(std::min(second.find_first_not_of(" "), second.size()));
	second.remove_suffix(std::min(second.find_last_not_of(" "), second.size()));

	// compare
	if (first.size() != second.size())
	{
		return false;
	}

	for (size_t i = 0; i < first.size(); ++i)
	{
		if (tolower(first[i]) != tolower(second[i]))
		{
			return false;
		}
	}

	return true;
}

ScopeTimer::ScopeTimer(const std::string& name)
	: m_name(name)
	, m_start(std::chrono::high_resolution_clock::now())
{
}

ScopeTimer::ScopeTimer(std::string&& name)
	: m_name(std::move(name))
	, m_start(std::chrono::high_resolution_clock::now())
{
}

ScopeTimer::~ScopeTimer()
{
	const auto dur = std::chrono::high_resolution_clock::now() - m_start;
	ThreadSafeLogger::instance().log("##TIMER##: ", m_name, ':', std::chrono::duration_cast<std::chrono::milliseconds>(dur).count(), " ms");
}

ThreadSafeLogger& ThreadSafeLogger::instance()
{
	static ThreadSafeLogger lg;
	return lg;
}
