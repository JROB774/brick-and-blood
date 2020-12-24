#pragma once

// Adapted from some old code so it's probably terrible but will function and be easy to implement:
//    https://github.com/JROB774/misc/blob/master/weekly-jam-59/source/game/pathfind.h

GLOBAL constexpr int PATH_FINDER_DIR_NONE = -9999;

struct PathPoint
{
    int x = PATH_FINDER_DIR_NONE;
    int y = PATH_FINDER_DIR_NONE;

    inline bool operator== (const PathPoint& rhs) const
    {
        return ((x == rhs.x) && (y == rhs.y));
    }
    inline bool operator!= (const PathPoint& rhs) const
    {
        return !(operator==(rhs));
    }
};

struct PathNode
{
    PathPoint direction;
    bool visited = false;
};

struct PathFinder
{
    std::vector<PathPoint> frontier;
    std::map<size_t,PathNode> node_map;

    PathPoint start;
    PathPoint end;
};

INTERNAL std::vector<PathPoint> FindPath (PathPoint start, PathPoint end, bool cardinal = true, bool diagonal = true);
