/*
 * Serial.cpp
 * 
 */

#include "Serial.h"

/********* ATmega32U4 ***********************************/
#if defined __AVR_ATmega32U4__

# define UART_CTL_STAT_REGA UCSR1A
# define UART_CTL_STAT_REGB UCSR1B
# define UART_CTL_STAT_REGC UCSR1C

# define UART_DATA_REG      UDR1

# define UART_DRE_INT       _BV(UDRIE1)
# define UART_RXC_INT       _BV(RXCIE1)
# define UART_TXC_INT       _BV(TXCIE1)

# define UART_RX_EN         _BV(RXEN1)
# define UART_TX_EN         _BV(TXEN1)

# define UART_TX_8BIT       (_BV(UCSZ11) | _BV(UCSZ10)) 
# define UART_TX_2STOP      _BV(USBS1)

# define UART_BRL_REG        UBRR1L
# define UART_BRH_REG        UBRR1H

# define UART_SEND_INTVEC   USART1_UDRE_vect
# define UART_RCV_INTVEC    USART1_RX_vect

/********* ATmega328P ***********************************/
#elif defined __AVR_ATmega328P__

# define UART_CTL_STAT_REGA UCSR0A
# define UART_CTL_STAT_REGB UCSR0B
# define UART_CTL_STAT_REGC UCSR0C

# define UART_DATA_REG      UDR0

# define UART_DRE_INT       _BV(UDRIE0)
# define UART_RXC_INT       _BV(RXCIE0)
# define UART_TXC_INT       _BV(TXCIE0)

# define UART_RX_EN         _BV(RXEN0)
# define UART_TX_EN         _BV(TXEN0)

# define UART_TX_8BIT       (_BV(UCSZ01) | _BV(UCSZ00)) 
# define UART_TX_2STOP      _BV(USBS0)

# define UART_BRL_REG        UBRR0L
# define UART_BRH_REG        UBRR0H

# define UART_SEND_INTVEC   USART_UDRE_vect
# define UART_RCV_INTVEC    USART_RX_vect

/********* AT90S4433 ************************************/
#elif defined __AVR_AT90S4433__

# define UART_CTL_STAT_REGA UCSRA
# define UART_CTL_STAT_REGB UCSRB
# undef UART_CTL_STAT_REGC

# define UART_DATA_REG      UDR

# define UART_DRE_INT       _BV(UDRIE)
# define UART_RXC_INT       _BV(RXCIE)
# define UART_TXC_INT       _BV(TXCIE)

# define UART_RX_EN         _BV(RXEN)
# define UART_TX_EN         _BV(TXEN)

# undef UART_TX_8BIT
# undef UART_TX_2STOP

# define UART_BRL_REG        UBRR
# define UART_BRH_REG        UBRRH

# define UART_SEND_INTVEC   UART_UDRE_vect
# define UART_RCV_INTVEC    UART_RX_vect

/********* ATtiny24 ************************************/
#elif defined __AVR_ATtiny24__

# error "ATtiny24 is not supported in Serial.cpp"

/********* unsupported *********************************/
#else
# error "This MCU type is not supported in Serial.cpp"
#endif

Buffer serialSndBuffer(40);
Buffer serialRcvBuffer(20);

ISR( UART_SEND_INTVEC)
{
    byte ch;
  
    if( !serialSndBuffer.isEmpty()) {
        serialSndBuffer.get( &ch);
        UART_DATA_REG = ch;
    }
  
    if( serialSndBuffer.isEmpty()) {
        UART_CTL_STAT_REGB &= ~UART_DRE_INT;
    }
}

ISR( UART_RCV_INTVEC)
{
    byte ch = UART_DATA_REG;
   
    if( !serialRcvBuffer.isFull()) {
        serialRcvBuffer.put( ch);
    }
}

