#!/bin/bash

LICSTR="Copyright (c) 20"

tot=`find . -name "*.h" -o -name "*.cpp" -o -name "*.ino" | wc -l`

with=`grep -R --include "*.h" --include "*.cpp" --include "*.ino" -l "$LICSTR" . | wc -l`

without=$(( $tot - $with ))

pct=$(( $with * 100 / $tot ))

echo Total Source Files: $tot
echo Files with License header: $with \( $pct % \)
echo Files without License header: $without
echo

if [ $without -lt 50 ]; then
    echo FILES WITHOUT COPYRIGHT STATEMENT
    grep -R --include "*.h" --include "*.cpp" --include "*.ino" -L "$LICSTR" .
fi
