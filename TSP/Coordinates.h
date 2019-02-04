#pragma once
#include <iosfwd>

struct Latitude
{
	enum class Direction
	{
		North = 0,
		South
	};

	inline static const float MIN = -90.0f;
	inline static const float MAX = 90.0f;

	static Direction getNegativeDirection();
	static Direction getPositiveDirection();

public:
	Latitude();
	explicit Latitude(float degree, float minute = 0.0f, Direction d = getPositiveDirection());

	bool isValid() const;

	operator float() const;

	float toRadians() const;

private:
	float m_degrees;
};

struct Longitude
{
	enum class Direction
	{
		East = 0,
		West
	};

	inline static const float MIN = -180.0f;
	inline static const float MAX = 180.0f;

	static Direction getNegativeDirection();
	static Direction getPositiveDirection();

public:
	Longitude();
	explicit Longitude(float degree, float minute = 0.0f, Direction d = getPositiveDirection());

	bool isValid() const;
	operator float() const;

	float toRadians() const;

private:
	float m_degrees;
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

bool operator == (Latitude first, Latitude second);
bool operator == (Longitude first, Longitude second);
bool operator == (Coordinate first, Coordinate second);
bool operator != (Latitude first, Latitude second);
bool operator != (Longitude first, Longitude second);
bool operator != (Coordinate first, Coordinate second);

std::ostream& operator << (std::ostream& stream, const Coordinate& cord);