/*
 * Initializes everything and enables RX and TX interrupts.
 * After the function completes interrupts are globally enabled.
 *
 * Parameter
 *  uint rbb - This goes to the baud rate generator register UBR.
 *             The baud rate will be     bd = Fclock / (16 * (rbb+1))
 *             Or the other way round   rbb = (Fclock / (16 * bd)) -1
 *
 *             Popular values:   Fclock   Baud   rbb
 *                                1Mhz    9600     6
 *                                       19200     2
 *                                8Mhz    9600    51    
 *                                       19200    25
 *                                       57600     8
 *                                      115200     3
 *                               16Mhz    9600   103
 *                                       19200    51
 *                                       57600    16
 *                                      115200     8
 *
 */
Serial::Serial( unsigned int bd)
{
    unsigned int rbb;

    cli();
    
    switch( bd) {
    case 9600: rbb = 103;
        break;
    case 19200: rbb = 51;
        break;
    case 57600: rbb = 16;
        break;
    default:
        rbb = 51;
    }
    
    UART_BRL_REG = (byte)(rbb & 0xff);
    UART_BRH_REG = (byte)((rbb >> 8) & 0xff);

#ifdef UART_CTL_STAT_REGC
    UART_CTL_STAT_REGC = UART_TX_8BIT | UART_TX_2STOP;
#endif

    UART_CTL_STAT_REGB = UART_RXC_INT | UART_RX_EN | UART_TX_EN;

    sei();
}

void Serial::printi( int v)
{
    unsigned int uv = (unsigned int)v;

    if( v < 0)
    {
        printc( '-');
        
        if( v == -32768)
        {
            uv = 32768;
        }
        else
        {
            uv = (unsigned int)-v;
        }
    }

    printu( uv);
}

void Serial::printu( unsigned int v)
{
    char ch[5];
    byte i = 0;
  
    do {
        ch[i++] = '0' + (v % 10);
        v /= 10;
    } while( v );
  
    while( i > 0)
    {
        printc( ch[--i]);
    }
}

void Serial::printc( char ch)
{
    while( true) {
        cli();
  
        if( !serialSndBuffer.isFull()) {
            serialSndBuffer.put( ch);

            if( !(UART_CTL_STAT_REGB & UART_DRE_INT))
            {
                UART_CTL_STAT_REGB |= UART_DRE_INT;
            }

            break;
        }

        sei();
    }
}

void Serial::prints( const char *ch)
{
    while( (*ch) != '\0') 
    {
        printc( *ch);
        ch++;
    }
}

boolean Serial::available()
{
    return !serialRcvBuffer.isEmpty();   
}

/*
 * Wait for a character to arrive.
 * The CPU goes to sleep while waiting.
 */
int Serial::read()
{
    byte ch;

    while( serialRcvBuffer.isEmpty()) {
        Dispatcher::sleep();
    }
   
    cli();
    serialRcvBuffer.get( &ch);
    sei();

    return ch;
}

/*
 * Get a character.
 * If no character is in the buffer -1 is returned.
 */
int Serial::getc()
{
    byte ch;

    if( !serialRcvBuffer.isEmpty()) {
        cli();
        serialRcvBuffer.get( &ch);
        sei();
    }
    else
    {
        ch = -1;
    }

    return ch;
}


/*
 * Writes a byte in hex format. ( 00 - ff )
 * Always writes two characters.
 *
 * Return
 *  WSYSRC_OK always 
 *
wsysrc wsys_serial_puth8( ubyte v)
{
  wsysrc rc = WSYSRC_OK;
  ubyte d;

  d = ((v >> 4) & 0x0f);
  d += ( d > 9) ? ('a'-10) : '0';
  rc = wsys_serial_putc( d);

  if( rc == WSYSRC_OK) 
  {
    d = (v & 0x0f);
    d += ( d > 9) ? ('a'-10) : '0';  
    rc = wsys_serial_putc( d);
  }

  return rc;
}
*/

/*
 * Writes an integer in hex format. ( 0000 - ffff )
 * Always writes four characters.
 *
 * Return
 *  WSYSRC_OK always 
 *
wsysrc wsys_serial_puth16( uint v)
{
  wsysrc rc = WSYSRC_OK;

  rc = wsys_serial_puth8( (ubyte)((v >> 8) & 0xff));
  if( rc == WSYSRC_OK)
  {
    rc = wsys_serial_puth8( (ubyte)(v & 0xff));
  }

  return rc;
}

*/
