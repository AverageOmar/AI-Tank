#include "Steering_H011063F.h"

#include "..\Commons.h"
#include "..\C2DMatrix.h"
#include "..\ObstacleManager.h"
#include "..\Collisions.h"

Steering_H011063F::Steering_H011063F()
{
	seek = false;
	arrive = true;
	flee = false;
	pursuit = false;

}

Steering_H011063F::~Steering_H011063F()
{

}

Vector2D Steering_H011063F::Combined(Vector2D position, Vector2D moveTo, Vector2D velocity, vector<BaseTank*> Tank, Vector2D frontOfTank, double maxSpeed)
{
	Vector2D direction = Vector2D(0.0, 0.0);

	if (arrive)
	{
		direction += Arrive(position, moveTo, velocity);
	}
	
	if (flee)
	{
		direction += Flee(position, moveTo);
	}
	
	if (seek)
	{
		direction += Seek(position, moveTo);
	}
	if (pursuit)
	{
		direction += Pursuit(position, Tank, velocity);
	}
	direction += ObstacleAvoidance(position, direction, velocity);

	direction.Truncate(maxSpeed);

	return direction - velocity;
}

Vector2D Steering_H011063F::Seek(Vector2D position, Vector2D moveTo)
{
	Vector2D direction;

	direction = moveTo - position;

	return direction;
}

Vector2D Steering_H011063F::Flee(Vector2D position, Vector2D moveFrom)
{
	Vector2D direction;

	direction = moveFrom - position;
	direction.x = -direction.x;
	direction.y = -direction.y;

	return direction;
}

Vector2D Steering_H011063F::Arrive(Vector2D position, Vector2D moveTo, Vector2D velocity)
{
	Vector2D direction;

	direction = moveTo - position;
	double mag = direction.Length();

	if (mag > 0)
	{
		return direction - velocity;
	}

	return Vector2D(0.0, 0.0) - velocity;
	
}

Vector2D Steering_H011063F::Pursuit(Vector2D position, vector<BaseTank*> Tank, Vector2D velocity)
{
	Vector2D direction = Vector2D(0.0, 0.0);

	if (Tank.size() > 0)
	{
		direction = ((Tank[0]->GetCentralPosition() + Tank[0]->GetHeading()) - position) - velocity;
	}

	return direction;
}

Vector2D Steering_H011063F::ObstacleAvoidance(Vector2D position, Vector2D direction, Vector2D velocity)
{
	bool colliding = false;
	bool sideColliding = false;

	vector<GameObject*> obstacles = ObstacleManager::Instance()->GetObstacles();

	Vector2D turnForce;
	turnForce.x = direction.y;
	turnForce.y = 0.0 - direction.x;

	for (int i = 0; i < obstacles.size() - (obstacles.size() - 3); i++)
	{
		Rect2D box(obstacles[i]->GetAdjustedBoundingBox()[i].x, obstacles[i]->GetAdjustedBoundingBox()[i].y, (obstacles[i]->GetCentralPosition().x - obstacles[i]->GetAdjustedBoundingBox()[i].x) * 2, (obstacles[i]->GetCentralPosition().y - obstacles[i]->GetAdjustedBoundingBox()[i].y) * 2);
		colliding = Collisions::Instance()->PointInBox((position + velocity), box);
		if (colliding)
		{
			break;
		}
	}

	if (colliding == true)
	{
		return (Vector2D(0.0, 0.0) - direction) + turnForce;
	}

	return Vector2D(0.0, 0.0);
}
