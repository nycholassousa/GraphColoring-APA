//http://mat.gsia.cmu.edu/COLOR08/

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <cstdlib>

#include "graph/Graph.h"
#include "node/Node.h"
#include "heuristic/Heuristics.h"

int readGraphFromFile(std::string path, Graph &g);

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "How to use it: " << argv[0] << " <type> <file>" << std::endl;
		std::cout << "Types:" << std::endl;
		std::cout << "0 - GRASP" << std::endl;
		std::cout << "1 - GREEDY" << std::endl;
		return EXIT_FAILURE;
	}

	int type = std::stoi(argv[1]);

	Graph g;
	readGraphFromFile(argv[2], g);

	switch (atoi(argv[1]))
	{
	case 0:
	{
		Heuristics grasp = Heuristics(&g);
		grasp.GRASP(10, 0.4);
		break;
	}
	case 1:
	{
		Heuristics *heuristics;
		heuristics = new Heuristics(&g);

		auto start1 = std::chrono::high_resolution_clock::now();
		heuristics->constructiveHeuristic();
		heuristics->getSolution(&g);
		auto end1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed1 = end1 - start1;

		heuristics->printSolution(&g);
		std::cout << "------------" << std::endl;

		auto start2 = std::chrono::high_resolution_clock::now();
		heuristics->VND(&g);
		auto end2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed2 = end2 - start2;

		heuristics->printSolution(&g);
		heuristics->printEmptyColors();

		// Imprime o tempo total
		double tempoTotal = elapsed1.count() + elapsed2.count();
		std::cout << "Execution Time: " << tempoTotal << " seconds" << std::endl;

		break;
	}

	default:
	{
		std::cout << "Invalid Type." << std::endl;
		break;
	}
	}

	return EXIT_SUCCESS;
}

int readGraphFromFile(std::string path, Graph &g)
{
	std::ifstream entrada(path);

	// Verifica se o arquivo pode ser aberto / existe
	if (!entrada.is_open())
	{
		std::cout << "ERROR - FILE CAN NOT BE LOADED." << std::endl;
		return 0;
	}

	// Percorre o arquivo e adiciona o numero ao vetor
	std::string x;

	// Lê a primeira linha e pega a quantidade de nós do Graph
	int qtdNos, qtdArestas, i, j;
	char leitura;

	while (entrada >> leitura)
	{
		// Comentário. Apenas joga o valor numa variável e ignora.
		if (leitura == 'c')
		{
			getline(entrada, x);
		}
		// Descritor do problema. Informa a qtd de nós e arestas
		else if (leitura == 'p')
		{
			entrada >> x; // Ignora a palavra "edge"
			entrada >> qtdNos;
			entrada >> qtdArestas;
			for (int i = 1; i <= qtdNos; i++)
			{
				g.adicionarNode(i);
			}
		}
		// Descritor dos nós. Indica a ligação w->v e v->w
		else if (leitura == 'e')
		{
			entrada >> i >> j;
			// Adiciona os nós como vizinhos
			g.getNode(i)->addNeighbor(g.getNode(j));
			g.getNode(j)->addNeighbor(g.getNode(i));
		}
	}
	return 1;
}