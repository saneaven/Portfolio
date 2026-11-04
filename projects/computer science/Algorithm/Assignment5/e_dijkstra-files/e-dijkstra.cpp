#include <vector>
#include <fstream>
#include <limits>
#include <queue>
#include <utility>

#include "e-dijkstra.h"

namespace nstd
{
    constexpr unsigned int INF = std::numeric_limits<unsigned int>::max();
}

class Car
{
    int maxFuel = 0;
    int remainFuel = -1;
    int remainRecharge = 0;
    int totalUsedFuel = 0;
public:

    unsigned int currentLocation = 0;
    unsigned int destination = 0;

    Car() = default;
    Car(int maxRecharge, int maxFuel) : maxFuel(maxFuel), remainRecharge(maxRecharge) {}
    Car(Car &&) = default;
    Car(Car const&) = default;

    Car& operator=(Car &&) = default;
    Car& operator=(Car const&) = default;

    bool operator<(Car const& other) const
    {
        return (remainRecharge * maxFuel) + remainFuel < (other.remainRecharge * other.maxFuel) + other.remainFuel;
    }
    bool operator>(Car const& other) const
    {
        return (remainRecharge * maxFuel) + remainFuel > (other.remainRecharge * other.maxFuel) + other.remainFuel;
    }

    bool operator==(Car const& other) const
    {
        return remainFuel == other.remainFuel && remainRecharge == other.remainRecharge;
    }

    void useFuel(unsigned int val) 
    {
        remainFuel -= val;
        totalUsedFuel += val;
    }
    unsigned int getRemainFuel() const { return remainFuel; }
    unsigned int getRemainRecharge() const {return remainRecharge; }
    unsigned int getTotalUsedFuel() const { return totalUsedFuel; }

    void recharge()
    {
        remainRecharge--;
        remainFuel = maxFuel;
    }

    unsigned int getMaxFuel() const { return maxFuel; }
};

using RoadMap = std::vector<std::vector<unsigned int>>;

void initRoadMap(RoadMap& roadMap, Car& car, char const* fileName, int range)
{
    int numLocation, maxRecharge, numEdge;
    std::ifstream file(fileName);

    if(file.is_open())
    {
        file >> numLocation >> maxRecharge >> numEdge;

        RoadMap tempRoadMap(numLocation, std::vector<unsigned int>(numLocation, nstd::INF));
        Car expectCar(maxRecharge, range);

        for(int i = 0; i< numEdge;++i)
        {
            int from;
            int to;
            int distance;
            
            file >> from >> to >> distance;
            
            tempRoadMap[from][to] = distance;
            tempRoadMap[to][from] = distance;
        }

        roadMap = std::move(tempRoadMap);
        car = std::move(expectCar);
    }
}

bool isExist(std::vector<Car> const& vec)
{
    for(Car const& i : vec)
    {
        if(i.getMaxFuel() == 0) return true;
    }
    
    return false;
}

bool e_dijkstra( char const* fileName, int range)
{
    RoadMap roadMap;
    Car car;

    initRoadMap(roadMap, car, fileName, range);

    std::vector<Car> visited(roadMap.size(), Car());
    std::priority_queue<Car> candidates;

    unsigned int mapSize = visited.size();

    // bool isPossible = false;
    Car currentCar(car);



    for(unsigned int i = 0; i < mapSize; ++i)
    {
        for(unsigned int j = 0; j < mapSize; ++j)
        {
            visited[j] = Car();
        }
        visited[i] = car;
        currentCar = car;
        currentCar.recharge();
        currentCar.currentLocation = i;

        candidates.push(currentCar);

        while(!candidates.empty())
        {
            candidates.pop();

            for(currentCar.destination = 0; currentCar.destination < mapSize; ++currentCar.destination)
            {
                if(currentCar.getMaxFuel() < roadMap[currentCar.currentLocation][currentCar.destination]) continue;

                Car expectCar(currentCar);

                if(expectCar.getRemainFuel() < roadMap[expectCar.currentLocation][expectCar.destination])
                {
                    if(expectCar.getRemainRecharge() > 0)
                    {
                        expectCar.recharge();
                    }
                    else
                    {
                        continue;
                    }
                }
                
                expectCar.useFuel(roadMap[expectCar.currentLocation][expectCar.destination]);

                if(expectCar > visited[expectCar.destination])
                {
                    visited[expectCar.destination] = expectCar;

                    expectCar.currentLocation = expectCar.destination;
                    candidates.push(expectCar);
                }
            }

            currentCar = candidates.top();
        }

        if(isExist(visited))
        {
            return false;
        }
    }


    return true;
}