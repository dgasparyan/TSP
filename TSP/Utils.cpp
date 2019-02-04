#include "Utils.h"

#include "Coordinates.h"
#include "City.h"

#include <iostream>
#include <chrono>
#include <cmath>
#include <algorithm>

bool isEqual(float first, float second, float epsilion)
{
	return fabs(first - second) < epsilion;
}
std::string_view trimStart(std::string_view str)
{
	str.remove_prefix(std::min(str.find_first_not_of(" "), str.size()));
	return str;
}

std::string_view trimEnd(std::string_view str)
{
	auto fLast = str.find_last_not_of(" ");
	if (fLast != str.npos)
	{
		str.remove_suffix(str.size() - fLast - 1);
	}
	return str;
}

std::string_view trim(std::string_view str)
{
	return trimStart(trimEnd(str));
}

std::string trimAndLowerCopy(const std::string& str)
{
	std::string result;
	std::string_view trimmed = trim(str);
	result.reserve(trimmed.size());
	std::transform(trimmed.begin(), trimmed.end(), std::back_inserter(result), tolower);
	return result;
}

bool trimAndLowerEqual(std::string_view first, std::string_view second)
{
	// trim both
	first = trim(first);
	second = trim(second);

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
