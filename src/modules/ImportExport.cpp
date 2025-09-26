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

#include "ImportExport.h"
#include "ModuleManager.h"

extern ModuleManager moduleManager;
extern void watchdog_reset();

const uint8_t STATE_INACTIVE = 0;
const uint8_t STATE_CONNECTING = 1;
const uint8_t STATE_EXPORT_MODEL = 2;
const uint8_t STATE_IMPORT_MODEL = 3;
const uint8_t STATE_EXPORT_SYSCF = 4;
const uint8_t STATE_IMPORT_SYSCF = 5;

ImportExport::ImportExport(Stream& stream) : Module(MODULE_IMPORTEXPORT_TYPE, TEXT_MODULE_IMPORTEXPORT, COMM_SUBPACKET_NONE), inOut(stream), comm(*new Comm(&stream))
{
    setDefaults();
}

COMM_RC_t ImportExport::runExport(const DICT_t* dict, const DICTROW_t* row[], uint8_t* config, moduleSize_t configSz)
{
    nameType_t name;

    name = pgm_read_word(&(dict->name));
    comm.openSub(name);

    name = pgm_read_word(&(dict->subName));

    if (name == 0) {

        exportModulePhase(row, config);
    }
    else {
        for (uint8_t p = 0; p < PHASES; p++) {

            comm.openSub(name);
            comm.addUInt8(COMM_FIELD_PHASE, p);
            comm.writePart();

            exportModulePhase(row, config);

            comm.close();
            config += configSz;
        }
    }

    comm.close();

    return COMM_RC_OK;
}

/**
 * @brief
 *
 * @param dict
 * @param row
 * @param config
 * @param configSz The size of a single config record. Always non-phased size!
 */
COMM_RC_t ImportExport::runImport(const DICT_t* dict, const DICTROW_t* row[], uint8_t* config, moduleSize_t configSz)
{
    nameType_t cmd;
    nameType_t sub;
    char dType;
    uint8_t width;
    uint16_t count;

    uint8_t dictDataType;
    size_t dictOffset;
    size_t dictSize;
    uint16_t dictCount;

    uint8_t phase;

    COMM_RC_t rc;

    do {
        rc = comm.nextField(&cmd, &dType, &width, &count);
        if( rc == COMM_RC_SUBSTART) {
            if ((rc = comm.nextPacket(&cmd)) == COMM_RC_OK) {

                sub = pgm_read_word(&(dict->subName));

                if( sub == cmd) {
                    LOGV("runImport: valid subpacket %c%c\n", PACKET_NAME(cmd));

                    if ((rc = comm.nextField(&cmd, &dType, &width, &count)) == COMM_RC_OK) {
                        if (cmd == COMM_FIELD_PHASE) {
                            comm.nextData(&phase, dType, width, count);
                            if( phase < PHASES) {
                                rc = runImport( dict, row, config + (phase*configSz), configSz);
                                if( rc == COMM_RC_END) {
                                    rc = COMM_RC_OK;
                                }
                            } else {
                                LOGV("ModuleManager::importModel(): ERROR: invalid phase %d\n", phase);
                                rc = COMM_RC_PROTOCOL;
                            }
                        } else {
                            LOG("ModuleManager::importModel(): ERROR: COMM_FIELD_ID expected\n");
                            rc = COMM_RC_PROTOCOL;
                        }
                    } else {
                        LOG("ModuleManager::importModel(): ERROR: Field expected\n");
                        rc = COMM_RC_PROTOCOL;
                    }
                }
            } else {
                LOG("ModuleManager::importModel(): ERROR: Failed to read subpacket\n");
                rc = COMM_RC_PROTOCOL;
            }
        } else if( rc == COMM_RC_OK) {
            LOGV("runImport: %c%c: dType=%c, width=%d, count=%d\n", PACKET_NAME(cmd), dType, width, count);
            if( findDictEntry( row, cmd, &dictDataType, &dictOffset, &dictSize, &dictCount)) {
                LOGV("runImport: dictType=%d, offset=%ld, size=%ld count=%d\n", dictDataType, dictOffset, dictSize, dictCount);
                rc = comm.nextData( config+dictOffset, dType, width, count);
            } else {
                LOGV("runImport: ERROR: No dictionary entry found for %c%c\n", PACKET_NAME(cmd));
                rc = COMM_RC_PROTOCOL;
            }
        }
    } while( rc == COMM_RC_OK);

    return rc;
}


bool ImportExport::findDictEntry( const DICTROW_t* row[], nameType_t cmd, uint8_t *dictDataType, size_t *dictOffset, size_t *dictSize, uint16_t *dictCount) {

    const DICTROW_t* drow;
    nameType_t name;

    uint8_t ri = 0;

    while ((drow = (const DICTROW_t*)pgm_read_ptr_far(&(row[ri++])))) {

        name = pgm_read_word(&(drow->rowName));

        if( name == cmd) {
            *dictDataType = pgm_read_byte(&(drow->dataType));
            *dictOffset = pgm_read_dword(&(drow->offset));
            *dictSize = pgm_read_dword(&(drow->size));
            *dictCount = pgm_read_word(&(drow->count));

            return true;
        }
    }

    return false;
}


/* private */
/*
 * Export a single module phase or a non-phased module.
 */
