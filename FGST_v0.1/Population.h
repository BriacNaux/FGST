#pragma once
#include "Tree.h"

//Non physical class, Very simple population class (intermediate between forest and tree class)
class Population {
private:

	std::vector<Tree> trees;

	int populationNumber;

	std::string archType;

	std::vector<float> ground;
public:

	Population(int populationNumber, std::string archType, std::vector<float> ground);
	~Population() {}

	void addTree();

	void render(Shader shader, bool line_mode, glm::mat4 scene_matrix);

};
