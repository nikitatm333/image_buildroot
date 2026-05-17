#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "fb.h"
#include "face.h"

int main() {
    Framebuffer fb;
    if (fb_open(&fb, "/dev/fb0") < 0) return 1;

    srand(time(NULL));

    FaceState states[] = {
        FACE_IDLE,
        FACE_TALKING,
        FACE_HAPPY,
        FACE_SURPRISED,
        FACE_ANGRY,
        FACE_WTF,
        FACE_SAD,
        FACE_STOIC,
    };
    int current = 0;
    time_t last_change = time(NULL);

    // while (1) {
    //     face_draw(&fb, states[current]);

    //     // Меняем эмоцию каждые 3 секунды
    //     if (time(NULL) - last_change >= 3) {
    //         current = (current + 1) % 7;
    //         last_change = time(NULL);
    //     }

    //     usleep(50000);
    // }
    while (1) {
        face_draw(&fb, FACE_SMILE2);
        usleep(50000);
    }

    fb_close(&fb);
    return 0;
}