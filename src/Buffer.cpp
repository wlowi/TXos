/*

  Buffer.cpp

  A fast ring buffer holding up to 254 chars.

   USAGE:

     main()
     {
       Buffer b = new Buffer( 16);

       ubyte ch = 'a';

       if( !b.isFull()) {
         b.put( ch );
       }

       if( !b.isEmpty()) {
         b.get( &ch );
       } 
     } 


  History     Who       What
  ----------  --------  ------------------------------------------------  
  2019/11/26  wolfi     Fix malloc error
  2012/10/28  Wolfi     Initial checkin

 */

#include "Buffer.h"

Buffer::Buffer( byte sz)
{	
  buffer = (byte*)malloc( sz);
	
  bufferSize = sz;
  clear();
}

boolean Buffer::isFull()
{
    return (out == (in + 1)) || (out == 0 && in == (bufferSize - 1));
}

boolean Buffer::isEmpty()
{
    return (out == in);
}

void Buffer::clear()
{
    in = out = 0;
}

byte Buffer::size()
{
    return bufferSize;
}

/* Returns false if the buffer is full.
 */
boolean Buffer::put( byte ch)
{
    if( isFull()) {
        return false;
    }

    buffer[ in++ ] = ch;
    in %= bufferSize;
    return true;
}

/* Returns false if the buffer is empty.
 */
boolean Buffer::get( byte *ch)
{
    if( isEmpty()) {
        return false;
    }

    *ch = buffer[ out++ ];
    out %= bufferSize;
    return true;
}
