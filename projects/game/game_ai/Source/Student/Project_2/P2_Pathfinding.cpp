#include <pch.h>
#include "Projects/ProjectTwo.h"
#include "P2_Pathfinding.h"

#pragma region Extra Credit 
bool ProjectTwo::implemented_floyd_warshall()
{
    return false;
}

bool ProjectTwo::implemented_goal_bounding()
{
    return false;
}
#pragma endregion

// Open list container ---------------------------------

AStarPather::OpenList::OpenList()
{
    buckets.resize(huristic_MAX * bucketIntervalReverse);
}

AStarPather::AStarNode* AStarPather::OpenList::pop()
{
    AStarNode* result = nullptr;
    while (!result && fastestBucketIdx < buckets.size())
    {
        result = buckets[fastestBucketIdx].pop();
        if (!result)
        {
            ++fastestBucketIdx;
        }
    }

    return result;
}

void AStarPather::OpenList::push(AStarNode* node)
{
    int bucketIdx = static_cast<int>(node->finalCost * bucketIntervalReverse);
    if (bucketIdx >= huristic_MAX)
    {
        bucketIdx = huristic_MAX - 1;
    }

    if (bucketIdx < fastestBucketIdx)
    {
        fastestBucketIdx = bucketIdx;
    }

    buckets[bucketIdx].push(node);
}

void AStarPather::OpenList::update(AStarNode* node, float newgivenCost, float newfinalCost)
{
    int bucketIdx = static_cast<int>(node->finalCost * bucketIntervalReverse);
    if (bucketIdx >= huristic_MAX)
    {
        bucketIdx = huristic_MAX - 1;
    }

    buckets[bucketIdx].erase(node);

    node->givenCost = newgivenCost;
    node->finalCost = newfinalCost;
    push(node);
}

void AStarPather::OpenList::clear()
{
    fastestBucketIdx = std::numeric_limits<int>::max();
    int size = buckets.size();
    for (int i = 0; i < size; ++i)
    {
        buckets[i].clear();
    }
}


// Bucket Codes----------------------------------------------------

AStarPather::Bucket::Bucket() : currentSize(initialSize), sizeAddVal(10), lastIdx(-1)
{
    nodes.reserve(currentSize * 2);
    nodes.resize(currentSize);
}

AStarPather::AStarNode* AStarPather::Bucket::pop()
{
    if (lastIdx == -1)
        return nullptr;

    AStarNode* minNode = nodes[lastIdx];
    int minIdx = lastIdx;
    for (int i = lastIdx; i > -1; --i)
    {
        if (*nodes[i] < *minNode)
        {
            minNode = nodes[i];
            minIdx = i;
        }
    }
    nodes[minIdx] = nodes[lastIdx];
    --lastIdx;

    return minNode;
}

void AStarPather::Bucket::push(AStarNode* node)
{

    if (++lastIdx >= currentSize)
    {
        currentSize += sizeAddVal;
        nodes.resize(currentSize);
    }

    nodes[lastIdx] = node;

    return;
}

void AStarPather::Bucket::erase(AStarNode* node)
{
    for (int i = 0; i <= lastIdx; ++i)
    {
        if (nodes[i] == node)
        {
            nodes[i] = nodes[lastIdx];
            --lastIdx;
        }
    }
}

void AStarPather::Bucket::update()
{
    // Does Nothing
}

void AStarPather::Bucket::clear()
{
    lastIdx = -1;
}


// Main Codes ------------------------------------------------------

bool AStarPather::initialize()
{
    // handle any one-time setup requirements you have

    resetNodeMap();
    initNodeMap();

    Callback cb = std::bind(&AStarPather::updateNodeMap, this);
    Messenger::listen_for_message(Messages::MAP_CHANGE, cb);

    return true; // return false if any errors actually occur, to stop engine initialization
}

void AStarPather::shutdown()
{
}

