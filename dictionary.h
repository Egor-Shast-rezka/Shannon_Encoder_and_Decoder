/*
    Egor Shastin st129457@student.spbu.ru
    
    > Custom implementation of a hash map using separate chaining.
    > Provides basic operations: insert, find, access via [], size, and iteration.
*/


#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <stdexcept>
#include <functional>


#ifndef DICTIONARY_H
#define DICTIONARY_H


// ================== map ==================

template <typename Key, typename Value>
class map {
private:

    std::vector<std::list<std::pair<Key, Value>>> table;
    size_t current_size;
    float load_factor_threshold;
    mutable std::vector<std::pair<Key, Value>> flat_data_cache;

    size_t get_index(const Key& key) const {

        std::hash<Key> hasher;
        return hasher(key) % table.size();
    }

    void resize() {

        size_t new_capacity = table.size() * 2;
        std::vector<std::list<std::pair<Key, Value>>> new_table(new_capacity);

        for (const auto& bucket : table) {
            for (const auto& elem : bucket) {

                size_t new_index = std::hash<Key>{}(elem.first) % new_capacity;
                new_table[new_index].emplace_back(elem.first, elem.second);
            }
        }

        table = std::move(new_table);
    }

public:

    map(size_t initial_capacity = 16, float load_factor = 0.5f)
        : table(initial_capacity), current_size(0), load_factor_threshold(load_factor) {}

    Value* find(const Key& key) {

        size_t index = get_index(key);
        auto& bucket = table[index];
        for (auto& pair : bucket) if (pair.first == key) return &pair.second;

        return nullptr;
    }

    void insert(const Key& key, const Value& value) {

        size_t index = get_index(key);
        auto& bucket = table[index];

        for (auto& pair : bucket) {
            if (pair.first == key) {

                pair.second = value;
                return;
            }
        }

        bucket.emplace_back(key, value);
        ++current_size;

        if (((float)current_size / table.size()) > load_factor_threshold) {
            resize();
        }
    }

    Value& operator[](const Key& key) {

        size_t index = get_index(key);
        auto& bucket = table[index];

        for (auto& pair : bucket) if (pair.first == key) return pair.second;

        bucket.emplace_back(key, Value{});
        ++current_size;

        if ((float)current_size / table.size() > load_factor_threshold) resize();

        return bucket.back().second;
    }

    size_t size() const {
        return current_size;
    }

    auto begin() {

        flat_data_cache.clear();

        for (const auto& bucket : table) {
            for (const auto& pair : bucket) {

                flat_data_cache.push_back(pair);
            }
        }
        return flat_data_cache.begin();
    }

    auto end() {
        return flat_data_cache.end();
    }
    
    void print() const {
    for (const auto& bucket : table) {
        for (const auto& pair : bucket) {
            std::cout << pair.first << " : " << pair.second << "\n";
        }
    }
}
};


#endif
