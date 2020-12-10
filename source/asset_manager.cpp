#if defined(PLATFORM_WIN32) // =================================================

AssetManager::AssetManager ()
{
    // Get the location of the executable as the assets folder should be defined relative to that.
    char exe_path[MAX_PATH] = {0};
    GetModuleFileNameA(NULL, exe_path, MAX_PATH);
    char* slash = NULL;
    for (char* c=exe_path; *c; ++c) if (*c == '\\' || *c == '/') slash = c;
    if (slash) *slash = '\0'; // Strip the executable's name from the path.

    asset_base_path = exe_path;
    asset_base_path += "/";

    // If there is a file in the current working directory specifying the assets relative path
    // then we append that to the executable path, otherwise we just expect the exe directory.
    std::ifstream file("asset_path.txt", std::ios::in);
    if (file.is_open())
    {
        std::string asset_path;
        std::getline(file, asset_path);
        asset_base_path += asset_path;
    }
    else
    {
        asset_base_path += "assets/";
    }

    // Make sure the asset path always ends in a slash.
    if (asset_base_path.back() != '\\' && asset_base_path.back() != '/')
    {
        asset_base_path += "/";
    }
}

#endif // PLATFORM_WIN32 =======================================================

#if defined(PLATFORM_WEB) // ===================================================

AssetManager::AssetManager ()
{
    asset_base_path = "assets/";
}

#endif // PLATFORM_WEB =========================================================

AssetManager::~AssetManager ()
{
    for (auto& [name,asset]: gAssetManager.assets)
    {
        asset->Free();
        delete asset;
    }
    gAssetManager.assets.clear();
}

//
// ASSET TYPES
//

void AssetImage::Load (std::string file_name)
{
    LoadImage(data, file_name);
}
void AssetImage::Free ()
{
    FreeImage(data);
}

void AssetShader::Load (std::string file_name)
{
    LoadShader(data, file_name);
}
void AssetShader::Free ()
{
    FreeShader(data);
}

void AssetAnimation::Load (std::string file_name)
{
    LoadAnimation(data, file_name);
}
void AssetAnimation::Free ()
{
    FreeAnimation(data);
}

void AssetFont::Load (std::string file_name)
{
    LoadFont(data, file_name);
}
void AssetFont::Free ()
{
    FreeFont(data);
}

void AssetSound::Load (std::string file_name)
{
    LoadSound(data, file_name);
}
void AssetSound::Free ()
{
    FreeSound(data);
}

void AssetMusic::Load (std::string file_name)
{
    LoadMusic(data, file_name);
}
void AssetMusic::Free ()
{
    FreeMusic(data);
}
