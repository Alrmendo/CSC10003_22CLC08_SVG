#ifndef __MAIN__
#define __MAIN__
#include "shape/shape.h"
#include "reader/read_file.h"
#include <Windowsx.h>

void render_shape(Gdiplus::Graphics &, Entity &);
void render_file(HDC &, Entity &);

LRESULT CALLBACK HandleMessage(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT);

// Global variables for render
string filename = "images/sample.svg";
SvgParser svg_parser(filename);
Entity svg_data;

// Navigation
double zoom_scale = 1;
Gdiplus::PointF scroll_offset(0, 0);
double rotation_angle = 0;
POINT last_mouse_position;
bool is_dragging = false;
int camera_speed = 48;
int rotate_speed = 15;
#endif
