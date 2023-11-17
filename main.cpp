#include "main.h"

VOID render_file(HDC &hdc, const vector<Entity> &main_data)
{
    Gdiplus::Graphics graphics(hdc);
    graphics.ScaleTransform(zoom_scale, zoom_scale);
    graphics.TranslateTransform(scroll_offset.X + rotate_offset.X, scroll_offset.Y + rotate_offset.Y);
    graphics.RotateTransform(rotation_angle);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

    for (const auto &entity : main_data)
    {
        if (entity.entity_type == "rect")
        {
            Shapes::Rectangle rect(entity);
            rect.render_rect(graphics);
        }
        else if (entity.entity_type == "text")
        {
            Shapes::Text text(entity);
            text.render_text(graphics);
        }
        else if (entity.entity_type == "circle")
        {
            Shapes::Circle circle(entity);
            circle.render_circle(graphics);
        }
        else if (entity.entity_type == "polyline")
        {
            Shapes::Polyline polyline(entity);
            polyline.render_polyline(graphics);
        }
        else if (entity.entity_type == "ellipse")
        {
            Shapes::Ellipse elip(entity);
            elip.render_ellipse(graphics);
        }
        else if (entity.entity_type == "line")
        {
            Shapes::Line line(entity);
            line.render_line(graphics);
        }
        else if (entity.entity_type == "polygon")
        {
            Shapes::Polygon polygon(entity);
            polygon.render_polygon(graphics);
        }
    }
}

LRESULT CALLBACK HandleMessage(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
    HDC hdc;
    PAINTSTRUCT ps;

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
            zoom_scale *= 1.1;
        else
            zoom_scale *= 0.9;
        InvalidateRect(window, NULL, TRUE);
        return 0;
    }
    case WM_KEYDOWN:
        if (w_param == 'A' || w_param == 'a')
        {
            scroll_offset.X += 24 / zoom_scale;
            InvalidateRect(window, NULL, TRUE);
        }
        else if (w_param == 'D' || w_param == 'd')
        {
            scroll_offset.X -= 24 / zoom_scale;
            InvalidateRect(window, NULL, TRUE);
        }
        else if (w_param == 'W' || w_param == 'w')
        {
            scroll_offset.Y += 24 / zoom_scale;
            InvalidateRect(window, NULL, TRUE);
        }
        else if (w_param == 'S' || w_param == 's')
        {
            scroll_offset.Y -= 24 / zoom_scale;
            InvalidateRect(window, NULL, TRUE);
        }
        else if (w_param == 'R' || w_param == 'r')
        {
            scroll_offset.X = 0;
            scroll_offset.Y = 0;
            zoom_scale = 1;
            rotation_angle = 0;
            rotate_offset.X = 0;
            rotate_offset.Y = 0;
            InvalidateRect(window, NULL, TRUE);
        }

        else if (w_param == 'Q' || w_param == 'q' || w_param == 'E' || w_param == 'e')
        {
            RECT window_rect;
            GetClientRect(window, &window_rect);
            Gdiplus::PointF window_size(static_cast<float>(window_rect.right), static_cast<float>(window_rect.bottom));

            if (w_param == 'E' || w_param == 'e')
                rotation_count += 1;
            else if (w_param == 'Q' || w_param == 'q')
                rotation_count -= 1;

            if (rotation_count >= max_rotations)
                rotation_count = 0;
            if (rotation_count < 0)
                rotation_count = max_rotations - 1;

            if (rotation_count == 0)
            {
                rotation_angle = 0;
                rotate_offset.X = 0;
                rotate_offset.Y = 0;
            }
            else if (rotation_count == 1)
            {
                rotation_angle = 90;
                rotate_offset.X = window_size.X;
                rotate_offset.Y = 0;
            }
            else if (rotation_count == 2)
            {
                rotation_angle = 180;
                rotate_offset.X = window_size.X;
                rotate_offset.Y = window_size.Y;
            }
            else if (rotation_count == 3)
            {
                rotation_angle = 270;
                rotate_offset.X = 0;
                rotate_offset.Y = window_size.Y;
            }
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
    window_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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
