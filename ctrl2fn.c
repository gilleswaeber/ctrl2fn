#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/input.h>

#define KEY_RELEASED 0
#define KEY_PRESSED 1
#define KEY_REPEATED 2

const __useconds_t key_delay = 20000; // µs

const struct input_event
        ctrl_up = {.type = EV_KEY, .code = KEY_RIGHTCTRL, .value = KEY_RELEASED},
        ctrl_down = {.type = EV_KEY, .code = KEY_RIGHTCTRL, .value = KEY_PRESSED},
        syn = {.type = EV_SYN, .code = SYN_REPORT, .value = 0};

typedef struct {
    __s32 old;
    __s32 new;
    int active;
} t_override;
t_override overrides[] = {
        {.old = KEY_LEFT, .new = KEY_HOME, .active = 0},
        {.old = KEY_RIGHT, .new = KEY_END, .active = 0},
        {.old = KEY_DOWN, .new = KEY_PAGEDOWN, .active = 0},
        {.old = KEY_UP, .new = KEY_PAGEUP, .active = 0},
};
const size_t n_overrides = sizeof(overrides) / sizeof(t_override);

int read_event(struct input_event *event) {
    return fread(event, sizeof(struct input_event), 1, stdin) == 1;
}

void write_event(const struct input_event *event) {
    if (fwrite(event, sizeof(struct input_event), 1, stdout) != 1)
        exit(EXIT_FAILURE);
}

int main(void) {
    setbuf(stdin, NULL), setbuf(stdout, NULL);

    struct input_event event;
    enum {
        START, CTRL_PRESSED, CTRL_CANCELLED
    } state = START;

    while (read_event(&event)) {
        if (event.type == EV_MSC && event.code == MSC_SCAN)
            continue;
        if (event.type != EV_KEY) {
            write_event(&event);
            continue;
        }

        switch (state) {
            case START: // RCTRL is currently up
                if (event.code == KEY_RIGHTCTRL) {
                    write_event(&event);
                    state = CTRL_PRESSED;
                } else if (event.value != KEY_PRESSED) {
                    int found = 0;
                    for (size_t i = 0; i < n_overrides; i++) {
                        if (overrides[i].active && event.code == overrides[i].old) {
                            event.code = overrides[i].new;
                            write_event(&event);
                            if (event.value == KEY_RELEASED) overrides[i].active = 0;
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        write_event(&event);
                    }
                } else { // event.value == KEY_PRESSED
                    write_event(&event);
                }
                break;

            case CTRL_PRESSED: // RCTRL is currently down
                if (event.code == KEY_RIGHTCTRL && event.value == KEY_RELEASED) {
                    write_event(&event);
                    state = START;
                } else if (event.value == KEY_PRESSED) {
                    int found = 0;
                    for (size_t i = 0; i < n_overrides; i++) {
                        if (event.code == overrides[i].old) {
                            write_event(&ctrl_up);
                            write_event(&syn);
                            usleep(key_delay);
                            event.code = overrides[i].new;
                            write_event(&event);
                            state = CTRL_CANCELLED;
                            overrides[i].active = 1;
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        write_event(&event);
                    }
                } else { // event.value != KEY_PRESSED
                    int found = 0;
                    for (size_t i = 0; i < n_overrides; i++) {
                        if (overrides[i].active && event.code == overrides[i].old) {
                            event.code = overrides[i].new;
                            write_event(&event);
                            if (event.value == KEY_RELEASED) overrides[i].active = 0;
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        write_event(&event);
                    }
                }
                break;

            case CTRL_CANCELLED: // RCTRL is currently down but has been released by this plugin
                if (event.code == KEY_RIGHTCTRL) {
                    if (event.value == KEY_RELEASED) state = START;
                } else if (event.value == KEY_PRESSED) {
                    int found = 0;
                    for (size_t i = 0; i < n_overrides; i++) {
                        if (event.code == overrides[i].old) {
                            event.code = overrides[i].new;
                            write_event(&event);
                            overrides[i].active = 1;
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        write_event(&event);
                    }
                } else { // event.value != KEY_PRESSED
                    int found = 0;
                    for (size_t i = 0; i < n_overrides; i++) {
                        if (overrides[i].active && event.code == overrides[i].old) {
                            event.code = overrides[i].new;
                            write_event(&event);
                            if (event.value == KEY_RELEASED) overrides[i].active = 0;
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        write_event(&event);
                    }
                }
                break;
            default: // Should never occur
                exit(EXIT_FAILURE);
        }
    }
}
