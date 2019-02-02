#pragma once
#include "Common.h"
#include "PathFinder.h"

#include <iostream>
#include <cassert>
#include <future>

template <class T>
class Solver
{
public:
	using FinderPtr = std::unique_ptr<PathFinder<T>>;
	using FinderColl = std::vector<FinderPtr>;
public:
	explicit Solver(ItemColl<T>&& cities, const T& start = T{})
		: m_cities(std::move(cities))
		, m_finders()
		, m_start(start)
	{
		assert(!m_cities.empty());
	}

	explicit Solver(const ItemColl<T>& cities, const T& start = T{})
		: m_cities(cities)
		, m_finders()
		, m_start(start)
	{
		assert(!m_cities.empty());
	}

	void emplaceFinder(FinderPtr&& finder)
	{
		m_finders.emplace_back(std::move(finder));
	}

	PathInfo<T> getOptimalPath() const
	{
		ScopeTimer t ("FINDING OPTIMAL PATH");

		assert(!m_finders.empty());
		assert(!m_cities.empty());

		if (m_finders.size() == 1)
		{
			return findPath(m_finders.front());
		}

		std::vector<std::future<PathInfo<T>>> futures;
		futures.reserve(m_finders.size() - 1);
		for (auto it = m_finders.begin() + 1; it != m_finders.end(); ++it)
		{
			futures.emplace_back(std::async(std::launch::async, &Solver<T>::findPath, this, std::cref(*it)));
		}

		PathInfo<T> minPath = findPath(m_finders.front());
		if (minPath.second)
		{
			return minPath;
		}
		ThreadSafeLogger::instance().log("current min path length = ", minPath.first.totalDistance());
		for(auto& fut : futures)
		{
			try
			{
				PathInfo<T> newPath = fut.get();
				if (newPath.first.totalDistance() < minPath.first.totalDistance())
				{
					minPath = std::move(newPath);
				}
			}
			catch (std::exception& exp)
			{
				std::cerr << " Exception while trying to find the path! " << exp.what() << std::endl;
				// swallow the exception.
			}
		}

		return minPath;
	}

private:
	PathInfo<T> findPath(const FinderPtr& finder) const
	{
		assert(finder);
		return finder->findPath(m_cities, m_start/*, TODO: handle cache*/);
	}

private:
	ItemColl<T> m_cities;
	FinderColl m_finders;
	T m_start;
};

