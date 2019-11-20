#include<iostream>
#include<vector>
#include<cmath>
#include<cstdio>
#include<ctime>
using namespace std;

#define NP 50			//population size
#define D 30			//dimension
#define GEN 6000		//maximum generation
#define UB 100.0		//upper bound
#define LB -UB			//lower bound
#define Cr 0.9			//crossover rate
#define e exp(1)
#define PI acos(-1)
double F = 0.5;			//amplification factor
int gen = 1;			//current generation
int best_index;			//index record of global best individual

/* individual structrue */
struct Individual {
	vector<double> pos;
	double fitness;
};

/* population information */
vector<Individual> population(NP);
vector<Individual> mutant(NP);
vector<Individual> offspring(NP);

/* function statement */
void Init();
double CalFitness(vector<double>&);
vector<int> GetRandNum(int);
vector<int> GetRandNum(int, int);
void Mutation_rand(int);
void Mutation_best(int);
void Crossover(int);
void EvaluationAndSelection(int); 
void Print();
inline double random(double min, double max) { return ((double)rand() / RAND_MAX) * (max - min) + min; }
inline int random(int min, int max) { return (rand() % (max - min + 1)) + min; }
Individual operator + (const Individual A, const Individual B);
Individual operator - (const Individual A, const Individual B);
Individual operator * (const double F, const Individual ind);

int main() {
	srand((int)time(NULL));
	Init();
	while (gen <= GEN) {
		for (int i = 0; i < NP; ++i) {
			//Mutation_rand(i);
			Mutation_best(i);
			Crossover(i);
			EvaluationAndSelection(i);
		}
		Print();
		gen++;
	}
}

/* initialize population with random number between lower bound(LB) and upper bound(UB) */
void Init() {
	double min_ftns = DBL_MAX;
	for (int i = 0; i < NP; ++i) {
		for (int j = 0; j < D; ++j) {
			population[i].pos.push_back(random(LB, UB));
		}
		population[i].fitness = CalFitness(population[i].pos);
		
		//get the best individual's index
		if (population[i].fitness < min_ftns) {
			min_ftns = population[i].fitness;
			best_index = i;
		}
	}
}

/* generate mutant vector through choose three random individuals and amplification factor F */
void Mutation_rand(int i) {
	vector<int> index = GetRandNum(i);
	mutant[i] = population[index[0]] + F * (population[index[1]] - population[index[2]]);
}

/* generate mutant vector through choose two random individuals, the best individual and amplification factor F */
void Mutation_best(int i) {
	vector<int> index = GetRandNum(best_index, i);
	mutant[i] = population[best_index] + F * (population[index[0]] - population[index[1]]);
}

/* generate offspring vector by crossover strategy */
void Crossover(int i) {
	offspring[i].pos.clear();
	int ran_dimension = random(0, D - 1);
	for (int j = 0; j < D; ++j) {
		double rnd = random(0.0, 1.0);
		if (rnd <= Cr || j == ran_dimension) 
			offspring[i].pos.push_back(mutant[i].pos[j]);
		else 
			offspring[i].pos.push_back(population[i].pos[j]);
	}
	offspring[i].fitness = CalFitness(offspring[i].pos);
}

/* select the offspring with smaller fitness as new population */
void EvaluationAndSelection(int i) {
	if (offspring[i].fitness <= population[i].fitness)
		population[i] = offspring[i];
	//keep the best
	if (population[i].fitness < population[best_index].fitness)
		best_index = i;
}

/* fitness evaluation function */
double CalFitness(vector<double>& pos) {
	double fitness = 0.0;

	///Sphere function
	for (auto i : pos) {
		fitness += i * i;
	}

	///Ackley function
	//double sum1 = 0.0, sum2 = 0.0;
	//for (auto i : pos) {
	//	sum1 += pow(i, 2);
	//	sum2 += cos(2 * PI * i);
	//}
	//fitness = 20 + e - 20 * exp(-0.2 * sqrt(sum1 / D)) - exp(sum2 / D);

	return fitness;
}

/* get index of other three random different individual */
vector<int> GetRandNum(int i) {
	vector<int> index;
	int x1 = random(0, NP - 1), x2 = random(0, NP - 1), x3 = random(0, NP - 1);
	while(x1 == i) x1 = random(0, NP - 1);
	while(x2 == x1 || x2 == i) x2 = random(0, NP - 1);
	while(x3 == x1 || x3 == x2 || x3 == i) x3 = random(0, NP - 1);
	index.push_back(x1);
	index.push_back(x2);
	index.push_back(x3);
	return index;
}

/* get index of other two random different individual */
vector<int> GetRandNum(int best, int i) {
	vector<int> index;
	int x1 = random(0, NP - 1), x2 = random(0, NP - 1);
	while (x1 == i || x1 == best) x1 = random(0, NP - 1);
	while (x2 == x1 || x2 == i || x2 == best) x2 = random(0, NP - 1);
	index.push_back(x1);
	index.push_back(x2);
	return index;
}

/* overload operator +, -, * to meet the need of individuals' vector calculating(only plus operation should check the bound) */
Individual operator + (const Individual A, const Individual B) {
	Individual temp_ind;
	for (int i = 0; i < D; ++i) {
		double value = A.pos[i] + B.pos[i];
		//if (value < LB || value > UB) value = random(LB, UB);
		if (value < LB) value = LB;
		if (value > UB) value = UB;
		temp_ind.pos.push_back(value);
	}
	return temp_ind;
}

Individual operator - (const Individual A, const Individual B) {
	Individual temp_ind;
	for (int i = 0; i < D; ++i) {
		double value = A.pos[i] - B.pos[i];
		temp_ind.pos.push_back(value);
	}
	return temp_ind;
}

Individual operator * (const double F, const Individual ind) {
	Individual temp_ind;
	for (int i = 0; i < D; ++i) {
		double value = F * ind.pos[i];
		temp_ind.pos.push_back(value);
	}
	return temp_ind;
}

/* print the individual with smallest fitness */
void Print() {
	cout << "Generation: " << gen << " best fitness: " << population[best_index].fitness << endl;
}