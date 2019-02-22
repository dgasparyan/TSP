#pragma once
#include "Path.h"
#include "Utils.h"

#include "PathOptimizer.h"

#include <cassert>

template <class T>
class PathFinder
{
public:
	PathFinder() = default;
	virtual ~PathFinder() = 0
	{}

	virtual const std::string& getName() const = 0;

	// TODO; CACHE
	virtual Path<T> findPath(const ItemColl<T>& items, const T& start/*, WeigthCachePtr cache = nullptr*/) const = 0;
};

template <class T>
using PathFinderUPtr = std::unique_ptr<PathFinder<T>>;

template <class T>
class NaivePathFinder : public PathFinder<T>
{
public:
	const std::string& getName() const override
	{
		static const std::string RESULT = "NaivePathFinder";
		return RESULT;
	}
	Path<T> findPath(const ItemColl<T>& items, const T& start/*, WeigthCachePtr cache = nullptr*/) const override
	{
		ScopeTimer st("Find Path: " + getName());
		// TODO: cache
		Path<T> result(start);
		for (const T& item : items)
		{
			assert(item);
			result.push_back(item);
		}

		return result;
	}
};

namespace
{
template <class T, class Container>
T findAndRemoveNearest(const T& from, Container& itemList);

}

template <class T>
class NearestNeighbourPathFinder : public PathFinder<T>
{
public:
	const std::string& getName() const override
	{
		static const std::string RESULT = "NearestNeighbourPathFinder";
		return RESULT;
	}

	Path<T> findPath(const ItemColl<T>& items, const T& startT/*, WeigthCachePtr cache = nullptr*/) const override
	{
		ScopeTimer st("Find Path: " + getName());

		assert(!items.empty());
		T start = startT ? startT : *items.begin();
		assert(start);

		std::list<T> itemList;
		for (const T& item : items)
		{
			if (item != start)
			{
				itemList.push_back(item);
			}
		}

		Path<T> result(start);
		T next = findAndRemoveNearest(start, itemList);
		while (next)
		{
			result.push_back(next);
			next = findAndRemoveNearest(next, itemList);
		}

		// make sure we processed all items
		assert(itemList.empty());
		return result;
	}
};

template <class T>
class OptimizedPathFinder : public PathFinder<T>
{
public:
	OptimizedPathFinder(PathFinderUPtr<T>&& finder, PathOptimizerUPtr<T>&& optimizer) 
		: m_finder(std::move(finder))
		, m_optimizer(std::move(optimizer))
	{}

	const std::string& getName() const override
	{
		static const std::string RESULT = "OptimizedPathFinder";
		return RESULT;
	}

	Path<T> findPath(const ItemColl<T>& items, const T& startT/*, WeigthCachePtr cache = nullptr*/) const override
	{
		assert(m_finder);
		assert(m_optimizer);
		ScopeTimer t("Find Path: Optimized " + m_finder->getName());
		return m_optimizer->optimize(m_finder->findPath(items, startT));
	}

private:
	PathFinderUPtr<T> m_finder;
	PathOptimizerUPtr<T> m_optimizer;
};

namespace
{
// TODO: cache support
template <class T, class Container>
T findAndRemoveNearest(const T& from, Container& itemList)
{
	assert(from);
	if (itemList.empty())
	{
		return T{};
	}

	float min = std::numeric_limits<float>::max();
	auto minIt = itemList.end();
	for (auto it = itemList.begin(); it != itemList.end(); ++it)
	{
		T item = *it;
		assert(item);
		if (item == from)
		{
			continue;
		}
		// TODO: cache support
		auto dist = distance(from, item);
		if (dist < min)
		{
			min = dist;
			minIt = it;
		}
	}

	T res;
	if (minIt != itemList.end())
	{
		res = *minIt;
		itemList.erase(minIt);
	}

	return res;
}
}
