#include "util/type_utils.hpp"
#include "util/variant.hpp"

using namespace CAS::_meta;

// for_each_all checks

static_assert(for_each_all<std::is_integral, char, short, int, long int, long long>::value,
              "for_each_all not working");

static_assert(for_each_all<std::is_integral, char, short, double, long int, long long>::value ==
                  false,
              "for_each_all not working");

// for_each_any checks

static_assert(for_each_any<std::is_integral, float, double, int, long>::count == 2,
              "for_each_any not return correct count");

static_assert(for_each_any<std::is_integral, float, double, long double>::count == 0,
              "for_each_all not return correct count");

// type_is_in checks

static_assert(type_is_in<bool, short, int, long, bool>::value == true,
              "Type_is_in returns wrong value");

static_assert(type_is_in<bool, short, int, long, bool>::index == 3,
              "Type_is_in returns wrong index");

static_assert(type_is_in<bool, short, int, long, long long>::value == false,
              "Type_is_in returns wrong value");

static_assert(type_is_in<bool, short, int, long, long long>::index == 4,
              "Type_is_in returns wrong index");

struct Struct_32 {
	char data[32];
};

struct Overaligned_Struct_64 {
	alignas(32) char data[64];
};

// max_size
static_assert(max_size<bool, int, long, long long, Struct_32, Overaligned_Struct_64>::size == 64,
              "max_size not giving correct size");

// max_align
static_assert(max_align<bool, int, long, long long, Struct_32, Overaligned_Struct_64>::size == 32,
              "max_align not giving correct size");