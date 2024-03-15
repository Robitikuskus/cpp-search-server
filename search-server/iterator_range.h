#include <iostream>

template <typename Iterator>
class IteratorRange {
    Iterator begin_, end_;

public:
    IteratorRange(Iterator begin, Iterator end)
    : begin_(begin), end_(end) {};

    Iterator begin() const {
        return begin_;
    }

    Iterator end() const {
        return end_;
    }

    int size() const {
        return distance(begin_, end_);
    }
};

template <typename Iterator>
std::ostream& operator<<(std::ostream& os, IteratorRange<Iterator> range) {
    for (auto it = range.begin(); it != range.end(); ++it) {
        os << *it;
    }
    return os;
}
