#include "Tanky_McTankFace_H011063F.h"
#include "..\TankManager.h"
#include "..\Commons.h"
#include "..\C2DMatrix.h"

//--------------------------------------------------------------------------------------------------

Tanky_McTankFace_H011063F::Tanky_McTankFace_H011063F(SDL_Renderer* renderer, TankSetupDetails details)
	: BaseTank(renderer, details)
{
	moving = new Steering_H011063F();
	move = GetPosition();
	waypointManager = WaypointManager::Instance();
	waypoints = waypointManager->GetAllWaypoints();
	i = 0;
	pathfinder = new Pathfinder();
	random = 0;
	closest = 0;
	state = "shoot";
	tankMoving = false;
}

//--------------------------------------------------------------------------------------------------

Tanky_McTankFace_H011063F::~Tanky_McTankFace_H011063F()
{
}

//--------------------------------------------------------------------------------------------------

void Tanky_McTankFace_H011063F::ChangeState(BASE_TANK_STATE newState)
{
	BaseTank::ChangeState(newState);
}

//--------------------------------------------------------------------------------------------------

void Tanky_McTankFace_H011063F::Update(float deltaTime, SDL_Event e)
{
	//Call parent update.
	BaseTank::Update(deltaTime, e);

	/*switch (e.type)
	{
	case SDL_MOUSEBUTTONUP:
		i = 0;
		move.x = e.button.x;
		move.y = e.button.y;
		path = pathfinder->GetPathBetween(GetCentralPosition(), move);
	break;
	}*/

	if (state == "shoot")
	{
		for (int i = 1; i < waypoints.size(); i++)
		{
			Vector2D dist = waypoints[i]->GetPosition() - GetCentralPosition();
			Vector2D distToCheck = waypoints[closest]->GetPosition() - GetCentralPosition();
			double mag = dist.Length();
			double magToCheck = distToCheck.Length();

			if (mag < magToCheck)
			{
				closest = i;
			}
		}
		move = waypoints[closest]->GetPosition();

		if (GetBullets() == 0)
		{
			state = "move";
		}

		if (GetBullets() > 0)
		{
			FireABullet();
			RotateManByRadian(0.25, 5.0, deltaTime);
		}
	}

	if (state == "move")
	{
		if (tankMoving == false)
		{
			i = 0;
			srand(GetTickCount());
			random = (rand() % waypoints.size()) + 1;
			path = pathfinder->GetPathBetween(GetCentralPosition(), waypoints[random]->GetPosition());
			tankMoving = true;
		}
		if (tankMoving == true)
		{
			if (i == path.size() - 1)
			{
				tankMoving = false;
			}
		}
	}

	MoveInHeadingDirection(deltaTime);

	collisionNodes[0] = GetCentralPosition() + (GetHeading().Perp() * 30);
	
	collisionNodes[1] = GetCentralPosition() + (GetHeading().Perp().GetReverse() * 30);

	collisionNodes[2] = collisionNodes[0] + GetVelocity();
	collisionNodes[3] = collisionNodes[1] + GetVelocity();

	if (path.size() > 0)
	{
		move = path[i];
		Vector2D dist = path[i] - GetCentralPosition();
		if (dist.Length() < 10)
		{
			if (i < path.size() - 1)
			{
				i++;
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------

void Tanky_McTankFace_H011063F::MoveInHeadingDirection(float deltaTime)
{
	//Get the force that propels in current heading.
	Vector2D force = moving->Combined(GetCentralPosition(), move, this->GetVelocity(), mTanksICanHear, this->GetPointAtFrontOfTank(), this->GetMaxSpeed());

	//Acceleration = Force/Mass
	Vector2D acceleration = force/GetMass();

	//Update velocity.
	mVelocity += acceleration * deltaTime;
	
	//Don't allow the tank does not go faster than max speed.
	mVelocity.Truncate(GetMaxSpeed()); //TODO: Add Penalty for going faster than MAX Speed.

	//Finally, update the position.
	Vector2D newPosition = GetPosition();
		newPosition.x += mVelocity.x*deltaTime;
		newPosition.y += (mVelocity.y/**-1.0f*/)*deltaTime;	//Y flipped as adding to Y moves down screen.

	Vector2D aheadDistance = mVelocity;
	if (aheadDistance.Length() == 0.0f)
		aheadDistance = mHeading;

	Vector2D ahead = GetCentralPosition() + aheadDistance;
	RotateHeadingToFacePosition(ahead, deltaTime);

	SetPosition(newPosition);
}

//--------------------------------------------------------------------------------------------------

void Tanky_McTankFace_H011063F::RotateHeadingByRadian(double radian, int sign)
{


}

//--------------------------------------------------------------------------------------------------

void Tanky_McTankFace_H011063F::Render()
{
	BaseTank::Render();

	for (int i = 0; i < 4; i++)
	{
		DrawDebugCircle(collisionNodes[i], 5.0, 255, 0, 0);
	}
	if (path.size() > 0)
	{
		for (int i = 0;i < path.size() - 1;i++)
		{
			if (i == 0)
			{
				DrawDebugLine(GetCentralPosition(), path[i], 255, 0, 0);
			}
			DrawDebugLine(path[i], path[i + 1], 255, 0, 0);
		}
	}
}
