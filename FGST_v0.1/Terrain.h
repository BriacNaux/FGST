#pragma once
#include "Mesh.h"

struct GrayPixel {
    unsigned char value;
};

class Terrain : public Mesh {
private:
    const char* filePath;

    int image_size;

    std::vector<GrayPixel> heightmap;

    float max_height;

	float prev_mh;
	float prev_s;

public:
	Terrain(const char* m_filePath, float m_max_height, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    ~Terrain() {}

	void process_terrain(float maximum_height);

	void map_height(float maximum_height);

	void subdivide();

	std::vector<GrayPixel> process_heightmap(const char* filePath);

	//Getters
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