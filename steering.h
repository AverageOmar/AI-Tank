#pragma once

#include "..\BaseTank.h"
#include <SDL.h>
#include "..\Commons.h"
#include "..\ObstacleManager.h"
#include "..\GameObject.h"
#include "..\Collisions.h"

class Steering_H011063F
{
public:
	Steering_H011063F();
	~Steering_H011063F();
	Vector2D Combined(Vector2D position, Vector2D moveTo, Vector2D velocity, vector<BaseTank*> Tank, Vector2D frontOfTank, double maxSpeed);
	Vector2D Seek(Vector2D position, Vector2D moveTo);
	Vector2D Flee(Vector2D position, Vector2D moveFrom);
	Vector2D Arrive(Vector2D position, Vector2D moveTo, Vector2D velocity);
	Vector2D Pursuit(Vector2D position, vector<BaseTank*> Tank, Vector2D velocity);
	Vector2D ObstacleAvoidance(Vector2D position, Vector2D direction, Vector2D velocity);

private:
	bool seek;
	bool flee;
	bool arrive;
	bool pursuit;
};
