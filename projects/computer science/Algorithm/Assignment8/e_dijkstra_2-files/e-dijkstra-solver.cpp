#include "e-dijkstra-solver.h"

#include <vector>
#include <fstream>
#include <limits>
#include <queue>
#include <utility>

namespace nstd
{
    constexpr unsigned int INF = std::numeric_limits<int>::max();
}

class Car
{
    int maxFuel = 0;
    int remainFuel = -1;
    int remainRecharge = 0;
    int totalUsedFuel = -1;
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
    void setMaxFuel(unsigned int fuel){ maxFuel = fuel; }
};

using RoadMap = std::vector<std::vector<unsigned int>>;

void initRoadMap(RoadMap& roadMap, Car& car, int& maxVal, char const* fileName)
{
    int numLocation, maxRecharge, numEdge;
    std::ifstream file(fileName);

    if(file.is_open())
    {
        file >> numLocation >> maxRecharge >> numEdge;

        RoadMap tempRoadMap(numLocation, std::vector<unsigned int>(numLocation, nstd::INF));
        Car expectCar(maxRecharge, nstd::INF);

        std::vector<int> maxDistances(numLocation, 0);

        for(int i = 0; i< numEdge;++i)
        {
            int from;
            int to;
            int distance;
            
            file >> from >> to >> distance;
            
            tempRoadMap[from][to] = distance;
            tempRoadMap[to][from] = distance;
            if(distance > maxDistances[to])
            {
                maxDistances[to] = distance;
            }
        }

        roadMap = std::move(tempRoadMap);
        car = std::move(expectCar);
        maxVal = 0;
        for(int val : maxDistances)
        {
            maxVal += val;
        }
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

bool e_dijkstra(const RoadMap& roadMap, Car car, std::vector<Car>& visited)
{
    visited.assign(roadMap.size(), Car());
    std::priority_queue<Car> candidates;

    unsigned int mapSize = visited.size();

    // bool isPossible = false;
    Car currentCar(car);
    Car expectCar;
    unsigned int optimizeVal = mapSize > 10 ? 10 : mapSize;
    for(unsigned int i = 0; i < optimizeVal; ++i)
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
            currentCar = candidates.top();
            candidates.pop();

            for(currentCar.destination = 0; currentCar.destination < mapSize; ++currentCar.destination)
            {
                if(currentCar.getMaxFuel() < roadMap[currentCar.currentLocation][currentCar.destination]) continue;

                expectCar = currentCar;

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
        }

        if(isExist(visited))
        {
            return false;
        }
    }


    return true;
}

int e_dijkstra_solver( char const * filename )
{
    RoadMap roadMap;
    Car car;
    int maxVal = -1;
    int minVal = 1;
    initRoadMap(roadMap, car, maxVal, filename);

    bool isSuccess = false;
    int currentFuel = -1;

    std::vector<Car> visited(roadMap.size(), Car());
    while(maxVal - minVal> 1)
    {
        currentFuel = unsigned((maxVal+minVal) / 2);
        car.setMaxFuel(currentFuel);
        isSuccess = e_dijkstra(roadMap, car, visited);
        if(isSuccess)
        {
            maxVal = currentFuel;
        }
        else
        {
            minVal = currentFuel;
        }
    }

    currentFuel = (maxVal+minVal) / 2;
    car.setMaxFuel(currentFuel);
    isSuccess = e_dijkstra(roadMap, car, visited);

    if(isSuccess)
    {
        return currentFuel;
    }
    else
    {
        return currentFuel + 1;
    }
}