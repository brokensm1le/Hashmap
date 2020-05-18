#pragma once

#include <vector>
#include <list>
#include <memory>
#include <utility>

template<class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
 public:
    using KeyValuePair = std::pair<const KeyType, ValueType>;
    using iterator = typename std::list<KeyValuePair>::iterator;
    using const_iterator = typename std::list<KeyValuePair>::const_iterator;

    iterator begin();

    const_iterator begin() const;

    iterator end();

    const_iterator end() const;

    HashMap(Hash hasher = Hash()): table_size_(defaultSize_),
                                    elements_count_(0), hasher_(hasher),
                                    table_(defaultSize_) {}

    HashMap(std::initializer_list<std::pair<KeyType, ValueType>> inlist,
            Hash hasher_ = Hash());

    template<class TypeIterator>
    HashMap(TypeIterator start, TypeIterator finish, Hash hasher_ = Hash());

    size_t size() const;

    bool empty() const;

    const Hash hash_function() const;

    void insert(const KeyValuePair& key_value_pair);

    void erase(const KeyType& key);

    void clear();

    iterator find(const KeyType& key);

    const_iterator find(const KeyType& key) const;

    const ValueType& at(const KeyType& key) const;

    ValueType &operator[](const KeyType& key);

    HashMap &operator= (HashMap const &other);

 private:
    const size_t defaultSize_ = 1000;
    size_t table_size_;
    size_t elements_count_;
    Hash hasher_;
    std::list<KeyValuePair> elements_;
    std::vector<std::list<iterator>> table_;
    const double rebuildRate_ = 2;

    void rebuild();
};

template<class KeyType, class ValueType, class Hash>
auto HashMap<KeyType, ValueType, Hash>::begin() -> iterator {
    return elements_.begin();
}

template<class KeyType, class ValueType, class Hash>
auto HashMap<KeyType, ValueType, Hash>::begin() const -> const_iterator {
    return elements_.cbegin();
}

template<class KeyType, class ValueType, class Hash>
auto HashMap<KeyType, ValueType, Hash>::end() -> iterator {
    return elements_.end();
}

template<class KeyType, class ValueType, class Hash>
auto HashMap<KeyType, ValueType, Hash>::end() const -> const_iterator {
    return elements_.cend();
}

template<class KeyType, class ValueType, class Hash>
size_t HashMap<KeyType, ValueType, Hash>::size() const {
    return elements_count_;
}

template<class KeyType, class ValueType, class Hash>
bool HashMap<KeyType, ValueType, Hash>::empty() const {
    return elements_count_ == 0;
}

template<class KeyType, class ValueType, class Hash>
template<class TypeIterator>
HashMap<KeyType, ValueType, Hash>::HashMap(TypeIterator start,
        TypeIterator finish, Hash hasher_) : HashMap() {
    for (auto&& i = start; i != finish; ++i) {
        insert(*i);
    }
}

template<class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::HashMap(std::initializer_list
        <std::pair<KeyType, ValueType>> inlist, Hash hasher_) : HashMap() {
    for (auto&& key_value_pair : inlist) {
        insert(key_value_pair);
    }
}

template<class KeyType, class ValueType, class Hash>
const Hash HashMap<KeyType, ValueType, Hash>::hash_function() const {
    return hasher_;
}

template<class KeyType, class ValueType, class Hash>
void HashMap<KeyType,
            ValueType, Hash>::insert(const KeyValuePair& key_value_pair) {
    KeyType key = key_value_pair.first;
    if (find(key) != elements_.end()) {
        return;
    }
    size_t new_hash = hasher_(key) % table_size_;
    elements_.push_back(key_value_pair);
    iterator new_it = prev(elements_.end());
    table_[new_hash].push_back(new_it);
    ++elements_count_;
    if (rebuildRate_ * elements_count_ >= table_size_) {
        rebuild();
    }
}

template<class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::erase(const KeyType& key) {
    size_t hash = hasher_(key) % table_size_;
    if (find(key) != elements_.end()) {
        --elements_count_;
        auto it = table_[hash].begin();
        for (auto&& j : table_[hash]) {
            if (j->first == key) {
                elements_.erase(j);
                table_[hash].erase(it);
                break;
            }
            it++;
        }
    }
}

template<class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::clear() {
    elements_.clear();
    table_.clear();
    elements_count_ = 0;
    table_size_ = defaultSize_;
    table_.resize(defaultSize_);
}

template<class KeyType, class ValueType, class Hash>
auto HashMap<KeyType, ValueType, Hash>::find(const KeyType& key) -> iterator {
    size_t hash = hasher_(key) % table_size_;
    for (auto&& it : table_[hash]) {
        if (key == it->first) {
            return it;
        }
    }
    return elements_.end();
}

template<class KeyType, class ValueType, class Hash>
auto HashMap<KeyType, ValueType, Hash>::find(
        const KeyType& key) const -> const_iterator {
    size_t hash = hasher_(key) % table_size_;
    for (auto&& it : table_[hash]) {
        if (key == it->first) {
            return it;
        }
    }
    return elements_.end();
}

template<class KeyType, class ValueType, class Hash>
const ValueType &HashMap<KeyType, ValueType, Hash>::at(
        const KeyType& key) const {
    auto it = find(key);
    if (it == elements_.end()) {
        throw std::out_of_range("");
    }
    return it->second;
}

template<class KeyType, class ValueType, class Hash>
ValueType &HashMap<KeyType, ValueType, Hash>::operator[](const KeyType& key) {
    auto it = find(key);
    if (it == elements_.end()) {
        insert(std::pair<KeyType, ValueType> (key, {}));
        return find(key)->second;
    } else {
        return it->second;
    }
}

template<class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash> &HashMap<KeyType, ValueType, Hash>::operator=(
        HashMap const &other) {
    if (this == &other) {
        return *this;
    }
    clear();
    for (auto&& i : other) {
        insert(i);
    }
    return *this;
}

template<class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::rebuild() {
    table_.clear();
    table_.resize(table_size_ * 2);
    for (iterator it = elements_.begin(); it != elements_.end(); ++it) {
        size_t hash = hasher_(it->first) % (table_size_ * 2);
        table_[hash].push_back(it);
    }
    table_size_ *= 2;
}
