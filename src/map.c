// Simple 2D Perlin noise implementation taken from here: https://gist.github.com/nowl/828013

GLOBAL unsigned int gPerlinSeed = 0;

GLOBAL const U8 PERLIN_HASH[] =
{
    208,34,231,213,32,248,233,56,161,78,24,140,71,48,140,254,245,255,247,247,40,
    185,248,251,245,28,124,204,204,76,36,1,107,28,234,163,202,224,245,128,167,204,
    9,92,217,54,239,174,173,102,193,189,190,121,100,108,167,44,43,77,180,204,8,81,
    70,223,11,38,24,254,210,210,177,32,81,195,243,125,8,169,112,32,97,53,195,13,
    203,9,47,104,125,117,114,124,165,203,181,235,193,206,70,180,174,0,167,181,41,
    164,30,116,127,198,245,146,87,224,149,206,57,4,192,210,65,210,129,240,178,105,
    228,108,245,148,140,40,35,195,38,58,65,207,215,253,65,85,208,76,62,3,237,55,89,
    232,50,217,64,244,157,199,121,252,90,17,212,203,149,152,140,187,234,177,73,174,
    193,100,192,143,97,53,145,135,19,103,13,90,135,151,199,91,239,247,33,39,145,
    101,120,99,3,186,86,99,41,237,203,111,79,220,135,158,42,30,154,120,67,87,167,
    135,176,183,191,253,115,184,21,233,58,129,233,142,39,128,211,118,137,139,255,
    114,20,218,113,154,27,127,246,250,1,8,198,250,209,92,222,173,21,88,102,219
};

INTERNAL int perlin_noise2i (int x, int y)
{
    int y_index = (y + gPerlinSeed) % 256;
    if (y_index < 0) y_index += 256;
    int x_index = (PERLIN_HASH[y_index] + x) % 256;
    if (x_index < 0) x_index += 256;
    return PERLIN_HASH[x_index];
}

INTERNAL float perlin_noise2f (float x, float y)
{
    int x_int = floorf(x);
    int y_int = floorf(y);
    float x_frac = x - x_int;
    float y_frac = y - y_int;
    int s = perlin_noise2i(x_int,  y_int  );
    int t = perlin_noise2i(x_int+1,y_int  );
    int u = perlin_noise2i(x_int,  y_int+1);
    int v = perlin_noise2i(x_int+1,y_int+1);
    float lo = smooth_lerp(s,t,x_frac);
    float hi = smooth_lerp(u,v,x_frac);
    return smooth_lerp(lo,hi, y_frac);
}

INTERNAL float perlin_noise (float x, float y)
{
    float amp = 1.0f;
    float div = 256 * amp;
    float fin = perlin_noise2f(x,y) * amp;
    return (fin/div);
}

INTERNAL void seed_perlin_noise ()
{
    gPerlinSeed = CAST(unsigned int, time(NULL)*random_int_range(0,100));
}

// MAP

INTERNAL void init_map ()
{
    seed_perlin_noise();
    for (int iy=0; iy<MAP_TILE_H; ++iy)
    {
        for (int ix=0; ix<MAP_TILE_W; ++ix)
        {
            float x = CAST(float,ix) / 64;
            float y = CAST(float,iy) / 64;
            float a = 0.500f * perlin_noise( 4*x, 4*y);
            float b = 0.250f * perlin_noise( 8*x, 8*y);
            float c = 0.125f * perlin_noise(16*x,16*y);
            float height = a+b+c;
            // float nx = CAST(float,ix)/(MAP_TILE_W) - 0.5f;
            // float ny = CAST(float,iy)/(MAP_TILE_H) - 0.5f;
            // float d = sqrt(nx*nx + ny*ny) / sqrt(0.5f);
            // height = (1.0f + height - d) / 2.0f;
            gMap.tile[iy*MAP_TILE_W+ix].height = height;
        }
    }
}

INTERNAL void quit_map ()
{
    // Nothing...
}

INTERNAL void render_map (int x, int y)
{
    int paloffset = 0;
    int palindex = 0;
    int palette[] =
    {
        PAL_DEEPWATER0,PAL_DEEPWATER1,
        PAL_WATER0,PAL_WATER1,
        PAL_SAND0,PAL_SAND1,
        PAL_DIRT0,PAL_DIRT1,
        PAL_GRASS0,PAL_GRASS1
    };
    for (int iy=0; iy<MAP_TILE_H; ++iy)
    {
        for (int ix=0; ix<MAP_TILE_W; ++ix)
        {
            if      (gMap.tile[iy*MAP_TILE_W+ix].height > 0.40f) paloffset = 8;
            else if (gMap.tile[iy*MAP_TILE_W+ix].height > 0.35f) paloffset = 6;
            else if (gMap.tile[iy*MAP_TILE_W+ix].height > 0.32f) paloffset = 4;
            else if (gMap.tile[iy*MAP_TILE_W+ix].height > 0.28f) paloffset = 2;
            else                                                 paloffset = 0;

            int sx = ((ix-iy)*((TILE_W  )/2)) + (SCREEN_W/2) - (TILE_W/2);
            int sy = ((ix+iy)*((TILE_H-1)/2)) + (SCREEN_H/2) - ((MAP_TILE_H*TILE_H)/4);
            render_bitmap(sx,sy, ((palindex%2 == 0) ? palette[paloffset+0] : palette[paloffset+1]), &SPR_TILE);

            // palindex++;
        }
        // SWAP(palette[0],palette[1], int);
        // SWAP(palette[2],palette[3], int);
        // SWAP(palette[4],palette[5], int);
        // SWAP(palette[6],palette[7], int);
        // SWAP(palette[8],palette[9], int);
    }
}

