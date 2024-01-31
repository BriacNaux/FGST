#pragma once

#include<vector>
#include<cmath>

class Cube {
public:
	Cube() {};
	~Cube() {};
	std::vector<float> vertices = {
		//Positon			  //TexCoords		//Normals
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,   1.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,   1.0f, 1.0f,		0.0f, 0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,		0.0f, 0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,   1.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,	  1.0f, 1.0f,		0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,		0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,		-1.0f, 0.0f, 0.0f,

		0.5f, -0.5f, -0.5f,   0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.5f,  0.5f, -0.5f,   1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,   1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,   0.0f, 1.0f,		1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,   1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,   1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

		0.5f,  0.5f, -0.5f,   0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,   0.0f, 1.0f,		0.0f, 1.0f, 0.0f
	};

	std::vector<int> indices = {
		
			0, 3, 2,
			2, 1, 0,
			4, 5, 6,
			6, 7 ,4,

			11, 8, 9,
			9, 10, 11,
			12, 13, 14,
			14, 15, 12,
			
			16, 17, 18,
			18, 19, 16,
			20, 21, 22,
			22, 23, 20 
	};

	std::vector<float> getVertices() {
		return vertices;
	}

	std::vector<int> getIndices() {
		return indices;
	}

	int getVerticesSize() {
		return vertices.size();
	}

	int getIndicesSize() {
		return indices.size();
	}
};

class Plane {
public:
	Plane() {};
	~Plane() {};
	std::vector<float> vertices = {
		//Position			   //texCoord		//Normals
		-0.5f, -0.5f, 0.0f,		0.0f,1.0f,		0.0f,0.0f,0.0f,
		0.5f, 0.5f, 0.0f,		1.0f,0.0f,		0.0f,0.0f,0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f,0.0f,		0.0f,0.0f,0.0f,
		0.5f, -0.5f, 0.0f,		1.0f,1.0f,		0.0f,0.0f,0.0f
	};

	std::vector<int> indices = {
	0,1,2,
	0,1,3
	};

	std::vector<float> getVertices() {
		return vertices;
	}

	std::vector<int> getIndices() {
		return indices;
	}

	int getVerticesSize() {
		return vertices.size();
	}

	int getIndicesSize() {
		return indices.size();
	}
};

class Seedling {
public:
	Seedling() {};
	~Seedling() {};
	std::vector<float> vertices = {
		//Position			   //texCoord		//Normals
		0.0f, 0.0f, 0.0f,		0.0f,0.0f,		0.0f,0.0f,0.0f,
		0.0f, 1.0f, 0.0f,		0.0f,0.0f,		0.0f,0.0f,0.0f,
		1.0f, 2.0f, 0.0f,		0.0f,0.0f,		0.0f,0.0f,0.0f,
	};

	std::vector<int> indices = {
		0,1,
		1,2,
		2,0
	};

	std::vector<float> getVertices() {
		return vertices;
	}

	std::vector<int> getIndices() {
		return indices;
	}

	int getVerticesSize() {
		return vertices.size();
	}

	int getIndicesSize() {
		return indices.size();
	}
};

class Octogone {
public:
	Octogone() {
		for (int i = 0; i < 8; i++) {
			float angle = glm::two_pi<float>() * static_cast<float>(i) / static_cast<float>(8);

			vertices.push_back(cos(angle)); //IMPROOVABLE HERE REPETITION
			vertices.push_back(sin(angle));
			vertices.push_back(0.0f);

			vertices.push_back(0.0f);
			vertices.push_back(0.0f);

			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}
	};
	~Octogone() {};

	std::vector<float> vertices;

	std::vector<int> indices = {
	0,1,
	1,2,
	2,3,
	3,4,
	4,5,
	5,6,
	6,7,
	7,0
	};

	std::vector<float> getVertices() {
		return vertices;
	}

	std::vector<int> getIndices() {
		return indices;
	}

	int getVerticesSize() {
		return vertices.size();
	}

	int getIndicesSize() {
		return indices.size();
	}
};

class Primitive {

private:
	std::vector<float> vertices;
	std::vector<int>  indices;
public:
	Primitive(std::string type = "NONE", const char* filePath = "", float max_height = 0) {

		
		if (type == "cube") {

			Cube cube;
			vertices = cube.getVertices();
			indices = cube.getIndices();
		}
		else if (type == "seedling") {
			Seedling seedling;
			vertices = seedling.getVertices();
			indices = seedling.getIndices();
		}
		else if (type == "plane") {
			Plane plane;
			vertices = plane.getVertices();
			indices = plane.getIndices();
		}
		else if (type == "octogone") {
			Octogone octogone;
			vertices = octogone.getVertices();
			indices = octogone.getIndices();
		}/*
		else if (type == "terrain") {
			Terrain terrain(filePath, max_height);
			vertices = terrain.getVertices();
			indices = terrain.getIndices();

		}*/
		else if (type == "NONE") {
			vertices = {};
			indices = {};
		}
		else {
			std::cout << "Primitive doesn't exist" << std::endl;
		}
	}

	~Primitive() {}

	//Getters and setters
	std::vector<float> getVertices() {
		return vertices;
	}

	std::vector<int> getIndices() {
		return indices;
	}

	int getVerticesSize() {
		return vertices.size();
	}

	int getIndicesSize() {
		return indices.size();
	}

	int getLastIndex() {
		return indices.back();
	}

	void addIndex(int newIndice) {
		indices.push_back(newIndice);
	}

	//Section dedicated to stems//
	glm::vec3 getLastPoint() {
		return glm::vec3(vertices[vertices.size() - 8],
						 vertices[vertices.size() - 7],
						 vertices[vertices.size() - 6]);
	}

	glm::vec3 getBeforeLastPoint() {
		return glm::vec3(vertices[vertices.size() - 16],
						 vertices[vertices.size() - 15],
						 vertices[vertices.size() - 14]);
	}

	void setLastPoint(glm::vec3 newPos) {
		vertices[vertices.size() - 8] = newPos.x;
		vertices[vertices.size() - 7] = newPos.y;
		vertices[vertices.size() - 6] = newPos.z;
	}

	void setBeforeLastPoint(glm::vec3 newPos) {
		vertices[vertices.size() - 16] = newPos.x;
		vertices[vertices.size() - 15] = newPos.y;
		vertices[vertices.size() - 14] = newPos.z;

	}

	void addVertex(glm::vec3 position, glm::vec2 texcoord, glm::vec3 normal) {
		vertices.push_back(position.x);
		vertices.push_back(position.y);
		vertices.push_back(position.z);

		vertices.push_back(texcoord.x);
		vertices.push_back(texcoord.y);

		vertices.push_back(normal.x);
		vertices.push_back(normal.y);
		vertices.push_back(normal.z);
	}
};


