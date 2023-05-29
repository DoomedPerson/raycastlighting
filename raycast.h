#pragma once
#include <vector>
#include <cmath>
#include <SFML\Graphics.hpp>
#include <iostream>

inline float hypotenuse(int point1, int point2) { return sqrt(point1*point1 + point2*point2); }
inline int random(int max, int min = 0) { return rand() % (max - min) + min; }


constexpr double PI = 3.14159265358979323846;
extern sf::Vector2f mouseposition;

struct Point {
    float x;
    float y;

    Point(float _x, float _y) : x(_x), y(_y) {}
};

struct wall {
	Point start;
	Point end;

	wall(Point _start, Point _end) : start(_start), end(_end) {}
};

class wallHandler
{
private:
	std::vector<wall> walls;
public:

	void addWall(Point start, Point end)
	{
		wall currWall(start,end);

		walls.push_back(currWall);
	}

	std::vector<wall> getWalls() {
		return walls;
	}


	bool intersects(int startx, int starty, int endx, int endy, int originx, int originy, int indexx, int indexy) {
		int minx = std::min(startx, endx);
		int maxx = std::max(startx, endx);
		int miny = std::min(starty, endy);
		int maxy = std::max(starty, endy);

		if (originx < minx && indexx < minx)
			return false;
		if (originx > maxx && indexx > maxx)
			return false;
		if (originy < miny && indexy < miny)
			return false;
		if (originy > maxy && indexy > maxy)
			return false;

		int intersectX = (originx * indexy - originy * indexx) * (startx - endx) - (originx - indexx) * (startx * endy - starty * endx);
		int intersectY = (originx * indexy - originy * indexx) * (starty - endy) - (originy - indexy) * (startx * endy - starty * endx);
		int denominator = (originx - indexx) * (starty - endy) - (originy - indexy) * (startx - endx);

		if (denominator != 0) {
			intersectX /= denominator;
			intersectY /= denominator;
		}

		if (intersectX >= minx && intersectX <= maxx && intersectY >= miny && intersectY <= maxy &&
			intersectX >= std::min(originx, indexx) && intersectX <= std::max(originx, indexx) &&
			intersectY >= std::min(originy, indexy) && intersectY <= std::max(originy, indexy)) {
			return true;
		}

		return false;
	}


	bool inWay(sf::Vector2f origin, sf::Vector2f index)
	{
		for (int i = 0; i < walls.size(); i++)
		{
			int startX = walls[i].start.x;
			int startY = walls[i].start.y;

			int endX = walls[i].end.x;
			int endY = walls[i].end.y;

			if (intersects(startX, startY, endX, startY, origin.x, origin.y, index.x, index.y) ||
				intersects(startX, endY, endX, endY, origin.x, origin.y, index.x, index.y) ||
				intersects(startX, startY, startX, endY, origin.x, origin.y, index.x, index.y) ||
				intersects(endX, startY, endX, endY, origin.x, origin.y, index.x, index.y)) {
				return true;
			}
		}

		return false; // No walls intersect the line segment
	}


};
