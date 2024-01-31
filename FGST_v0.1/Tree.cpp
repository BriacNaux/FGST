#include "Tree.h"

Tree::Tree(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::string m_archType)
	: Model(position, rotation, scale) {

	archType = m_archType;

	//Variable ici : number of nodes per branches , 

	if (archType == "Leeuwenberg") {
		//Leeuwenberg sympodial setup
		addStem(Stem(0, glm::vec3(0.0f), rotation, scale, 3, 1, 0.01));

		nbrOfSplits = 1;
		decayStep = 0.1f;
		secondaryNbrOfSplits = 4;
		ageLimit = 250; //Can be computed every time to have more chance to die when time flies
	}

	if (archType == "monoLeeuwenberg") {
		//Leeuwenberg monosympodial setup
		addStem(Stem(0, glm::vec3(0.0f), rotation, scale, 3, 1, 0.01));

		nbrOfSplits = 1;
		decayStep = 0.14f;
		secondaryNbrOfSplits = 4;
		ageLimit = 250;
	}

	if (archType == "Massart") {
		//Massart setup
		addStem(Stem(0, glm::vec3(0.0f), rotation, scale, 13, 0.6, 0.01));

		nbrOfSplits = 12;
		secondaryNbrOfSplits = 3;
		decay = 1.0f;
		decayStep = 0.1f;
		ageLimit = 300;
	}

}

void Tree::addStem(Stem stem) {

	stems.push_back(stem);
	stemsMaturity.push_back(0);
	stemsState.push_back(0);
}

//Sympodial Version of Leeuwenberg
void Tree::buildLeeuwenberg() {
	
	age = age + 0.40;  //Weird factor ...

	if (age <= ageLimit) {
		
		for (int i = 0; i < stems.size(); i++) {

			int nbrOfNodes = stems[i].getStemIndicesSize() / 2;
			
			//The nmber 3 after getNbrOfNodes is the number of node of the stem before splitting up
			if (stems[i].getNbrOfNodes() == 3 && stemsMaturity[i] != nbrOfSplits) {

				//Compute the good position for the new stems using StemMatrix
				glm::mat4 newPos = glm::translate(stems[i].getStemMatrix(), stems[i].getMeristemPoint());
				glm::vec3 newPoint = glm::vec3(newPos[3]);


				std::vector<int> tempPath = stems[i].getPath();
				
				tempPath.push_back(i);

				addStem(Stem(stems[i].getGeneration() + 1, glm::vec3(newPoint.x, newPoint.y, newPoint.z), stems[i].getRotation() + glm::vec3(-30.0f, 0.0f, -15.0f), glm::vec3(1.0f), 3, stems[i].getInternodeLength() - decayStep, 0.03, tempPath));
				addStem(Stem(stems[i].getGeneration() + 1, glm::vec3(newPoint.x, newPoint.y, newPoint.z), stems[i].getRotation() + glm::vec3(30.0f, 0.0f, -15.0f), glm::vec3(1.0f), 3, stems[i].getInternodeLength() -decayStep, 0.03, tempPath));
				addStem(Stem(stems[i].getGeneration() + 1, glm::vec3(newPoint.x, newPoint.y, newPoint.z), stems[i].getRotation() + glm::vec3(0.0f, 0.0f, 30.0f), glm::vec3(1.0f), 3, stems[i].getInternodeLength() - decayStep, 0.03, tempPath));

				stemsMaturity[i]++;
			}
		}
	}
}

//Monosympodiale version of Leeuwenberg
void Tree::buildMonoLeeuwenberg() {

	age = age + 0.45;  //Weird factor ...

	if (age <= ageLimit) {

		for (int i = 0; i < stems.size(); i++) {

			if (stems[i].getNbrOfNodes() == 3 && stemsMaturity[i] != nbrOfSplits) {

				//Compute the good position for the new stems using StemMatrix
				glm::mat4 newPos = glm::translate(stems[i].getStemMatrix(), stems[i].getMeristemPoint());
				glm::vec3 newPoint = glm::vec3(newPos[3]);
				
				std::vector<int> tempPath = stems[i].getPath();
				
				tempPath.push_back(i);
				
				
				addStem(Stem(stems[i].getGeneration() + 1, glm::vec3(newPoint.x, newPoint.y, newPoint.z), stems[i].getRotation() + glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), 3, stems[i].getInternodeLength() - decayStep, 0.02, tempPath));
				addStem(Stem(stems[i].getGeneration() + 1, glm::vec3(newPoint.x, newPoint.y, newPoint.z), stems[i].getRotation() + glm::vec3(-50.0f, 0.0f, -25.0f), glm::vec3(1.0f), 3, stems[i].getInternodeLength() - decayStep, 0.02, tempPath));
				addStem(Stem(stems[i].getGeneration() + 1, glm::vec3(newPoint.x, newPoint.y, newPoint.z), stems[i].getRotation() + glm::vec3(50.0f, 0.0f, -25.0f), glm::vec3(1.0f), 3, stems[i].getInternodeLength() - decayStep, 0.02, tempPath));
				addStem(Stem(stems[i].getGeneration() + 1, glm::vec3(newPoint.x, newPoint.y, newPoint.z), stems[i].getRotation() + glm::vec3(0.0f, 0.0f, 50.0f),  glm::vec3(1.0f), 3, stems[i].getInternodeLength() - decayStep, 0.02, tempPath));

				stemsMaturity[i]++;
			}
		}
	}
}

