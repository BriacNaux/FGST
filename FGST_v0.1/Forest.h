#pragma once
#include "Population.h"

//Non physical class
class Forest {
private:
	std::vector<Population> populations;

public:
	Forest(int trees_nb, unsigned int fir_perc, unsigned int beech_perc, std::vector<float> ground = {}) {

		//Fir

		populations.push_back(Population(std::round(trees_nb * (fir_perc / static_cast<float>(100))), "Massart", ground));

		//Beech
		populations.push_back(Population(std::round(trees_nb * (beech_perc / static_cast<float>(100))), "monoLeeuwenberg", ground));

	}
	void render(Shader shader, bool line_mode, glm::mat4 scene_matrix) {
		for (int i = 0; i < populations.size(); i++) {

			populations[i].render(shader, line_mode, scene_matrix);
		}
	}
};