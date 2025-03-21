#include "precompheaders.h"
#include "Math.h"

sf::Vector2f normalize(const sf::Vector2f& vector)
{
    return vector / static_cast<float>(sqrt(pow(vector.x, 2) + pow(vector.y, 2)));
}

float findAngleCos(const sf::Vector2f& pointA, const sf::Vector2f& pointB)
{
	float angle{};
	float adjecent = pointA.x - pointB.x;
	float hypotenuse = sqrt((pointA.x - pointB.x) * (pointA.x - pointB.x) + (pointA.y - pointB.y) * (pointA.y - pointB.y));
	float cos = adjecent / hypotenuse;
	angle = static_cast<float>(acos(cos));
	angle = (angle * 180) / 3.1415;
	return angle;
}

float findAngleTan(const sf::Vector2f& pointA, const sf::Vector2f& pointB)
{
	float angle{};
	float opposite = pointB.y - pointA.y;
	float adjecent = pointB.x - pointA.x;
	float tangens = opposite / adjecent;
	angle = static_cast<float>(atan(tangens));
	angle = (angle * 180) / 3.1415;
	return angle;
}

float vectorLenght(const sf::Vector2f& vector)
{
	return static_cast<float>(sqrt(pow(vector.x, 2) + pow(vector.y, 2)));
}
