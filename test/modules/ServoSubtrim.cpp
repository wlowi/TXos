
#include "ServoSubtrim.h"

ServoSubtrim::ServoSubtrim() : Module( MODULE_SERVO_SUBTRIM_TYPE, TEXT_MODULE_SERVO_SUBTRIM) {

    setDefaults();
}

void ServoSubtrim::run( channelSet_t &channels) {

    for( channel_t ch = 0; ch < CHANNELS; ch++) {
        
        channels.analogChannel[ch] += PCT_TO_CHANNEL( cfg.trim_pct[ch]);

        if( channels.analogChannel[ch] > CHANNELVALUE_MAX) {
            channels.analogChannel[ch] = CHANNELVALUE_MAX;
        }

        if( channels.analogChannel[ch] < CHANNELVALUE_MIN) {
            channels.analogChannel[ch] = CHANNELVALUE_MIN;
        }
    }
}

void ServoSubtrim::setDefaults() {

    for( channel_t ch=0; ch < CHANNELS; ch++) {
        cfg.trim_pct[ch] = 0;
    }
}

moduleSize_t ServoSubtrim::getConfigSize() {

    return (moduleSize_t)sizeof( cfg);
}

uint8_t *ServoSubtrim::getConfig() {

    return (uint8_t*)&cfg;
}

void ServoSubtrim::setConfig( uint8_t *config, moduleSize_t size) {


}

/* From TableEditable */
uint8_t ServoSubtrim::getItemCount() {

    return CHANNELS;
}

const char *ServoSubtrim::getItemName( uint8_t row) {

    return ChannelNames[row];
}

uint8_t ServoSubtrim::getValueCount() {

    return 1;
}

TableEditType_t ServoSubtrim::getValueType( uint8_t col) {

    return INT8_T;
}

void ServoSubtrim::getValue( uint8_t row, uint8_t col, CellType *val) {

    val->int8V = cfg.trim_pct[row];
}

void ServoSubtrim::setValue( uint8_t row, uint8_t col, CellType *val) {

    cfg.trim_pct[row] = val->int8V;
}
