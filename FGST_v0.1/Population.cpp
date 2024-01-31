#include "Population.h"
#include <random>

Population::Population(int populationNumber, std::string archType, std::vector<float> ground) : populationNumber(populationNumber), archType(archType), ground(ground) {

	for (int i = 0; i < populationNumber; i++) {
		addTree();
	}
}

void Population::addTree() {

	std::random_device rd;
	std::mt19937 gen(rd());

	// Générer un entier aléatoire positif dans la plage [0, limite - 1]
	std::uniform_int_distribution<int> distribution(0, (ground.size()/8) - 1);
	int rdm = distribution(gen);
	rdm = rdm * 8;
	

	float randomPosX = ground[rdm];
	float randomPosY = ground[rdm+ 1];
	float randomPosZ = ground[rdm + 2];

	trees.push_back(Tree(glm::vec3(randomPosX, randomPosY, randomPosZ), glm::vec3(0.0f), glm::vec3(1.0f), archType));

}

void Population::render(Shader shader, bool line_mode, glm::mat4 scene_matrix) {
	
	for (int i = 0; i < trees.size(); i++) {
		trees[i].render(shader, line_mode, scene_matrix);
	}
}