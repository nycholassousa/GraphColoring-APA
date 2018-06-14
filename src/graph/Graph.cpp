#include "Graph.h"

Graph::Graph()
{
	this->missingToColor = 0;
}

void Graph::generateGrafoFrom(Graph &other)
{
	this->nodes.clear();
	// Cria cópia dos nós
	for (auto node : other.getNodes())
	{
		this->adicionarNode(node->getId());
		this->getNode(node->getId())->setColor(node->getColor());
		this->getNode(node->getId())->setDegree(node->getDegree());
		this->getNode(node->getId())->setSaturation(node->getSaturation());
	}

	// Cria cópia dos vizinhos para cada nó
	for (auto node : other.getNodes())
	{
		for (auto vizinho : node->addNeighbors())
		{
			this->getNode(node->getId())->addNeighbor(this->getNode(vizinho->getId()));
		}
	}
	this->missingToColor = other.missingToColor;
}

Graph::~Graph()
{
}

void Graph::adicionarNode(int id)
{
	this->nodes.push_back(new Node(id));
	this->missingToColor++;
}

void Graph::removerNodeById(int id)
{
	int position = 0;
	for (auto v : nodes)
	{
		if (v->getId() == id)
		{
			nodes.erase(nodes.begin() + position);
			break;
		}
		position++;
	}
}

void Graph::removerNodeByPosition(int position)
{
	nodes.erase(nodes.begin() + position);
}

Node *Graph::getNode(int id)
{
	for (auto v : nodes)
	{
		if (v->getId() == id)
		{
			return v;
		}
	}
	return nullptr;
}

std::vector<Node *> Graph::getNodes()
{
	return this->nodes;
}

int Graph::getMaxColor()
{
	int maxColor = -1;
	for (auto v : nodes)
	{
		if (v->getColor() > maxColor)
		{
			maxColor = v->getColor();
		}
	}
	return maxColor;
}

Node *Graph::getMaxColorElement()
{
	Node *maxElement = nullptr;
	int maxColor = -1;

	for (auto v : nodes)
	{
		if (v->getColor() > maxColor)
		{
			maxColor = v->getColor();
			maxElement = v;
		}
	}

	return maxElement;
}

void Graph::decrementMissing()
{
	this->missingToColor--;
}

void Graph::orderBySaturation()
{
	std::sort(nodes.begin(), nodes.end(), [](const Node *lhs, const Node *rhs) {
		if (lhs->getSaturation() == rhs->getSaturation())
			return lhs->getDegree() > rhs->getDegree();

		return lhs->getSaturation() > rhs->getSaturation();
	});
}

void Graph::orderByDegree()
{
	std::sort(nodes.begin(), nodes.end(), [](const Node *lhs, const Node *rhs) {
		return lhs->getDegree() > rhs->getDegree();
	});
}

bool Graph::isColoringDone()
{
	return (missingToColor == 0);
}
bool Graph::isColorabilityValid()
{
	for (auto node : this->getNodes())
	{
		if (node->isAnyAdjacentColoredWith(node->getColor()))
		{
			return false;
		}
	}
	return true;
}

void Graph::decrementAllGreatherThan(int cor)
{
	for (auto v : nodes)
	{
		if (v->getColor() > cor)
		{
			v->decrementColor();
		}
	}
}