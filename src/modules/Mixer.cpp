/*
  TXos. A remote control transmitter OS.

  MIT License

  Copyright (c) 2023 wlowi

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "Mixer.h"

extern const char* const LogicalChannelNames[LOGICAL_CHANNELS];

/* The import/export dictionary. 
 * See ImportExport.h
 */
DICTROWA( r1, COMM_DATATYPE_UINTARR, COMM_FIELD_MODEL_SWITCH_MIX, mixer_t, mixSw, MIXER)
DICTROWA( r2, COMM_DATATYPE_UINTARR, COMM_FIELD_CHANNEL_SOURCE, mixer_t, source, MIXER)
DICTROWA( r3, COMM_DATATYPE_UINTARR, COMM_FIELD_CHANNEL_TARGET, mixer_t, target, MIXER)
DICTROWA( r4, COMM_DATATYPE_INTARR, COMM_FIELD_MODEL_PERCENT_MIX, mixer_t, mixPct, MIXER)
DICTROWA( r5, COMM_DATATYPE_INTARR, COMM_FIELD_MODEL_OFFSET_MIX, mixer_t, mixOffset, MIXER)
DICT( Mixer, COMM_SUBPACKET_MIXER, &r1, &r2, &r3, &r4, &r5)

Mixer::Mixer() : Module( MODULE_MIXER_TYPE, TEXT_MODULE_MIXER, COMM_SUBPACKET_MIXER) {

    setDefaults();
}

channelValue_t Mixer::mixValue( channelValue_t v, uint8_t mix) {

    return (channelValue_t)( ((long)(v) - (long)PCT_TO_CHANNEL( CFG->mixOffset[mix])) * (long)CFG->mixPct[mix] / 100L);
}

void Mixer::limitChannels( Controls &controls) {

    for( channel_t ch = 0; ch < LOGICAL_CHANNELS; ch++) {
        channelValue_t v = controls.logicalGet( ch);
        if( v > CHANNELVALUE_MAX_LIMIT) {
            controls.logicalSet( ch, CHANNELVALUE_MAX_LIMIT);
            continue;
        }
        if( v < CHANNELVALUE_MIN_LIMIT ) {
            controls.logicalSet( ch, CHANNELVALUE_MIN_LIMIT);
        }
    }
}

/* From Module */

COMM_RC_t Mixer::exportConfig( ImportExport *exporter, uint8_t *config) const {

    return exporter->runExport( DICT_ptr(Mixer), DICTROW_ptr(Mixer), config, sizeof(mixer_t));
}

COMM_RC_t Mixer::importConfig( ImportExport *importer, uint8_t *config) const {

    return importer->runImport( DICT_ptr(Mixer), DICTROW_ptr(Mixer), config, sizeof(mixer_t));
}

void Mixer::run( Controls &controls) {

    channelValue_t sourceValue;
    channelValue_t mixedValue;
    channel_t sourceChannel;
    channel_t targetChannel;

    for( uint8_t mix = 0; mix < MIXER; mix++) {
        sourceChannel = CFG->source[mix];
        targetChannel = CFG->target[mix];

        if( controls.evalSwitches( CFG->mixSw[mix])) {
            sourceValue = controls.logicalGet( sourceChannel );
            mixedValue = mixValue( sourceValue, mix);

            controls.logicalSet( targetChannel, controls.logicalGet( targetChannel) + mixedValue);

            /* Some channels have a second auxiliary channel */
            switch( targetChannel) {
            case CHANNEL_AILERON:
                controls.logicalSet( CHANNEL_AILERON2, controls.logicalGet( CHANNEL_AILERON2) - mixedValue);
                break;

            case CHANNEL_FLAP:
                controls.logicalSet( CHANNEL_FLAP2, controls.logicalGet( CHANNEL_FLAP2) + mixedValue);
                break;

            case CHANNEL_SPOILER:
                controls.logicalSet( CHANNEL_SPOILER2, controls.logicalGet( CHANNEL_SPOILER2) + mixedValue);
                break;

            default:
                break;
            }
        }
    }

    limitChannels( controls);
}

void Mixer::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        for( uint8_t mix = 0; mix < MIXER; mix++) {
            INIT_SWITCH( CFG->mixSw[mix]);
            CFG->source[mix] = CHANNEL_THROTTLE;
            CFG->target[mix] = CHANNEL_THROTTLE;
            CFG->mixPct[mix] = 0;
            CFG->mixOffset[mix] = 0;
        }
    )

    strcpy( mixerName, "Mix*");
}

/* From TableEditable */

uint8_t Mixer::getRowCount() {

    return 3 * MIXER;
}

const char *Mixer::getRowName( uint8_t row) {

    if ((row % 3) == 0) {
        mixerName[3] = '1' + row/3;
        return mixerName;
    } else {
        return TEXT_MSG_NONE;
    }
}

uint8_t Mixer::getColCount( uint8_t row) {

    if( (row % 3) == 0) {
        return 1;
    } else {
        return 3;
    }
}

void Mixer::getValue( uint8_t row, uint8_t col, Cell *cell) {

    uint8_t r = row % 3;
    uint8_t mix = row / 3;

    if( r == 0) {
        cell->setSwitchState( 8, CFG->mixSw[(mix)]);
    } else if( r == 1) {
        if( col == 0) {
            cell->setList( 3, LogicalChannelNames, MIX_CHANNELS, CFG->source[mix]);
        } else if (col == 1) {
            cell->setLabel( 7, TEXT_MIX_ARROW, 2);
        } else {
            cell->setList( 10, LogicalChannelNames, MIX_CHANNELS, CFG->target[mix]);
        }
    } else {
        if( col == 0) {
            cell->setInt8( 1, CFG->mixPct[(mix)], 4, PERCENT_MIN, PERCENT_MAX);
        } else if (col == 1) {
            cell->setLabel( 6, TEXT_MIX_OFFSET, 3);
        } else {
            cell->setInt8( 9, CFG->mixOffset[(mix)], 4, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
        }
    }
}

void Mixer::setValue( uint8_t row, uint8_t col, Cell *cell) {

    uint8_t r = row % 3;
    uint8_t mix = row / 3;

    if( r == 0) {
        CFG->mixSw[(mix)] = cell->getSwitchState();
    } else if( r == 1) {
        if( col == 0) {
            CFG->source[mix] = cell->getList();
        } else if (col == 2) {
            CFG->target[mix] = cell->getList();
        }
    } else {
        if( col == 0) {
            CFG->mixPct[(mix)] = cell->getInt8();
        } else if (col == 2) {
            CFG->mixOffset[(mix)] = cell->getInt8();
        }
    }
}
