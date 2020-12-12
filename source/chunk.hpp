#pragma once

struct Chunk
{

};

// SHOULD NOT BE USED DIRECTLY -- USE THE ASSET MANAGER INSTEAD!!!
INTERNAL void LoadChunk (Chunk& chunk, std::string file_name);
INTERNAL void FreeChunk (Chunk& chunk);
