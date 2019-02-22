#pragma once
#pragma once

#include <vector>
#include <iosfwd>

//NOTE: does NOT prvent cycles!
template <class T>
class Path
{
public:
	using ItemColl = std::vector<T>;
	using const_iterator = typename ItemColl::const_iterator;
public:
	Path()
		: m_path()
		, m_distance(0.0f)
	{
	}
	explicit Path(size_t initCapacity)
		: Path()
	{
		m_path.reserve(initCapacity);
	}

	explicit Path(const T& startCity)
		: Path()
	{
		if (startCity)
		{
			m_path.push_back(startCity);
		}
	}

	Path(const Path&) = default;
	Path(Path&&) = default;
	Path& operator=(const Path&) = default;
	Path& operator=(Path&&) = default;

	explicit operator bool() const
	{
		return !m_path.empty();
	}

	// TODO: add estimatePath
	void push_back(const T& t)
	{
		if (!m_path.empty())
		{
			// TODO: add cache support
			m_distance += distance(m_path.back(), t);
		}
		m_path.push_back(t);
	}

	template <class Iterator>
	void push_back(Iterator beg, Iterator end)
	{
		while (beg != end)
		{
			push_back(*beg);
			beg++;
		}
	}

	const ItemColl& getPath() const
	{
		return m_path;
	}

	float totalDistance() const
	{
		return m_distance;
	}

	size_t size() const
	{
		return m_path.size();
	}

	const_iterator cbegin() const
	{
		return m_path.cbegin();
	}
	const_iterator cend() const
	{
		return m_path.cend();
	}

private:
	ItemColl m_path;
	float m_distance;
};

template <class T>
std::ostream& operator << (std::ostream& str, const Path<T>& path)
{
	for (const T& t : path.getPath())
	{
		str << t.getName() << "->";
	}
	str << std::endl << "## Distance ## = " << path.totalDistance() << std::endl;

	return str;

}
