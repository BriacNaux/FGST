#pragma once
#include "Model.h"
#include "Stem.h"


class Tree : public Model {
protected : 

	//Tree system works with three vectors,
	//one containing stems and the others containing stem maturity and actual state.
	std::vector<Stem> stems;
	std::vector<unsigned int> stemsMaturity; //Use to know if the stem have to stop splitting. (split enough)
	std::vector<unsigned int> stemsState; //Stem state is used to know if at a given nbrOFnodes, the stem already splitted or not (prevent from splitting indefinitely) (split good)

	//Can be choosen by user
    std::string archType;
	unsigned int limit;
	unsigned int nbrOfSplits; 
	unsigned int secondaryNbrOfSplits;

	float decay; //not sure of this one
	float decayStep;
	
	float age = 0;
	unsigned int ageLimit;
	bool stopped; //Signal to stop secondary growth
	
	float competitionFactor;

public:	
	Tree(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::string m_archType);

	~Tree() {}

	void addStem(Stem stem);
	
	void buildLeeuwenberg();

	void buildMonoLeeuwenberg();

	void buildMassart();
	
	void render(Shader shader, bool line_mode, glm::mat4 scene_matrix);

};

