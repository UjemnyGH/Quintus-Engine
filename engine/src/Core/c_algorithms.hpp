#pragma once
#ifndef __C_ALGORITHMS_
#define __C_ALGORITHMS_

#include <iostream>
#include <vector>

namespace qe {
    template<class T>
    void swapValues(T* a, T* b) {
        T t = *a;
        *a = *b;
        *b = t;
    }

    /**
     * @brief Quick sort algorithm
     *
     * @tparam T typename
     * @tparam Arr data to sort
     * @param _begin first index
     * @param _end last index
     * @param arr data to sort
     * @return T* 
     */
    template<class T>
    void quickSort(T *arr, size_t _begin, size_t _end) {
        if (_begin < _end) {
            T pivot = arr[_end];
            T i = (_begin - 1);

            for (int j = _begin; j <= _end; j++)
            {
                if (arr[j] < pivot) {
                    i++;
                    swap(&arr[i], &arr[j]);
                }
            }

            swap(&arr[i + 1], &arr[_end]);

            T part = i + 1;

            quickSort(arr, _begin, part - 1);
            quickSort(arr, part + 1, _end);
        }
    }

    /**
     * @brief Quick sort algorithm
     *
     * @tparam T typename
     * @tparam Arr data to sort
     * @param _begin first index
     * @param _end last index
     * @param arr data to sort
     * @return T* 
     */
    template<class T>
    void quickSort(std::vector<T>* arr, size_t _begin, size_t _end) {
        if (_begin < _end) {
            T pivot = arr[_end];
            T i = (_begin - 1);

            for (int j = _begin; j <= _end; j++)
            {
                if (arr[j] < pivot) {
                    i++;
                    swap(&arr[i], &arr[j]);
                }
            }

            swap(&arr[i + 1], &arr[_end]);

            T part = i + 1;

            quickSort(arr, _begin, part - 1);
            quickSort(arr, part + 1, _end);
        }
    }

    /**
     * @brief Quick sort algorithm
     *
     * @tparam T typename
     * @tparam Arr data to sort
     * @param _begin first index
     * @param _end last index
     * @param arr data to sort
     * @return T* 
     */
    template<class T>
    std::vector<T> quickSort(std::vector<T> arr, size_t _begin, size_t _end) {
        if (_begin < _end) {
            T pivot = arr[_end];
            T i = (_begin - 1);

            for (int j = _begin; j <= _end; j++)
            {
                if (arr[j] < pivot) {
                    i++;
                    swap(&arr[i], &arr[j]);
                }
            }

            swap(&arr[i + 1], &arr[_end]);

            T part = i + 1;

            quickSort(arr, _begin, part - 1);
            quickSort(arr, part + 1, _end);
        }
        else {
            return arr;
        }
    }

    /**
     * @brief Binary search algorithm
     *
     * @tparam T typename
     * @param _first first element
     * @param _last last element
     * @param arr data to research (sorted!!!!!!)
     * @param value value to be founded bt address
     * @return size_t
     */
    template<class T>
    size_t binSearch(size_t _first, size_t _last, std::vector<T> arr, T value) {
        if (_last >= _first) {
            int pos = _first + (_last - _first) / 2;

            if (arr[pos] == value)
                return pos;
            if (arr[pos] > value)
                return binSearch(_first, pos - 1, arr, value);

            return binSearch(pos + 1, _last, arr, value);
        }

        return -1;
    }

    template<class T>
    size_t Search(std::vector<T> arr, T value, size_t optional_return_val = -1) {
        size_t beg = arr.size() / 2;
        size_t end = arr.size() / 2;

        for(size_t i = 0; i < arr.size() / 2; i++) {
            if(arr[end] == value) {
                return end;
            }

            if(arr[beg] == value) {
                return beg;
            }
            
            beg--;
            end++;
        }

        return optional_return_val;
    }
}

#endif