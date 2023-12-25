/**
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <https://unlicense.org>
 */
#include <stdint.h>
#include <string.h>
#include "quantum.h"

#ifdef CONSOLE_ENABLE
  #include <print.h>
#endif


#ifndef ONE_MORE_TIME_BUFFER_SIZE
#  ifdef ONE_MORE_TIME_TEST
#    define ONE_MORE_TIME_BUFFER_SIZE 6
#  else
#    define ONE_MORE_TIME_BUFFER_SIZE 32
#  endif
#endif

// ring buffer
static uint8_t one_more_time_buffer[ONE_MORE_TIME_BUFFER_SIZE];
static uint8_t one_more_time_buffer_index = 0;

#ifdef CONSOLE_ENABLE
static void one_more_time_dump(void) {
    uprintf("  idx=%u\n", one_more_time_buffer_index);

    for (int i=0; i<ONE_MORE_TIME_BUFFER_SIZE; i++) {
        uprintf("  i=%u keycode=%d %c\n",
            i,
            one_more_time_buffer[i],
            (one_more_time_buffer_index-1 == i) ? '*' : ' ');
    }
}
#endif

static void one_more_time_record(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        one_more_time_buffer[one_more_time_buffer_index] = keycode;
        one_more_time_buffer_index = (one_more_time_buffer_index + 1) % ONE_MORE_TIME_BUFFER_SIZE;
    }
}

static bool is_duplicated(int i, int8_t modifiers_cnt) {
    for (int j=0; j<i; j++) {
        int nn = one_more_time_buffer_index-modifiers_cnt-1-j;
        if (nn<0) {
            nn += ONE_MORE_TIME_BUFFER_SIZE;
        }
        int mm = one_more_time_buffer_index-modifiers_cnt-1-i-j;
        if (mm<0) {
            mm += ONE_MORE_TIME_BUFFER_SIZE;
        }
        bool result = one_more_time_buffer[nn] == one_more_time_buffer[mm];
#ifdef CONSOLE_ENABLE
        uprintf("   i=%d j=%d idx=%d nn=%d mm=%d x=%d y=%d result=%d\n", i, j, one_more_time_buffer_index, nn, mm, one_more_time_buffer[nn], one_more_time_buffer[mm], result ? 1 : 0);
#endif
        if (!result) {
            return false;
        }
    }
    return true;
}

bool is_modifier_key(uint16_t keycode) {
    return (keycode >= KC_LCTL && keycode <= KC_RGUI);
}


static int16_t check_duplication(uint8_t modifiers_cnt) {
    for (int i=(ONE_MORE_TIME_BUFFER_SIZE-modifiers_cnt)/2; i>0; i--) {
        if (is_duplicated(i, modifiers_cnt)) {
            return i;
        }
    }
    return -1; // no dups
}

static uint8_t count_modifiers(void) {
    uint8_t modifiers_cnt = 0;
    for (int i = 1; i < ONE_MORE_TIME_BUFFER_SIZE/2; i++) {
        int nn = one_more_time_buffer_index - i;
        if (nn < 0) {
            nn += ONE_MORE_TIME_BUFFER_SIZE;
        }

        if (is_modifier_key(one_more_time_buffer[nn])) {
            modifiers_cnt++;
        } else {
            break;
        }
    }
    return modifiers_cnt;
}

bool one_more_time_play(void) {
    uint8_t modifiers = count_modifiers();
    int16_t n = check_duplication(modifiers);

#ifdef CONSOLE_ENABLE
        uprintf("Play: n=%u modifiers=%u\n", n, modifiers);
#endif

    if (n >= 0) {
        // unregister modifiers
        for (int j = modifiers-1; j >= 0; j--) {
            int nn = one_more_time_buffer_index-1-j;
            if (nn < 0) {
                nn += ONE_MORE_TIME_BUFFER_SIZE;
            }

#ifdef CONSOLE_ENABLE
            uprintf("unregister modifier: key=%d\n", one_more_time_buffer[nn]);
#endif
            unregister_code(one_more_time_buffer[nn]);
        }

        for (int j = n-1; j >= 0; j--) {
            int nn = one_more_time_buffer_index-modifiers-1-j;
            if (nn < 0) {
                nn += ONE_MORE_TIME_BUFFER_SIZE;
            }

#ifdef CONSOLE_ENABLE
            uprintf("register: key=%d\n", one_more_time_buffer[nn]);
#endif
            register_code(one_more_time_buffer[nn]);
            unregister_code(one_more_time_buffer[nn]);
        }

        // restore modifiers
        for (int j = modifiers-1; j >= 0; j--) {
            int nn = one_more_time_buffer_index-1-j;
            if (nn < 0) {
                nn += ONE_MORE_TIME_BUFFER_SIZE;
            }

#ifdef CONSOLE_ENABLE
            uprintf("unregister modifier: key=%d\n", one_more_time_buffer[nn]);
#endif
            register_code(one_more_time_buffer[nn]);
        }

        return true;
    } else {
        return false;
    }
}

#ifdef ONE_MORE_TIME_TEST

#include <stdio.h>
#include <assert.h>

void clear_mods(void) {
}

uint8_t get_mods() {
    return 0;
}

void set_mods(uint8_t mods) {
}

void register_code(uint8_t keycode) {
    printf("  reigster=%d\n", keycode);
}

void unregister_code(uint8_t keycode) {
    printf("  unreigster=%d\n", keycode);
}

void run_test(uint16_t test_sequence[], int sequence_length, int expected_result) {
    for (int i = 0; i < sequence_length; i++) {
        one_more_time_record(test_sequence[i], &(keyrecord_t){ .event = { .pressed = true } });
    }
    int modifiers = count_modifiers();
    int n = check_duplication(modifiers);
    printf("n=%d modifiers=%d\n", n, modifiers);
    assert(n == expected_result);

    one_more_time_play();
}

#define TEST(x, e) run_test((x), sizeof((x)) / sizeof((x)[0]), (e))

void test_one_more_time_play() {
    uint16_t test_sequence1[] = { KC_A, KC_B, KC_C, KC_A, KC_B, KC_C };
    TEST(test_sequence1, 3);

    uint16_t test_sequence2[] = { KC_E, KC_A, KC_B, KC_C, KC_A, KC_B, KC_C };
    TEST(test_sequence2, 3);

    uint16_t test_sequence3[] = { KC_E, KC_N, KC_B, KC_C, KC_B, KC_C };
    TEST(test_sequence3, 2);

    uint16_t test_sequence4[] = { KC_E, KC_N, KC_B, KC_C, KC_B, KC_C, KC_LCTL };
    TEST(test_sequence4, 2);

    uint16_t test_sequence6[] = { KC_A, KC_B, KC_C, KC_A, KC_Z, KC_Z };
    TEST(test_sequence6, 1);

    uint16_t test_sequence5[] = { KC_E, KC_N, KC_B, KC_C, KC_B, KC_C, KC_LCTL, KC_LCTL };
    TEST(test_sequence5, 2);
}


int main() {
    test_one_more_time_play();

    printf("OK\n");
    return 0;

}
#endif // ONE_MORE_TIME_TEST

