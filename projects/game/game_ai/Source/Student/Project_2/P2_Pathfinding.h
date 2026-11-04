#pragma once
#include "Misc/PathfindingDetails.hpp"
#include <queue>
#include <functional>

constexpr int MapX_Max = 40, MapY_Max = 40;
constexpr float Sqrt2 = 1.4142136f;

constexpr int neighborNum = 8;
constexpr byte neighborMasks[neighborNum] = { 0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80 };
constexpr GridPos neighborPosDelta[neighborNum] = { {-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1} };
constexpr float moveCost[neighborNum] = { Sqrt2, 1.f, Sqrt2, 1.f, 1.f, Sqrt2, 1.f, Sqrt2 };


constexpr int huristic_MAX = 160;
constexpr float bucketInterval = 0.5f;
constexpr float bucketIntervalReverse = 1 / bucketInterval;

constexpr float smoothMaxDistanceSq = 1.5f * 1.5f;

class AStarPather
{
public:
    /* 
        The class should be default constructible, so you might need to define a constructor.
        If needed, you can modify the framework where the class is constructed in the
        initialize functions of ProjectTwo and ProjectThree.
    */

    /* ************************************************** */
    // DO NOT MODIFY THESE SIGNATURES
    bool initialize();
    void shutdown();
    PathResult compute_path(PathRequest &request);
    /* ************************************************** */

    /*
        You should create whatever functions, variables, or classes you need.
        It doesn't all need to be in this header and cpp, structure it whatever way
        makes sense to you.
    */
private:
    
    struct AStarNode
    {
    public:
        enum ListStatus
        {
            open, closed, free
        };

        AStarNode* parent;
        GridPos gridPos;
        float finalCost;
        float givenCost;
        ListStatus onList = ListStatus::free;
        byte neighbors;
        
        bool operator<(const AStarNode& other) { return this->finalCost < other.finalCost; }
        bool operator>(const AStarNode& other) { return this->finalCost > other.finalCost; }
    };

    class Bucket
    {
        std::vector<AStarNode*> nodes;
        const std::size_t initialSize = 15;
        std::size_t currentSize;
        const std::size_t sizeAddVal;
        int lastIdx;

    public:
        Bucket();

        AStarNode* pop();
        void push(AStarNode* node);
        void erase(AStarNode* node);
        void update();
        void clear();
    };

    class OpenList // Use bucket thing
    {
        std::vector<Bucket> buckets;
        int fastestBucketIdx = std::numeric_limits<int>::max();

    public:
        OpenList();

        AStarNode* pop();
        void push(AStarNode* node);
        void update(AStarNode* node, float newgivenCost, float newfinalCost);
        void clear();
    };

    AStarNode nodeMap[MapX_Max][MapY_Max];
    OpenList openLists;

    void initNodeMap();
    void resetNodeMap();
    void updateNodeMap();

    inline float calcHeuristic(const GridPos& currentPos, const GridPos& endPos, const Heuristic& mode);


    // Post Processes
    void rubberbandPath(AStarNode* goalNode);
    void smoothPath(WaypointList& path);

    //Extra Credit
    //static constexpr int DIAGONAL_COST = 14142136;
    //static constexpr int NORMAL_COST = 10000000;
    //static constexpr int PATH_MOVE_Cost[neighborNum] = { DIAGONAL_COST, NORMAL_COST, DIAGONAL_COST, NORMAL_COST, NORMAL_COST, DIAGONAL_COST, NORMAL_COST, DIAGONAL_COST };

    //int path[MapY_Max][MapX_Max];
    //void initFloydWarshall();
};