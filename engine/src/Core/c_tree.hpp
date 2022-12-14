#pragma once
#ifndef __C_TREE_
#define __C_TREE_

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <cstddef>
#include <iterator>
#include <memory>    
#include <initializer_list>
#include "c_data_structs.hpp"

namespace qe {
    template<class _Tp, class _Alloc = Allocator<_Tp>>
    class Tree {
    private:
        typedef _Tp type;
        typedef _Alloc alloc;
        typedef Allocator<Tree> tree_alloc;

        uint64_t _size_tree = 0;
        uint64_t _size_values = 0;
        alloc _values;
        tree_alloc _tree;

    public:
        struct iterator {
            using iterator_category = std::random_access_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = type;
            using pointer = value_type*;
            using reference = value_type&;

            iterator(pointer ptr) : _ptr(ptr) {}

            reference operator*() const { return *_ptr; }
            pointer operator->() { return _ptr; }

            iterator& operator++() { _ptr++; return *this; }
            iterator& operator++(int) { iterator t = *this; ++(*this); return t; }

            friend iterator& operator+=(const iterator &r, const difference_type &n) {
                difference_type m = n;

                if(m >= 0) while (m--) ++r;
                else while(m++) --r;

                return r;
            }

            friend iterator operator+(const iterator &a, const difference_type &n) { if(a + n == n + a) { iterator temp = a; return temp += n; } }
            friend iterator operator+(const difference_type &n, const iterator &a) { if(a + n == n + a) { iterator temp = a; return temp += n; } }

            friend iterator& operator-=(const iterator &r, const difference_type &n) { return r += -n; }
            friend iterator operator-(const iterator &i, const difference_type &n) { iterator temp = i; return temp -= n; }

            friend difference_type operator-(const iterator &a, const iterator &b) { return (*a - *b); }

            reference operator[](uint64_t n) { return *(_ptr + n); }

            friend bool operator==(const iterator &a, const iterator &b) { return a._ptr == b._ptr; }
            friend bool operator!=(const iterator &a, const iterator &b) { return a._ptr != b._ptr; }

            friend bool operator>(const iterator &a, const iterator &b) { return a._ptr > b._ptr; }
            friend bool operator<(const iterator &a, const iterator &b) { return b._ptr - a._ptr > 0; }

            friend bool operator>=(const iterator &a, const iterator &b) { return !(a._ptr < b._ptr); }
            friend bool operator<=(const iterator &a, const iterator &b) { return !(a._ptr > b._ptr); }

        private:
            pointer _ptr;
        };

        struct const_iterator {
            using iterator_category = std::random_access_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = type;
            using pointer = value_type*;
            using reference = value_type&;

            const_iterator(pointer ptr) : _ptr(ptr) {}

            reference operator*() const { return *_ptr; }
            pointer operator->() { return _ptr; }

            const_iterator& operator++() { _ptr++; return *this; }
            const_iterator& operator++(int) { const_iterator t = *this; ++(*this); return t; }

            friend const_iterator& operator+=(const const_iterator &r, const difference_type &n) {
                difference_type m = n;

                if(m >= 0) while (m--) ++r;
                else while(m++) --r;

                return r;
            }

            friend const_iterator operator+(const const_iterator &a, const difference_type &n) { if(a + n == n + a) { const_iterator temp = a; return temp += n; } }
            friend const_iterator operator+(const difference_type &n, const const_iterator &a) { if(a + n == n + a) { const_iterator temp = a; return temp += n; } }

            friend const_iterator& operator-=(const const_iterator &r, const difference_type &n) { return r += -n; }
            friend const_iterator operator-(const const_iterator &i, const difference_type &n) { const_iterator temp = i; return temp -= n; }

            friend difference_type operator-(const const_iterator &a, const const_iterator &b) { return (*a - *b); }

            reference operator[](uint64_t n) { return *(_ptr + n); }

            friend bool operator==(const const_iterator &a, const const_iterator &b) { return a._ptr == b._ptr; }
            friend bool operator!=(const const_iterator &a, const const_iterator &b) { return a._ptr != b._ptr; }

            friend bool operator>(const const_iterator &a, const const_iterator &b) { return a._ptr > b._ptr; }
            friend bool operator<(const const_iterator &a, const const_iterator &b) { return b._ptr - a._ptr > 0; }

