#pragma once
#include "Mesh.h"



class Model {
protected:
	std::vector<Mesh> meshes;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	
	glm::mat4 ModelMatrix;

public:
	Model(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
		: position(position), rotation(rotation), scale(scale) {

		updateMatrix();

	}
	~Model() {}

	void addMesh(Mesh mesh) {

		meshes.push_back(mesh);

	}

	void render(Shader shader, bool line_mode, glm::mat4 scene_matrix = glm::mat4(1.0f)) {

		updateMatrix(scene_matrix);

		for (int i = 0; i < meshes.size(); i++) {

			meshes[i].render(shader, line_mode, ModelMatrix);
		}
	}

	//update matrix bellow !
	void updateMatrix(glm::mat4 scene_matrix = glm::mat4(1.0f)) {

		ModelMatrix = glm::mat4(1.0f); //set mesh mat

		ModelMatrix = glm::translate(ModelMatrix, position); //object to this positon

		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); //rotate x
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); //rotate y
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); //rotate z

		ModelMatrix = glm::scale(ModelMatrix, scale); //scale

		ModelMatrix = scene_matrix * ModelMatrix;

	}

	//Setters
	void move(glm::vec3 offset) {
		position = position + offset;
	}

	void rotate(glm::vec3 offset) {
		rotation = rotation + offset;
		updateMatrix();

	}

	void scaleUp(glm::vec3 offset) {
		scale = scale + offset;
	}

	void setRotation(glm::vec3 newRotation) {
		rotation = newRotation;
		updateMatrix();
	}

	void setPosition(glm::vec3 newPosition) {
		position = newPosition;
	}

	void setScale(glm::vec3 newScale) {
		scale = newScale;
	}

	//Getters
	glm::vec3 getRotation() {
		return rotation;
	}

	glm::vec3 getPosition() {
		return position;
	}

	glm::vec3 getScale() {
		return scale;
	}
};