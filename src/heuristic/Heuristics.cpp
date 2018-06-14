#include "Heuristics.h"

Heuristics::Heuristics(Graph *graph)
{
	this->graph = graph;
	this->solucaoCount = 0;
}

Heuristics::~Heuristics()
{
}

void Heuristics::constructiveHeuristic()
{
	for (auto v : graph->getNodes())
	{
		int cor = 0;

		// Enquanto existir nós adjacentes que possuam a mesma cor
		while (v->isAnyAdjacentColoredWith(cor))
		{
			cor++; // Incrementa para uma nova cor
		}

		// Atualize a cor do Node e decremente a quantidade de Nodes que faltam pintar
		v->setColor(cor);
		graph->decrementMissing();
	}
}

void Heuristics::DSATUR()
{
	// Define o degree de cada um dos nós
	// Quanto mais vizinhos um nó possuir, maior seu degree
	for (auto v : graph->getNodes())
	{
		for (auto vizinhos : v->addNeighbors())
		{
			v->incrementDegree();
		}
	}

	// Ordena os vértices do Graph por degree
	graph->orderByDegree();

	// Pinta o Graph de maior degree (primeiro após a ordenação)
	graph->getNodes()[0]->setColor(0);
	// Define ele como o menor numero possível, para sempre ficar no fim da prioridade quando for ordenar por saturação
	graph->getNodes()[0]->setSaturation(INT_MIN);
	// Muda a saturação de seus nós adjacentes
	graph->getNodes()[0]->updateAdjacentSaturation();
	graph->decrementMissing();

	// Enquanto todos os nós não estiverem coloridos
	while (!graph->isColoringDone())
	{
		// Ordene os vértices pela saturação
		graph->orderBySaturation();
		// Define ele como o menor numero possível, para sempre ficar no fim da prioridade quando for ordenar por saturação
		graph->getNodes()[0]->setSaturation(INT_MIN);
		// Muda a saturação de seus nós adjacentes
		graph->getNodes()[0]->updateAdjacentSaturation();

		int cor = 0;

		// Enquanto existir vetores adjacentes com a mesma cor
		while (graph->getNodes()[0]->isAnyAdjacentColoredWith(cor))
		{
			cor++; // Incrementa a cor enquanto houver vizinhos com a mesma cor
		}
		graph->getNodes()[0]->setColor(cor);
		graph->decrementMissing();
	}
}

// Percorre as cores na ordem inversa, esvaziando os buckets um por um e pega a melhor solução no final
void Heuristics::VND(Graph *graph)
{
	// Obtem o bucket de soluções inicial
	getSolution(graph);

	// Percorre os buckets de cores
	for (int i = solucao.size() - 1; i > 0; i--)
	{
		buscaLocal(i);

		// Corrige a quantidade de cores
		getSolution(graph);
		//colorCorrect(graph);
	}
}

std::vector<std::vector<Node *>> Heuristics::getSolution(Graph *graph)
{
	// Pega a cor do último elemento do vetor de cores
	int cores = (graph->getMaxColor() + 1);

	std::vector<std::vector<Node *>> temp;

	// Cria uma lista com n buckets representando as cores
	for (int i = 0; i < cores; i++)
	{
		std::vector<Node *> bucket;
		temp.push_back(bucket);
	}

	// Insere os nós em cada um dos buckets
	for (auto v : graph->getNodes())
	{
		temp.at(v->getColor()).push_back(v);
	}

	//  Atualiza a quantidade de cores da solução
	this->solucaoCount = cores;

	//  Atualiza a solução
	this->solucao = temp;

	return solucao;
}

// Corrige a quantidade de cores no Graph
// Caso não seja utilizado, mostrará sempre a quantidade de cores da primeira solução, mas essa cor não terá nenhum nó
void Heuristics::colorCorrect(Graph *graph)
{
	// aux = quantidade de cores "perdidas" desde a ultima posição válida
	int cores = 0, aux = 0;
	std::vector<int> temp;

	// Verifica a quantidade de cores no Graph
	for (auto bucket : solucao)
	{
		// Verifica o tamanho do bucket. Se não for zero, incrementa o numero de cores no Graph
		// E zera a quantidade de cores p
		if (bucket.size() != 0)
		{
			cores++;
			aux = 0;
		}
		else
		{
			// Caso o bucket esteja vazio, decrementa todos os nós maiores que o indice atual do bucket
			graph->decrementAllGreatherThan(cores);

			// E joga a cor na lista de cores esvaziadas
			temp.push_back(cores + aux);
			aux++;
		}
	}
	this->solucaoCount = cores;
	this->solucaoEmpty = temp;
	getSolution(graph);
}

void Heuristics::printSolution(Graph *graph)
{
	// Exibe na tela a solucao
	int index = 0;
	for (auto c : this->solucao)
	{
		std::cout << "Color " << index << " - Nodes [ ";
		for (auto v : c)
		{
			std::cout << v->getId() << " ";
		}
		std::cout << "]" << std::endl;
		index++;
	}
	std::cout << "Total Graph Colors: " << solucaoCount << std::endl;
	std::cout << "Total Graph Nodes: " << graph->getNodes().size() << std::endl;
}

void Heuristics::printEmptyColors()
{
	// Verifica se a quantidade de cores eliminadas foi maior que 0 e imprime na tela
	if (this->solucaoEmpty.size() > 0)
	{
		std::cout << "Total Eliminated Colors: " << solucaoEmpty.size() << std::endl;
		std::cout << "Colors Eliminated: ";
		for (int cor : solucaoEmpty)
		{
			std::cout << cor << " - ";
		}
		std::cout << std::endl;
	}
}

