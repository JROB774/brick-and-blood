// Adapted from some old code so it's probably terrible but will function and be easy to implement:
//    https://github.com/JROB774/misc/blob/master/weekly-jam-59/source/game/pathfind.cpp

INTERNAL void CreatePathFinder (PathFinder& path_finder, PathPoint start, PathPoint end)
{
    // Clear any potentially old data that is present.
    path_finder.frontier.clear();
    path_finder.node_map.clear();
    // Set the start and end points of the path to be found.
    path_finder.start = start;
    path_finder.end = end;
    // Adds the starting position as the beginning of the frontier.
    path_finder.frontier.push_back(start);
    // The starting point should be checked as already visited.
    size_t index = start.y*WORLD_W_IN_TILES+start.x;
    path_finder.node_map[index].visited = true;
}

INTERNAL void CheckPathFrontierNeighbor (PathFinder& path_finder, PathPoint current, int xoff, int yoff)
{
    size_t index = (current.y+yoff)*WORLD_W_IN_TILES+(current.x+xoff);
    if (!path_finder.node_map[index].visited) // If we haven't already visited this node.
    {
        PathPoint dir = { PATH_FINDER_DIR_NONE, PATH_FINDER_DIR_NONE };
        // Make sure we do not bother checking and adding solid tiles to the node mpa.
        Tile* tile = MapGetTileAtPos(current.x+xoff,current.y+yoff);
        Entity* entity = MapGetEntityAtPos(current.x+xoff,current.y+yoff);
        if ((!entity || entity->type == "player") && (!tile || !tile->solid))
        {
            path_finder.frontier.push_back({ current.x+xoff, current.y+yoff });
            dir = { current.x, current.y };
        }
        path_finder.node_map[index].direction = dir;
        path_finder.node_map[index].visited = true;
    }
}

INTERNAL std::vector<PathPoint> FindPath (PathPoint start, PathPoint end, bool cardinal, bool diagonal)
{
    // Creates a new path-finder to create a path from start to end.
    PathFinder path_finder;
    CreatePathFinder(path_finder, start, end);

    std::vector<PathPoint> path;

    // Construct a node map until completion or the end point is found.
    while (path_finder.frontier.size() > 0)
    {
        // Pops the elment from the front of the frontier "queue".
        PathPoint current = path_finder.frontier[0];
        path_finder.frontier.erase(path_finder.frontier.begin());

        // If we go out of the path finding bounds we just give up.
        if (current.x < (start.x-20) || current.x > (start.x+20) ||
            current.y < (start.y-20) || current.y > (start.y+20))
        {
            return std::move(path);
        }

        // Expand the frontier in the desired directions to search.
        if (cardinal)
        {
            CheckPathFrontierNeighbor(path_finder, current,  0,-1);
            CheckPathFrontierNeighbor(path_finder, current,  1, 0);
            CheckPathFrontierNeighbor(path_finder, current,  0, 1);
            CheckPathFrontierNeighbor(path_finder, current, -1, 0);
        }
        if (diagonal)
        {
            CheckPathFrontierNeighbor(path_finder, current,  1,-1);
            CheckPathFrontierNeighbor(path_finder, current,  1, 1);
            CheckPathFrontierNeighbor(path_finder, current, -1, 1);
            CheckPathFrontierNeighbor(path_finder, current, -1,-1);
        }

        // Break early if we found our destination.
        if (current == end) break;
    }

    // Now we can construct a path backwards from the goal to the start.
    PathPoint current = end;
    while (current != start)
    {
        path.push_back(current);
        size_t index = current.y*WORLD_W_IN_TILES+current.x;
        current = path_finder.node_map[index].direction;
    }

    return std::move(path); // Should be able to move this for speed?
}