PathResult AStarPather::compute_path(PathRequest &request)
{

    // WRITE YOUR CODE HERE
    GridPos start = terrain->get_grid_position(request.start);
    GridPos goal = terrain->get_grid_position(request.goal);

    if (request.newRequest)
    {
        resetNodeMap();
        openLists.clear();
        AStarNode* startNode = &(nodeMap[start.row][start.col]);
        startNode->parent = nullptr;
        startNode->givenCost = 0.f;
        startNode->finalCost = calcHeuristic(start, goal, request.settings.heuristic) * request.settings.weight;
        openLists.push(startNode);
    }

    AStarNode* parentNode = nullptr;

    while (parentNode = openLists.pop())
    {
        parentNode->onList = AStarNode::ListStatus::closed;
        if (request.settings.debugColoring)
        {
            terrain->set_color(parentNode->gridPos, Colors::Yellow);
        }

        if (parentNode->gridPos == goal) // When path is found
        {
            if (request.settings.rubberBanding)
            {
                rubberbandPath(parentNode);
            }

            while (parentNode)
            {
                request.path.push_back(terrain->get_world_position(parentNode->gridPos));
                parentNode = parentNode->parent;
            }

            std::reverse(request.path.begin(), request.path.end());
            
            if (request.settings.smoothing)
            {
                smoothPath(request.path);
            }

            return PathResult::COMPLETE;
        }

        
        for (int i = 0; i < neighborNum; ++i)
        {
            if (neighborMasks[i] & parentNode->neighbors) // if neighbor is not null
            {
                AStarNode* childNode = nullptr;
                GridPos childPos{ parentNode->gridPos.row + neighborPosDelta[i].row, parentNode->gridPos.col + neighborPosDelta[i].col };
                
                float costAdd = moveCost[i];
                float givenCost = parentNode->givenCost + costAdd;
                float finalCost = givenCost + calcHeuristic(childPos, goal, request.settings.heuristic) * request.settings.weight;

                childNode = &nodeMap[childPos.row][childPos.col];

                if (childNode->onList == AStarNode::ListStatus::free)
                {
                    childNode->onList = AStarNode::ListStatus::open;
                    childNode->givenCost = givenCost;
                    childNode->finalCost = finalCost;
                    childNode->parent = parentNode;

                    if (request.settings.debugColoring)
                    {
                        terrain->set_color(childNode->gridPos, Colors::Blue);
                    }
                    

                    openLists.push(childNode);
                }
                else if (finalCost < childNode->finalCost)
                {
                    childNode->onList = AStarNode::ListStatus::open;
                    childNode->parent = parentNode;
                    
                    if (request.settings.debugColoring)
                    {
                        terrain->set_color(childNode->gridPos, Colors::Blue);
                    }

                    openLists.update(childNode, givenCost, finalCost);
                }
            }
        }

        if (request.settings.singleStep)
        {
            return PathResult::PROCESSING;
        }
    }

    return PathResult::IMPOSSIBLE;
}

void AStarPather::initNodeMap()
{
    for (int i = 0; i < MapY_Max; ++i)
    {
        for (int j = 0; j < MapX_Max; ++j)
        {
            nodeMap[i][j].gridPos.row = i;
            nodeMap[i][j].gridPos.col = j;
        }
    }
}

void AStarPather::resetNodeMap()
{
    for (int i = 0; i < MapY_Max; ++i)
    {
        for (int j = 0; j < MapX_Max; ++j)
        {
            nodeMap[i][j].onList = AStarNode::ListStatus::free;
        }
    }
}

void AStarPather::updateNodeMap()
{
    for (int i = 0; i < MapY_Max; ++i)
    {
        for (int j = 0; j < MapX_Max; ++j)
        {
            byte neighborBitmap = 0;

            if (!terrain->is_valid_grid_position(i, j)) 
            {
                continue;
            }

            //Wall Detecting
            for (int k = 0; k < neighborNum; ++k)
            {
                GridPos neighborPos{ i + neighborPosDelta[k].row , j + neighborPosDelta[k].col };
                
                if (terrain->is_valid_grid_position(neighborPos) && !terrain->is_wall(neighborPos))
                {
                    neighborBitmap |= neighborMasks[k];
                }
            }

            /*
            567
            3+4
            012
            */

            //Neighbor PostProcess: Delete diagonal moving
            if (!(neighborBitmap & neighborMasks[1]))
                neighborBitmap &= ~(neighborMasks[0] | neighborMasks[2]);
            if (!(neighborBitmap & neighborMasks[3]))
                neighborBitmap &= ~(neighborMasks[0] | neighborMasks[5]);
            if (!(neighborBitmap & neighborMasks[4]))
                neighborBitmap &= ~(neighborMasks[2] | neighborMasks[7]);
            if (!(neighborBitmap & neighborMasks[6]))
                neighborBitmap &= ~(neighborMasks[5] | neighborMasks[7]);

            nodeMap[i][j].neighbors = neighborBitmap;
        }
    }
}

