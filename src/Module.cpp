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

#include "Module.h"
#include "ModuleManager.h"
#include "SystemConfig.h"
#include "ModelSelect.h"

extern ModuleManager moduleManager;
extern SystemConfig systemConfig;
extern ModelSelect modelSelect;

Module::Module( moduleType_t type, const char *name) : moduleName( name), moduleType( type) {

}

moduleType_t Module::getConfigType() const {

    return moduleType;
}

const char *Module::getMenuName() {

    return moduleName;
}

void Module::activate(TextUI *ui)
{
    moduleEnter();
}

void Module::deactivate(TextUI *ui)
{
    moduleExit();

    if( moduleManager.inSystemSet( this)) {
        systemConfig.save();

    } else if( moduleManager.inModelSet( this)) {
        moduleManager.saveModel( modelSelect.getModelID());
    }
}