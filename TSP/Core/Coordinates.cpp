#include "Coordinates.h"

#include "Utils.h"

#include <iostream>

Latitude::Latitude()
	:Base()
{
}

Latitude::Latitude(float degree, float minute, Direction d)
	: Base(degree, minute, d)
{
}

Longitude::Longitude()
	:Base()
{
}

Longitude::Longitude(float degree, float minute, Direction d)
	: Base(degree, minute, d)
{

}

Coordinate::Coordinate(Latitude lat, Longitude lon)
	: m_lat(lat)
	, m_lon(lon)
{
}

Coordinate::operator bool() const
{
	return m_lat && m_lon;
}

Latitude Coordinate::lat() const
{
	assert(*this);
	return m_lat;
}

Longitude Coordinate::lon() const
{
	assert(*this);
	return m_lon;
}

Latitude operator"" _lat(long double degree)
{
	assert(degree >= Latitude::MIN() && degree <= Latitude::MAX());
	return Latitude(static_cast<float>(degree));
}

Longitude operator"" _lon(long double degree)
{
	assert(degree >= Longitude::MIN() && degree <= Longitude::MAX());
	return Longitude(static_cast<float>(degree));
}

float distance(const Coordinate & first, const Coordinate & second)
{
	assert(first);
	assert(second);

	if (first == second)
	{
		return 0.0f;
	}

	const float lat1 = first.lat().toRadians();
	const float lat2 = second.lat().toRadians();
	const float lonDiff = std::abs(second.lon().toRadians() - first.lon().toRadians());

	return Constants::EARTH_RADIUS * acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(lonDiff));
}

bool operator == (Coordinate first, Coordinate second)
{
	return first.lat() == second.lat() && first.lon() == second.lon();
}

bool operator != (Coordinate first, Coordinate second)
{
	return !(first == second);
}

std::ostream & operator<<(std::ostream & stream, const Coordinate & cord)
{
	stream << cord.lat() << ',' << cord.lon();
	return stream;
}
