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

#include "Model.h"

extern const char* const WingMixNames[TEXT_WINGMIX_count];
extern const char* const MixNames[TEXT_MIX_count];

/* The import/export dictionary. 
 * See ImportExport.h
 */
DICTROW( r1, COMM_DATATYPE_STRING, COMM_FIELD_MODEL_NAME, model_t, modelName)
DICTROW( r2, COMM_DATATYPE_UINT8, COMM_FIELD_MODEL_MIX, model_t, wingMix)
DICTROW( r3, COMM_DATATYPE_UINT8, COMM_FIELD_MODEL_SWITCH_QDIFF, model_t, qrDiffSw)
DICTROW( r4, COMM_DATATYPE_INT8, COMM_FIELD_MODEL_QDIFF_PCT, model_t, qrDiffPct)
DICTROWA( r5, COMM_DATATYPE_UINTARR, COMM_FIELD_MODEL_SWITCH_MIX, model_t, mixSw, TEXT_MIX_count)
DICTROWA( r6, COMM_DATATYPE_INTARR, COMM_FIELD_MODEL_PERCENT_MIX, model_t, mixPct, TEXT_MIX_count)
DICTROWA( r7, COMM_DATATYPE_INTARR, COMM_FIELD_MODEL_OFFSET_MIX, model_t, mixOffset, TEXT_MIX_count)
DICT( Model, COMM_SUBPACKET_MODEL, &r1, &r2, &r3, &r4, &r5, &r6, &r7)

Model::Model() : Module( MODULE_MODEL_TYPE, TEXT_MODULE_MODEL, COMM_SUBPACKET_MODEL) {

    setDefaults();
}

COMM_RC_t Model::exportConfig( ImportExport *exporter, uint8_t *config) const {

    return exporter->runExport( DICT_ptr(Model), DICTROW_ptr(Model), config, sizeof(model_t));
}

COMM_RC_t Model::importConfig( ImportExport *importer, uint8_t *config) const {

    return importer->runImport( DICT_ptr(Model), DICTROW_ptr(Model), config, sizeof(model_t));
}

channelValue_t Model::mixValue( channelValue_t v, uint8_t mix) {

    return (channelValue_t)( ((long)(v) - (long)PCT_TO_CHANNEL( CFG->mixOffset[mix])) * (long)CFG->mixPct[mix] / 100L);
}

void Model::limitChannels( Controls &controls) {

    for( channel_t ch = 0; ch < LOGICAL_CHANNELS; ch++) {
        channelValue_t v = controls.logicalGet( ch);
        if( v > CHANNELVALUE_MAX_LIMIT) {
            controls.logicalSet( ch, CHANNELVALUE_MAX_LIMIT);
        } else if( v < CHANNELVALUE_MIN_LIMIT ) {
            controls.logicalSet( ch, CHANNELVALUE_MIN_LIMIT);
        }
    }
}

/* From Module */

