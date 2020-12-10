INTERNAL SDL_Color ColorToSDLColor (Vec4 color)
{
    SDL_Color c;
    c.r = (U8)(color.r * 255.0f);
    c.g = (U8)(color.g * 255.0f);
    c.b = (U8)(color.b * 255.0f);
    c.a = (U8)(color.a * 255.0f);
    return c;
}

INTERNAL std::vector<std::string> TokenizeString (std::string str, char delim)
{
    std::stringstream test(str);
    std::string segment;
    std::vector<std::string> seglist;
    while (std::getline(test, segment, delim)) seglist.push_back(segment);
    return seglist;
}

INTERNAL std::string FixPathSlashes (std::string path_name)
{
    std::replace(path_name.begin(), path_name.end(), '\\', '/');
    return path_name;
}

INTERNAL std::string FormatStringV (const char* format, va_list args)
{
    std::string str;
    int size = vsnprintf(NULL, 0, format, args) + 1;
    char* buffer = (char*)malloc(size*sizeof(char));
    if (buffer)
    {
        vsnprintf(buffer, size, format, args);
        str = buffer;
        free(buffer);
    }
    return str;
}

INTERNAL std::string FormatTime (const char* format)
{
    time_t     raw_time = time(NULL);
    struct tm* cur_time = localtime(&raw_time);

    size_t length = 256;
    size_t result = 0;

    // We go until our buffer is big enough.
    char* buffer = NULL;
    do
    {
        if (buffer) free(buffer);
        buffer = (char*)malloc(length*sizeof(char));
        if (!buffer) return std::string();

        result = strftime(buffer, length, format, cur_time);
        length *= 2;
    }
    while (!result);

    std::string time_str(buffer);
    free(buffer);
    return time_str;
}

INTERNAL float Lerp (float a, float b, float t)
{
    return a + t * (b - a);
}

INTERNAL float DegToRad (float deg)
{
    return (deg * (float)M_PI) / 180.0f;
}
INTERNAL float RadToDeg (float rad)
{
    return (rad * 180.0f) / (float)M_PI;
}

INTERNAL Vec2 RotateVec2 (Vec2 vec, float rad)
{
    float x = vec.x * cosf(rad) - vec.y * sinf(rad);
    float y = vec.x * sinf(rad) + vec.y * cosf(rad);
    return { x, y };
}

INTERNAL float SinRange (float min, float max, float t)
{
    float half_range = (max - min) / 2;
    return min + half_range + sinf(t) * half_range;
}

INTERNAL float RoundToMultiple (float num, float multiple)
{
    if (multiple == 0) return num;

    float rem = fmod(abs(num), multiple);
    if (rem == 0) return num;

    if (num < 0) return -(abs(num) - rem);
    else return num + multiple - rem;
}

GLOBAL std::random_device gRandomDevice;
GLOBAL std::mt19937 gRandomGenerator(gRandomDevice());

INTERNAL void RandomSeed (int seed)
{
    if (seed < 0) seed = (int)time(NULL);
    gRandomGenerator.seed(seed);
}

INTERNAL int RandomRange (int min, int max)
{
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(gRandomGenerator);
}

INTERNAL int Random ()
{
    return RandomRange(0, RAND_MAX);
}

INTERNAL float RandomFloatRange (float min, float max)
{
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(gRandomGenerator);
}

INTERNAL float RandomFloat ()
{
    return RandomFloatRange(0, (float)RAND_MAX);
}

#if defined(PLATFORM_WIN32) // =================================================

INTERNAL AlertResult ShowAlert (std::string title, std::string msg, AlertType type, AlertButtons buttons)
{
    // NOTE: We don't allow hidden windows because it causes program hang.
    HWND hwnd = NULL;
    bool hidden = SDL_GetWindowFlags(gWindow.window) & SDL_WINDOW_HIDDEN;

    UINT win32_type = 0;
    UINT win32_buttons = 0;

    switch (type)
    {
    case (ALERT_TYPE_INFO   ): win32_type = MB_ICONINFORMATION; break;
    case (ALERT_TYPE_WARNING): win32_type = MB_ICONWARNING;     break;
    case (ALERT_TYPE_ERROR  ): win32_type = MB_ICONERROR;       break;
    }
    switch (buttons)
    {
    case (ALERT_BUTTONS_YES_NO_CANCEL): win32_buttons = MB_YESNOCANCEL; break;
    case (ALERT_BUTTONS_YES_NO       ): win32_buttons = MB_YESNO;       break;
    case (ALERT_BUTTONS_OK           ): win32_buttons = MB_OK;          break;
    case (ALERT_BUTTONS_OK_CANCEL    ): win32_buttons = MB_OKCANCEL;    break;
    }

    if (!hidden)
    {
        SDL_SysWMinfo win_info = {};
        SDL_VERSION(&win_info.version);
        if (SDL_GetWindowWMInfo(gWindow.window, &win_info))
        {
            hwnd = win_info.info.win.window;;
        }
    }

    int win32_result = MessageBoxA(hwnd, msg.c_str(), title.c_str(), (win32_type|win32_buttons));

    AlertResult result = ALERT_RESULT_INVALID;
    switch (win32_result)
    {
    case (IDCANCEL): result = ALERT_RESULT_CANCEL; break;
    case (IDOK    ): result = ALERT_RESULT_OK;     break;
    case (IDNO    ): result = ALERT_RESULT_NO;     break;
    case (IDYES   ): result = ALERT_RESULT_YES;    break;
    }

    return result;
}

#endif // PLATFORM_WIN32 =======================================================

#if defined(PLATFORM_WEB) // ===================================================

INTERNAL AlertResult ShowAlert (std::string title, std::string msg, AlertType type, AlertButtons buttons)
{
    // This function doesn't do anything when building the application for web...
    return ALERT_RESULT_INVALID;
}

#endif // PLATFORM_WEB =========================================================
