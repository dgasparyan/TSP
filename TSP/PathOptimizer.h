#pragma once
#include "Common.h"

#include <assert.h>

template <class T>
class PathOptimizer
{
public:
	virtual ~PathOptimizer() = 0
	{}

	virtual PathInfo<T> optimize(Path<T>&& path, bool keepStart = true, bool keepEnd = true) const = 0;
};

template <class T>
using PathOptimizerUPtr = std::unique_ptr<PathOptimizer<T>>;

namespace
{
template <class T>
Path<T> twoOptSwap(const Path<T>& origPath, typename Path<T>::const_iterator beg, typename Path<T>::const_iterator end);

}

template <class T>
class TwoOptOptimizer : public PathOptimizer<T>
{
public:
	// TODO: support keepEnd
	PathInfo<T> optimize(Path<T>&& path, bool keepStart, bool keepEnd) const override
	{
		Path<T> result = std::move(path);
		if (result.size() >= 2)
		{
			restart_from_here: // hope goto-s are ok :)

			// NOTE: we start from 1 since we don't want to change the departure point
			// TODO: support keepEnd in same manner
			for (auto it = keepStart ? result.cbegin() + 1 : result.cbegin(); it < result.cend() - 1; ++it)
			{
				for (auto jit = it + 1; jit < result.cend(); ++jit)
				{
					// TODO: support "EstimateDistance" and swap if less
					Path<T> n = twoOptSwap(result, it, jit);
					if (n.totalDistance() < result.totalDistance())
					{
						result = std::move(n);
						goto restart_from_here;
					}
				}
			}
		}

		return std::make_pair(result, false);
	}

};

namespace
{
template <class T>
Path<T> twoOptSwap(const Path<T>& origPath, typename Path<T>::const_iterator beg, typename Path<T>::const_iterator end)
{
	assert(origPath);
	assert(beg < end && end < origPath.cend());

	Path<T> newPath(origPath.size());

	newPath.push_back(origPath.cbegin(), beg);
	for (auto it = end; it >= beg; --it)
	{
		newPath.push_back(*it);
	}
	newPath.push_back(end + 1, origPath.cend());

	return newPath;
}

}