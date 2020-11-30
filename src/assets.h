#ifndef ASSETS_H
#define ASSETS_H

#if defined(PLATFORM_WIN32)
#if defined(BUILD_DEBUG)
#define ASSET_IMGBIT   "../../../dat/imgbit.bmp"
#define ASSET_IMGPAL   "../../../dat/imgpal.bmp"
#else
#define ASSET_IMGBIT   "dat/imgbit.bmp"
#define ASSET_IMGPAL   "dat/imgpal.bmp"
#endif
#endif

#if defined(PLATFORM_WEB)
#define ASSET_IMGBIT   "dat/imgbit.bmp"
#define ASSET_IMGPAL   "dat/imgpal.bmp"
#endif

#endif /* ASSETS_H */
