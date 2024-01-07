#ifndef __MAIN__
#define __MAIN__
#include "shape/shape.h"
#include "reader/read_file.h"
#include <Windowsx.h>

void render_shape(Gdiplus::Graphics &, Entity &, PAINTSTRUCT &);
void render_file(HDC &, Entity &, PAINTSTRUCT &);

LRESULT CALLBACK HandleMessage(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT);

// Navigation
double zoom_scale = 1;
Gdiplus::PointF scroll_offset(0, 0);
int rotation_angle = 0;
POINT last_mouse_position;
bool is_dragging = false;
int camera_speed = 48;
int rotate_speed = 15;

// Window information
int window_width = 960;
int window_height = 540;
int open_width = 320;
int open_height = 90;
#endif