#ifndef __MAIN__
#define __MAIN__
#include "shape/shape.h"
#include "reader/read_file.h"

double zoom_scale = 1;
double rotation_angle = 0;
int max_rotations = 4;
int rotation_count = 0;
Gdiplus::PointF scroll_offset(0, 0);
Gdiplus::PointF rotate_offset(0, 0);
string file_name = "images/sample.svg";
vector<Entity> main_data;

VOID render_file(HDC &, const vector<Entity> &);
LRESULT CALLBACK HandleMessage(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, INT);
#endif