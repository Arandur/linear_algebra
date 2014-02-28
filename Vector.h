#ifndef __INCL_VECTOR_H__
#define __INCL_VECTOR_H__

#include <vector>
#include <iostream>
#include <initializer_list>
#include <type_traits>
#include <exception>

struct VectorBase {};

template < class T >
class Vector
	: public VectorBase
{
public:
	Vector()
		{}
	Vector( const Vector< T >& cVector ) :
		_values( cVector._values )
		{}
	Vector( unsigned int size ) :
		_values( size, T( 0 ) )
		{}
	template < class InputIterator >
		Vector( InputIterator first, InputIterator last ) :
			_values( first, last )
			{}
	Vector( std::initializer_list< T > iList ) :
		_values( iList.begin(), iList.end() )
		{}
	
	typename std::vector< T >::iterator begin();
	typename std::vector< T >::iterator end();
	typename std::vector< T >::reverse_iterator rbegin();
	typename std::vector< T >::reverse_iterator rend();
	typename std::vector< T >::const_iterator cbegin() const noexcept;
	typename std::vector< T >::const_iterator cend() const noexcept;
	typename std::vector< T >::const_reverse_iterator crbegin() const noexcept;
	typename std::vector< T >::const_reverse_iterator crend() const noexcept;
	
	template < class R >
		static R add( const Vector< T >&, const Vector< T >& );
	template < class R >
		static R subtract( const Vector< T >&, const Vector< T >& );
	template < class R, class U >
		static R multiply( const Vector< T >&, const U& );
	template < class R, class U >
		static R multiply( const U&, const Vector< T >& );
	template < class R, class U >
		static R divide( const Vector< T >&, const U& );
	
	Vector< T >& operator= ( const Vector< T >& );
	Vector< T >& operator+= ( const Vector< T >& );
	Vector< T >& operator-= ( const Vector< T >& );
	template < class V > Vector< T >& operator*= ( const V& );
	template < class V > Vector< T >& operator/= ( const V& );
	
	T& operator[] ( const unsigned int& );
	const T& operator[] ( const unsigned int& ) const;
	
	operator bool() const;
	
	template < class U > friend bool operator! ( const Vector< U >& );
	
	constexpr unsigned int length() const;
	virtual void resize( unsigned int );
	
protected:
	std::vector< T > _values;
};

// Iterators

template < class T >
typename std::vector< T >::iterator Vector< T >::begin()
{
	return _values.begin();
}

template < class T >
typename std::vector< T >::iterator Vector< T >::end()
{
	return _values.end();
}

template < class T >
typename std::vector< T >::reverse_iterator Vector< T >::rbegin()
{
	return _values.rbegin();
}

template < class T >
typename std::vector< T >::reverse_iterator Vector< T >::rend()
{
	return _values.rend();
}

template < class T >
typename std::vector< T >::const_iterator Vector< T >::cbegin() const noexcept
{
	return _values.cbegin();
}

template < class T >
typename std::vector< T >::const_iterator Vector< T >::cend() const noexcept
{
	return _values.cend();
}

template < class T >
typename std::vector< T >::const_reverse_iterator Vector< T >::crbegin() const noexcept
{
	return _values.crbegin();
}

template < class T >
typename std::vector< T >::const_reverse_iterator Vector< T >::crend() const noexcept
{
	return _values.crend();
}

// Arithmetic helper functions

template < class T >
template < class R >
R Vector< T >::add( const Vector< T > &lhVector, const Vector< T > &rhVector )
{
	static_assert( std::is_base_of< VectorBase, R >::value,
				   "R needs to be derived from Vector< T >" );
	
	R sumVector( lhVector );
	sumVector.resize( rhVector.length() );
	
	for( unsigned int i = 0; i < rhVector.length(); i++ )
		sumVector[i] += rhVector[i];
		
	return sumVector;
}

template < class T >
template < class R >
R Vector< T >::subtract( const Vector< T > &lhVector, const Vector< T > &rhVector )
{
	static_assert( std::is_base_of< VectorBase, R >::value,
				   "R needs to be derived from Vector< T >" );
				   
	R differenceVector( lhVector );
	differenceVector.resize( rhVector.length() );
	
	for( unsigned int i = 0; i < rhVector.length(); i++ )
		differenceVector[i] -= rhVector[i];
		
	return differenceVector;
}

template < class T >
template < class R, class U >
R Vector< T >::multiply( const Vector< T >& lhVector, const U& scalar )
{
	static_assert( std::is_base_of< VectorBase, R >::value,
				   "R needs to be derived from Vector< T >" );
				   
	R productVector( lhVector );
	
	for( unsigned int i = 0; i < productVector.length(); i++ )
		productVector[i] *= scalar;
		
	return productVector;
}

template < class T >
template < class R, class U >
R Vector< T >::multiply( const U& scalar, const Vector< T >& rhVector )
{
	static_assert( std::is_base_of< VectorBase, R >::value,
				   "R needs to be derived from Vector< T >" );
				   
	R productVector( rhVector );
	
	for( unsigned int i = 0; i < productVector.length(); i++ )
		productVector[i] = scalar * productVector[i];
		
	return productVector;
}