void Model::run( Controls &controls) {

    channelValue_t ail;
    channelValue_t elv;
    channelValue_t rud;
    channelValue_t flp;
    channelValue_t spl;

    channelValue_t mixedValue;

    channelValue_t trim;
    channelValue_t reduction;

   /* get original unmixed values */
    ail = controls.logicalGet( CHANNEL_AILERON);
    elv = controls.logicalGet( CHANNEL_ELEVATOR);
    rud = controls.logicalGet( CHANNEL_RUDDER);
    flp = controls.logicalGet( CHANNEL_FLAP);
    spl = controls.logicalGet( CHANNEL_SPOILER);

    controls.logicalSet( CHANNEL_FLAP2, controls.logicalGet( CHANNEL_FLAP));
    controls.logicalSet( CHANNEL_SPOILER2, controls.logicalGet( CHANNEL_SPOILER));

    switch( CFG->wingMix) {

    case WINGMIX_DELTA:
        if( CFG->qrDiffPct != 0 && controls.evalSwitches( CFG->qrDiffSw)) {
            trim = controls.trimGet( CHANNEL_AILERON);
            long d = ail - trim;
            reduction = (channelValue_t)(d * (100L - abs(CFG->qrDiffPct)) / 100L);

            if( (CFG->qrDiffPct > 0) == (d > 0) ) {
                controls.logicalSet( CHANNEL_AILERON, elv + ail);
                controls.logicalSet( CHANNEL_ELEVATOR, elv - trim - reduction);
            } else {
                controls.logicalSet( CHANNEL_AILERON, elv + trim + reduction);
                controls.logicalSet( CHANNEL_ELEVATOR, elv - ail);
            } 

        } else {
            controls.logicalSet( CHANNEL_AILERON, elv + ail);
            controls.logicalSet( CHANNEL_ELEVATOR, elv - ail);
        }
        break;

    case WINGMIX_VTAIL:
        controls.logicalSet( CHANNEL_RUDDER, elv + rud);
        controls.logicalSet( CHANNEL_ELEVATOR, elv - rud);

        [[fallthrough]];

    case WINGMIX_NORMAL:
        if( CFG->qrDiffPct != 0 && controls.evalSwitches( CFG->qrDiffSw)) {
            trim = controls.trimGet( CHANNEL_AILERON);
            long d = ail - trim;
            reduction = (channelValue_t)(d * (100L - abs(CFG->qrDiffPct)) / 100L);

            if( (CFG->qrDiffPct > 0) == (d > 0) ) {
                controls.logicalSet( CHANNEL_AILERON, ail);
                controls.logicalSet( CHANNEL_AILERON2, -trim - reduction);
            } else {
                controls.logicalSet( CHANNEL_AILERON, trim + reduction);
                controls.logicalSet( CHANNEL_AILERON2, -ail);
            } 

        } else {
            controls.logicalSet( CHANNEL_AILERON2, -ail);
        }
        break;

    default:
        // ignore
        break;
    }

    limitChannels( controls);

    for( uint8_t mix = 0; mix < TEXT_MIX_count; mix++) {

        if( controls.evalSwitches( CFG->mixSw[mix])) {

            switch( mix) {
            case MIX_AIL_RUD:
                mixedValue = mixValue( ail, mix);
                controls.logicalSet( CHANNEL_RUDDER, controls.logicalGet( CHANNEL_RUDDER) + mixedValue);
                if( CFG->wingMix == WINGMIX_VTAIL) {
                    controls.logicalSet( CHANNEL_ELEVATOR, controls.logicalGet(  CHANNEL_ELEVATOR) - mixedValue);
                }
                break;

            case MIX_AIL_FLP:
                mixedValue = mixValue( ail, mix);
                if( CFG->qrDiffPct != 0 && controls.evalSwitches( CFG->qrDiffSw)) {
                    trim = controls.trimGet( CHANNEL_AILERON);
                    long d = mixedValue - trim;
                    reduction = (channelValue_t)(d * (100L - abs(CFG->qrDiffPct)) / 100L);

                    if( (CFG->qrDiffPct > 0) == (d > 0) ) {
                        controls.logicalSet( CHANNEL_FLAP, controls.logicalGet( CHANNEL_FLAP) + mixedValue);
                        controls.logicalSet( CHANNEL_FLAP2, controls.logicalGet( CHANNEL_FLAP2) - trim - reduction);
                    } else {
                        controls.logicalSet( CHANNEL_FLAP, controls.logicalGet( CHANNEL_FLAP) + trim + reduction);
                        controls.logicalSet( CHANNEL_FLAP2, controls.logicalGet( CHANNEL_FLAP2) -mixedValue);
                    }
                } else {
                    controls.logicalSet( CHANNEL_FLAP, controls.logicalGet( CHANNEL_FLAP) + mixedValue);
                    controls.logicalSet( CHANNEL_FLAP2, controls.logicalGet( CHANNEL_FLAP2) - mixedValue);
                }
                break;

            case MIX_SPL_AIL:
                mixedValue = mixValue( spl, mix);
                controls.logicalSet( CHANNEL_AILERON, controls.logicalGet( CHANNEL_AILERON) + mixedValue);
                controls.logicalSet( CHANNEL_AILERON2, controls.logicalGet( CHANNEL_AILERON2) + mixedValue);
                if( CFG->wingMix == WINGMIX_DELTA) {
                    controls.logicalSet( CHANNEL_ELEVATOR, controls.logicalGet(  CHANNEL_ELEVATOR) + mixedValue);
                }
                break;

            case MIX_SPL_FLP:
                mixedValue = mixValue( spl, mix);
                controls.logicalSet( CHANNEL_FLAP, controls.logicalGet( CHANNEL_FLAP) + mixedValue);
                controls.logicalSet( CHANNEL_FLAP2, controls.logicalGet( CHANNEL_FLAP2) + mixedValue);
                break;

            case MIX_SPL_ELV:
                mixedValue = mixValue( spl, mix);
                controls.logicalSet( CHANNEL_ELEVATOR, controls.logicalGet( CHANNEL_ELEVATOR) + mixedValue);
                if( CFG->wingMix == WINGMIX_VTAIL) {
                    controls.logicalSet( CHANNEL_RUDDER, controls.logicalGet(  CHANNEL_RUDDER) + mixedValue);
                } else if( CFG->wingMix == WINGMIX_DELTA) {
                    controls.logicalSet( CHANNEL_AILERON, controls.logicalGet(  CHANNEL_AILERON) + mixedValue);
                }
                break;

            case MIX_FLP_AIL:
                mixedValue = mixValue( flp, mix);
                controls.logicalSet( CHANNEL_AILERON, controls.logicalGet( CHANNEL_AILERON) + mixedValue);
                controls.logicalSet( CHANNEL_AILERON2, controls.logicalGet( CHANNEL_AILERON2) + mixedValue);
                if( CFG->wingMix == WINGMIX_DELTA) {
                    controls.logicalSet( CHANNEL_ELEVATOR, controls.logicalGet(  CHANNEL_ELEVATOR) + mixedValue);
                }
                break;

            case MIX_FLP_ELV:
                mixedValue = mixValue( flp, mix);
                controls.logicalSet( CHANNEL_ELEVATOR, controls.logicalGet( CHANNEL_ELEVATOR) + mixedValue);
                if( CFG->wingMix == WINGMIX_VTAIL) {
                    controls.logicalSet( CHANNEL_RUDDER, controls.logicalGet(  CHANNEL_RUDDER) + mixedValue);
                } else if( CFG->wingMix == WINGMIX_DELTA) {
                    controls.logicalSet( CHANNEL_AILERON, controls.logicalGet(  CHANNEL_AILERON) + mixedValue);
                }
                break;

            case MIX_ELV_AIL:
                mixedValue = mixValue( elv, mix);
                controls.logicalSet( CHANNEL_AILERON, controls.logicalGet( CHANNEL_AILERON) + mixedValue);
                controls.logicalSet( CHANNEL_AILERON2, controls.logicalGet( CHANNEL_AILERON2) + mixedValue);
                if( CFG->wingMix == WINGMIX_DELTA) {
                    controls.logicalSet( CHANNEL_ELEVATOR, controls.logicalGet(  CHANNEL_ELEVATOR) + mixedValue);
                }
                break;

            case MIX_ELV_FLP:
                mixedValue = mixValue( elv, mix);
                controls.logicalSet( CHANNEL_FLAP, controls.logicalGet( CHANNEL_FLAP) + mixedValue);
                controls.logicalSet( CHANNEL_FLAP2, controls.logicalGet( CHANNEL_FLAP2) + mixedValue);
                break;

            case MIX_RUD_ELV:
                mixedValue = mixValue( abs(rud), mix);
                controls.logicalSet( CHANNEL_ELEVATOR, controls.logicalGet( CHANNEL_ELEVATOR) + mixedValue);
                if( CFG->wingMix == WINGMIX_VTAIL) {
                    controls.logicalSet( CHANNEL_RUDDER, controls.logicalGet(  CHANNEL_RUDDER) + mixedValue);
                } else if( CFG->wingMix == WINGMIX_DELTA) {
                    controls.logicalSet( CHANNEL_AILERON, controls.logicalGet(  CHANNEL_AILERON) + mixedValue);
                }
                break;

            default:
                // ignore
                break;
            }
        }
    }

    limitChannels( controls);
}

