#include <iostream>
#include <fstream>
#include <queue>
#include "tsp.h"

class MapHash
{
public:
    int idx = -1;
    int value = -1;

    MapHash(int a, int b):idx(a),value(b) {}

    bool operator<(const MapHash& other) const
    {
        return value > other.value;
    }
};

std::pair<MAP, MAP> makeMap(char const* filename)
{
    std::ifstream file(filename);
    
    if (!file.is_open()) 
    {
        std::cerr << "cannot open file!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
        exit(1);
    }

    int citiesNum;
    file >> citiesNum;

    MAP roadMap(citiesNum, std::vector<int>(citiesNum, -1));
    int num;

    for(int i = 0; i < citiesNum; ++i)
    {
        for(int j = i + 1; j < citiesNum; ++j)
        {
            file >> num;
            roadMap[i][j] = num;
            roadMap[j][i] = num;
        }
    }

    file.close();

    MAP priorityIdxMap(citiesNum, std::vector<int>(citiesNum, -1));

    for(int i = 0; i < citiesNum; ++i)
    {
        std::priority_queue<MapHash> hashes;

        for(int j = 0; j < citiesNum; ++j)
        {
            MapHash hash = MapHash(j,roadMap[i][j]);

            hashes.push(hash);
        }

        for(int j = 0; j < citiesNum; ++j)
        {
            priorityIdxMap[i][j] = hashes.top().idx;

            hashes.pop();
        }
    }

    return {roadMap, priorityIdxMap};
}

std::vector<int> precalMinvals(MAP& roadMap)
{
    int cityNum = roadMap.size();
    std::vector<int> minVals(cityNum);

    for (int i = 0; i < cityNum; ++i)
    {
        int minVal = 0x7fffffff;
        for (int j = 0; j < cityNum; ++j)
        {
            if (roadMap[i][j] != -1 && roadMap[i][j] < minVal)
                minVal = roadMap[i][j];
        }
        minVals[i] = minVal;
    }

    return minVals;
}

int RelexCheck(std::vector<int>& minVals, std::vector<bool>& visitMap)
{
    int cityNum = minVals.size();

    int sum = 0;

    for(int i = 0; i < cityNum; ++i)
    {
        if(visitMap[i] && i != 0) 
            continue;

        sum += minVals[i];
    }

    return sum;
}

std::vector<int> TSPRecur(MAP& roadMap, MAP& priorIdxMap, std::vector<int>& minVals,
                             std::vector<int> visited, std::vector<bool> visitMap,
                            int currVal, int& bestVal, int currCity)
{
    
    if(visited.size() > 0)
    {
        int previousCity = visited[visited.size()-1];
        currVal += roadMap[previousCity][currCity];
    }

    visitMap[currCity] = true;
    visited.push_back(currCity);

    if(visited.size() == roadMap.size())
    {
        currVal += roadMap[currCity][0];

        if(currVal < bestVal)
        {
            visited.push_back(0);
            bestVal = currVal < bestVal ? currVal : bestVal;
        }

        return visited;
    }
    int relaxVal = RelexCheck(minVals, visitMap);
    if(relaxVal + currVal >= bestVal)
    {
        return visited;
    }

    std::vector<int> result, temp;
    int cityNum = roadMap.size();

    for(int i = 0; i < cityNum; ++i)
    {
        if(visitMap[priorIdxMap[currCity][i]]) 
            continue;
        
        temp = TSPRecur(roadMap, priorIdxMap, minVals, visited, visitMap, currVal, bestVal, priorIdxMap[currCity][i]);
        if(temp[temp.size()-1] == 0)
        {
            result = std::move(temp);
        }
    }

    if(result.size() == 0)
        result = std::move(temp);

    return result;
}

std::vector<int> SolveTSP(char const* filename)
{
    MAP roadMap;
    MAP priorIdxMap;
    std::pair<MAP,MAP> temp = makeMap(filename);

    roadMap = temp.first;
    priorIdxMap = temp.second;

    std::vector<int> minVals(precalMinvals(roadMap));
    std::vector<bool> visitMap(roadMap.size(), false);

    std::vector<int> result, start;
    int bestVal = 0x7fffffff;
    result = TSPRecur(roadMap, priorIdxMap, minVals, start, visitMap, 0, bestVal, 0);

    return result;
} 