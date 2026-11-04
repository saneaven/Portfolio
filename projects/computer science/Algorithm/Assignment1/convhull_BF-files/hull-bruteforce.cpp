#include "hull-bruteforce.h"
#include <algorithm>
#include <iostream>

bool Point::operator==( Point const& arg2 ) const {
    return ( (x==arg2.x) && (y==arg2.y) );
}

std::ostream& operator<< (std::ostream& os, Point const& p) {
	os << "(" << p.x << " , " << p.y << ") ";
	return os;
}

std::istream& operator>> (std::istream& os, Point & p) {
	os >> p.x >> p.y;
	return os;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//return value is (on left, on right)
std::pair<bool,bool> get_location(
		float const& p1x, //check which side of the line (p1x,p1y)-->(p2x,p2y) 
		float const& p1y, //point (qx,qy) is on
		float const& p2x,
		float const& p2y,
		float const& qx,
		float const& qy
		) 
{
	Point dir   = {p2x-p1x,p2y-p1y};
	Point norm  = {dir.y, -dir.x};
	Point point = {qx-p1x,qy-p1y};
	//scalar product is positive if on right side
	float scal_prod = norm.x*point.x + norm.y*point.y;
	return std::make_pair( (scal_prod<0), (scal_prod>0));
}

std::pair<bool,bool> get_location(const Point& p1, const Point& p2, const Point& p_val)
{
	return get_location(p1.x, p1.y, p2.x, p2.y, p_val.x, p_val.y);
} 

bool isOnOneSide(const Point& p1, const Point& p2, const std::vector< Point >& points)
{
	int max_idx = points.size();

	std::pair<bool, bool> last_location = get_location(p1, p2, points[0]);
	bool is_start = (last_location.first == true) || (last_location.second == true);

	for(int i = 0; i < max_idx; i++)
	{
		std::pair<bool, bool> result = get_location(p1, p2, points[i]);

		if(result == std::pair<bool, bool>(false, false)) //한 직선에 있는 경우 예외처리
		{
			continue;
		}

		if(last_location != result)
		{
			if(is_start == false) //처음부터 한 직선에 있다가 처음 직선에 없는 걸 찾은 경우 예외처리
			{
				is_start = true;
				last_location = result;
				continue;
			}

			return false;
		}
	}

	return true;
}

//returns a set of indices of points that form convex hull
std::set<int> hullBruteForce ( std::vector< Point > const& points ) {
	int num_points = points.size();
	//std::cout << "number of points " << num_points << std::endl;
	if ( num_points < 3 ) throw "bad number of points";

	std::set<int> hull_indices;
	int max_idx = points.size();


	for(int i = 0; i < max_idx - 1; i++)
	{
		for(int j = i + 1; j < max_idx; j++)
		{
			if(isOnOneSide(points[i], points[j], points))
			{
				hull_indices.insert(i);
				hull_indices.insert(j);
			} 
		}
	}
		
	return hull_indices;
}

bool isEveryThingOnLeft(const Point& p1, const Point& p2, const std::vector< Point >& points)
{
	int max_idx = points.size();

	for(int i = 0; i < max_idx; i++)
	{
		if(get_location(p1, p2, points[i]).second)
		{
			return false;
		}
	}
	return true;
}

std::vector<int> hullBruteForce2 ( std::vector< Point > const& points ) {
	int num_points = points.size();
	if ( num_points < 3 ) throw "bad number of points";

	std::vector<int> hull_indices;
	int max_idx = points.size();
	int start_idx = 0, cur_idx = 0;
	for(int i = 0; i < max_idx; i++)
	{
		if(points[i].x < points[start_idx].x)
		{
			start_idx = i;
		}
	}
	cur_idx = start_idx;
	do
	{
		for(int i = 0; i < max_idx; i++)
		{
			if(i == cur_idx) continue;
			
			if(isEveryThingOnLeft(points[cur_idx], points[i], points))
			{
				hull_indices.push_back(cur_idx);
				cur_idx = i;
				break;
			}
		}
	} while(cur_idx != start_idx);

	return hull_indices;
}
