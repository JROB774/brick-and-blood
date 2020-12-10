#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

//
// ASSET MANAGER
//

// Base type for assets, all custom asset types should inherit from this!
struct AssetBase
{
             AssetBase () = default;
    virtual ~AssetBase () = default;
    virtual void Load (std::string file_name) = 0;
    virtual void Free () = 0;
};

GLOBAL struct AssetManager
{
    AssetManager ();
   ~AssetManager ();
    std::map<std::string,AssetBase*> assets;
    std::string asset_base_path;

} gAssetManager;

//
// INTERFACE
//

template<typename T>
INTERNAL auto* GetAsset (std::string name)
{
    std::string file_name = name + T::Ext;
    std::string full_file_name = gAssetManager.asset_base_path + T::Path + file_name;
    T* asset = dynamic_cast<T*>(gAssetManager.assets[file_name]); // Will create the asset if it doesn't exist.
    if (!asset)
    {
        // Allocate and load the asset if it hasn't been loaded.
        asset = new T;
        asset->Load(full_file_name);
        gAssetManager.assets[file_name] = asset;
    }
    return &asset->data;
}

//
// ASSET TYPES
//

struct AssetImage: public AssetBase
{
    static inline const std::string Path = "images/";
    static inline const std::string Ext = ".png";
    void Load (std::string file_name) override;
    void Free () override;
    Image data;
};

struct AssetData: public AssetBase
{
    static inline const std::string Path = "data/";
    static inline const std::string Ext = ".data";
    void Load (std::string file_name) override;
    void Free () override;
    GonObject data;
};

struct AssetAnimation: public AssetBase
{
    static inline const std::string Path = "anims/";
    static inline const std::string Ext = ".anim";
    void Load (std::string file_name) override;
    void Free () override;
    Animation data;
};

struct AssetFont: public AssetBase
{
    static inline const std::string Path = "fonts/";
    static inline const std::string Ext = ".font";
    void Load (std::string file_name) override;
    void Free () override;
    Font data;
};

struct AssetSound: public AssetBase
{
    static inline const std::string Path = "sounds/";
    static inline const std::string Ext = ".wav";
    void Load (std::string file_name) override;
    void Free () override;
    Sound data;
};

struct AssetMusic: public AssetBase
{
    static inline const std::string Path = "music/";
    static inline const std::string Ext = ".ogg";
    void Load (std::string file_name) override;
    void Free () override;
    Music data;
};

#endif /* ASSET_MANAGER_HPP */
