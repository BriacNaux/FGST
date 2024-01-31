#pragma once
#include"Mesh.h"
#include"Model.h"
#include"Terrain.h"
#include"Forest.h"



class Scene {
protected:
	std::vector<Mesh> sceneMeshes;
	std::vector<Model> sceneModels;
	
	Terrain terrain = Terrain("", 2, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(35.0f, 1.0f, 35.0f));
	Forest forest = Forest(0,0,0);

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	glm::mat4 SceneMatrix;

public:
	Scene() {

		updateMatrix();

	}
	~Scene() {}

	void addMesh(Mesh mesh) {
		sceneMeshes.push_back(mesh);
	}

	void addModel(Model model) {
		sceneModels.push_back(model);
	}

	void addTerrain(const char* filePath, float max_height, float width) {

		terrain = Terrain(filePath,
						  max_height,
						  glm::vec3(0.0f, 0.0f, 0.0f),
						  glm::vec3(0.0f, 0.0f, 0.0f),
						  glm::vec3(width, 1.0f, width));
	}

	void addForest(int trees_nb, unsigned int fir_perc, unsigned int beech_perc, std::vector<float> ground) {

		terrain.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));

		ground = terrain.udpateVerticesCoordinates();

		forest = Forest(trees_nb, fir_perc, beech_perc, ground);
		
	}


	void updateMatrix() {

		SceneMatrix = glm::mat4(1.0f); //set mesh mat

		SceneMatrix = glm::translate(SceneMatrix, position); //object to this positon

		SceneMatrix = glm::rotate(SceneMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); //rotate x
		SceneMatrix = glm::rotate(SceneMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); //rotate y
		SceneMatrix = glm::rotate(SceneMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); //rotate z

		SceneMatrix = glm::scale(SceneMatrix, scale); //scale
	}

	void render(Shader shader, bool line_mode) {

		updateMatrix();

		if (sceneMeshes.size() != 0) {
			for (int i = 0; i < sceneMeshes.size(); i++) {

				sceneMeshes[i].render(shader, line_mode, SceneMatrix);
			}
		}
		if (sceneModels.size() != 0) {
			for (int i = 0; i < sceneModels.size(); i++) {

				sceneModels[i].render(shader, line_mode, SceneMatrix);
			}
		}
		terrain.render(shader, line_mode, SceneMatrix);
		forest.render(shader, line_mode, SceneMatrix); //Include line mode and scenematrix
	}

	//Setters
	void move(glm::vec3 offset) {
		position = position + offset;
		updateMatrix();
	}

	void rotate(glm::vec3 offset) {
		rotation = rotation + offset;
		updateMatrix();
	}

	void scaleUp(glm::vec3 offset) {
		scale = scale + offset;
		updateMatrix();
	}

	void setRotation(glm::vec3 newRotation) {
		rotation = newRotation;
		updateMatrix();
	}

	void setPosition(glm::vec3 newPosition) {
		position = newPosition;
		updateMatrix();
	}

	void setScale(glm::vec3 newScale) {
		scale = newScale;
		updateMatrix();
	}

	void setTerrainScale(glm::vec3 newScale) {
		terrain.setScale(newScale);
	}

	//Getters
	int getTerrainIndices() {
		return terrain.getIndicesSize();
	}

	std::vector<float> getTerrainVertices() {
		return terrain.getVertices();
	}

	glm::vec3 getRotation() {
		return rotation;
	}

	glm::vec3 getPosition() {
		return position;
	}

	glm::vec3 getScale() {
		return scale;
	}

	Terrain getTerrain() {
		return	terrain;
	}
};