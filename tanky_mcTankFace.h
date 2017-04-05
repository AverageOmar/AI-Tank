#ifndef Tanky_McTankFace_H011063F_H
#define Tanky_McTankFace_H011063F_H

#include "Steering_H011063F.h"

#include "..\BaseTank.h"
#include <SDL.h>
#include "..\Commons.h"
#include "..\ObstacleManager.h"
#include "..\GameObject.h"
#include "..\Waypoint.h"
#include "..\WaypointManager.h"
#include "Pathfinder_H011063F.h"
#include "Structs_H011063F.h"

//---------------------------------------------------------------

class Tanky_McTankFace_H011063F : public BaseTank
{
	//---------------------------------------------------------------
public:
	Tanky_McTankFace_H011063F(SDL_Renderer* renderer, TankSetupDetails details);
	~Tanky_McTankFace_H011063F();

	void ChangeState(BASE_TANK_STATE newState);

	void Update(float deltaTime, SDL_Event e);
	void Render();

	void RotateHeadingByRadian(double radian, int sign);

	//---------------------------------------------------------------
protected:
	void	MoveInHeadingDirection(float deltaTime);

private:
	Steering_H011063F* moving;
	Vector2D move;
	Vector2D collisionNodes[4];
	vector<Waypoint*> waypoints;
	WaypointManager* waypointManager;
	Pathfinder* pathfinder;
	vector<Vector2D> path;
	int i;
	int random;
	int closest;
	string state;
	bool tankMoving;

};

//---------------------------------------------------------------

#endif //CONTROLLEDTANK_H
