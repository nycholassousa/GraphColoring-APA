#pragma once

#include <vector>
#include <string>
#include <stdio.h>
#include <algorithm>

#include "../node/Node.h"

class Graph
{
private:
	std::vector<Node *> nodes;
	int missingToColor;

public:
	Graph();
	void generateGrafoFrom(Graph &other); // Copy constructor
	~Graph();
	void adicionarNode(int id);
	void removerNodeById(int id);
	void removerNodeByPosition(int position);
	Node *getNode(int id);
	std::vector<Node *> getNodes();
	int getMaxColor();
	Node *getMaxColorElement();
	bool isColoringDone();
	bool isColorabilityValid();
	void decrementMissing();
	void orderByDegree();
	void orderBySaturation();
	void decrementAllGreatherThan(int cor);
};