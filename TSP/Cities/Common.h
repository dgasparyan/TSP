#pragma once

#include "City.h"
#include "Path.h"
#include <unordered_set>

template <class T>
using ItemColl = std::unordered_set<T>;
using CityPath = Path<City>;
