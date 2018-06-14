#pragma once

#include <map>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <climits>
#include <time.h>
#include <chrono>

#include "../graph/Graph.h"
#include "../node/Node.h"

class Heuristics
{
  private:
	Graph *graph;
	std::vector<std::vector<Node *>> solucao;
	int solucaoCount;
	std::vector<int> solucaoEmpty;

  public:
	Heuristics(Graph *graph);
	~Heuristics();

	// Heuristica de Construção
	void constructiveHeuristic();
	void DSATUR();
	void VND(Graph *graph);
	void buscaLocal(int cor);
	void GRASP(int gInst, float alfa);

	// Funções para solução
	std::vector<std::vector<Node *>> getSolution(Graph *graph);
	void colorCorrect(Graph *graph);
	void printSolution(Graph *graph);
	void printEmptyColors();

	// GRASP
	Graph graspConstructor(float alfa);

	//getters
	int getSolution(){
		return solucaoCount;
	}
};