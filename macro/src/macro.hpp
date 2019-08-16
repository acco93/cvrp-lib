//
// Created by acco on 7/2/18.
//

#ifndef VRP_MACRO_HPP
#define VRP_MACRO_HPP

#define likely(condition) __builtin_expect(static_cast<bool>(condition), 1)
#define unlikely(condition) __builtin_expect(static_cast<bool>(condition), 0)

#endif //VRP_MACRO_HPP
