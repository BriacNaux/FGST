#pragma once

#include "Shader.h"
#include "Primitives.h"

class Mesh {

protected:
	Primitive primitive;
	
	std::vector<float> vertices;
	std::vector<int> indices;

	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	
	glm::mat4 MeshMatrix;
	glm::mat4 ModelMatrix;


public:
	Mesh(Primitive primitive, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
		: primitive(primitive), position(position), rotation(rotation), scale(scale) {

		initBuffers();

		updateMatrix();

	}
	
	//Constructor for Terrains and Stems
	Mesh(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
		: position(position), rotation(rotation), scale(scale) {

		initBuffers();

		if (primitive.getVerticesSize() != 0) {
			updateMatrix();
		}
		

	}
	~Mesh() {}

	void initBuffers() {

		//create VAO
		glGenVertexArrays(1, &VAO);

		//Create Buffers
		glGenBuffers(1, &VBO); 
		glGenBuffers(1, &EBO);
		glBindVertexArray(VAO);

		updateBuffers();

		//Link vertex attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //Texcoords
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))); //Normals
		glEnableVertexAttribArray(2);

		glBindVertexArray(0); //Debind
	}

	void updateBuffers() {

		//Bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, primitive.getVerticesSize() * sizeof(float), primitive.getVertices().data(), GL_STATIC_DRAW);
		//Bind VAO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, primitive.getIndicesSize() * sizeof(int), primitive.getIndices().data(), GL_STATIC_DRAW);
	}

	//dot this for stems too pleaase
	void updateTerrainsBuffer() {
		//Bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
		//Bind VAO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int),indices.data(), GL_STATIC_DRAW);

	}

	//Use to scatter
	std::vector<float> udpateVerticesCoordinates() {

		for (int i = 0; i < vertices.size(); i += 8) {

			glm::vec4 originalPosition(vertices[i], vertices[i + 1], vertices[i + 2], 1.0f);
			glm::vec4 transformedPosition = MeshMatrix * originalPosition;
			
			vertices[i] = transformedPosition.x;
			vertices[i + 1] = transformedPosition.y;
			vertices[i + 2] = transformedPosition.z;
		}
		return vertices;
	}

	void updateMatrix(glm::mat4 model_matrix = glm::mat4(1.0f)) {

		MeshMatrix = glm::mat4(1.0f); //set mesh mat
		
		MeshMatrix = glm::translate(MeshMatrix, position); //object to this positon
	
		MeshMatrix = glm::rotate(MeshMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); //rotate x
		MeshMatrix = glm::rotate(MeshMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); //rotate y
		MeshMatrix = glm::rotate(MeshMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); //rotate z
		
		MeshMatrix = glm::scale(MeshMatrix, scale); //scale
		
		MeshMatrix = model_matrix * MeshMatrix;
		
	}

	void calculate_normals() {
		int current_triangle = 0;

		for (int i = 0; i < indices.size() / 3; i++) {

			//Compute for the three points of the triangle the normals for each of them

			glm::vec3 A = glm::vec3(vertices[indices[current_triangle] * 8], vertices[(indices[current_triangle] * 8) + 1], vertices[(indices[current_triangle] * 8) + 2]);
			glm::vec3 B = glm::vec3(vertices[indices[current_triangle + 1] * 8], vertices[(indices[current_triangle + 1] * 8) + 1], vertices[(indices[current_triangle + 1] * 8) + 2]);
			glm::vec3 C = glm::vec3(vertices[indices[current_triangle + 2] * 8], vertices[(indices[current_triangle + 2] * 8) + 1], vertices[(indices[current_triangle + 2] * 8) + 2]);

			glm::vec3 AB = glm::vec3(B - A);
			glm::vec3 AC = glm::vec3(C - A);
			glm::vec3 BA = glm::vec3(A - B);
			glm::vec3 BC = glm::vec3(C - B);
			glm::vec3 CA = glm::vec3(A - C);
			glm::vec3 CB = glm::vec3(B - C);

			glm::vec3 Anorm = glm::normalize(glm::cross(AB, AC));
			glm::vec3 Bnorm = glm::normalize(glm::cross(BA, BC));
			glm::vec3 Cnorm = glm::normalize(glm::cross(CA, CB));

			vertices[(indices[current_triangle] * 8) + 5] = Anorm.x;
			vertices[(indices[current_triangle] * 8) + 6] = Anorm.y;
			vertices[(indices[current_triangle] * 8) + 7] = Anorm.z;

			vertices[(indices[current_triangle + 1] * 8) + 5] = Bnorm.x;
			vertices[(indices[current_triangle + 1] * 8) + 6] = Bnorm.y;
			vertices[(indices[current_triangle + 1] * 8) + 7] = Bnorm.z;

			vertices[(indices[current_triangle + 2] * 8) + 5] = Cnorm.x;
			vertices[(indices[current_triangle + 2] * 8) + 6] = Cnorm.y;
			vertices[(indices[current_triangle + 2] * 8) + 7] = Cnorm.z;

			current_triangle = current_triangle + 3;
		}
	}

	void render(Shader shader, bool line_mode = false, glm::mat4 model_matrix = glm::mat4(1.0f)) {
		
		updateMatrix(model_matrix);

		shader.use();

		//Send matrix
		shader.setMat4("MeshMatrix", MeshMatrix);

		glBindVertexArray(VAO); //Bind le vao au vbo

		//Draw Elements Array
		if (line_mode == true) {
			
			if (primitive.getVerticesSize() != 0) {
				glDrawElements(GL_LINES, primitive.getIndicesSize(), GL_UNSIGNED_INT, 0);
			}
			else {
				glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
			}
			
			//glDrawElements(GL_POINTS, primitive.getIndicesSize(), GL_UNSIGNED_INT, 0);
		}
		else {

			if (primitive.getVerticesSize() != 0) {
				glDrawElements(GL_TRIANGLES, primitive.getIndicesSize(), GL_UNSIGNED_INT, 0);
			}
			else {
				glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			}
		}

		//Cleanup
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

	void setModelMatrix(glm::mat4 model_matrix) {
		ModelMatrix = model_matrix;
	}

	
	//Getters
	int getMeshIndicesSize() {
		return primitive.getIndicesSize();
	}

	int getMeshVerticesSize() {
		return primitive.getVerticesSize();
	}

	glm::vec3 getRotation() {
		return rotation;
	}

	glm::vec3 getScale() {
		return scale;
	}
	
	glm::vec3 getPosition() {
		return position;
	}

	Primitive getPrimitive() {
		return primitive;
	}
};

