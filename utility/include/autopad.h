//
// Created by user on 10/5/2022.
//

#ifndef WHATTHEHACK_UTILITY_INCLUDE_AUTOPAD_H_
#define WHATTHEHACK_UTILITY_INCLUDE_AUTOPAD_H_

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

#endif //WHATTHEHACK_UTILITY_INCLUDE_AUTOPAD_H_
