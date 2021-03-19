#pragma once

#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include "../Entry/Entry.h"

template <typename T>
using SortPredicate = std::function<bool(const T&, const T&)>;

template <typename T>
using FilterPredicate = std::function<bool(const T&)>;

template <typename T>
void sort(std::vector<T>& elems, SortPredicate<T> predicate) {
    std::sort(elems.begin(), elems.end(), predicate);
}

template <typename T>
void filter(std::vector<T>& elems, FilterPredicate<T> predicate) {
    std::vector<T> newVector = {};
    for (auto elem : elems) {
        if (predicate(elem)) {
            newVector.push_back(elem);
        }
    }

    elems = std::move(newVector);
}

struct EntryFilters {
    static FilterPredicate<Entry> isEqualTo(const Entry &elem) {
        return [&](const Entry& entry) {
            return elem.title == entry.title;
        };
    }

    static FilterPredicate<Entry> isTitleEqualTo(const std::string &str) {
        return [&](const Entry& entry) {
            return str == entry.title;
        };
    }

    static FilterPredicate<Entry> isTitleEqualTo(const std::string &str, bool caseInsensitive) {
        auto makeCaseInsensitive = [](const std::string& str) {
            std::string copy = "";
            for (auto it = str.begin(); it != str.end(); it++){
                char lowerCase = (char)tolower(*(it));
                copy.push_back(lowerCase);
            }
            return std::move(copy);
        };
        return [&](const Entry& entry) {
            std::string caseInsensitiveStr = std::string(str);
            std::string caseInsensitiveTitle = std::string(entry.title);
            if (caseInsensitive) {
                caseInsensitiveStr =  makeCaseInsensitive(str);
                caseInsensitiveTitle = makeCaseInsensitive(entry.title);
            }
            return caseInsensitiveStr == caseInsensitiveTitle;
        };
    }

    static FilterPredicate<Entry> titleContains(const std::string &str) {
        return [&](const Entry& entry) {
            return entry.title.find(str) != std::string::npos;
        };
    }

    static SortPredicate<Entry> ascendingTitle() {
        return [&](const Entry& a, const Entry& b) {
            return a.title.compare(b.title) < 0;
        };
    }

    static SortPredicate<Entry> descendingTitle() {
        return [&](const Entry& a, const Entry& b) {
            return a.title.compare(b.title) > 0;
        };
    }  
};
