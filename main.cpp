#include "main.h"
double zoom_scale = 1;
Gdiplus::PointF pan_offset(0, 0);

VOID render_file(HDC &hdc, const vector<Entity> &main_data)
{
    Gdiplus::Graphics graphics(hdc);
    graphics.Clear(Gdiplus::Color(255, 255, 255, 255));
    graphics.ScaleTransform(zoom_scale, zoom_scale);
    graphics.TranslateTransform(pan_offset.X, pan_offset.Y);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

    for (const auto &entity : main_data)
    {
        if (entity.entity_type == "rect")
        {
            Rect rect(entity);
            rect.render_rect(graphics);
        }
        else if (entity.entity_type == "text")
        {
            Text text(entity);
            text.render_text(graphics);
        }
        else if (entity.entity_type == "circle")
        {
            Circle circle(entity);
            circle.render_circle(graphics);
        }
        else if (entity.entity_type == "polyline")
        {
            Plyline polyline(entity);
            polyline.render_polyline(graphics);
        }
        else if (entity.entity_type == "ellipse")
        {
            Elip elip(entity);
            elip.render_ellipse(graphics);
        }
        else if (entity.entity_type == "line")
        {
            Line line(entity);
            line.render_line(graphics);
        }
        else if (entity.entity_type == "polygon")
        {
            Plygon polygon(entity);
            polygon.render_polygon(graphics);
        }
    }
}

LRESULT CALLBACK HandleMessage(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
    HDC hdc;
    PAINTSTRUCT ps;
    static string file_name = "images/sample.svg";
    static vector<Entity> main_data;

    switch (message)
    {
    case WM_CREATE:
    {
        ReadAndParse file(file_name);
        main_data = file.get_data();
        return 0;
    }
    case WM_PAINT:
    {
        hdc = BeginPaint(window, &ps);
        render_file(hdc, main_data);
        EndPaint(window, &ps);
        return 0;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }

    case WM_MOUSEWHEEL:
    {
        if (GET_WHEEL_DELTA_WPARAM(w_param) > 0)
            zoom_scale += 0.2;
        else
            zoom_scale -= 0.2;
        InvalidateRect(window, NULL, TRUE);
        return 0;
    }
    case WM_KEYDOWN:
        if (w_param == 'A' || w_param == 'a')
        {
            pan_offset.X += 20;
            InvalidateRect(window, NULL, TRUE);
        }
        else if (w_param == 'D' || w_param == 'd')
        {
            pan_offset.X -= 20;
            InvalidateRect(window, NULL, TRUE);
        }
        else if (w_param == 'W' || w_param == 'w')
        {
            pan_offset.Y += 20;
            InvalidateRect(window, NULL, TRUE);
        }
        else if (w_param == 'S' || w_param == 's')
        {
            pan_offset.Y -= 20;
            InvalidateRect(window, NULL, TRUE);
        }

        return 0;

    default:
        return DefWindowProc(window, message, w_param, l_param);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    HWND window;
    MSG message;
    WNDCLASS window_class;
    Gdiplus::GdiplusStartupInput gdiplus_startup_input;
    ULONG_PTR gdiplus_token;

    Gdiplus::GdiplusStartup(&gdiplus_token, &gdiplus_startup_input, nullptr);
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpfnWndProc = HandleMessage;
    window_class.cbClsExtra = 0;
    window_class.cbWndExtra = 0;
    window_class.hInstance = hInstance;
    window_class.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
    window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    window_class.lpszMenuName = nullptr;
    window_class.lpszClassName = TEXT("GettingStarted");

    RegisterClass(&window_class);

    window = CreateWindow(
        TEXT("GettingStarted"), // window class name
        TEXT("SVG Demo"),       // window caption
        WS_OVERLAPPEDWINDOW,    // window style
        CW_USEDEFAULT,          // initial x position
        CW_USEDEFAULT,          // initial y position
        CW_USEDEFAULT,          // initial x size
        CW_USEDEFAULT,          // initial y size
        nullptr,                // parent window handle
        nullptr,                // window menu handle
        hInstance,              // program instance handle
        nullptr);               // creation parameters

    ShowWindow(window, iCmdShow);
    UpdateWindow(window);

    while (GetMessage(&message, nullptr, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    Gdiplus::GdiplusShutdown(gdiplus_token);
    return message.wParam;
}