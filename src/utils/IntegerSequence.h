#ifndef INTEGER_SEQUENCE_H
#define INTEGER_SEQUENCE_H

/**
 * Compile-time integer sequence.
 * Used to iterate through containers of fixed size (such as tuples or variadic template arguments) using compile-time recursion.
 * \todo Remove this file and replace usages with std::integer_sequence once it is available with all target compilers.
 */
template<int... Ints> struct IntegerSequence
{
};

template<int N, int... Rest> struct GenerateIntegerSequence : GenerateIntegerSequence<N - 1, N - 1, Rest...>
{
};

template<int... Rest> struct GenerateIntegerSequence<0, Rest...> : IntegerSequence<Rest...>
{
};

#endif
