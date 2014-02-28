#ifndef __INCL_MATRIX_H__
#define __INCL_MATRIX_H__

#include "Vector.h"
#include <iterator>
#include <algorithm>
#include <utility>
#include <initializer_list>
#include <type_traits>
#include <iomanip>
#include <sstream>
#include <exception>

template < class T >
class Matrix
	: Vector< T >
{
public:
	using Vector< T >::length;
    using Vector< T >::resize;
    using Vector< T >::operator bool;
    
	Matrix() :
		Vector< T >::Vector()
		{}
	Matrix( const Matrix< T >& cMatrix ) :
    	Vector< T >::Vector( cMatrix ),
		_numRows( cMatrix._numRows ),
    	_numColumns( cMatrix._numColumns )
    	{}
    Matrix( const unsigned int r, const unsigned int c ) :
    	Vector< T >::Vector( r * c ),
    	_numRows( r ),
    	_numColumns( c )
    	{}
    Matrix( std::initializer_list< std::initializer_list< T > > iList ) :
    	Vector< T >::Vector(),
    	_numRows( iList.size() ),
    	_numColumns( iList.begin()->size() )
    	{
    		std::vector< T > values;
    		for( auto list : iList )
    			for( auto val : list )
    				values.push_back( val );
    		
    		_values = values;
		}

    Matrix< T >& operator= ( const Matrix< T >& );
    
    template < class InputIterator >
    	typename std::enable_if< std::is_same< T, typename std::iterator_traits< InputIterator >::value_type >::value, void >::type
    		setValues( InputIterator, InputIterator );
    
    void setValues( std::initializer_list< T > );
    
    T* operator[] ( const unsigned int );
    const T* operator[] ( const unsigned int ) const;
    
    Vector< T > getRow( const unsigned int ) const;
    Vector< T > getColumn( const unsigned int ) const;
    
    unsigned int numRows() const;
    unsigned int numColumns() const;
    
    Matrix< T > transpose() const;
    
    Matrix< T > rref() const;
	
protected:
	void swapRows( const unsigned int, const unsigned int );

private:
	using Vector< T >::_values;
	
	unsigned int _numRows;
	unsigned int _numColumns;
};

template < class T >
Matrix< T >& Matrix< T >::operator= ( const Matrix< T >& cMatrix )
{
	_numRows = cMatrix._numRows;
	_numColumns = cMatrix._numColumns;
	_values = cMatrix._values;
	
	return *this;
}

template < class T >
template < class InputIterator >
typename std::enable_if< std::is_same< T, typename std::iterator_traits< InputIterator >::value_type >::value, void >::type Matrix< T >::setValues( InputIterator begin, InputIterator end )
{
	_values.clear();
	for( auto it = begin; it != end; it++ )
		if( _values.size() < _numRows * _numColumns )
			_values.push_back( *it );
}

template < class T >
void Matrix< T >::setValues( std::initializer_list< T > iList )
{
	setValues( iList.begin(), iList.end() );
}

template < class T >
T* Matrix< T >::operator[] ( const unsigned int r )
{
	return &( _values[ r * _numColumns ] );
}

template < class T >
const T* Matrix< T >::operator[] ( const unsigned int r ) const
{
	return &( _values[ r * _numColumns ] );
}

template < class T >
Vector< T > Matrix< T >::getRow( const unsigned int r ) const
{
	Vector< T > rowVector;
	rowVector.resize( _numColumns );
	
	for( unsigned int c = 0; c < _numColumns; c++ )
		rowVector[c] = (*this)[r][c];
	
	return rowVector;
}

template < class T >
Vector< T > Matrix< T >::getColumn( const unsigned int c ) const
{
	Vector< T > columnVector;
	columnVector.resize( _numRows );
	
	for( unsigned int r = 0; r < _numRows; r++ )
		columnVector[r] = (*this)[r][c];
	
	return columnVector;
}

template < class T >
unsigned int Matrix< T >::numRows() const
{
	return _numRows;
}

template < class T >
unsigned int Matrix< T >::numColumns() const
{
	return _numColumns;
}

template < class T >
Matrix< T > Matrix< T >::transpose() const
{
	Matrix< T > transposeMatrix( _numColumns, _numRows );
	
	for( unsigned int c = 0; c < _numColumns; c++ )
		for( unsigned int r = 0; r < _numRows; r++ )
			transposeMatrix[c][r] = (*this)[r][c];
	
	return transposeMatrix;
}

template < class T >
Matrix< T > operator* ( const Matrix< T >& lhs, const Matrix< T >& rhs )
{
	if( lhs.numColumns() != rhs.numRows() )
	{
		class MatrixMultiplicationException
			: public std::exception
		{
			virtual const char* what() const throw()
			{
				return "Cannot multiply matrices of incompatible dimensions.";
			}
		} ex;
		
		throw ex;
	}
	
	Matrix< T > productMatrix( lhs.numRows(), rhs.numColumns() );
	
	for( unsigned int r = 0; r < lhs.numRows(); r++ )
		for( unsigned int c = 0; c < rhs.numColumns(); c++ )
			productMatrix[r][c] = dot( lhs.getRow( r ), rhs.getColumn( c ) );
	
	return productMatrix;
}

