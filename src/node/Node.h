#pragma once

#include <vector>
#include <iostream>

class Node
{
private:
	int id;
	int color;
	int saturation;
	int degree;
	std::vector<Node *> vizinhos;

public:
	Node(int id);
	int getId();
	void addNeighbor(Node *vizinho);
	std::vector<Node *> addNeighbors();

	int getColor();
	bool isColored();
	void incrementColor();
	void decrementColor();
	void setColor(int clr);

	int getDegree() const;
	void incrementDegree();
	void setDegree(int dgr);

	int getSaturation() const;
	void incrementSaturation();
	void setSaturation(int str);

	bool isAnyAdjacentColoredWith(int color);
	void updateAdjacentSaturation();
};