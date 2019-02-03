#include "Coordinates.h"

#include "Constants.h"
#include "Utils.h"

#include <string>
#include <iostream>
#include <limits>
#include <cassert>

namespace
{
float getInvalid()
{
	return std::numeric_limits<float>::lowest();
}

float toDegree(float degree, float minute)
{
	return degree + (minute / 60);
}

float toRadian(float degree)
{
	return degree * Constants::PI / 180;
}

}

Latitude::Direction Latitude::getNegativeDirection()
{
	return Direction::South;
}

Latitude::Direction Latitude::getPositiveDirection()
{
	return Direction::North;
}

Latitude::Latitude()
	:m_degrees(getInvalid())
{
}

Latitude::Latitude(float degree, float minute, Direction d)
	: m_degrees(toDegree(degree, minute))
{
	if (d == getNegativeDirection())
	{
		m_degrees *= -1;
	}

}

bool Latitude::isValid() const
{
	return m_degrees >= MIN && m_degrees <= MAX;
}

Latitude::operator float() const
{
	assert(isValid());
	return m_degrees;
}

float Latitude::toRadians() const
{
	assert(isValid());
	return toRadian(m_degrees);
}

Longitude::Direction Longitude::getNegativeDirection()
{
	return Direction::West;
}

Longitude::Direction Longitude::getPositiveDirection()
{
	return Direction::East;
}

Longitude::Longitude()
	:m_degrees(getInvalid())
{
}

Longitude::Longitude(float degree, float minute, Direction d)
	: m_degrees(toDegree(degree, minute))
{
	if (d == getNegativeDirection())
	{
		m_degrees *= -1;
	}

}

bool Longitude::isValid() const
{
	return m_degrees >= MIN && m_degrees <= MAX;
}

Longitude::operator float() const
{
	assert(isValid());
	return m_degrees;
}

float Longitude::toRadians() const
{
	assert(isValid());
	return toRadian(m_degrees);
}

Coordinate::Coordinate(Latitude lat, Longitude lon)
	: m_lat(lat)
	, m_lon(lon)
{
}

bool Coordinate::isValid() const
{
	return m_lat.isValid() && m_lon.isValid();
}

Latitude Coordinate::lat() const
{
	assert(isValid());
	return m_lat;
}

Longitude Coordinate::lon() const
{
	assert(isValid());
	return m_lon;
}

// TODO: test!
float distance(const Coordinate & first, const Coordinate & second)
{
	assert(first.isValid());
	assert(second.isValid());

	if (first == second)
	{
		return 0.0f;
	}

	const float lat1 = first.lat().toRadians();
	const float lat2 = second.lat().toRadians();
	const float lonDiff = std::abs(static_cast<Longitude>(second.lon() - first.lon()).toRadians());
	// TODO: remove after testing
	//const float lonDiff = std::abs(second.lon().toRadians() - first.lon().toRadians());

	return Constants::EARTH_RADIUS * acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(lonDiff));
}

bool operator == (Latitude first, Latitude second)
{
	return isEqual(first, second);
}
bool operator == (Longitude first, Longitude second)
{
	return isEqual(first, second);
}

bool operator == (Coordinate first, Coordinate second)
{
	return first.lat() == second.lat() && first.lon() == second.lon();
}

bool operator != (Latitude first, Latitude second)
{
	return !(first == second);
}

bool operator != (Longitude first, Longitude second)
{
	return !(first == second);
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
