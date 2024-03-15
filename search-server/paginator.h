#pragma once

#include <vector>

template <typename Iterator>
class Paginator {
    vector<IteratorRange<Iterator>> ranges;

public:
    Paginator(Iterator begin, Iterator end, int page_size) {
        int offset = 0;
        Iterator range_begin = begin;

        for (auto it = begin; it != end; ++it) {
            if (offset == page_size) {
                ranges.push_back({range_begin, it});
                range_begin = it;
                offset = 0;
            }

            if (distance(it, end) <= page_size) {
                ranges.push_back({it, end});
                break;
            }
            ++offset;
        }
    }

    auto begin() const {
        return ranges.begin();
    }

    auto end() const {
        return ranges.end();
    }
};

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}