void ImportExport::exportModulePhase(const DICTROW_t* row[], uint8_t* config)
{
    const DICTROW_t* drow;
    nameType_t name;
    uint8_t dataType;
    size_t offset;
    size_t size;
    uint16_t count;

    uint8_t ri = 0;

    while ((drow = (const DICTROW_t*)pgm_read_ptr_far(&(row[ri++])))) {

        dataType = pgm_read_byte(&(drow->dataType));
        name = pgm_read_word(&(drow->rowName));
        offset = pgm_read_dword(&(drow->offset));
        size = pgm_read_dword(&(drow->size));
        count = pgm_read_word(&(drow->count));

        switch (dataType) {
        case COMM_DATATYPE_STRING:
            comm.addString(name, (const char*)(config + offset));
            break;

        case COMM_DATATYPE_CHAR:
            comm.addChar(name, *((char*)(config + offset)));
            break;

        case COMM_DATATYPE_BOOL:
            comm.addBool(name, *((bool*)(config + offset)));
            break;

        case COMM_DATATYPE_INT8:
            comm.addInt8(name, *((int8_t*)(config + offset)));
            break;

        case COMM_DATATYPE_UINT8:
            comm.addUInt8(name, *((uint8_t*)(config + offset)));
            break;

        case COMM_DATATYPE_INT16:
            comm.addInt16(name, *((int16_t*)(config + offset)));
            break;

        case COMM_DATATYPE_UINT16:
            comm.addUInt16(name, *((uint16_t*)(config + offset)));
            break;

        case COMM_DATATYPE_INT32:
            comm.addInt32(name, *((int32_t*)(config + offset)));
            break;

        case COMM_DATATYPE_UINT32:
            comm.addUInt32(name, *((uint32_t*)(config + offset)));
            break;

        case COMM_DATATYPE_INTARR:
            comm.addIntArr(name, (const byte*)(config + offset), size, count);
            break;

        case COMM_DATATYPE_UINTARR:
            comm.addUIntArr(name, (const byte*)(config + offset), size, count);
            break;
        }

        comm.writePart();
    }
}

/* From Module */

/* Initial state is STATE_CONNECTING.
 * In this state we are waiting for a connnect request.
 * Once the connect request is received we send an info packet.
 * The info packet contains the firmware release and the number of supported models.
*/

void ImportExport::run(Controls& controls)
{
    nameType_t cmd;
    char dType;
    uint8_t width;
    uint16_t count;
    uint8_t rc;
    bool wasEnabled;

    if (state == STATE_INACTIVE) {
        return;
    }

    if ((rc = comm.nextPacket(&cmd)) == COMM_RC_OK) {

        wasEnabled = watchdog_disable();

        switch (cmd) {
        case COMM_PACKET_GET_MODELCONFIG:
            /* Read packet end marker */
            comm.nextField(&cmd, &dType, &width, &count);
            state = STATE_EXPORT_MODEL;
            moduleManager.exportModels(this);
            break;

        case COMM_PACKET_GET_SYSCONFIG:
            comm.nextField(&cmd, &dType, &width, &count);
            state = STATE_EXPORT_SYSCF;
            moduleManager.exportSystemConfig(this);
            break;

        case COMM_PACKET_MODELCONFIG:
            state = STATE_IMPORT_MODEL;
            moduleManager.importModel(this);
            break;

        case COMM_PACKET_SYSCONFIG:
            state = STATE_IMPORT_SYSCF;
            moduleManager.importSystemConfig(this);
            break;

        default:
            state = STATE_CONNECTING;
            comm.nextField(&cmd, &dType, &width, &count);
            comm.open(COMM_PACKET_ERROR);
            comm.close();
            comm.write();
        }

        if( wasEnabled) { watchdog_enable(); }

        changed = true;
    }
}

void ImportExport::setDefaults()
{
    state = STATE_INACTIVE;
    changed = true;
}

void ImportExport::moduleEnter()
{
    comm.open(COMM_PACKET_INFO);

    comm.addString( COMM_FIELD_VERSION, TXOS_VERSION);

    comm.close();
    comm.write();

    comm.drain();

    state = STATE_CONNECTING;
    changed = true;
}

void ImportExport::moduleExit()
{
    setDefaults();
}

/* From TextUIScreen */

bool ImportExport::hasChanged(uint8_t row, uint8_t col)
{
    bool ret = false;

    if (row == 2) {
        ret = changed;
        changed = false;
    }

    return ret;
}

uint8_t ImportExport::getRowCount()
{
    return 3;
}

const char* ImportExport::getRowName(uint8_t row)
{
    if (row == 1) {
        return TEXT_STATUS;
    }

    return "";
}

uint8_t ImportExport::getColCount(uint8_t row)
{
    if (row == 2) {
        return 1;
    }

    return 0;
}

void ImportExport::getValue(uint8_t row, uint8_t col, Cell* cell)
{
    if (row == 2) {
        switch (state) {

        case STATE_INACTIVE:
            cell->setLabel(0, TEXT_OFF, 10);
            break;

        case STATE_CONNECTING:
            cell->setLabel(0, TEXT_CONNECTING, 10);
            break;

        case STATE_EXPORT_MODEL:
            cell->setLabel(0, TEXT_EXPORT, 10);
            break;

        case STATE_IMPORT_MODEL:
            cell->setLabel(0, TEXT_IMPORT, 10);
            break;

        case STATE_EXPORT_SYSCF:
            cell->setLabel(0, TEXT_EXPORT_SYS, 10);
            break;

        case STATE_IMPORT_SYSCF:
            cell->setLabel(0, TEXT_IMPORT_SYS, 10);
            break;
        }
    }
}
