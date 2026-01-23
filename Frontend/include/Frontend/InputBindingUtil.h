#pragma once

#include <array>
#include <utility>

namespace rbrown::frontend::binding {
template<typename T>
constexpr auto U(const T& v) { return std::to_underlying(v); }

template<typename K, typename V>
constexpr auto KV(const K& k, const V& v) -> std::pair<K, V> {
    return {k, v};
}

template<typename T, std::size_t N>
constexpr bool AllUnique(const std::array<T, N>& arr) {
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = i + 1; j < N; ++j) {
            if (arr[i] == arr[j])
                return false;
        }
    }
    return true;
}

template<typename K, typename V, std::size_t N>
constexpr bool IsBijection(const std::array<std::pair<K, V>, N>& map) {
    std::array<K, N> keys{};
    std::array<V, N> values{};

    for (std::size_t i = 0; i < N; ++i) {
        keys[i] = map[i].first;
        values[i] = map[i].second;
    }

    return AllUnique(keys) && AllUnique(values);
}

template<typename K, typename V, std::size_t N>
constexpr auto MakeForwardLookup(const std::array<std::pair<K, V>, N>& map,
                                 const V defaultMapping) -> std::array<V, N> {
    std::array<V, N> lookup{};
    lookup.fill(defaultMapping);
    for (const auto& [key, value]: map) {
        lookup[U(key)] = value;
    }
    return lookup;
}

template<std::size_t N, typename K, typename V, std::size_t M>
constexpr auto MakeReverseLookup(const std::array<std::pair<K, V>, M>& map,
                                 const K defaultMapping) -> std::array<K, N> {
    std::array<K, N> lookup{};
    lookup.fill(defaultMapping);
    for (const auto& [key, value]: map) {
        if (value >= 0 && static_cast<std::size_t>(value) < N) {
            lookup[value] = key;
        }
    }
    return lookup;
}
}
