#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <random>
#include <cstring>

namespace GeneticAlgorithm {
    constexpr int POPULATION_SIZE = 100;
    constexpr const char* GENES = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOP"
                                   "QRSTUVWXYZ 1234567890 ";
    constexpr const char* TARGET = "I love Applied Data Structures";

    int random_num(int start, int end) {
        // Use a more modern random number generator
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> distribution(start, end);
        return distribution(rng);
    }

    char mutated_genes() {
        int len = std::char_traits<char>::length(GENES);
        int r = random_num(0, len - 1);
        return GENES[r];
    }

    std::string create_gnome() {
        int len = std::char_traits<char>::length(TARGET);
        std::string gnome = "";
        for (int i = 0; i < len; i++)
            gnome += mutated_genes();
        return gnome;
    }

    class Individual {
    public:
        std::string chromosome;
        int fitness;

        explicit Individual(std::string chromosome);

        Individual mate(const Individual& parent2);

        int cal_fitness() const;
    };

    bool operator<(const Individual& ind1, const Individual& ind2) {
        return ind1.fitness < ind2.fitness;
    }
}

GeneticAlgorithm::Individual::Individual(std::string chromosome) : chromosome(std::move(chromosome)), fitness(cal_fitness()) {}

GeneticAlgorithm::Individual GeneticAlgorithm::Individual::mate(const Individual& par2) {
    std::string child_chromosome = "";

    int len = chromosome.size();
    for (int i = 0; i < len; i++) {
        float p = static_cast<float>(random_num(0, 100)) / 100;

        if (p < 0.45)
            child_chromosome += chromosome[i];
        else if (p < 0.90)
            child_chromosome += par2.chromosome[i];
        else
            child_chromosome += mutated_genes();
    }

    return Individual(child_chromosome);
}

int GeneticAlgorithm::Individual::cal_fitness() const {
    int len = std::char_traits<char>::length(TARGET);
    int fitness = 0;
    for (int i = 0; i < len; i++) {
        if (chromosome[i] != TARGET[i])
            fitness++;
    }
    return fitness;
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    int generation = 0;
    std::vector<GeneticAlgorithm::Individual> population;
    bool found = false;

    for (int i = 0; i < GeneticAlgorithm::POPULATION_SIZE; i++) {
        std::string gnome = GeneticAlgorithm::create_gnome();
        population.emplace_back(gnome);
    }

    while (!found) {
        std::sort(population.begin(), population.end());

        if (population[0].fitness <= 0) {
            found = true;
            break;
        }

        std::vector<GeneticAlgorithm::Individual> new_generation;
        int s = (10 * GeneticAlgorithm::POPULATION_SIZE) / 100;
        for (int i = 0; i < s; i++)
            new_generation.emplace_back(population[i]);

        s = (90 * GeneticAlgorithm::POPULATION_SIZE) / 100;
        for (int i = 0; i < s; i++) {
            int len = population.size();
            int r = GeneticAlgorithm::random_num(0, 50);
            GeneticAlgorithm::Individual parent1 = population[r];
            r = GeneticAlgorithm::random_num(0, 50);
            GeneticAlgorithm::Individual parent2 = population[r];
            GeneticAlgorithm::Individual offspring = parent1.mate(parent2);
            new_generation.emplace_back(offspring);
        }

        population = new_generation;
        std::cout << "Generation: " << generation << "\t";
        std::cout << "String: " << population[0].chromosome << "\t";
        std::cout << "Fitness: " << population[0].fitness << "\n";

        generation++;
    }

    std::cout << "Generation: " << generation << "\t";
    std::cout << "String: " << population[0].chromosome << "\t";
    std::cout << "Fitness: " << population[0].fitness << "\n";

    return 0;
}

