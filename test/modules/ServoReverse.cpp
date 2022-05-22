
#include "ServoReverse.h"

ServoReverse::ServoReverse() : Module( MODULE_SERVO_REVERSE_TYPE, TEXT_MODULE_SERVO_REVERSE) {

    setDefaults();
}

void ServoReverse::run( channelSet_t &channels) {

    for( channel_t ch = 0; ch < CHANNELS; ch++) {
        if( IS_BIT_SET( cfg.revBits, ch)) {
            channels.analogChannel[ch] = -channels.analogChannel[ch];
        }
    }
}

void ServoReverse::setDefaults() {

    cfg.revBits = 0;
}

moduleSize_t ServoReverse::getConfigSize() {

    return (moduleSize_t)sizeof( cfg);
}

uint8_t *ServoReverse::getConfig() {

    return (uint8_t*)&cfg;
}

/* From TableEditable */
uint8_t ServoReverse::getItemCount() {

    return CHANNELS;
}

const char *ServoReverse::getItemName( uint8_t row) {

    return ChannelNames[row];
}

uint8_t ServoReverse::getValueCount() {

    return 1;
}

TableEditType_t ServoReverse::getValueType( uint8_t col) {

    return BOOLEAN_T;
}

void ServoReverse::getValue( uint8_t row, uint8_t col, Cell *cell) {

    cell->value.boolV = IS_BIT_SET( cfg.revBits, row);
    cell->type = BOOLEAN_T;
}

void ServoReverse::setValue( uint8_t row, uint8_t col, Cell *cell) {
    
    if( cell->value.boolV) {
        BIT_SET( cfg.revBits, row);
    } else {
        BIT_CLEAR( cfg.revBits, row);
    }
}
