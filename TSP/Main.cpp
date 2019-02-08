// Main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Common.h"
#include "Solver.h"
#include "PathFinder.h"

#include <iostream>
#include <string_view>

#ifdef TESTING
#include <gtest/gtest.h>
#endif

// TODO: make more generic (?)
#include <fstream>
namespace
{
	ItemColl<City> loadCities(std::string_view fileName)
	{
		std::fstream stream(fileName.data());
		if (!stream.is_open())
		{
			std::cerr << "Failed to open the file " << fileName << std::endl;
			return ItemColl<City>();
		}

		ItemColl<City> cities;
		std::string line;
		while (std::getline(stream, line))
		{
			if (line.empty())
			{
				std::cout << "WARNING: Empty line, will skip!" << std::endl;
				continue;
			}

			try
			{
				City c = fromString(line);
				if (c)
				{
					cities.emplace(std::move(c));
				}
				else
				{
					std::cout << "WARNING: Failed to convert following line into valid City! " << line << std::endl;
				}
			}
			catch (std::exception& exp)
			{
				std::cout << "WARNING: EXCEPTION while processing line " << line << "!! Exception is " << exp.what() << std::endl;
			}
		}

		return cities;

	}

}

int main(int argc, char* argv[])
{
#ifdef TESTING
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#endif // TESTING


	ScopeTimer t("Main");
	if (argc < 2)
	{
		std::cout << "ERROR: Expected file name!" << std::endl;
		return -1;
	}
	ItemColl<City> cities = loadCities(argv[1]);
	if (cities.empty())
	{
		std::cout << "Could not load list of cities" << std::endl;
		return -1;
	}

	std::string startCity = argc >= 3 ? argv[2] : std::string();

	auto it = startCity.empty() ? cities.end() : cities.find(City(startCity, Coordinate()));
	City start = it == cities.end() ? City() : *it;
	Solver<City> s(std::move(cities), start);

	s.emplaceFinder(std::make_unique<NaivePathFinder<City>>());
	s.emplaceFinder(std::make_unique<NearestNeighbourPathFinder<City>>());

	std::unique_ptr finder = std::make_unique<NearestNeighbourPathFinder<City>>();
	std::unique_ptr optimizer = std::make_unique<TwoOptOptimizer<City>>();
	s.emplaceFinder(std::make_unique<OptimizedPathFinder<City>>(std::move(finder), std::move(optimizer)));

	auto[path, isOptimal] = s.getOptimalPath();
	std::cout << "Optimal Path is: " << std::endl << path << std::endl;
}

