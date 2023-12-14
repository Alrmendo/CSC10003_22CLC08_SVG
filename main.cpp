#include "main.h"

void render_shape(Gdiplus::Graphics &graphics, Entity &entity, PAINTSTRUCT &ps)
{
    if (entity.type == "svg" || entity.type == "g")
    {
        if (entity.type == "svg" && entity.attributes.find("viewbox") != entity.attributes.end())
        {
            istringstream stream(entity.attributes["viewbox"]);
            float min_x, min_y, width, height;
            stream >> min_x >> min_y >> width >> height;
            graphics.ScaleTransform(static_cast<float>(ps.rcPaint.bottom) / height, static_cast<float>(ps.rcPaint.bottom) / height);
            graphics.TranslateTransform(-min_x, -min_y);
        }
        for (auto &child : entity.children)
        {
            for (const auto &attr : entity.attributes)
            {
                if (child.attributes.find(attr.first) == child.attributes.end())
                    child.attributes[attr.first] = attr.second;
                else
                {
                    if (attr.first == "transform")
                        child.attributes["transform"] = attr.second + " " + child.attributes["transform"];
                }
            }
        }
    }
    else if (entity.type == "rect")
    {
        Shapes::Rectangle rectangle(entity);
        rectangle.render(graphics);
    }
    else if (entity.type == "text")
    {
        Shapes::Text text(entity);
        text.render(graphics);
    }
    else if (entity.type == "circle")
    {
        Shapes::Circle circle(entity);
        circle.render(graphics);
    }

    else if (entity.type == "ellipse")
    {
        Shapes::Ellipse ellipse(entity);
        ellipse.render(graphics);
    }
    else if (entity.type == "line")
    {
        Shapes::Line line(entity);
        line.render(graphics);
    }
    else if (entity.type == "polyline")
    {
        Shapes::Polyline polyline(entity);
        polyline.render(graphics);
    }
    else if (entity.type == "polygon")
    {
        Shapes::Polygon polygon(entity);
        polygon.render(graphics);
    }
    else if (entity.type == "path")
    {
        Shapes::Path path(entity);
        path.render(graphics);
    }
}

void render_file(HDC &hdc, Entity &svg_root, PAINTSTRUCT &ps)
{
    Gdiplus::Graphics graphics(hdc);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);

    graphics.ScaleTransform(zoom_scale, zoom_scale);
    graphics.TranslateTransform(scroll_offset.X, scroll_offset.Y);
    graphics.RotateTransform(rotation_angle);

    vector<Entity> stack;
    stack.push_back(svg_root);

    while (!stack.empty())
    {
        Entity entity = stack.back();
        stack.pop_back();
        render_shape(graphics, entity, ps);
        for (auto iter = entity.children.rbegin(); iter != entity.children.rend(); ++iter)
            stack.push_back(*iter);
    }
}

LRESULT CALLBACK HandleMessage(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
    HDC hdc;
    PAINTSTRUCT ps;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(window, &ps);
        render_file(hdc, svg_data, ps);
        EndPaint(window, &ps);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_MOUSEWHEEL:
        if (GET_WHEEL_DELTA_WPARAM(w_param) > 0)
            zoom_scale *= 1.1;
        else
            zoom_scale *= 0.9;
        InvalidateRect(window, NULL, TRUE);
        return 0;

    case WM_KEYDOWN:
        if (w_param == 'A' || w_param == 'a')
        {
            scroll_offset.X += camera_speed / zoom_scale;
            InvalidateRect(window, NULL, TRUE);
        }
        else if (w_param == 'D' || w_param == 'd')
        {
            scroll_offset.X -= camera_speed / zoom_scale;
            InvalidateRect(window, NULL, TRUE);
        }
        else if (w_param == 'W' || w_param == 'w')
        {
            scroll_offset.Y += camera_speed / zoom_scale;
            InvalidateRect(window, NULL, TRUE);
        }
        else if (w_param == 'S' || w_param == 's')
        {
            scroll_offset.Y -= camera_speed / zoom_scale;
            InvalidateRect(window, NULL, TRUE);
        }
        else if (w_param == 'R' || w_param == 'r')
        {
            scroll_offset.X = 0;
            scroll_offset.Y = 0;
            zoom_scale = 1;
            rotation_angle = 0;
            InvalidateRect(window, NULL, TRUE);
        }
        else if (w_param == 'Q' || w_param == 'q')
        {
            rotation_angle = (rotation_angle - rotate_speed) % 360;
            InvalidateRect(window, NULL, TRUE);
        }
        else if (w_param == 'E' || w_param == 'e')
        {
            rotation_angle = (rotation_angle + rotate_speed) % 360;
            InvalidateRect(window, NULL, TRUE);
        }
        return 0;

    case WM_LBUTTONDOWN:
        last_mouse_position.x = GET_X_LPARAM(l_param);
        last_mouse_position.y = GET_Y_LPARAM(l_param);
        is_dragging = true;
        SetCapture(window);
        return 0;

    case WM_MOUSEMOVE:
        if (is_dragging)
        {
            int delta_X = GET_X_LPARAM(l_param) - last_mouse_position.x;
            int delta_Y = GET_Y_LPARAM(l_param) - last_mouse_position.y;

            scroll_offset.X += delta_X / zoom_scale;
            scroll_offset.Y += delta_Y / zoom_scale;

            last_mouse_position.x = GET_X_LPARAM(l_param);
            last_mouse_position.y = GET_Y_LPARAM(l_param);

            InvalidateRect(window, NULL, TRUE);
        }
        return 0;

    case WM_LBUTTONUP:
        is_dragging = false;
        ReleaseCapture();
        return 0;

    default:
        return DefWindowProc(window, message, w_param, l_param);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, INT iCmdShow)
{
    int argc;
    LPWSTR *argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    wstring wide_filename;
    if (argc > 1)
    {
        wide_filename = argv[1];
        filename = string(wide_filename.begin(), wide_filename.end());
    }
    LocalFree(argv);
    svg_parser = SvgParser(filename);
    svg_parser.parse_file();
    svg_data = svg_parser.get_data();

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

    // Register the window class
    RegisterClass(&window_class);

    // Load the icon from file
    HICON hIcon = static_cast<HICON>(LoadImage(NULL, TEXT("misc/dreamnail.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED));

    if (hIcon)
    {
        // Create the window with the loaded icon
        window = CreateWindow(
            TEXT("GettingStarted"), // window class name
            TEXT("Sealed Vessel"),  // window caption
            WS_TILEDWINDOW | WS_BORDER, // window style
            open_width,                 // initial x position
            open_height,                // initial y position
            window_width,               // initial x size
            window_height,              // initial y size
            nullptr,                // parent window handle
            nullptr,                // window menu handle
            hInstance,              // program instance handle
            nullptr);               // creation parameters
    }

    if (window)
    {
        // Set the loaded icon as the application icon
        SendMessage(window, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        SendMessage(window, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        // Show and update the window
        ShowWindow(window, SW_SHOWMAXIMIZED);
        UpdateWindow(window);
    }
    
    while (GetMessage(&message, nullptr, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    Gdiplus::GdiplusShutdown(gdiplus_token);
    return message.wParam;
}
