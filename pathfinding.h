#pragma once
#include "Structures_H011063F.h"

class Pathfinder
{
public:
	Pathfinder();
	~Pathfinder();

	vector<Vector2D> GetPathBetween(Vector2D startPosition, Vector2D endPosition);

private:
	vector<EdgeCost*> EDGECOST_List;

	vector<AStarNode*> OPEN_List;
	vector<AStarNode*> CLOSED_List;

	void SetEdgeCost();
	Waypoint* GetNearestWaypointToPosition(Vector2D position);
	double GetCostBetweenWaypoints(Waypoint* from, Waypoint* to);
	vector<Vector2D> ConstructPath(AStarNode* targetNode, Vector2D endPos);
	bool IsInList(vector<AStarNode*> listToCheck, Waypoint* waypointToCheck);
	double GetHeuristicCost(Vector2D pos1, Vector2D pos2);

	WaypointManager* manager;
};
