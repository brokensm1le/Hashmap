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

using namespace std;

template<class KeyType, class ValueType, class Hash = std::hash<KeyType> > class HashMap {

public:
    using iterator = typename list < pair <const KeyType, ValueType > > :: iterator;
    using const_iterator = typename list < pair < const KeyType, ValueType > > :: const_iterator;
    const size_t size_default = 1000;

    iterator begin() noexcept{
        return listnode.begin();
    }

    const_iterator begin() const noexcept{
        return listnode.begin();
    }

    iterator end() noexcept {
        return listnode.end();
    }

    const_iterator end() const noexcept {
        return listnode.end();
    }


    HashMap(Hash hasher = Hash()) : size_tab(size_default), hasher(hasher), tab(size_default) {} // 1 task

    HashMap(initializer_list < pair < KeyType, ValueType > > inlist, Hash hasher = Hash()): HashMap() {   // 3 task
        for (auto&& i: inlist) {
            insert(i);
        }
    }

    template<class TypeIterator>
    HashMap(TypeIterator start,TypeIterator finish, Hash hasher = Hash()): HashMap() {// 2 task
        for (auto&& i = start; i != finish; ++i){
            insert(*i);
        }
    }

    size_t size() const {                        // 5 task
        return listnode.size();
    }

    bool empty() const {                           // 5 task
        return listnode.empty();
    }

    const Hash hash_function() const {             // 6 task
        return hasher;
    }


    void insert(pair <const KeyType, ValueType> ins_pair) {       // task 7
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

    void erase(KeyType key) {                                     // task 8
        size_t hash = hasher(key) % size_tab;
        if (find(key) != listnode.end()){
            auto it = tab[hash].begin();
            for (auto&& j: tab[hash]){
                if (j->first == key){
                    listnode.erase(j);
                    tab[hash].erase(it);
                    break;
                }
                it++;
            }
        }
    }


    void clear(){                                                  // task 13
        listnode.clear();
        tab.clear();
        size_tab = size_default;
        tab.resize(size_default);
    }


    iterator find(KeyType key)  noexcept {                              // task 10
        size_t hash = hasher(key) % size_tab;
        for (auto&& j : tab[hash]) {
            if (key == j->first)
                return j;
        }
        return listnode.end();
    }

    const_iterator find(KeyType key) const noexcept {                    // task 10
        size_t hash = hasher(key) % size_tab;
        for (auto&& j : tab[hash]) {
            if (key == j->first)
                return j;
        }
        return listnode.end();
    }

    const ValueType &at (KeyType key) const {
        if (find(key) == listnode.end())
            throw out_of_range("");
        return find(key)->second;
    }

    ValueType &operator[](const KeyType key) {                           //task 11
        if (find(key) == listnode.end()){
            insert(pair<KeyType, ValueType> (key, {}));
            return find(key)->second;
        }
        return find(key)->second;
    }


    HashMap &operator=(HashMap &mp) {
        if (this == &mp) {
            return *this;
        }
        clear();
        for (auto&& i: mp) {
            insert(i);
        }
        return *this;
    }



private:
    size_t size_tab;
    Hash hasher;
    list <pair <const KeyType, ValueType> > listnode;
    vector <list <iterator> > tab;

    void rebuild() {
        tab.clear();
        tab.resize(size_tab * 2);
        for (iterator j = listnode.begin(); j != listnode.end(); ++j){
            size_t hash = hasher(j->first) % (size_tab * 2);
            tab[hash].push_back(j);
        }
        size_tab *= 2;
    }
};

#endif //UNTITLED2_HASHMAP_H
