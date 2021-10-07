/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <assert.h>
#include <string>

using namespace std;

// the encoded string returned from "cleos get scope"
// E.g. below input string is returned from running `cleos -u https://api.canfoundation.io get scope eosio.token --table stat`
const string input_string = "........el.o3";

uint64_t char_to_symbol( char c ) {
  if( c >= 'a' && c <= 'z' )
     return (c - 'a') + 6;
  if( c >= '1' && c <= '5' )
     return (c - '1') + 1;
  else if( c == '.')
     return 0;
  else
     cout << "Name contains invalid character" << endl;

  //unreachable
  return 0;
}


uint64_t string_to_uint64_t( std::string_view str ) {
  assert(str.size() <= 13);

  uint64_t n = 0;
  int i = (int) str.size();
  if (i >= 13) {
     // Only the first 12 characters can be full-range ([.1-5a-z]).
     i = 12;

     // The 13th character must be in the range [.1-5a-j] because it needs to be encoded
     // using only four bits (64_bits - 5_bits_per_char * 12_chars).
     n = char_to_symbol(str[12]);
     assert(n <= 0x0Full);
  }
  
  // Encode full-range characters.
  while (--i >= 0) {
     n |= char_to_symbol(str[i]) << (64 - 5 * (i + 1));
  }
  return n;
}

char* write_as_string( char* begin, char* end, bool dry_run = false ) {
 constexpr uint64_t mask = 0xFFull;

 if( dry_run || (begin + 7 < begin) || (begin + 7 > end) ) {
    char* actual_end = begin + 3;
    if( dry_run || (actual_end < begin) || (actual_end > end) ) return actual_end;
 }

 auto v = string_to_uint64_t(input_string);
 for( auto i = 0; i < 7; ++i, v >>= 8 ) {
    if( v == 0 ) return begin;

    *begin = static_cast<char>(v & mask);
    ++begin;
 }

 return begin;
}

int main()
{
    char buffer[7];
    auto end = write_as_string( buffer, buffer + sizeof(buffer) );
    if( buffer < end )
    printf( buffer, (end-buffer) );
    return 0;
}
