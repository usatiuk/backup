//
// Created by Stepan Usatiuk on 13.04.2023.
//

#ifndef SEMBACKUP_HELPFULASSERT_H
#define SEMBACKUP_HELPFULASSERT_H

#include <functional>
#include <iostream>
#include <ostream>
#include <sstream>
#include <utility>

#include "../../src/Exception.h"

template<typename T, typename comp = std::equal_to<>>
class HelpfulAssert {
public:
    bool operator()(const T &lhs, const T &rhs) {
        if (!comp()(lhs, rhs)) {
            std::stringstream out;
            if constexpr (has_print_op<T>::value) {
                out << "Expected lhs to compare to " << rhs << std::endl;
                out << "But lhs is " << lhs << std::endl;

            } else {
                out << "Error comparing!" << std::endl;
            }
            throw Exception(out.str());
        }
        return true;
    }

private:
    template<typename, typename = void>
    struct has_print_op : std::false_type {};

    template<typename E>
    struct has_print_op<E, std::void_t<decltype(std::declval<std::ostream>() << std::declval<E>())>> : std::true_type {};
};


#endif//SEMBACKUP_HELPFULASSERT_H