void Model::setDefaults() {

    INIT_NON_PHASED_CONFIGURATION(

        memcpy( CFG->modelName, MODEL_NAME_DEFAULT, MODEL_NAME_LEN +1);
        CFG->wingMix = WINGMIX_NORMAL;
        CFG->qrDiffPct = 0;
        INIT_SWITCH( CFG->qrDiffSw);

        for( uint8_t mix = 0; mix < TEXT_MIX_count; mix++) {
            INIT_SWITCH( CFG->mixSw[mix]);
            CFG->mixPct[mix] = 0;
            CFG->mixOffset[mix] = 0;
        }
    )
}

/* From TableEditable */

uint8_t Model::getRowCount() {

    /* name, wingmix, 2 * qr-diff + (pct + offs) * mix */
    return 4 + 2 * TEXT_MIX_count;
}

const char *Model::getRowName( uint8_t row) {

    if( row == 0) {
        return TEXT_MODEL_NAME;
    } else if (row == 1) {
        return TEXT_WINGMIX;
    } else if (row == 2) {
        return TEXT_AIL_DIFF;
    } else if ((row % 2) == 0) { // 4..6..8
        return MixNames[ (row / 2) -2];
    } else { // 3..5..7
        return TEXT_MSG_NONE;
    }
}

uint8_t Model::getColCount( uint8_t row) {

    // 1,1,1,1,1,3,1,3...
    if( row < 5 || (row % 2) == 0) {
        return 1;
    } else {
        return 3;
    }
}

