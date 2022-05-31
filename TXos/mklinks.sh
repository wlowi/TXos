#!/bin/bash
# 
# Create links for all cpp and header files from ../test hierarchy
#

SOURCE="../test ../test/controls ../test/modules ../test/output ../test/ui atmega2560"

sh rmlinks.sh

for SOURCEDIR in $SOURCE ; do
    echo Link from $SOURCEDIR

    for FILE in $SOURCEDIR/*.cpp ; do
        TARGET=`basename $FILE`
        echo "  " $FILE " => " $TARGET
        ln -s $FILE $TARGET
    done

    for FILE in $SOURCEDIR/*.h ; do
        TARGET=`basename $FILE`
        echo "  " $FILE " => " $TARGET
        ln -s $FILE $TARGET
    done
done

rm Arduino.h TXosTest.cpp

mv TXos.cpp TXos.ino

