#include "Node.h"

// Construtor com os dados
Node::Node(int id)
{
    this->id = id;
    this->color = -1;
    this->degree = 0;
    this->saturation = 0;
}

void Node::addNeighbor(Node *vizinho)
{
    this->vizinhos.push_back(vizinho);
}

int Node::getId()
{
    return this->id;
}

std::vector<Node *> Node::addNeighbors()
{
    return this->vizinhos;
}

int Node::getColor()
{
    return this->color;
}

bool Node::isColored()
{
    if (this->color != -1)
        return false;
    else
        return true;
}

void Node::incrementColor()
{
    this->color++;
}
void Node::decrementColor()
{
    this->color--;
}

void Node::setColor(int clr)
{
    this->color = clr;
}

int Node::getDegree() const
{
    return this->degree;
}

void Node::incrementDegree()
{
    this->degree++;
}

void Node::setDegree(int dgr)
{
    this->degree = dgr;
}

int Node::getSaturation() const
{
    return this->saturation;
}

void Node::incrementSaturation()
{
    this->saturation++;
}

void Node::setSaturation(int str)
{
    this->saturation = str;
}

bool Node::isAnyAdjacentColoredWith(int color)
{
    bool existe = false;
    // Percorre todos os vizinhos do nó
    for (auto v : vizinhos)
    {
        // Verifica se os nós adjacentes possuem a cor informada
        if (v->getColor() == color)
        {
            existe = true;
        }
    }
    return existe;
}

void Node::updateAdjacentSaturation()
{
    for (auto v : vizinhos)
    {
        // Só atualiza se o nó não tiver sido colorido ainda
        if (!v->isColored())
        {
            v->incrementSaturation();
        }
    }
}