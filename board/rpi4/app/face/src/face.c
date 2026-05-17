#include "face.h"
#include "draw.h"
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

/* ═══════════════════════════════════════════════════
 * Константы — координаты и размеры
 * ═══════════════════════════════════════════════════ */

#define L_EYE_X   45
#define L_EYE_Y   50
#define R_EYE_X   111
#define R_EYE_Y   50
#define EYE_R     13

#define SMILE_CX  78
#define SMILE_CY  88
#define SMILE_RX  17
#define SMILE_RY   5
#define SMILE_T    2

/* ═══════════════════════════════════════════════════
 * Утилиты (примитивы более высокого уровня)
 * ═══════════════════════════════════════════════════ */

/* Скруглённый прямоугольник */
static void draw_rrect(Framebuffer *fb, int x, int y, int w, int h, int r) {
    draw_rect(fb, x+r,   y,   w-2*r, h,     COLOR_WHITE);
    draw_rect(fb, x,     y+r, r,     h-2*r, COLOR_WHITE);
    draw_rect(fb, x+w-r, y+r, r,     h-2*r, COLOR_WHITE);
    draw_filled_circle(fb, x+r,     y+r,     r, COLOR_WHITE);
    draw_filled_circle(fb, x+w-r-1, y+r,     r, COLOR_WHITE);
    draw_filled_circle(fb, x+r,     y+h-r-1, r, COLOR_WHITE);
    draw_filled_circle(fb, x+w-r-1, y+h-r-1, r, COLOR_WHITE);
}

/* Толстая линия через filled_circle */
static void thick_line(Framebuffer *fb,
                       int x0, int y0, int x1, int y1, int thick) {
    int dx = abs(x1-x0), sx = x0<x1 ? 1:-1;
    int dy = -abs(y1-y0), sy = y0<y1 ? 1:-1;
    int err = dx+dy, e2;
    while (1) {
        draw_filled_circle(fb, x0, y0, thick, COLOR_WHITE);
        if (x0==x1 && y0==y1) break;
        e2 = 2*err;
        if (e2>=dy){err+=dy; x0+=sx;}
        if (e2<=dx){err+=dx; y0+=sy;}
    }
}

/* ═══════════════════════════════════════════════════
 * КОМПОНЕНТЫ — ГЛАЗА
 * ═══════════════════════════════════════════════════ */

/* (●)(●) круглые открытые глаза */
static void eyes_circle(Framebuffer *fb) {
    draw_filled_circle(fb, L_EYE_X, L_EYE_Y, EYE_R, COLOR_WHITE);
    draw_filled_circle(fb, R_EYE_X, R_EYE_Y, EYE_R, COLOR_WHITE);
}

/* (—)(—) закрытые глаза (моргание) */
static void eyes_closed(Framebuffer *fb) {
    draw_rect(fb, L_EYE_X - EYE_R, L_EYE_Y - 2, EYE_R * 2, 5, COLOR_WHITE);
    draw_rect(fb, R_EYE_X - EYE_R, R_EYE_Y - 2, EYE_R * 2, 5, COLOR_WHITE);
}

/* (^)(^) глаза-каретки, уголок смотрит вверх */
static void eyes_caret(Framebuffer *fb) {
    int hw = 12, hh = 9;
    thick_line(fb, L_EYE_X - hw, L_EYE_Y + hh, L_EYE_X,      L_EYE_Y - hh, SMILE_T * 2);
    thick_line(fb, L_EYE_X,      L_EYE_Y - hh, L_EYE_X + hw, L_EYE_Y + hh, SMILE_T * 2);
    thick_line(fb, R_EYE_X - hw, R_EYE_Y + hh, R_EYE_X,      R_EYE_Y - hh, SMILE_T * 2);
    thick_line(fb, R_EYE_X,      R_EYE_Y - hh, R_EYE_X + hw, R_EYE_Y + hh, SMILE_T * 2);
}

/* Вертикальные скруглённые прямоугольники
 * Ширина = диаметр круглого глаза, вытянуты вверх и вниз на stretch
 * r = EYE_R → боковые стороны прямые, верх и низ — полукруги */
