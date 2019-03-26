/*
 * Rectangle.cpp
 *
 */

#include "Rectangle.h"
#include "Math/Z2k.h"
#include "OT/BitMatrix.h"

#include <assert.h>

template<class U, class V>
bool Rectangle<U, V>::operator ==(const Rectangle<U, V>& other) const
{
	for (int i = 0; i < N_ROWS; i++)
		if (rows[i] != other.rows[i])
			return false;
	return true;
}

template<class U, class V>
Rectangle<U, V>& Rectangle<U, V>::operator +=(const Rectangle<U, V>& other)
{
	for (int i = 0; i < N_ROWS; i++)
		rows[i] += other.rows[i];
	return *this;
}

template<class U, class V>
Rectangle<U, V> Rectangle<U, V>::operator -(const Rectangle<U, V>& other)
{
	Rectangle<U, V> res = other;
	res.rsub_(*this);
	return res;
}

template<class U, class V>
Rectangle<U, V>& Rectangle<U, V>::rsub_(Rectangle<U, V>& other)
{
	for (int i = 0; i < N_ROWS; i++)
		rows[i] = other.rows[i] - rows[i];
	return *this;
}

template<class U, class V>
Rectangle<U, V>& Rectangle<U, V>::sub_(const void* other)
{
	for (int i = 0; i < N_ROWS; i++)
		rows[i] = rows[i] - V(other);
	return *this;
}

template<class U, class V>
void Rectangle<U, V>::mul(const BitVector& a, const V& b)
{
	assert(a.size() == N_ROWS);
	for (int i = 0; i < N_ROWS; i++)
		rows[i] = b * a.get_bit(i);
}

template<class U, class V>
void Rectangle<U, V>::randomize(PRNG& G)
{
	for (int i = 0; i < N_ROWS; i++)
		rows[i].randomize(G);
}

template<class U, class V>
void Rectangle<U, V>::conditional_add_(BitVector& conditions,
		Rectangle<U, V>& other, int offset)
{
	for (int i = 0; i < N_ROWS; i++)
		if (conditions.get_bit(N_ROWS * offset + i))
			rows[i] += other.rows[i];
}

template<class U, class V>
template<class T>
void Rectangle<U, V>::to(T& result)
{
	result = bigint(0);
	for (int i = 0; i < min(N_ROWS, result.N_BITS); i++)
	{
		result += T(rows[i]) << i;
	}
}

template<class U, class V>
void Rectangle<U, V>::pack(octetStream& o) const
{
	for (int i = 0; i < N_ROWS; i++)
		rows[i].pack(o);
}

template<class U, class V>
void Rectangle<U, V>::unpack(octetStream& o)
{
	for (int i = 0; i < N_ROWS; i++)
		rows[i].unpack(o);
}

#define X(N,L) \
		template class Rectangle<Z2<N>, Z2<L> > ; \
		template void Rectangle<Z2<N>, Z2<L> >::to(Z2<L>& result); \
		template void Rectangle<Z2<N>, Z2<L> >::to(Z2<N>& result); \
		template void Rectangle<Z2<N>, Z2<L> >::to(Z2<N + L>& result); \
		template void Rectangle<Z2<N>, Z2<L> >::to(Z2<L - N>& result); \


Y(64, 96)
Y(64, 64)
Y(32, 32)