            friend bool operator>=(const const_iterator &a, const const_iterator &b) { return !(a._ptr < b._ptr); }
            friend bool operator<=(const const_iterator &a, const const_iterator &b) { return !(a._ptr > b._ptr); }

        private:
            pointer _ptr;
        };

        Tree() { 
            _tree.allocate(_size_tree);
            _values.allocate(_size_values);
        }

        Tree(uint32_t values_size, uint32_t tree_size = 1) : _size_values(values_size), _size_tree(tree_size) {
            _tree.allocate(_size_tree);
            _values.allocate(_size_values);
        }


        // Value functions
        bool empty() { return _size_values == 0; }

        void push_back() {
            _values.allocate(1);

            _size_values++;

            _values[_size_values];
        }

        iterator begin() { return iterator(&_tree[0]); } 
        iterator end() { return iterator(&_tree[_size_values]); } 

        const_iterator cbegin() { return const_iterator(&_tree[0]); } 
        const_iterator cend() { return const_iterator(&_tree[_size_values]); } 

        // Tree functions
        bool tree_empty() { return _size_tree == 0; }

        iterator tree_begin() { return iterator(&_tree[0]); } 
        iterator tree_end() { return iterator(&_tree[_size_tree]); } 

        const_iterator tree_cbegin() { return const_iterator(&_tree[0]); } 
        const_iterator tree_cend() { return const_iterator(&_tree[_size_tree]); } 

        ~Tree() {
            
        }
    };

    /*class Tree {
    private:
        std::unordered_map<std::string, Tree> m_tree;
        std::unordered_map<std::string, std::string> m_values;
        uint32_t m_current_node;

    public:
        Tree() = default;

        void push_node(std::string node_name) {
            m_tree.insert(std::make_pair(node_name, Tree()));
        }

        void pop_node(std::string node_name) {
            m_tree.erase(m_tree.find(node_name));
        }

        void pop_back_node() {
            m_tree.erase(m_tree.end());
        }

        void push_value(std::string value_name, std::string value) {
            auto iter = m_tree.begin();

            std::advance(iter, m_current_node);

            iter->second.m_values.insert(std::make_pair(value_name, value));
        } 

        void insert(std::pair<std::string, std::string> pair) {
            auto iter = m_tree.begin();

            std::advance(iter, m_current_node);

            iter->second.m_values.insert(pair);
        } 

        void pop_value(std::string value_name) {
            auto iter = m_tree.begin();

            std::advance(iter, m_current_node);

            iter->second.m_values.erase(m_values.find(value_name));
        }

        void pop_back_value() {
            auto iter = m_tree.begin();

            std::advance(iter, m_current_node);

            iter->second.m_values.erase(m_values.end());
        }

        std::string find_value(std::string value_name) {
            auto iter = m_tree.begin();

            std::advance(iter, m_current_node);

            return iter->second.m_values[value_name];
        }

        void set_current_node(uint32_t node) {
            m_current_node = node;
        }

        void operator++() { m_current_node++; }
        void operator++(int) { m_current_node++; }
        void operator--() { m_current_node--; }
        void operator--(int) { m_current_node--; }

        Tree operator=(Tree const &_tree) {
            m_tree = _tree.m_tree;
            m_values = _tree.m_values;
            m_current_node = _tree.m_current_node;

            return *this;
        }

        template<class T>
        T recast_value(std::string value_name) {
            T val;
            
            auto iter = m_tree.begin();

            std::advance(iter, m_current_node);

            std::stringstream ss(iter->second.m_values[value_name].c_str());

            ss >> val;

            return val;
        }

        void set_value(std::string value_name, std::string value) {
            auto iter = m_tree.begin();

            std::advance(iter, m_current_node);

            iter->second.m_values[value_name] = value;
        }

        std::string find_value(uint32_t index_value) {
            auto iter2 = get_current_node().m_values.begin();

            std::advance(iter2, index_value);

            return iter2->second;
        }

        std::string get_current_node_name() {
            auto iter = m_tree.begin();

            std::advance(iter, m_current_node);

            return iter->first;
        }

        Tree get_current_node() {
            auto iter = m_tree.begin();

            std::advance(iter, m_current_node);

            return iter->second;
        }

        Tree find_node(std::string node_name) {
            return m_tree[node_name];
        }

        Tree operator[](std::string node_name) {
            return m_tree[node_name];
        }

        void clear() {
            m_tree.clear();
            m_values.clear();
        }
    };*/
}

#endif