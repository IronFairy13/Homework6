#pragma once
#include <iostream>
#include <unordered_map>
#include <tuple>
#include <cstdint>
#include <utility>
#include <type_traits>
#include <iterator>

template <typename T, T Default, typename Index = long long>
class Matrix
{
public:
    static_assert(std::is_copy_constructible<T>::value, "T must be copyable!");
    using value_type = T;
    using index_type = Index;

private:
    using Key = std::pair<index_type, index_type>;

    struct KeyHash
    {
        std::size_t operator()(const Key &k) const noexcept
        {
            std::size_t h1 = std::hash<index_type>{}(k.first);
            std::size_t h2 = std::hash<index_type>{}(k.second);
            return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
        }
    };

    std::unordered_map<Key, value_type, KeyHash> data_;
    static constexpr value_type kDefault = Default;

    value_type get(index_type x, index_type y) const
    {
        auto it = data_.find({x, y});
        return it == data_.end() ? kDefault : it->second;
    }

    void set(index_type x, index_type y, const value_type &v)
    {
        if (v == kDefault)
        {
            auto it = data_.find({x, y});
            if (it != data_.end())
                data_.erase(it);
        }
        else
        {
            data_[{x, y}] = v;
        }
    }

public:
    class CellProxy
    {
        Matrix &m_;
        index_type x_, y_;

    public:
        CellProxy(Matrix &m, index_type x, index_type y) : m_(m), x_(x), y_(y) {}
        operator value_type() const { return m_.get(x_, y_); }
        CellProxy &operator=(const value_type &v)
        {
            m_.set(x_, y_, v);
            return *this;
        }
        friend bool operator==(const CellProxy &c, const value_type &v) { return static_cast<value_type>(c) == v; }
        friend bool operator!=(const CellProxy &c, const value_type &v) { return !(c == v); }
    };

    class RowProxy
    {
        Matrix &m_;
        index_type x_;

    public:
        RowProxy(Matrix &m, index_type x) : m_(m), x_(x) {}
        CellProxy operator[](index_type y) { return CellProxy(m_, x_, y); }
    };

    class ConstCellProxy
    {
        const Matrix &m_;
        index_type x_, y_;

    public:
        ConstCellProxy(const Matrix &m, index_type x, index_type y) : m_(m), x_(x), y_(y) {}
        operator value_type() const { return m_.get(x_, y_); }
    };

    class ConstRowProxy
    {
        const Matrix &m_;
        index_type x_;

    public:
        ConstRowProxy(const Matrix &m, index_type x) : m_(m), x_(x) {}
        ConstCellProxy operator[](index_type y) const { return ConstCellProxy(m_, x_, y); }
    };

    class const_iterator
    {
        using map_citer = typename std::unordered_map<Key, value_type, KeyHash>::const_iterator;
        map_citer it_;

    public:
        using value_type_it = std::tuple<index_type, index_type, value_type>;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;
        using reference = value_type_it;
        using pointer = void;

        explicit const_iterator(map_citer it) : it_(it) {}
        value_type_it operator*() const { return {it_->first.first, it_->first.second, it_->second}; }
        const_iterator &operator++()
        {
            ++it_;
            return *this;
        }
        const_iterator operator++(int)
        {
            auto tmp = *this;
            ++(*this);
            return tmp;
        }
        bool operator==(const const_iterator &rhs) const { return it_ == rhs.it_; }
        bool operator!=(const const_iterator &rhs) const { return !(*this == rhs); }
    };

    RowProxy operator[](index_type x) { return RowProxy(*this, x); }                 
    ConstRowProxy operator[](index_type x) const { return ConstRowProxy(*this, x); } 

    std::size_t size() const noexcept { return data_.size(); }
    const_iterator begin() const { return const_iterator(data_.cbegin()); }
    const_iterator end() const { return const_iterator(data_.cend()); }
};
