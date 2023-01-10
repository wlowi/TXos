#!/bin/bash
# 
# Remove links for all cpp and header files from ../src hierarchy
#

find . -lname "*.cpp" -exec rm -f \{\} \;
find . -lname "*.h" -exec rm -f \{\} \;