static void eyes_oval_v(Framebuffer *fb) {
    int stretch = 7;
    draw_rrect(fb,
               L_EYE_X - EYE_R, L_EYE_Y - EYE_R - stretch,
               EYE_R * 2, EYE_R * 2 + stretch * 2, EYE_R);
    draw_rrect(fb,
               R_EYE_X - EYE_R, R_EYE_Y - EYE_R - stretch,
               EYE_R * 2, EYE_R * 2 + stretch * 2, EYE_R);
}

/* Моргание — передай указатель на статический таймер из face-функции
 * Рисует eyes_closed + usleep при моргании, иначе eyes_circle */
static void eyes_blink(Framebuffer *fb, time_t *last_blink) {
    time_t now = time(NULL);
    if (now - *last_blink >= 4) {
        eyes_closed(fb);
        usleep(130000);
        *last_blink = now;
    } else {
        eyes_circle(fb);
    }
}

/* ═══════════════════════════════════════════════════
 * КОМПОНЕНТЫ — БРОВИ
 * ═══════════════════════════════════════════════════ */

/* \_/ злые брови под углом, почти касаются глаз */
static void brows_angry(Framebuffer *fb) {
    int inner_y = L_EYE_Y - EYE_R + 2;
    int outer_y = 32;
    thick_line(fb, L_EYE_X - EYE_R + 5, outer_y,
                   L_EYE_X + EYE_R - 2, inner_y, SMILE_T);
    thick_line(fb, R_EYE_X - EYE_R + 2, inner_y,
                   R_EYE_X + EYE_R - 5, outer_y, SMILE_T);
}

/* Прямые горизонтальные брови над глазами
 * Толщина = SMILE_T, расстояние от глаза = gap */
static void brows_straight(Framebuffer *fb) {
    int stretch = 7;                                  /* должно совпадать с eyes_oval_v */
    int gap     = 6;
    int brow_y  = L_EYE_Y - EYE_R - stretch - gap - SMILE_T * 2;
    draw_rrect(fb, L_EYE_X - EYE_R, brow_y, EYE_R * 2, SMILE_T * 2, SMILE_T);
    draw_rrect(fb, R_EYE_X - EYE_R, brow_y, EYE_R * 2, SMILE_T * 2, SMILE_T);
}

/* ═══════════════════════════════════════════════════
 * КОМПОНЕНТЫ — РОТ
 * ═══════════════════════════════════════════════════ */

/* ∪ маленькая плавная улыбка */
static void mouth_smile(Framebuffer *fb) {
    for (int x = -SMILE_RX; x <= SMILE_RX; x++) {
        float t = 1.0f - ((float)x / SMILE_RX) * ((float)x / SMILE_RX);
        if (t < 0.0f) continue;
        int y = (int)(SMILE_RY * sqrtf(t) * 0.7f);
        draw_filled_circle(fb, SMILE_CX + x, SMILE_CY + y, SMILE_T, COLOR_WHITE);
    }
}

/* — прямой скруглённый рот во всю ширину (злой) */
static void mouth_flat(Framebuffer *fb) {
    draw_rrect(fb, L_EYE_X, SMILE_CY,
               R_EYE_X - L_EYE_X, SMILE_T * 4, SMILE_T);
}

/* — короткий скруглённый рот (невозмутимый) */
static void mouth_flat_short(Framebuffer *fb) {
    draw_rrect(fb, L_EYE_X + 10, SMILE_CY,
               R_EYE_X - L_EYE_X - 20, SMILE_T * 2, SMILE_T);
}

/* ∨ рот галочкой, уголок смотрит вниз */
static void mouth_v(Framebuffer *fb) {
    int hw = 10;
    int hh = (hw * 9) / 14;
    thick_line(fb, SMILE_CX - hw, SMILE_CY - hh,
                   SMILE_CX,      SMILE_CY + hh, SMILE_T);
    thick_line(fb, SMILE_CX,      SMILE_CY + hh,
                   SMILE_CX + hw, SMILE_CY - hh, SMILE_T);
}

