#pragma once
#include "Constants.h"
#include "Utils.h"

#include <iosfwd>
#include <limits>
#include <cassert>
#include <type_traits>

namespace
{
float getInvalid()
{
	return std::numeric_limits<float>::lowest();
}
}

// NOTE: the whole CRTP is not really necessary (or wise in this case as it 
// overcomplicates things with very little gains in avoiding code-duplication
// , it's just done because we can (and want to practice :) ).

// This helps with CRTP below. We can't directly refer to enums declared in Derived class
// from the base class since Derived is not fully defined while instantiating Base.
// So we'll use a trait class to provide those enums, and typedef(using...) in the derived class.
template<class> struct SingleCoordinateTraits;

struct Latitude;
template <>
struct SingleCoordinateTraits<Latitude>
{
	enum class Direction
	{
		North = 0,
		South
	};
	static float MIN()
	{
		return -90.0f;
	}

	static float MAX()
	{
		return 90.0f;
	}
	static Direction getNegativeDirection()
	{
		return Direction::North;
	}
	static Direction getPositiveDirection()
	{
		return Direction::South;
	}

};

struct Longitude;
template <>
struct SingleCoordinateTraits<Longitude>
{
	enum class Direction
	{
		East = 0,
		West
	};

	static float MIN()
	{
		return -180.0f;
	}
	static float MAX()
	{
		return 180.0f;
	}
	static Direction getNegativeDirection()
	{
		return Direction::East;
	}
	static Direction getPositiveDirection()
	{
		return Direction::West;
	}

};

template <class Derived>
struct SingleCoordinateCRTP
{
	using DTraits = typename SingleCoordinateTraits<Derived>;
	using Direction = typename DTraits::Direction;

	static Direction getNegativeDirection()
	{
		return DTraits::getNegativeDirection();
	}
	static Direction getPositiveDirection()
	{
		return DTraits::getPositiveDirection();
	}
	
	static float MIN()
	{
		return DTraits::MIN();
	}
	static float MAX()
	{
		return DTraits::MAX();
	}

	SingleCoordinateCRTP()
		:m_degrees(getInvalid())
	{}

	explicit SingleCoordinateCRTP(float degree, float minute = 0.0f, 
							 typename DTraits::Direction d = Derived::getPositiveDirection())
		: m_degrees(degree + (minute / 60))
	{
		if (d == Derived::getNegativeDirection())
		{
			m_degrees *= -1;
		}
	}

	bool isValid() const
	{
		return m_degrees >= MIN() && m_degrees <= MAX();
	}

	operator float() const
	{
		assert(isValid());
		return m_degrees;
	}

	float toRadians() const
	{
		assert(isValid());
		return m_degrees * Constants::PI / 180;
	}

private:
	float m_degrees;
};

struct Latitude : public SingleCoordinateCRTP<Latitude>
{
	Latitude();
	explicit Latitude(float degree, float minute = 0.0f, Direction d = getPositiveDirection());
private:
	using Base = SingleCoordinateCRTP<Latitude>;
};

struct Longitude : public SingleCoordinateCRTP<Longitude>
{
	Longitude();
	explicit Longitude(float degree, float minute = 0.0f, Direction d = getPositiveDirection());
private:
	using Base = SingleCoordinateCRTP<Longitude>;
};

struct Coordinate
{
	Coordinate() = default;
	Coordinate(Latitude lat, Longitude lon);

	bool isValid() const;

	Latitude lat() const;
	Longitude lon() const;

private:
	Latitude m_lat;
	Longitude m_lon;
};

// returns the distance between the cities based on spherical law of cosines formula
// https://en.wikipedia.org/wiki/Great-circle%5Fdistance#Formulas
// This does have rounding errors when distances are too small, but it works well for our case
// since we are calculating flight distances(which would be more than 1km :) )
// alternative would be to use haversine formula
float distance(const Coordinate& first, const Coordinate& second);

namespace
{
template <class T> using enable_equality_opeartor_v = std::enable_if_t<std::is_base_of_v<SingleCoordinateCRTP<T>, T>>;
}

template<class T, enable_equality_opeartor_v<T>>
bool operator == (T first, T second)
{
	return isEqual(first, second);
}

template<class T, enable_equality_opeartor_v<T>>
bool operator != (T first, T second)
{
	return !(first == second);
}

bool operator == (Coordinate first, Coordinate second);
bool operator != (Coordinate first, Coordinate second);

std::ostream& operator << (std::ostream& stream, const Coordinate& cord);