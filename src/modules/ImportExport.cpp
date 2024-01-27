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

const uint8_t STATE_INACTIVE = 0;
const uint8_t STATE_CONNECTING = 1;

ImportExport::ImportExport(Stream& stream) : Module(MODULE_IMPORTEXPORT_TYPE, TEXT_MODULE_IMPORTEXPORT), inOut(stream), comm(*new Comm(&stream))
{

    setDefaults();
}

void ImportExport::runExport(const DICT_t* dict, const DICTROW_t* row[], uint8_t* config, moduleSize_t configSz)
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

    while ((drow = pgm_read_ptr_far(&(row[ri])))) {

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
        ri++;
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

    if (state != STATE_CONNECTING) {
        return;
    }

    if ((rc = comm.nextPacket(&cmd)) == COMM_RC_OK) {

        switch (cmd) {
        case COMM_PACKET_GET_MODELCONFIG:
            /* Read packet end marker */
            comm.nextField(&cmd, &dType, &width, &count);
            moduleManager.exportModels(this);
            break;

        case COMM_PACKET_GET_SYSCONFIG:
            comm.nextField(&cmd, &dType, &width, &count);
            moduleManager.exportSystemConfig(this);
            break;

        default:
            comm.nextField(&cmd, &dType, &width, &count);
            comm.open(COMM_PACKET_ERROR);
            comm.close();
            comm.write();
        }
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
    comm.close();
    comm.write();

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
            cell->setLabel(0, TEXT_OFF, strlen(TEXT_OFF));
            break;

        case STATE_CONNECTING:
            cell->setLabel(0, TEXT_CONNECTING, strlen(TEXT_CONNECTING));
            break;
        }
    }
}
