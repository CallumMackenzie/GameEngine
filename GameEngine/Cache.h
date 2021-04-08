#pragma once
#include <unordered_map>

template <typename KEY, typename VALUE>
struct Cache
{
	bool use = false;
	std::unordered_map<KEY, VALUE> cache = std::unordered_map<KEY, VALUE>();

	bool containsKey(KEY key) {
		return cache.find(key) != cache.end();
	}

	void add(KEY key, VALUE value) {
		if (use)
			cache[key] = value;
	}
};

