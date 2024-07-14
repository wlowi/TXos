@echo off
rem
rem Remove links for all cpp and header files from ../src hierarchy
rem

set LOCALCONFIG=TXosLocalConfig.h

del TXos.ino
del *.cpp
if exist %LOCALCONFIG% (
    ren %LOCALCONFIG% keep_this
    del *.h
    ren keep_this %LOCALCONFIG%
) else (
    del *.h
)

