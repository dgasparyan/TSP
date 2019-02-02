#pragma once

#include "Utils.h"
#include "Coordinates.h"

#include <iosfwd>
#include <string>

class City
{
public:
	City() = default;
	City(const std::string& name, const Coordinate& coord);
	City(const std::string& name, Latitude lat, Longitude lon);

	bool isValid() const;

	const std::string& getName() const;
	const Coordinate& getCoordinates() const;

private:
	std::string m_name;
	Coordinate  m_coordinate;
};

City fromString(const std::string& str);
float distance(const City& first, const City& second);

bool operator == (const City& first, const City& second);
bool operator != (const City& first, const City& second);
std::ostream& operator << (std::ostream& stream, const City& city);

namespace std
{
template <>
struct hash<City>
{
	size_t operator() (const City& city) const
	{
		return hash<std::string>()(toLowerCopy(city.getName()));
	}
};
}
