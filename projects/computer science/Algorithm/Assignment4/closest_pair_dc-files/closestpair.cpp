#include "closestpair.h"
#include <algorithm>
#include <limits>
#include <cmath>
#include <iostream>
#include <utility>

std::ostream& operator<< (std::ostream& os, Point const& p) {
	os << "(" << p.x << " , " << p.y << ") ";
	return os;
}

std::istream& operator>> (std::istream& os, Point & p) {
	os >> p.x >> p.y;
	return os;
}

////////////////////////////////////////////////////////////////////////////////
float closestPair_aux ( std::vector<Point>const& points, std::vector<int> indices );

////////////////////////////////////////////////////////////////////////////////
float closestPair ( std::vector< Point >const& points ) {
	int size = points.size();

	if (size==0) throw "zero size subset";
	if (size==1) return std::numeric_limits<float>::max();

	std::vector<int> indices(size);
	for(int i = 0; i < size; ++i)
	{
		indices[i] = i;
	}

	return closestPair_aux( points, indices );
}

float distanceSquare(const Point& p1, const Point& p2)
{
	float xDist = p2.x - p1.x;
	float yDist = p2.y - p1.y;
	return xDist*xDist + yDist*yDist;
}

float DivideVertical(std::vector<int>& leftIndices, std::vector<int>& rightIndices, std::vector<Point>const& points, std::vector<int>const& indices)
{
	float axis = (points[indices[0]].x + points[indices[1]].x) / 2;

	for(int i : indices)
	{
		if(points[i].x < axis)
		{
			leftIndices.push_back(i);
		}
		else
		{
			rightIndices.push_back(i);
		}
	}

	float d1 = closestPair_aux(points, leftIndices);
	float d2 = closestPair_aux(points, rightIndices);

	float minDistSquare = d1 < d2 ? d1 * d1 : d2 * d2;
	float xDist;
	for(int i: leftIndices)
	{
		xDist = points[i].x - axis;
		if(xDist * xDist > minDistSquare) continue;

		for(int j : rightIndices)
		{
			xDist = points[j].x - axis;
			if(xDist * xDist > minDistSquare) continue;

			float currentSquare = distanceSquare(points[i], points[j]);
			if(currentSquare < minDistSquare)
			{
				minDistSquare = currentSquare;
			}
		}
	}

	return std::sqrt(minDistSquare);
}

float DivideHorizontal(std::vector<int>& leftIndices, std::vector<int>& rightIndices, std::vector<Point>const& points, std::vector<int> const& indices)
{
	float axis = (points[indices[0]].y + points[indices[1]].y) / 2;

	for(int i : indices)
	{
		if(points[i].y < axis)
		{
			leftIndices.push_back(i);
		}
		else
		{
			rightIndices.push_back(i);
		}
	}

	if(leftIndices.empty() || rightIndices.empty())
	{
		return 0.f;
	}

	float d1 = closestPair_aux(points, leftIndices);
	float d2 = closestPair_aux(points, rightIndices);

	float minDistSquare = d1 < d2 ? d1 * d1 : d2 * d2;
	float yDist;

	for(int i: leftIndices)
	{
		yDist = points[i].y - axis;
		if((yDist * yDist) > minDistSquare) continue;

		for(int j : rightIndices)
		{
			yDist = points[j].y - axis;
			if(yDist * yDist > minDistSquare) continue;

			float currentSquare = distanceSquare(points[i], points[j]);
			if(currentSquare < minDistSquare)
			{
				minDistSquare = currentSquare;
			}
		}
	}

	return std::sqrt(minDistSquare);
}

////////////////////////////////////////////////////////////////////////////////
float closestPair_aux ( std::vector<Point>const& points, std::vector<int> indices )
{
	int size = indices.size();

	if (size==0) throw "zero size subset";
	if (size==1) return std::numeric_limits<float>::max();


	std::vector<int> leftIndices, rightIndices;


	if(points[indices[0]].x != points[indices[1]].x)
	{
		return DivideVertical(leftIndices, rightIndices, points, indices);
	}
	else
	{
		return DivideHorizontal(leftIndices, rightIndices, points, indices);
	}
}

