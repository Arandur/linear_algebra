#ifndef __INCL_POLYNOMIAL_H__
#define __INCL_POLYNOMIAL_H__

#include "Vector.h"
#include <iterator>
#include <initializer_list>

template < class T >
class Polynomial
    : public Vector< T >
{
public:
    using Vector< T >::length;
    using Vector< T >::resize;
    
    Polynomial() :
    	Vector< T >::Vector()
    	{}
    Polynomial( const Vector< T >& cVector ) :
    	Vector< T >::Vector( cVector )
    	{}
    template < class InputIterator >
    	Polynomial( InputIterator first, InputIterator last ) :
    		Vector< T >::Vector( first, last )
    		{}
	Polynomial( std::initializer_list< T > iList ) :
    	Vector< T >::Vector( iList )
    	{}
    
    Polynomial< T >& operator*= ( const Polynomial< T >& );
    
    template <class U > U operator() ( const U& );
    
    template < class U >
    	friend std::ostream& operator<< ( std::ostream&, const Polynomial< U >& );
    
    const int degree() const;
    
private:
    using Vector< T >::_values;
};

template < class T >
Polynomial< T >& Polynomial< T >::operator*= ( const Polynomial< T >& rhVector )
{
    Polynomial< T > temp( _values );
    
    _values = ( temp * rhVector )._values;
}

template < class T >
template < class U >
U Polynomial< T >::operator() ( const U& param )
{
    U eval(0);
    U mult(1);
    
    for( unsigned int i = 0; i < length(); i++ )
    {
        eval += mult * _values[i];
        mult *= param;
    }
    
    return eval;
}

template < class U >
std::ostream& operator<< ( std::ostream &out, const Polynomial< U > &cPolynomial )
{
    if( ! cPolynomial )
        return out << 0;

    for( int i = cPolynomial.degree(); i >= 0; i-- )
    {
        if( cPolynomial[i] )
        {
            if( abs( cPolynomial[i] ) != 1 || !i )
            {
                if( i == cPolynomial.degree() )
                    out << cPolynomial[i];
                else
                    out << abs( cPolynomial[i] );
                out << ' ';
            }
            
            if( cPolynomial[i] == -1 && i == cPolynomial.degree() )
                out << '-';
                
            if( i )
            {
                out << 'x';
                if( i > 1 )
                    out << '^' << i;
            }
        }
        else
            continue;
            
        for( int j = i - 1; j >= 0; j-- )
            if( cPolynomial[j] )
            {
                if( cPolynomial[j] > 0 )
                    out << " + ";
                else
                    out << " - ";
                break;
            }
    }
    
    return out;
}

template < class T >
const int Polynomial< T >::degree() const
{
    return _values.size() - 1;
}

template < class T >
Polynomial< T > operator* ( const Polynomial< T >& lhPolynomial, const Polynomial< T >& rhPolynomial )
{
    Polynomial< T > product;
    product.resize( lhPolynomial.degree() + rhPolynomial.degree() + 1 );
    
    for( unsigned int i = 0; i < lhPolynomial.length(); i++ )
        for( unsigned int j = 0; j < rhPolynomial.length(); j++ )
            product[ i + j ] += lhPolynomial[i] * rhPolynomial[j];
    
    return product;
}

template < class T >
Polynomial< T > pow( const Polynomial< T >& cPolynomial, unsigned int exponent )
{
    Polynomial< T > result( std::vector< T > { T( 1 ) } );
    
    for( unsigned int i = 0; i < exponent; i++ )
        result *= cPolynomial;
        
    return result;
}

template < class T >
Polynomial< T > derivative( const Polynomial< T >& cPolynomial, unsigned int iter = 1 )
{
    Polynomial< T > result;
    if( !cPolynomial )
        return result;
        
    result.resize( cPolynomial.length() - 1 );
    
    for( unsigned int i = 0; i < result.length(); i++ )
        result[i] = cPolynomial[ i + 1 ] * ( i + 1 );
    
    if( iter == 1 )
        return result;
    else
        return derivative( result, iter - 1 );
}

#endif
