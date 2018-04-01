//
// Created by islam on 18.03.17.
//

#pragma once

#include <unordered_set>
#include <unordered_map>


/**
 * @brief Simple generic bimap. Doesn't support duplicate values and has other problems for sure
 * @tparam K1
 * @tparam K2
 */
template <class K1, class K2>
class bimap {
    using self = bimap<K1, K2>;

    std::unordered_set<K1> keys1;
    std::unordered_set<K2> keys2;
    std::unordered_map<const K1*, const K2*> k1k2;
    std::unordered_map<const K2*, const K1*> k2k1;
public:
    bimap() = default;

    bimap(std::initializer_list<std::pair<K1, K2>> initializer) {
        for (auto entry : initializer) {
            auto k1 = keys1.insert(entry.first).first;
            auto k2 = keys2.insert(entry.second).first;
            k1k2[&*k1] = &*k2;
            k2k1[&*k2] = &*k1;
        }
    }

    auto begin() const {
        return k1k2.begin();
    }

    auto end() const {
        return k1k2.end();
    }

    auto find(const K1& key) const {
        auto k1 = keys1.find(key);
        return k1 != keys1.end() ? keys2.find(*k1k2.at(&*k1)) : keys2.end();
    }

    auto find(const K2& key) const {
        auto k2 = keys2.find(key);
        return k2 != keys2.end() ? keys1.find(*k2k1.at(&*k2)) : keys1.end();
    }

    bool has(const K1& key) const {
        return find(key) != keys2.end();
    }

    bool has(const K2& key) const {
        return find(key) != keys1.end();
    }

    K2& operator[](const K1& key) {
        auto k1 = keys1.insert(key).first;
        {
            auto k2 = k1k2.find(&*k1);
            if (k2 != k1k2.end()) {
                return *keys2.find(*k2->second);
            }
        }
        auto k2 = keys2.emplace().first;
        k1k2[&*k1] = &*k2;
        k2k1[&*k2] = &*k1;
        return *k2;
    }

    K1& operator[](const K2& key) {
        auto k2 = keys2.insert(key).first;
        {
            auto k1 = k2k1.find(&*k2);
            if (k1 != k2k1.end()) {
                return *keys1.find(*k1->second);
            }
        }
        auto k1 = keys1.emplace().first;
        k1k2[&*k1] = &*k2;
        k2k1[&*k2] = &*k1;
        return *k1;
    }

    const K2& at(const K1& key) const {
        auto k1 = keys1.find(key);

        if (k1 != keys1.end()) {
            return *k1k2.at(&*k1);
        }

        throw std::out_of_range("no such key");
    }

    K2& at(const K1& key) {
        return const_cast<K2&>(static_cast<const self &>(*this).at(key));
    }

    const K1& at(const K2& key) const {
        auto k2 = keys2.find(key);

        if (k2 != keys2.end()) {
            return *k2k1.at(&*k2);
        }

        throw std::out_of_range("no such key");
    }

    K1& at(const K2& key) {
        return const_cast<K1&>(static_cast<const self &>(*this).at(key));
    }
};
