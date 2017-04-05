#include "Pathfinder_H011063F.h"

Pathfinder::Pathfinder()
{
	manager = WaypointManager::Instance();
	SetEdgeCost();
}

Pathfinder::~Pathfinder()
{
	EDGECOST_List.clear();
	OPEN_List.clear();
	CLOSED_List.clear();
}

vector<Vector2D> Pathfinder::GetPathBetween(Vector2D startPosition, Vector2D endPosition)
{
	OPEN_List.clear();
	CLOSED_List.clear();

	vector<Vector2D> path;
	Waypoint* nearestToTank = GetNearestWaypointToPosition(startPosition);
	Waypoint* nearestToEnd = GetNearestWaypointToPosition(endPosition);

	if (nearestToTank == NULL || nearestToEnd == NULL || nearestToTank == nearestToEnd)
	{
		path.push_back(endPosition);
		return path;
	}

	OPEN_List.push_back(new AStarNode(nearestToTank, NULL, 0.0));
	AStarNode* currentNode = NULL;

	while (OPEN_List.empty() == false)
	{
		for (int i = 0; i < OPEN_List.size(); i++)
		{
			if (currentNode == NULL || OPEN_List[i]->length < currentNode->length)
			{
				currentNode = OPEN_List[i];

				if (currentNode->currentNode == nearestToEnd)
				{
					path = ConstructPath(currentNode, endPosition);
					return path;
				}
			}

			vector<int> ID = currentNode->currentNode->GetConnectedWaypointIDs();

			for (int i = 0; i < ID.size(); i++)
			{
				Waypoint* connectedNode = manager->GetWaypointWithID(ID[i]);

				if (!IsInList(OPEN_List, connectedNode) && !IsInList(CLOSED_List, connectedNode))
				{
					double g = currentNode->length + GetCostBetweenWaypoints(currentNode->currentNode, connectedNode);
					double h = GetHeuristicCost(connectedNode->GetPosition(), endPosition);
					double f = g + h;

					OPEN_List.push_back(new AStarNode(connectedNode, currentNode, f));
				}
			}
		}

		vector<AStarNode*>::iterator iter = OPEN_List.begin();
		while (iter != OPEN_List.end())
		{
			if (*iter == currentNode)
			{
				iter = OPEN_List.erase(iter);
			}
			else
			{
				++iter;
			}
		}

		currentNode = NULL;
	}

	return path;
}

void Pathfinder::SetEdgeCost()
{
	vector<Waypoint*> waypoints = manager->GetAllWaypoints();

	for (int i = 0;i < waypoints.size();i++)
	{
		vector<int> ID = waypoints[i]->GetConnectedWaypointIDs();

		for (int i = 0; i < ID.size(); i++)
		{
			Waypoint* temp = manager->GetWaypointWithID(ID[i]);
			Vector2D fromDist = temp->GetPosition() - waypoints[i]->GetPosition();
			double mag = sqrt((fromDist.x * fromDist.x) + (fromDist.y * fromDist.y));
			EDGECOST_List.push_back(new EdgeCost(waypoints[i], temp, mag));
		}
	}
}

Waypoint* Pathfinder::GetNearestWaypointToPosition(Vector2D position)
{
	vector<Waypoint*> waypoints = manager->GetAllWaypoints();
	float MaxDistance = 100.0f;
	Waypoint* nearest = NULL;

	for (int i = 0; i < waypoints.size(); i++)
	{
		Vector2D dist = waypoints[i]->GetPosition() - position;
		float mag = sqrt((dist.x * dist.x) + (dist.y * dist.y));
		if (mag < MaxDistance)
		{
			MaxDistance = mag;
			nearest = waypoints[i];
		}
	}
	return nearest;
}

double Pathfinder::GetCostBetweenWaypoints(Waypoint* from, Waypoint* to)
{
	for (int i = 0; i < EDGECOST_List.size(); i++)
	{
		if (EDGECOST_List[i]->origin == from && EDGECOST_List[i]->destination == to)
		{
			return EDGECOST_List[i]->length;
		}
	}
	return MaxFloat;
}

vector<Vector2D> Pathfinder::ConstructPath(AStarNode* targetNode, Vector2D endPos)
{
	vector<Vector2D> path;
	vector<Vector2D> pathInReverse;
	pathInReverse.push_back(endPos);

	AStarNode* currentNode = targetNode;
	while (currentNode != NULL)
	{
		pathInReverse.push_back(currentNode->currentNode->GetPosition());
		currentNode = currentNode->parentNode;
	}
	vector <Vector2D>::iterator iter = pathInReverse.end();
	while (iter != pathInReverse.begin())
	{
		path.push_back(*(--iter));
	}
	return path;
}

bool Pathfinder::IsInList(vector<AStarNode*> listToCheck, Waypoint* waypointToCheck)
{
	for (int i = 0; i < listToCheck.size(); i++)
	{
		if (listToCheck[i]->currentNode == waypointToCheck)
		{
			return true;
		}
	}
	return false;
}

double Pathfinder::GetHeuristicCost(Vector2D pos1, Vector2D pos2)
{
	Vector2D dist = pos2 - pos1;
	double mag = sqrt((dist.x * dist.x) + (dist.y * dist.y));
	return mag;
}
