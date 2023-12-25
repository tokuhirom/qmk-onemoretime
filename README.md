# qmk-onemoretime

https://scrapbox.io/self-made-kbds-ja/%E5%A2%97%E4%BA%95%E4%BF%8A%E4%B9%8B%E3%81%8C%E3%82%AD%E3%83%BC%E3%83%9C%E3%83%BC%E3%83%89%E3%81%AB%E6%AC%B2%E3%81%97%E3%81%84%E6%A9%9F%E8%83%BD
https://gihyo.jp/dev/serial/01/masui-columbus/0006

This is a implementation of the Toshiyuki Masui san's Dynamic macro in QMK firmware.

This library is implemented as a single file C library.
You can import this to your own `keymap.c`.

You can use Dynamic macro everywhere.

## What's dynamic macro?

In Japanese:

    Dynamic Macroの原理は非常に単純で、

        「同じ編集操作を2回繰り返したあとで［CTRL］＋［t］を押すと繰り返された操作が再実行される」

    というものです。「二度あることは三度ある」と言うように、同じことが二度あればもう一度あるのは世の中でごく普通のことです。二度実行した操作をもう一度実行することもよくあることですので、この方法はたいへん効果的です。

In English:

    The principle of Dynamic Macro is very simple.

    "If you press [CTRL] + [t] after repeating the same editing operation twice, the repeated operation will be re-executed."

    That's what it means. As the saying goes, ``What happens twice happens three times,'' and it is a very common thing in this world that if the same thing happens twice, it will happen again. This method is very effective because it is common to repeat an operation that has been performed twice.


## Motivation

Originally, DynamicMacro is implemented in elisp. And other implementations are implemented as an editor extensions.
This library is implemented as a QMK firmware, as a result, you can use Dynamic Macro everywhere. e.g. the editor, etc..

## Naming...

So, there's a QMK's DynamicMacro feature. It's not same as a toshiyuki masui's DynamicMacro.
As a result, I named this library as a `qmk-onemoretime`.

## Usage

    #include "onemoretime.c"

    bool process_record_user(uint16_t keycode, keyrecord_t *record) {
        if (record->event.pressed) {
            if (keycode == KC_L && (keyboard_report->mods & MOD_BIT(KC_LCTL))) {
                it (one_more_time_play()) {
                    return false;
                }
            } else {
                one_more_time_record(keycode, record);
            }
        }
        return true;
    }

In this case, a user types `Ctrl-L`, DynamicMacro runs.

## Author

Tokuhiro Matsuno <tokuhirom@gmail.com>

## LICENSE

    This is free and unencumbered software released into the public domain.

    Anyone is free to copy, modify, publish, use, compile, sell, or
    distribute this software, either in source code form or as a compiled
    binary, for any purpose, commercial or non-commercial, and by any
    means.

    In jurisdictions that recognize copyright laws, the author or authors
    of this software dedicate any and all copyright interest in the
    software to the public domain. We make this dedication for the benefit
    of the public at large and to the detriment of our heirs and
    successors. We intend this dedication to be an overt act of
    relinquishment in perpetuity of all present and future rights to this
    software under copyright law.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
    OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.

    For more information, please refer to <https://unlicense.org>

