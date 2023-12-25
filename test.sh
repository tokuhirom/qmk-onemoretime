#!/bin/bash
QMK_BASE=$HOME/work/qmk/
gcc -DCONSOLE_ENABLE -Wall -Wno-unused-function -DONE_MORE_TIME_TEST  -x c -I $QMK_BASE/quantum -I $QMK_BASE/platforms/test -I $QMK_BASE/platforms -I $QMK_BASE/quantum/keymap_extras -I $QMK_BASE/quantum/sequencer -I $QMK_BASE/quantum/bootmagic -I $QMK_BASE/tmk_core/protocol -I $QMK_BASE/quantum/logging onemoretime.c && ./a.out