void Tree::buildMassart() {

	age = age + 0.40;  //Weird factor ...

	//std::cout << age << std::endl;

	if (age <= ageLimit) {


		for (int i = 0; i < stems.size(); i++) {

			//Work on trunk
			if (i == 0) {



				
				if (stems[i].getNbrOfNodes() % 1 == 0 && stemsState[i] != stems[i].getNbrOfNodes() && stemsMaturity[i] != nbrOfSplits ) {
				
					//Compute the good position for the new stems using StemMatrix
					glm::mat4 newPos = glm::translate(stems[i].getStemMatrix(), stems[i].getMeristemPoint());
					glm::vec3 newPoint = glm::vec3(newPos[3]);

					std::vector<int> tempPath = stems[i].getPath();
					if (i != 0)
						tempPath.push_back(i);

					//PROBLEME AVEC LE SCALE , FAIT DES RESSAUT AUX NOEUDS A CAUSE DE 2nd CROISSANCE
					addStem(Stem(stems[i].getGeneration() + 1, glm::vec3(newPoint.x, newPoint.y, newPoint.z), stems[i].getRotation() + glm::vec3(-70.0f, -20.0f, -40.0f), glm::vec3(1.0f), 3, stems[i].getInternodeLength() - decayStep, 0.01, tempPath));
					addStem(Stem(stems[i].getGeneration() + 1, glm::vec3(newPoint.x, newPoint.y, newPoint.z), stems[i].getRotation() + glm::vec3(70.0f, 20.0f, -40.0f), glm::vec3(1.0f), 3, stems[i].getInternodeLength() - decayStep, 0.01, tempPath));
					addStem(Stem(stems[i].getGeneration() + 1, glm::vec3(newPoint.x, newPoint.y, newPoint.z), stems[i].getRotation() + glm::vec3(-90.0f, 20.0f, 90.0f), glm::vec3(1.0f), 3, stems[i].getInternodeLength() - decayStep, 0.01, tempPath));


					stemsMaturity[i]++;
					stemsState[i] = stems[i].getNbrOfNodes();
					//decay = decay + decayStep;
				}
			}

			//Work on side stems
			else {
				//Le deux correspond au nombre de noeud entre les splits
				if (stems[i].getNbrOfNodes() % 3 == 0 && stemsState[i] != stems[i].getNbrOfNodes() && stemsMaturity[i] != 1 && stems[i].getGeneration() < secondaryNbrOfSplits) {
					
					//Compute the good position for the new stems using StemMatrix
					glm::mat4 newPos = glm::translate(stems[i].getStemMatrix(), stems[i].getMeristemPoint());
					glm::vec3 newPoint = glm::vec3(newPos[3]);

					std::vector<int> tempPath = stems[i].getPath();
					tempPath.push_back(i);

					addStem(Stem(stems[i].getGeneration() + 1, glm::vec3(newPoint.x, newPoint.y, newPoint.z), stems[i].getRotation() + glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), 3, stems[i].getInternodeLength() - decayStep, 0.01, tempPath));
					addStem(Stem(stems[i].getGeneration() + 1, glm::vec3(newPoint.x, newPoint.y, newPoint.z), stems[i].getRotation() + glm::vec3(0.0f, 0.0f, 50.0f), glm::vec3(1.0f), 3, stems[i].getInternodeLength() - decayStep, 0.01, tempPath));
					addStem(Stem(stems[i].getGeneration() + 1, glm::vec3(newPoint.x, newPoint.y, newPoint.z), stems[i].getRotation() + glm::vec3(0.0f, 0.0f, -50.0f), glm::vec3(1.0f), 3, stems[i].getInternodeLength() - decayStep, 0.01, tempPath));

					stemsMaturity[i]++;
					stemsState[i] = stems[i].getNbrOfNodes();
				}
			}
		}
	}
}



void Tree::render(Shader shader, bool line_mode, glm::mat4 scene_matrix) {

	//put in split the iteration number
	if (archType == "Leeuwenberg") {
		buildLeeuwenberg();
	}
	else if (archType == "monoLeeuwenberg") {
		buildMonoLeeuwenberg();
	}
	else if (archType == "Massart") {
		buildMassart();
	}

	if (age <= ageLimit)
		stopped = false;
	else stopped = true;

	for (int i = 0; i < stems.size(); i++) {
			
			//CA C'EST PAS DU TOUT OPTIMISE !!!!!!!!!
			if (stems[i].getGeneration() == secondaryNbrOfSplits || stems[0].getNbrOfNodes() == 12) {

				float thin = ((float(rand()) / float(RAND_MAX)) * (1.0 - (0.0))) + (0.0);

				if (thin < 0.05) {

					//stems[i].setDead(true);

					//std::vector<int> tempPath = stems[i].getPath();

					//for (int i = 0; i < tempPath.size(); i++) {

						//stems[tempPath[i]].setDead(true);
					//}
					NULL;
				}
				else {

					stems[i].setStopped(true);

					std::vector<int> tempPath = stems[i].getPath();

					for (int i = 0; i < tempPath.size(); i++) {

						stems[tempPath[i]].setStopped(true);

						
						
						//stems[0].setStopped(true);
					}
				}
			}
	}
	

	for (int i = 0; i < stems.size(); i++) {

		updateMatrix(scene_matrix);

		if (stems[i].getStatus() == false)
			stems[i].render(shader, line_mode, ModelMatrix);
	}
}
