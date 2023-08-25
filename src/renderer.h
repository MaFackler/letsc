#ifndef RENDERER_H
#define RENDERER_H
#include <vector.h>

typedef enum RenderCommandType {
    RENDER_COMMAND_RECT,
    RENDER_COMMAND_TEXT,
} RenderCommandType;


typedef struct RenderCommand {
    RenderCommandType type;
    int layer;
    int color;
    union {
        struct {
            float x;
            float y;
            float w;
            float h;
        } cmd_rect;
        struct {
            float x;
            float y;
            char *text;
            size_t n;
        } cmd_text;
    };
} RenderCommand;

#define RENDERER_NUM_LAYERS 2

typedef struct Renderer {
    unsigned int color;
    unsigned int layer;
    unsigned int width;
    unsigned int height;
    RenderCommand* layer_commands[RENDERER_NUM_LAYERS];
} Renderer;

void renderer_init(Renderer *r, int width, int height) {
    for (int i = 0; i < RENDERER_NUM_LAYERS; ++i) {
        r->layer_commands[RENDERER_NUM_LAYERS] = NULL;
    }
    r->width = width;
    r->height = height;
}

void renderer_set_color(Renderer *r, unsigned int color) {
    r->color = color;
}

void renderer_set_layer(Renderer *r, unsigned int layer) {
    r->layer = layer;
}

unsigned int renderer_get_width(Renderer *r) {
    return r->width;
}

unsigned int renderer_get_height(Renderer *r) {
    return r->height;
}

void renderer_push_rect(Renderer *r, float x, float y, float w, float h) {
    RenderCommand cmd = {
        .type = RENDER_COMMAND_RECT,
        .layer = r->layer, 
        .color = r->color,
        .cmd_rect = {
            .x = x,
            .y = y,
            .w = w,
            .h = h
        }
    };
    vec_push(r->layer_commands[cmd.layer], cmd);
}

void renderer_push_rect4f(Renderer *r, float rect[4]) {
    renderer_push_rect(r, rect[0], rect[1], rect[2], rect[3]);
}

void renderer_push_text(Renderer *r, float x, float y, char *text, size_t n) {
    RenderCommand cmd = {
        .type = RENDER_COMMAND_TEXT,
        .layer = r->layer, 
        .color = r->color,
        .cmd_text = {
            .x = x,
            .y = y,
            .text = text,
            .n = n
        }
    };
    vec_push(r->layer_commands[cmd.layer], cmd);
}

void renderer_end_frame(Renderer *r) {
    for (int i = 0; i < RENDERER_NUM_LAYERS; ++i) {
        vec_clear(r->layer_commands[i]);
    }
}

#endif // RENDERER_H
