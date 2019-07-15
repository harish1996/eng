#ifndef _HASH_H
#define _HASH_H

#include <string>
#include <cstdlib>

/**
 * @func chartonibble
 * @brief Converts a character to a hexadecimal lower nibble ( 4 bits )
 *
 * @param in The input character to be converted
 *
 * @return the lower nibble if `in` is a valid hexadecimal digit
 * 	16 if `in` is not a valid hexadecimal digit
 */
static unsigned char chartonibble( char in )
{
	if( in >= '0' && in <= '9' )
		return in - '0';
	else if( in >= 'a' && in <= 'f' )
		return in - 'a' + 10;
	else
		return 16;
}

/**
 * @func nibbletochar
 * @brief Converts a nibble to a character
 *
 * @param in The input byte which contains the nibble.
 * @param is_lower Whether lower nibble ? ( 1 in case of lower nibble, 1 in case of higher nibble )
 *
 * @return a valid hexadecimal digit in character. -1 in case of error
 */
static char nibbletochar( unsigned char in, char is_lower )
{
	in = is_lower ? ( in & 0x0F ) : (( in & 0xF0 ) >> 4) ;
       	if( in <= 9 )
		return '0' + in;
	else if( in <= 15 )
		return 'a' + in - 10;
	else
		return -1;
}

enum hash_strtonum_status {
	SUCCESS = 0,
	ERR_STR_FORMAT,
	ERR_INV_NUMBER,
};

/**
 * @func hash_strtonum
 * @brief Converts the hash string into a 20 byte long number
 *
 * @param hash String which contains the hash
 * @param arr Pointer to an array of unsigned characters, of size 20 bytes. There are no checks
 * 	for size inside the function )
 *
 * @return SUCCESS if success, -ERR_STR_FORMAT if the input string is not 40 characters long,
 * 	-ERR_INV_NUMBER if the string contains anything other than hex digits.
 */
static int hash_strtonum( std::string hash, unsigned char *arr )
{
	if( hash.length() != 40 )
		return -ERR_STR_FORMAT;
	for( int i=0; i<20; i++ ){
		unsigned char ret;
		arr[i] = 0;
		ret = chartonibble( hash[2*i] ) ;
	      	if( ret <= 15 ){
			arr[i] |= ( ( ret & 0x0F ) << 4 );
		}
		else
			return -ERR_INV_NUMBER;
		ret = chartonibble( hash[2*i+1] );
		if( ret <= 15 ){
			arr[i] |= ( ret & 0x0F );
		}
		else
			return -ERR_INV_NUMBER;
	}
	
	return SUCCESS;
}

/**
 * @func hash_numtostr
 * @brief Converts a byte array of size 20, into a string hash
 *
 * @param hash Reference to a string variable. This will be resized and overwritten without
 * 	any checks.
 * @param arr An unsigned character array of size 20 which contains the hash value as 20 byte
 * 	number
 *
 * @return 0 on success
 */
static int hash_numtostr( std::string& hash, unsigned char *arr )
{
	hash.resize( 40 );
	for( int i=0; i<20; i++ ){
		hash[2*i] = nibbletochar( arr[i], 0 ); // Upper nibble first
		hash[2*i + 1] = nibbletochar( arr[i], 1 ); // Lower nibble
	}
	return 0;
}

#endif