/* Широкооткрытый рот с изгибом верхнего края */
static void mouth_wide_open(Framebuffer *fb) {
    int ml = L_EYE_X - EYE_R;
    int mr = R_EYE_X + EYE_R;
    int mt = 78;
    int mb = 110;
    int r      = 10;
    int cd     = 6;
    int cd_bot = 4;

    int cx = (ml + mr) / 2;
    int hw = (mr - ml) / 2 - r;

    for (int x = ml; x <= mr; x++) {
        int top_y = mt;
        int bot_y = mb;

        if (x - ml < r) {
            int dx  = r - (x - ml);
            int arc = (int)sqrtf((float)(r*r - dx*dx));
            top_y = mt + r - arc;
            bot_y = mb - r + arc;   /* было mb + r - arc — неверно */
        } else if (mr - x < r) {
            int dx  = r - (mr - x);
            int arc = (int)sqrtf((float)(r*r - dx*dx));
            top_y = mt + r - arc;
            bot_y = mb - r + arc;   /* исправлено */
        } else {
            float t = (float)(x - cx) / hw;
            top_y = mt + (int)(cd     * (1.0f - t*t));
            bot_y = mb + (int)(cd_bot * (1.0f - t*t));
        }

        for (int y = top_y; y <= bot_y; y++)
            draw_pixel(fb, x, y, COLOR_WHITE);
    }
}

/* ═══════════════════════════════════════════════════
 * ЭМОЦИИ — собираются из компонентов
 * ═══════════════════════════════════════════════════ */

/* IDLE: круглые глаза + улыбка + моргание */
static void face_idle(Framebuffer *fb) {
    static time_t lb = 0;
    fb_clear(fb, COLOR_BLACK);
    mouth_smile(fb);
    eyes_blink(fb, &lb);
}

/* ANGRY: круглые глаза + злые брови + прямой рот */
static void face_angry(Framebuffer *fb) {
    fb_clear(fb, COLOR_BLACK);
    brows_angry(fb);
    eyes_circle(fb);
    mouth_flat(fb);
}

/* STOIC: круглые глаза + моргание + короткий рот */
static void face_stoic(Framebuffer *fb) {
    static time_t lb = 0;
    fb_clear(fb, COLOR_BLACK);
    mouth_flat_short(fb);  /* рисуем до моргания — не исчезает */
    eyes_blink(fb, &lb);
}

/* CUTE1: ^ глаза + короткий рот */
static void face_cute1(Framebuffer *fb) {
    fb_clear(fb, COLOR_BLACK);
    mouth_flat_short(fb);
    eyes_caret(fb);
}

/* CUTE2: ^ глаза + V рот */
static void face_cute2(Framebuffer *fb) {
    fb_clear(fb, COLOR_BLACK);
    eyes_caret(fb);
    mouth_v(fb);
}

/* CUTE3: круглые глаза + моргание + V рот */
static void face_cute3(Framebuffer *fb) {
    static time_t lb = 0;
    fb_clear(fb, COLOR_BLACK);
    mouth_v(fb);           /* рисуем до моргания — не исчезает */
    eyes_blink(fb, &lb);
}

/* SMILE1: круглые глаза + моргание + широкооткрытый рот */
static void face_smile1(Framebuffer *fb) {
    static time_t lb = 0;
    fb_clear(fb, COLOR_BLACK);
    mouth_wide_open(fb);  /* рот первым — не моргает */
    eyes_blink(fb, &lb);
}

/* SMILE2: вертикальные глаза + прямые брови + широкий рот */
static void face_smile2(Framebuffer *fb) {
    fb_clear(fb, COLOR_BLACK);
    mouth_wide_open(fb);
    brows_straight(fb);
    eyes_oval_v(fb);
}

static void face_stub(Framebuffer *fb) { face_idle(fb); }

/* ═══════════════════════════════════════════════════
 * Публичный интерфейс
 * ═══════════════════════════════════════════════════ */
void face_draw(Framebuffer *fb, FaceState state) {
    switch (state) {
        case FACE_IDLE:      face_idle(fb);  break;
        case FACE_HAPPY:     face_stub(fb);  break;
        case FACE_SURPRISED: face_stub(fb);  break;
        case FACE_ANGRY:     face_angry(fb); break;
        case FACE_WTF:       face_stub(fb);  break;
        case FACE_SAD:       face_stub(fb);  break;
        case FACE_TALKING:   face_stub(fb);  break;
        case FACE_STOIC:     face_stoic(fb); break;
        case FACE_CUTE1:     face_cute1(fb); break;
        case FACE_CUTE2:     face_cute2(fb); break;
        case FACE_CUTE3:     face_cute3(fb); break;
        case FACE_SMILE1:    face_smile1(fb); break;
        case FACE_SMILE2:    face_smile2(fb); break;
        default:             face_idle(fb);  break;
    }
}