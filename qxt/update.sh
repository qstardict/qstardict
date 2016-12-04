#!/bin/sh

git clone https://bitbucket.org/libqxt/libqxt || (cd libqxt && git pull && cd ..) || exit 1
for i in AUTHORS COPYING src/core/qxtglobal.* src/widgets/{win,mac,x11,}/qxtglobalshortcut*; do
    for j in libqxt/$i; do
        echo "cp $j ."
        cp $j .
    done
done