template < class T >
template < class R, class U >
R Vector< T >::divide( const Vector< T >& lhVector, const U& scalar )
{
	static_assert( std::is_base_of< VectorBase, R >::value,
				   "R needs to be derived from Vector< T >" );
				   
	R quotientVector( lhVector );
	
	for( unsigned int i = 0; i < quotientVector.length(); i++ )
		quotientVector[i] /= scalar;
		
	return quotientVector;
}

// Member operator overloads

template < class T >
Vector< T >& Vector< T >::operator= ( const Vector< T > &otherVector )
{
	resize( otherVector.length() );
	
	for( unsigned int i = 0; i < otherVector.length(); i++ )
		_values[i] = otherVector[i];
}

template < class T >
Vector< T >& Vector< T >::operator+= ( const Vector< T > &otherVector )
{
	resize( otherVector.length() );

	for( unsigned int i = 0; i < otherVector.length(); i++ )
		_values[i] += otherVector[i];
}

template < class T >
Vector< T >& Vector< T >::operator-= ( const Vector< T > &otherVector )
{
	resize( otherVector.length() );

	for( unsigned int i = 0; i < otherVector.length(); i++ )
		_values[i] -= otherVector[i];
}

template < class T >
template < class V >
Vector< T >& Vector< T >::operator*= ( const V &scalar )
{
	for( unsigned int i = 0; i < length(); i++ )
		_values[i] *= scalar;
}

template < class T >
template < class V >
Vector< T >& Vector< T >::operator/= ( const V &scalar )
{
	for( unsigned int i = 0; i < length(); i++ )
		_values[i] /= scalar;
}

template < class T >
T& Vector< T >::operator[] ( const unsigned int &index )
{
	return _values[ index ];
}

template < class T >
const T& Vector< T >::operator[] ( const unsigned int &index ) const
{
	return _values[ index ];
}

template < class T >
Vector< T >::operator bool() const
{
	T test();
	
	for( unsigned int i = 0; i < length(); i++ )
		if( _values[i] != test )
			return true;
			
	return false;
}

template < class U >
bool operator! ( const Vector< U > &cVector )
{
	return !bool( cVector );
}

// Free operator overloads

template < class R >
typename std::enable_if< std::is_base_of< VectorBase, R >::value, R >::type
operator+ ( const R &lhs, const R &rhs )
{
	return R::template add<R>( lhs, rhs );
}

template < class R >
typename std::enable_if< std::is_base_of< VectorBase, R >::value, R >::type
operator- ( const R &lhs, const R &rhs )
{
	return R::template subtract<R>( lhs, rhs );
}

template < class R, class U >
typename std::enable_if< ( std::is_base_of< VectorBase, R >::value && !( std::is_base_of< VectorBase, U >::value ) ), R >::type
operator* ( const R &lhs, const U &rhs )
{
	return R::template multiply<R>( lhs, rhs );
}

template < class R, class U >
typename std::enable_if< ( std::is_base_of< VectorBase, R >::value && !( std::is_base_of< VectorBase, U >::value ) ), R >::type
operator* ( const U &lhs, const R &rhs )
{
	return R::template multiply<R>( lhs, rhs );
}

template < class R, class U >
typename std::enable_if< std::is_base_of< VectorBase, R >::value, R >::type
operator/ ( const R &lhs, const U &rhs )
{
	return R::template divide<R>( lhs, rhs );
}

template < class U >
std::ostream& operator<< ( std::ostream &out, const Vector< U > &cVector )
{
	out << "< ";

	for( unsigned int i = 0; i < cVector.length(); i++ )
	{
		out << cVector[i];
		if( i < cVector.length() - 1 )
			out << ", ";
	}
	
	out << " >";
	
	return out;
}

// Vector arithmetic

template < class T > 
const T dot( const Vector< T > &firstVector, const Vector< T > &secondVector )
{
	Vector< T > temp( firstVector );
	temp.resize( secondVector );
	T out(0);
	
	for( unsigned int i = 0; i < secondVector.length(); i++ )
		temp[i] *= secondVector[i];
		
	for( unsigned int i = 0; i < temp.length(); i++ )
		out += temp[i];
		
	return out;
}

template < class T >
Vector< T > cross( const Vector< T > &lhVector, const Vector< T > &rhVector )
{
	if( lhVector.size() != 3 || rhVector.size() != 3 )
	{
		class CrossProductException
			: public std::exception
		{
			virtual const char* what() const throw()
			{
				return "Cross product is only defined for 3-dimensional vectors.";
			}
		} ex;
		
		throw ex;
	}
				   
	Vector< T > out;
	
	out.resize( 3 );
	
	out[0] = lhVector[1] * rhVector[2] - lhVector[2] * rhVector[1];
	out[1] = lhVector[2] * rhVector[0] - lhVector[0] * rhVector[2];
	out[2] = lhVector[0] * rhVector[1] - lhVector[1] * rhVector[0];
	
	return out;
}

// Utility functions

template < class T >
constexpr unsigned int Vector< T >::length() const
{
	return _values.size();
}

template < class T >
void Vector< T >::resize( unsigned int newSize )
{
	while( newSize > _values.size() )
		_values.push_back( T( 0 ) );
}

#endif
