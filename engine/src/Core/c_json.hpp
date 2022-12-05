#pragma once
#ifndef __C_JSON_
#define __C_JSON_

#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <memory>
#include <initializer_list>
#include <iterator>
#include <cstddef>
#include "c_tree.hpp"

namespace qe {
    /*template<typename _Key, typename _T>
    class JsonTree {
    private:
        typedef _Key key_t;
        typedef _T tree_t;
        typedef std::pair<const _Key, _T> value_t;
        typedef std::allocator<std::pair<const _Key, _T>> allocator_t;

        allocator_t _tree_allocator;
        uint32_t _allocator_size = 0;

    public: 
        struct Iterator {
            using iterator_category = std::random_access_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = tree_t;
            using pointer = value_type*;
            using reference = value_type&;

            Iterator(pointer ptr) : m_ptr(ptr) {}

            reference operator*() const {return *m_ptr; }
            pointer operator->() { return m_ptr; }

            Iterator& operator++() { m_ptr++; return *this; }

            Iterator operator++(int) { Iterator t = *this; ++(*this); return tmp; }

            friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; }
            friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; }

        private:
            pointer m_ptr;
        };

        struct ConstIterator {
            using iterator_category = std::random_access_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = tree_t;
            using pointer = value_type*;
            using reference = value_type&;

            ConstIterator(pointer ptr) : m_ptr(ptr) {}

            const reference operator*() const {return *m_ptr; }
            const pointer operator->() { return m_ptr; }

            ConstIterator& operator++() { m_ptr++; return *this; }

            ConstIterator operator++(int) { ConstIterator t = *this; ++(*this); return tmp; }

            friend bool operator==(const ConstIterator& a, const ConstIterator& b) { return a.m_ptr == b.m_ptr; }
            friend bool operator!=(const ConstIterator& a, const ConstIterator& b) { return a.m_ptr != b.m_ptr; }

        private:
            pointer m_ptr;
        };

        JsonTree() = default;

        JsonTree(JsonTree tree) : _allocator_size(tree._allocator_size), _tree_allocator(tree._tree_allocator) {}

        void push_node(value_t pair) {
            _allocator_size++

            _tree_allocator.allocate(_allocator_size);

            _tree_allocator[_allocator_size] = pair;
        }

        void insert(JsonTree tree) {
            _allocator_size += tree._allocator_size;

            _tree_allocator.allocate(_allocator_size);

            std::move(begin(), end(), std::back_inserter(tree));
        }

        void find(_Key key) {
            auto it = std::find(begin(), end(), key);
        }

        void erase(value_t pair) {

        }

        Iterator begin() { return Iterator(&_tree_allocator[0]); }
        Iterator end() { return Iterator(&_tree_allocator[_allocator_size]); }

        ConstIterator cbegin() { return ConstIterator(&_tree_allocator[0]); }
        ConstIterator cend() { return ConstIterator(&_tree_allocator[_allocator_size]); }

        Iterator rend() { return Iterator(&_tree_allocator[0]); }
        Iterator rbegin() { return Iterator(&_tree_allocator[_allocator_size]); }

        ConstIterator crend() { return ConstIterator(&_tree_allocator[0]); }
        ConstIterator crbegin() { return ConstIterator(&_tree_allocator[_allocator_size]); }
    };*/