float AStarPather::calcHeuristic(const GridPos& currentPos, const GridPos& endPos, const Heuristic& mode)
{
    float result = -1;
    float xDiff = std::abs(endPos.col - currentPos.col);
    float yDiff = std::abs(endPos.row - currentPos.row);

    switch (mode)
    {
    case Heuristic::OCTILE:
        result = std::min(xDiff, yDiff) * Sqrt2 + std::max(xDiff, yDiff) - std::min(xDiff, yDiff);
        break;
    case Heuristic::MANHATTAN:
        result = xDiff + yDiff;
        break;
    case Heuristic::CHEBYSHEV:
        result = std::max(xDiff, yDiff);
        break;
    case Heuristic::EUCLIDEAN:
        result = std::sqrtf(static_cast<float>(xDiff * xDiff + yDiff * yDiff));
        break;
    case Heuristic::INCONSISTENT:
        if (currentPos.row + currentPos.col % 2 > 0)
            result = result = std::sqrtf(xDiff * xDiff + yDiff * yDiff);
        else
            result = 0.f;
        break;
    }

    return result;
}

void AStarPather::rubberbandPath(AStarNode* goalNode)
{
    AStarNode* childNode = goalNode;
    AStarNode* parentNode = childNode->parent;

    if (parentNode) //to prevent crash
        parentNode = parentNode->parent; 

    while (parentNode)
    {
        GridPos parentPos = parentNode->gridPos;
        GridPos childPos = childNode->gridPos;

        int rowMin = std::min(parentPos.row, childPos.row);
        int rowMax = std::max(parentPos.row, childPos.row);
        int colMin = std::min(parentPos.col, childPos.col);
        int colMax = std::max(parentPos.col, childPos.col);

        bool isBandable = true;
        for (int i = rowMin; i <= rowMax; ++i)
        {
            for (int j = colMin; j <= colMax; ++j)
            {
                if (terrain->is_wall(i, j))
                {
                    isBandable = false;
                }
            }
        }

        if (isBandable)
        {
            childNode->parent = parentNode;
            parentNode = parentNode->parent;
        }
        else
        {
            childNode = childNode->parent;
            parentNode = parentNode->parent;
        }
    }
}

void AStarPather::smoothPath(WaypointList& path)
{
    auto iter1 = path.begin();
    auto iter2 = std::next(iter1);
    
    //Add points back in when > 1.5 grid
    while (iter2 != path.end())
    {
        GridPos gridPos1 = terrain->get_grid_position(*iter1);
        GridPos gridPos2 = terrain->get_grid_position(*iter2);

        int xDiff = gridPos2.col - gridPos1.col;
        int yDiff = gridPos2.row - gridPos1.row;

        float lengthSq = static_cast<float>(xDiff * xDiff + yDiff * yDiff);

        if (lengthSq > smoothMaxDistanceSq)
        {
            iter2 = path.insert(iter2, (*iter1 + *iter2) / 2);
        }
        else
        {
            ++iter1;
            ++iter2;
        }
    }

    if (path.size() < 4) // Catmull-Rom Spline must need more than 4 points
        return;

    //Smooth using catmullrom spline
    iter1 = path.begin();
    iter2 = iter1;
    auto iter3 = std::next(iter2);
    auto iter4 = std::next(iter3);

    while (iter3 != path.end())
    {
        if (iter4 == path.end())
        {
            iter4 = iter3;
        }

        for (float t = 0.25; t < 0.9f; t += 0.25)
        {
            path.insert(iter3, Vec3::CatmullRom(*iter1, *iter2, *iter3, *iter4, t));
        }

        if (iter2 == path.begin())
        {
            iter2 = iter3;
            ++iter3;
            ++iter4;
        }
        else
        {
            iter1 = iter2;
            iter2 = iter3;
            ++iter3;
            ++iter4;
        }
    }
}



//void AStarPather::initFloydWarshall()
//{
//    for (int i = 0; i < MapY_Max; ++i)
//    {
//        for (int j = 0; j < MapX_Max; ++j)
//        {
//            if (!terrain->is_valid_grid_position(i, j))
//                continue;
//
//            for (int k = 0; k < neighborNum; ++k)
//            {
//                if (!(neighborMasks[i] & nodeMap[i][j].neighbors)) // if neighbor is null
//                    continue;
//
//                
//            }
//        }
//    }
//
//    for (int k = 0; k < MapX_Max; ++k)
//    {
//        for (int i = 0; i < MapY_Max; ++i)
//        {
//            for (int j = 0; j < MapX_Max; ++j)
//            {
//                path[i][j] = std::min(path[i][j], path[i][k] + path[k][j]);
//            }
//        }
//    }
//
//}

