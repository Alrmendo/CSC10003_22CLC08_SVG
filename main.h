#ifndef __MAIN__
#define __MAIN__
#include "shape/shape.h"
#include "reader/read_file.h"

VOID render_file(HDC &, const vector<Entity> &);
LRESULT CALLBACK HandleMessage(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, INT);
#endif