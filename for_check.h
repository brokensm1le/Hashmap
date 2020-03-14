//
// Created by user on 15.02.2020.
//

#ifndef UNTITLED2_HASHMAP_H
#define UNTITLED2_HASHMAP_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <memory>

template<class KeyType, class ValueType, class Hash = std::hash<KeyType> > class
HashMap {
public:
    using iterator = typename list < pair <const KeyType, ValueType > > :: iterator;
    using const_iterator = typename list < pair < const KeyType, ValueType > > :: const_iterator;
    const size_t size_default = 1000;

    iterator begin() noexcept;

    const_iterator begin() const noexcept;

    iterator end() noexcept;

    const_iterator end() const noexcept;

    HashMap(Hash hasher = Hash());

    HashMap(initializer_list < pair < KeyType, ValueType > > inlist, Hash hasher = Hash());

    template<class TypeIterator>
    HashMap(TypeIterator start, TypeIterator finish, Hash hasher = Hash());

    size_t size() const;

    bool empty() const;

    const Hash hash_function() const;

    void insert(pair <const KeyType, ValueType> ins_pair);

    void erase(KeyType key);

    void clear();

    iterator find(KeyType key)  noexcept;

    const_iterator find(KeyType key) const noexcept;

    const ValueType &at(KeyType key) const;

    ValueType &operator[](const KeyType key);

    HashMap &operator=(HashMap &mp);
private:
    size_t size_tab;
    Hash hasher;
    list <pair<const KeyType, ValueType>> listnode;
    vector <list<iterator>> tab;

    void rebuild() {
        tab.clear();
        tab.resize(size_tab * 2);
        for (iterator j = listnode.begin(); j != listnode.end(); ++j) {
            size_t hash = hasher(j->first) % (size_tab * 2);
            tab[hash].push_back(j);
        }
        size_tab *= 2;
    }
};

template<class KeyType, class ValueType, class Hash>
HashMap::iterator HashMap<KeyType, ValueType, Hash>::begin() noexcept {
    return listnode.begin();
}

template<class KeyType, class ValueType, class Hash>
HashMap::const_iterator HashMap<KeyType, ValueType, Hash>::begin() const noexcept {
    return listnode.begin();
}

template<class KeyType, class ValueType, class Hash>
HashMap::iterator HashMap<KeyType, ValueType, Hash>::end() noexcept {
    return listnode.end();
}

template<class KeyType, class ValueType, class Hash>
HashMap::const_iterator HashMap<KeyType, ValueType, Hash>::end() const noexcept {
    return listnode.end();
}

template<class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::HashMap(Hash hasher) : size_tab(size_default), hasher(hasher), tab(size_default) {}

template<class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::HashMap(initializer_list<pair<KeyType, ValueType>> inlist, Hash hasher): HashMap() {
    for (auto&& i : inlist) {
        insert(i);
    }
}

template<class KeyType, class ValueType, class Hash>

template<class TypeIterator>
HashMap<KeyType, ValueType, Hash>::HashMap(TypeIterator start, TypeIterator finish, Hash hasher): HashMap() {
    for (auto&& i = start; i != finish; ++i) {
        insert(*i);
    }
}

template<class KeyType, class ValueType, class Hash>
size_t HashMap<KeyType, ValueType, Hash>::size() const {
    return listnode.size();
}

template<class KeyType, class ValueType, class Hash>
bool HashMap<KeyType, ValueType, Hash>::empty() const {
    return listnode.empty();
}

template<class KeyType, class ValueType, class Hash>
const Hash HashMap<KeyType, ValueType, Hash>::hash_function() const {
    return hasher;
}

template<class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::insert(pair<const KeyType, ValueType> ins_pair) {
    KeyType key = ins_pair.first;
    if (find(key) != listnode.end()) {
        return;
    }
    size_t new_hash = hasher(key) % size_tab;
    listnode.push_back(ins_pair);
    iterator new_it = prev(listnode.end());
    tab[new_hash].push_back(new_it);
    if (tab[new_hash].size() > size_tab / size_default) {
        rebuild();
    }
}

template<class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::erase(KeyType key) {
    size_t hash = hasher(key) % size_tab;
    if (find(key) != listnode.end()) {
        auto it = tab[hash].begin();
        for (auto&& j : tab[hash]) {
            if (j->first == key) {
                listnode.erase(j);
                tab[hash].erase(it);
                break;
            }
            it++;
        }
    }
}

template<class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::clear() {
    listnode.clear();
    tab.clear();
    size_tab = size_default;
    tab.resize(size_default);
}

template<class KeyType, class ValueType, class Hash>
HashMap::iterator HashMap<KeyType, ValueType, Hash>::find(KeyType key) noexcept {
    size_t hash = hasher(key) % size_tab;
    for (auto&& j : tab[hash]) {
        if (key == j->first)
            return j;
    }
    return listnode.end();
}

template<class KeyType, class ValueType, class Hash>
HashMap::const_iterator HashMap<KeyType, ValueType, Hash>::find(KeyType key) const noexcept {
    size_t hash = hasher(key) % size_tab;
    for (auto&& j : tab[hash]) {
        if (key == j->first)
            return j;
    }
    return listnode.end();
}

template<class KeyType, class ValueType, class Hash>
const ValueType &HashMap<KeyType, ValueType, Hash>::at(KeyType key) const {
    if (find(key) == listnode.end())
        throw out_of_range("");
    return find(key)->second;
}

template<class KeyType, class ValueType, class Hash>
ValueType &HashMap<KeyType, ValueType, Hash>::operator[](const KeyType key) {
    if (find(key) == listnode.end()) {
        insert(pair<KeyType, ValueType> (key, {}));
        return find(key)->second;
    }
    return find(key)->second;
}

template<class KeyType, class ValueType, class Hash>
HashMap &HashMap<KeyType, ValueType, Hash>::operator=(HashMap &mp) {
    if (this == &mp) {
        return *this;
    }
    clear();
    for (auto&& i : mp) {
        insert(i);
    }
    return *this;
}

#endif //UNTITLED2_HASHMAP_H