/*
* Dada uma cor, tenta eliminar ela do grafo recolorindo todos os nós com essa cor com as outras opções de cores disponíveis. 
* Não pode aumentar, só pode ser no intervalo de cores da solução já gerada.
*/
void Heuristics::buscaLocal(int cor)
{
	// Percorre todos os nós de uma determinada cor
	for (auto node : this->solucao[cor])
	{
		// Percorre todas as opções de cores
		for (int candidateColor = 0; candidateColor < solucao.size(); candidateColor++)
		{
			// Verifica se a cor que está sendo trocada não é identica a cor atual
			// E se a cor candidata não é a cor que estamos tentando esvaziar
			if (candidateColor != cor)
			{
				// Verifica se não existe nenhuma cor adjacente com a cor candidata
				if (!node->isAnyAdjacentColoredWith(candidateColor))
				{
					// Troca a cor do nó
					node->setColor(candidateColor);

					// Após achar uma cor, encerra o laço.
					break;
				}
			}
		}
	}
}

void Heuristics::GRASP(int gInst, float alfa)
{
	// Alimenta o randomizador de tempo
	srand(time(NULL));

	// Inicia as variaveis que guardarão a melhor solução
	Graph bestSolution;
	int lastBestSolution = INT_MAX;
	double mediaSolution = 0;
	double mediaTempo = 0;
	int momentum = 0;

	// Gera a qtd de soluções fornecida como argumento
	for (int i = 0; i < gInst; i++)
	{
		std::cout << "#" << i;

		auto start = std::chrono::high_resolution_clock::now();
		// Fase de Construção do GRASP
		Graph k = graspConstructor(alfa);
		std::cout << " | GRASP: " << solucaoCount;

		// Fase de Melhoria do GRASP por Busca Local
		// Em relação a uma cor randomica de sua solução
		int randomColor = rand() % (solucao.size());
		buscaLocal(randomColor);

		getSolution(&k);
		colorCorrect(&k);
		auto finish = std::chrono::high_resolution_clock::now();

		// Contagem de tempo das execuções
		std::chrono::duration<double> elapsed = finish - start;

		// Fase de Definição da Melhor Solução
		mediaSolution += solucaoCount; // Média das Execuções

		// Se a solução atual for melhor que a anterior
		if (solucaoCount < lastBestSolution)
		{
			// Troca a melhor solução e atualiza o Graph bestSolution
			lastBestSolution = solucaoCount;
			bestSolution = k;
			momentum = i;
		}
		double tempo = elapsed.count();
		mediaTempo += tempo;

		std::cout << " | LocalSearch: " << solucaoCount;
		std::cout << " | Elapsed time: " << tempo << " seconds" << std::endl;
	}

	// Imprime a melhor solução final
	getSolution(&bestSolution);
	printSolution(&bestSolution);

	// Imprime os resultados gerais do GRASP
	std::cout << "------------------------------------------------------" << std::endl;
	mediaSolution = (mediaSolution / gInst);
	mediaTempo = mediaTempo / gInst;
	std::cout << "Best Solution: " << lastBestSolution << std::endl;
	std::cout << "Best Solution Momentum: " << momentum << std::endl;
	std::cout << "Average of " << gInst << " executions: " << mediaSolution << std::endl;
	std::cout << "Average Time: " << mediaTempo << " seconds" << std::endl;
}

Graph Heuristics::graspConstructor(float alfa)
{
	// Cria cópia do grafo padrão fornecido.
	// A ideia é manter o grafo original descolorido, já que o GRASP será executado várias vezes
	Graph *grafoSolucao = new Graph();
	grafoSolucao->generateGrafoFrom(*graph);

	// Cria um grafo contendo todos os nós que até o momento são candidatos
	Graph grafoCandidatos = *grafoSolucao;

	// Enquanto a lista de nós descoloridos não for vazia
	while (grafoCandidatos.getNodes().size() != 0)
	{
		grafoCandidatos.orderBySaturation();

		// Calcula o criterio que que vai definir a RCL
		// Formula = alpha * (maiorSaturacao + menorSaturacao)
		int formula = ceil(grafoCandidatos.getNodes()[0]->getSaturation() + grafoCandidatos.getNodes()[grafoCandidatos.getNodes().size() - 1]->getSaturation()) * alfa;

		// Cria uma Lista Restrita de Candidatos
		// Essa lista apresenta os "melhores" vértices candidatos para melhorar o problema
		std::vector<Node *> LRC;

		for (auto cInicial : grafoCandidatos.getNodes())
		{
			// Adiciona o LRC todos os vértices com saturação maior que a formula
			if (cInicial->getSaturation() >= formula)
			{
				LRC.push_back(cInicial);
			}
		}

		// Define o indice aleatorio com base na LRC
		int rndIndex = rand() % LRC.size();

		// Procedimento padrão de construção gulosa
		int cor = 0;

		// Enquanto existir vetores adjacentes com a mesma cor
		while (grafoCandidatos.getNodes().at(rndIndex)->isAnyAdjacentColoredWith(cor))
		{
			cor++; // Incrementa a cor enquanto houver vizinhos com a mesma cor
		}
		grafoCandidatos.getNodes().at(rndIndex)->setColor(cor);

		// Atualiza a saturação dos nós próximos
		grafoCandidatos.getNodes().at(rndIndex)->updateAdjacentSaturation();

		// Decrementa a quantidade de colorações faltantes em ambos os grafos
		grafoSolucao->decrementMissing();
		grafoCandidatos.decrementMissing();

		// Remove o nó da lista de candidatos iniciais (não-coloridos) para que o valor da saturação
		// não prejudique o calculo da fórmula
		grafoCandidatos.removerNodeByPosition(rndIndex);
	}

	getSolution(grafoSolucao);

	return *grafoSolucao;
}