    /*class Json {
    private:
        //std::unordered_multimap<std::string, std::string> m_json_map;
        Tree m_json_tree;

        std::string rfind(std::string str, char const ch) const {
            std::string::iterator iter = std::find(str.begin(), str.end(), ch);

            std::string result;

            std::copy(iter, str.end(), std::back_inserter(result));

            return result;
        }

        std::string lfind(std::string str, char const ch) const {
            std::string::iterator iter = std::find(str.begin(), str.end(), ch);

            std::string result;

            std::copy(str.begin(), iter, std::back_inserter(result));

            return result;
        }

    public:
        void setTree(Tree m_part_json_tree) {
            m_json_tree = m_part_json_tree;
        }

        const Tree getTree() const { return m_json_tree; }

        void LoadJSON(std::string path) {
            m_json_tree.clear();

            std::ifstream f(path.c_str(), std::ios::binary);

            if(f.bad() || f.fail() || !f.is_open()) {
                std::cerr << "Cannot read JSON file: " << path << std::endl;

                return;
            }

            std::string line;

            while(!f.eof()) {
                getline(f, line);

                std::string name = lfind(line, ':');
                std::string value = rfind(line, ':');

                name.erase(std::remove(name.begin(), name.end(), '\"'), name.end());
                name.erase(std::remove(name.begin(), name.end(), ' '), name.end());
                name.erase(std::remove(name.begin(), name.end(), ':'), name.end());
                name.erase(std::remove(name.begin(), name.end(), ','), name.end());
                name.erase(std::remove(name.begin(), name.end(), ';'), name.end());

                value.erase(std::remove(value.begin(), value.end(), ':'), value.end());
                value.erase(std::remove(value.begin(), value.end(), '\"'), value.end());
                value.erase(std::remove(value.begin(), value.end(), ' '), value.end());
                value.erase(std::remove(value.begin(), value.end(), ','), value.end());
                value.erase(std::remove(value.begin(), value.end(), ';'), value.end());

                if(name.find("0") == 0 ||
                    name.find("1") == 0 ||
                    name.find("2") == 0 ||
                    name.find("3") == 0 ||
                    name.find("4") == 0 ||
                    name.find("5") == 0 ||
                    name.find("6") == 0 ||
                    name.find("7") == 0 ||
                    name.find("8") == 0 ||
                    name.find("9") == 0 ||
                    name.find("-") == 0 &&
                    value.empty()) {
                    
                    value = name;
                    name = "NUM_IGNORE";
                }

                if(value == "{" || value == "[" || value == "(" && !name.empty()) {
                    m_json_tree.get_current_node().push_node(name);

                    m_json_tree.get_current_node()++;
                }

                if(name == "{" || name == "[" || name == "(") {
                    value = name;
                    name = "OPENBRC_IGNORE";

                    m_json_tree.get_current_node().push_node(name);

                    m_json_tree.get_current_node()++;
                }

                if(name == "}" || name == "]" || name == ")") {
                    value.clear();
                    name.clear();

                    m_json_tree.get_current_node()--;
                }

                if(!name.empty() || !value.empty()) {
                    m_json_tree.get_current_node().insert(std::make_pair(name, value));
                }
            }
        }

        std::string findValue(std::string name, uint32_t node) {
            m_json_tree.set_current_node(node);

            return m_json_tree.find_value(name);
        }

        std::string findValueInTree(std::string name) {

        }

        void __debug_show_tree__() {
            for(int i = 0 ;)
        }

        /*Json findGroup(std::string name) {
            Json group;

            std::string value = findValue(name);
            uint32_t position = findPosition(name);

            uint32_t brackets_found = 0;

            Tree json_tree_copy = m_json_tree;
            Tree result_tree;

            std::string open_bracket = value;

            if(value == "{" || value == "[" || value == "(") {
                for(uint32_t i = position; i < std::distance(m_json_tree.begin(), m_json_tree.end()); i++) {
                    auto iter = json_tree_copy.begin();

                    std::advance(iter, i);
                    
                    std::string n = iter->first;
                    std::string v = iter->second;

                    result_tree.insert(std::make_pair(n, v));

                    if(v == "{" || v == "[" || v == "(") {
                        brackets_found++;
                    }

                    if(v == "}" || v == "]" || v == ")") {
                        brackets_found--;
                    }

                    if(v == "]" && open_bracket == "[" && brackets_found == 0) {
                        break;
                    }

                    if(v == "}" && open_bracket == "{" && brackets_found == 0) {
                        break;
                    }

                    if(v == ")" && open_bracket == "(" && brackets_found == 0) {
                        break;
                    }

                }
            }
            else {
                result_map.insert(std::make_pair(name, value));
            }

            group.setMap(result_map);

            return group;
        }*/

        /*template<typename T>
        T convertTo(std::string name, uint32_t node) {
            T type;

            std::stringstream ss(findValue(name, node).c_str());
            ss >> type;

            return type;
        }

        Json operator=(Json const *json) {
            m_json_tree = json->getTree();

            return *this;
        }
    };*/
}

#endif