#include "City.h"

#include <cassert>
#include <vector>
#include <iostream>

City::City(const std::string & name, const Coordinate & coord)
	: m_name(trim(name))
	, m_coordinate(coord)
{
	assert(!m_name.empty());
}

City::City(const std::string & name, Latitude lat, Longitude lon)
	: m_name(trim(name))
	, m_coordinate(lat, lon)
{
	assert(!m_name.empty());
}

City::operator bool() const
{
	return !m_name.empty() && m_coordinate;
}

const std::string & City::getName() const
{
	return m_name;
}

const Coordinate & City::getCoordinates() const
{
	return m_coordinate;
}

City fromString(const std::string & str)
{
	std::vector<std::string> stringVec;
	stringVec.reserve(5);

	splitString(str, ",", std::back_inserter(stringVec));
	if (stringVec.size() != 5)
	{
		std::cout << "Failed to parse the string" << str << std::endl;
		return City();
	}

	// TODO: support SOUTH/EAST as well
	Latitude lat(stof(stringVec[1]), stof(stringVec[2]), Latitude::Direction::North);
	Longitude lon(stof(stringVec[3]), stof(stringVec[4]), Longitude::Direction::West);

	return City(stringVec[0], lat, lon);
}

float distance(const City & first, const City & second)
{
	return distance(first.getCoordinates(), second.getCoordinates());
}

bool operator == (const City& first, const City& second)
{
	// TODO: replace this with a comparison without the copy stuff after testing
	//return trimAndLowerEqual(first.getName(), second.getName());
	return trimAndLowerCopy(first.getName()) == trimAndLowerCopy(second.getName());
}

bool operator != (const City& first, const City& second)
{
	return !(first == second);
}

std::ostream & operator<<(std::ostream & stream, const City & city)
{
	stream << city.getName() << ' ' << city.getCoordinates();
	return stream;
}
