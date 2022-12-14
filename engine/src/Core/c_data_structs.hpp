#pragma once
#ifndef __C_DATA_STRUCTS_
#define __C_DATA_STRUCTS_

#include <iterator>
#include <initializer_list>
#include <cstddef>
#include <memory>
#include <algorithm>
#include "c_engine_functions.hpp"

namespace qe {
    template<class T>
    struct Allocator {
        T *m_data;
        size_t m_current_size;

        Allocator() {
            m_data = (T*)malloc(sizeof(T));
        }

        /**
         * @brief Allocates `size` amount of bytes
         * 
         * @param size 
         */
        void allocate(size_t size) {
            m_current_size = size;
            m_data = realloc(m_data, m_current_size * sizeof(T));
        }

        /**
         * @brief Return allocator current max
         * 
         * @return size_t 
         */
        size_t allocator_current_max() { return m_current_size; }

        /**
         * @brief Return reference to data location, if `n` is wrong location return last element and send warn to console
         * 
         * @param n 
         * @return T& 
         */
        T& operator[](size_t n) {
            if(n > m_current_size) {
                qe::qe_warn("Allocator size out of bounds. Retutning last object instead. Object pointer" + std::to_string(&m_data[n < m_current_size ? n : m_current_size]));
            }

            return m_data[n < m_current_size ? n : m_current_size];
        }

        /**
         * @brief Return allocator itself
         * 
         * @param alloc 
         * @return T& 
         */
        T& operator=(Allocator const &alloc) {
            this->m_data = alloc.m_data;
            this->m_current_size = alloc.m_current_size;

            return *this;
        }

        ~Allocator() {
            free(m_data);
        }
    };

    template<class _Tp, class _Alloc = Allocator<_Tp>>
    class Heap {
    private:
        typedef _Tp type;
        typedef _Alloc alloc;

        alloc m_data;

    public:
        struct iterator {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = type;
            using pointer = value_type*;
            using reference = value_type&;

            iterator(pointer ptr) : __ptr(ptr) {}

            reference operator*() const { return *__ptr; }
            pointer operator->() { return __ptr; }

            iterator operator++() { __ptr++; return *this; }
            iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }

        private:
            pointer __ptr;
        };

        struct const_iterator {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = type;
            using pointer = value_type*;
            using reference = value_type&;

            const_iterator(pointer ptr) : __ptr(ptr) {}

            reference operator*() const { return *__ptr; }
            pointer operator->() { return __ptr; }

            const_iterator operator++() { __ptr++; return *this; }
            const_iterator operator++(int) { const_iterator tmp = *this; ++(*this); return tmp; }

        private:
            pointer __ptr;
        };
        
        Heap() = default;

        Heap(std::initializer_list<type> list) {            
            std::copy(begin(), end(), std::back_inserter(list));
        }

        iterator begin() { return iterator(&m_data[0]); }
        iterator end() { return iterator(&m_data[m_data.m_current_size]); }

        iterator rend() { return iterator(&m_data[0]); }
        iterator rbegin() { return iterator(&m_data[m_data.m_current_size]); }

        const_iterator cbegin() { return const_iterator(&m_data[0]); }
        const_iterator cend() { return const_iterator(&m_data[m_data.m_current_size]); }

        const_iterator crend() { return const_iterator(&m_data[0]); }
        const_iterator crbegin() { return const_iterator(&m_data[m_data.m_current_size]); }

        size_t size() { return m_data.m_current_size; }

        size_t max_size() { return (size_t)-1; }

        void resize(size_t size) { m_data.allocate(size); }

        size_t capacity() { return sizeof(m_data); }

        bool empty() { return m_data.m_current_size == 0 ? true : false; }

        type& operator[](size_t n) { return m_data[n]; }

        type at(size_t index) { return m_data[index]; }

        type front() { return m_data[0]; }

        type back() { return m_data[m_data.m_current_size]; }

        type* data() { return m_data.m_data; }

        void push_back(const type& value) {
            m_data.allocate(m_data.m_current_size + 1);

            m_data[m_data.m_current_size] = value;
        }

        void pop_back() { m_data.allocate(m_data.m_current_size - 1); }

        void insert(std::initializer_list<type> list) { std::copy(begin(), end(), std::back_inserter(list)); }

        /*void insert(const size_t position, type value) {
            begin() + position = iterator(&value);
        }*/

        void clear() { m_data.allocator(0); }

        alloc get_allocator() { return m_data; }
    };
}

#endif