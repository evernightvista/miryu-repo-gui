#!/usr/bin/env bash
set -e

podir="${podir:-po}"
mkdir -p "$podir"

"${XGETTEXT:-xgettext}" \
    --from-code=UTF-8 \
    --language=C++ \
    --keyword=i18n:1 \
    --keyword=i18nc:1c,2 \
    --keyword=i18np:1,2 \
    --keyword=i18ncp:1c,2,3 \
    --keyword=i18nd:2 \
    --keyword=i18ndc:2c,3 \
    --keyword=i18ndp:2,3 \
    --keyword=i18ndcp:2c,3,4 \
    src/*.cpp src/*.h kcm/*.cpp \
    -o "$podir/miryu-repo-gui.pot"