template < class T >
constexpr T abs( const T a )
{
	return ( a < 0 ) ? -a : a;
}

template < class T >
constexpr T gcd( const T a, const T b )
{
	return ( a == b || b == 0 ) ? a : ( ( a < b ) ? gcd( abs( b ), abs( a ) ) : gcd( abs( a ) - abs( b ), abs( b ) ) );
}

template < typename T, typename... Ts >
constexpr T gcd( const T a, const T b, const Ts... others )
{
	return gcd( gcd( a, b ), others... );
}

template < class T >
Matrix< T > Matrix< T >::rref() const
{
	Matrix< T > rrefMatrix = (*this);
	unsigned int p = rrefMatrix._numColumns;
	T mult = 0, mult1 = 0, mult2 = 0;
	
	for( unsigned int r1 = 0; r1 < rrefMatrix._numRows; r1++ )
	{
		p = rrefMatrix._numColumns;
		for( unsigned int r2 = r1; r2 < rrefMatrix._numRows; r2++ )
		{
			for( unsigned int c = 0; c < rrefMatrix._numColumns; c++ )
			{
				if( rrefMatrix[r2][c] != T( 0 ) )
				{
					p = c;
					break;
				}
			}
			if( p != rrefMatrix._numColumns )
			{
				if( r2 > r1 )
				{
					rrefMatrix.swapRows( r1, r2 );
				}
				break;
			}
		}
		if( p == rrefMatrix._numColumns )
			break;
		
		for( unsigned int r2 = 0; r2 < rrefMatrix._numRows; r2++ )
		{
			if( r1 == r2 ) continue;
			
			if( std::is_integral< T >::value )
				mult = gcd( rrefMatrix[r1][p], rrefMatrix[r2][p] );
			else
				mult = 1;
			
			mult1 = mult * rrefMatrix[r2][p];
			mult2 = mult * rrefMatrix[r1][p];
			
			for( unsigned int c = 0; c < rrefMatrix._numColumns; c++ )
			{
				rrefMatrix[r1][c] *= mult1;
				rrefMatrix[r2][c] *= mult2;
			}
			
			for( unsigned int c = 0; c < rrefMatrix._numColumns; c++ )
			{
				rrefMatrix[r2][c] -= rrefMatrix[r1][c];
				rrefMatrix[r1][c] /= mult1;
			}
		}
	}
	
	for( unsigned int r = 0; r < rrefMatrix._numRows; r++ )
	{
		p = _numColumns;
		for( unsigned int c = 0; c < rrefMatrix._numColumns; c++ )
		{
			if( rrefMatrix[r][c] != 0 )
			{
				p = c;
				break;
			}
		}
		
		if( p == rrefMatrix._numColumns ) continue;
		
		mult = rrefMatrix[r][p];
		
		for( unsigned int c = 0; c < rrefMatrix._numColumns; c++ )
		{
			rrefMatrix[r][c] /= mult;
			if( rrefMatrix[r][c] == -0 )
				rrefMatrix[r][c] = 0;
		}
	}
	
	return rrefMatrix;
}

template < class T >
void Matrix< T >::swapRows( const unsigned int r1, const unsigned int r2 )
{
	for( unsigned int c = 0; c < _numColumns; c++ )
		std::swap( (*this)[r1][c], (*this)[r2][c] );
}

template < class T >
size_t strLength( const T& item )
{
	std::stringstream ss;
	ss << item;
	std::string s = ss.str();
	std::cout << s << ' ' << s.length() << std::endl;
	return s.length();
}

template < class T >
std::ostream& operator<< ( std::ostream& out, const Matrix< T >& cMatrix )
{
	unsigned int maxLength = 0, currLength = 0;
	
	for( unsigned int r = 0; r < cMatrix.numRows(); r++ )
		for( unsigned int c = 0; c < cMatrix.numColumns(); c++ )
		{
			//currLength = strLength( cMatrix[r][c] );
			currLength = 10;
			if( currLength > maxLength )
				maxLength = currLength;
		}
	
	for( unsigned int r = 0; r < cMatrix.numRows(); r++ )
	{
		for( unsigned int c = 0; c < cMatrix.numColumns(); c++ )
			out << std::setw( maxLength ) << cMatrix[r][c] << ' ';
		out << std::endl;
	}
	
	return out;
}

template < class T >
Matrix< T > append( const Matrix< T >& lhs, const Matrix< T >& rhs )
{
	Matrix< T > appendMatrix( lhs.numRows(), lhs.numColumns() + rhs.numColumns() );
	
	for( unsigned int r = 0; r < appendMatrix.numRows(); r++ )
	{
		for( unsigned int c = 0; c < lhs.numColumns(); c++ )
		{
			appendMatrix[r][c] = lhs[r][c];
		}
		
		for( unsigned int c = 0; c < rhs.numColumns(); c++ )
		{
			appendMatrix[r][ c + lhs.numColumns() ] = rhs[r][c];
		}
	}
	
	return appendMatrix;
}

template < class T >
void rref( Matrix< T >& mat )
{
	mat = mat.rref();
}

#endif
