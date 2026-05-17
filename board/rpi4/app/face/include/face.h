#ifndef FACE_H
#define FACE_H

#include "fb.h"

typedef enum {
    FACE_IDLE,
    FACE_TALKING,
    FACE_HAPPY,
    FACE_SURPRISED,
    FACE_ANGRY,
    FACE_WTF,
    FACE_SAD,
    FACE_STOIC,
    FACE_CUTE1,   /* ^ глаза + рот от stoic      */
    FACE_CUTE2,   /* ^ глаза + V рот             */
    FACE_CUTE3,   /* круглые глаза + V рот       */ 
    FACE_SMILE1,
    FACE_SMILE2,
} FaceState;

void face_draw(Framebuffer *fb, FaceState state);

#endif