void Model::getValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        cell->setString( 5, CFG->modelName, MODEL_NAME_LEN);
    } else if( row == 1) {
        cell->setList( 5, WingMixNames, TEXT_WINGMIX_count, CFG->wingMix);
    } else if( row == 2) {
        cell->setSwitchState( 8, CFG->qrDiffSw);
    } else if( row == 3) {
        cell->setInt8( 1, CFG->qrDiffPct, 4, PERCENT_MIN, PERCENT_MAX);
    } else if( (row % 2) == 0) {
        cell->setSwitchState( 8, CFG->mixSw[(row / 2) -2]);
    } else {
        if( col == 0) {
            cell->setInt8( 1, CFG->mixPct[(row/2) -2], 4, PERCENT_MIN, PERCENT_MAX);
        } else if (col == 1) {
            cell->setLabel( 6, TEXT_MIX_OFFSET, 3);
        } else {
            cell->setInt8( 9, CFG->mixOffset[(row/2) -2], 4, PERCENT_MIN_LIMIT, PERCENT_MAX_LIMIT);
        }
    }
}

void Model::setValue( uint8_t row, uint8_t col, Cell *cell) {

    if( row == 0) {
        // nothing to do. inplace string edit
    } else if( row == 1) {
        CFG->wingMix = cell->getList();
    } else if( row == 2) {
        CFG->qrDiffSw = cell->getSwitchState();
    } else if( row == 3) {
        CFG->qrDiffPct = cell->getInt8();
    } else if( (row % 2) == 0) {
        CFG->mixSw[(row / 2) -2] = cell->getSwitchState();
    } else {
        if( col == 0) {
            CFG->mixPct[(row/2) -2] = cell->getInt8();
        } else if (col == 2) {
            CFG->mixOffset[(row/2) -2] = cell->getInt8();
        }
    }
}
