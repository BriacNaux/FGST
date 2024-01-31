#include "Terrain.h"

Terrain::Terrain(const char* m_filePath, float m_max_height, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : Mesh(position, rotation, scale) {

	filePath = m_filePath;
	max_height = m_max_height;
	prev_mh = m_max_height;

	if (filePath != "") {
		initBuffers();

		//Create Flat map
		heightmap = process_heightmap(filePath);
		subdivide();

		//Process terrain with user inputs (does contain the update buffer)
		process_terrain(max_height);
	}
}

void Terrain::process_terrain(float maximum_height) {

	map_height(maximum_height);
	calculate_normals();
	updateTerrainsBuffer();
}

void Terrain::map_height(float maximum_height) {

	float height = 0;
	float mapped_height = 0;

	int pixel_iterator = 0;

	for (unsigned int i = 0; i < vertices.size() - 6; i++) {

		if (i % 8 == 1) {
			height = static_cast<float>(heightmap[pixel_iterator].value);
			mapped_height = (height * maximum_height) / 255;
			vertices[i] = mapped_height;

			pixel_iterator++;
		}
	}
}

void Terrain::subdivide() {
	//Vertices algorithm
	//Tres possible d'implementer le rectangulaire pour le coup ici
	int subdivide_value = image_size;

	float side_increment = static_cast<float>(1) / subdivide_value;

	for (float c = 0; c < 0.999; c += side_increment) {

		for (float r = 0; r < 0.999; r += side_increment) {


			glm::vec3 coordinates(c, 0.0f, r);

			vertices.push_back(coordinates.x - 0.5);
			vertices.push_back(coordinates.y);
			vertices.push_back(coordinates.z - 0.5);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}
	}

	//Indices algorithm
	bool alternator = false;

	std::vector<int> current_triangle = { 0, 1, subdivide_value };

	int prev_sv = subdivide_value - 1;

	indices.push_back(current_triangle[0]);
	indices.push_back(current_triangle[1]);
	indices.push_back(current_triangle[2]);

	for (int i = 0; i < ((6 * (prev_sv * prev_sv)) / 3) - 1; i++) {

		if (alternator == false) {

			current_triangle[0] += 1;
			current_triangle[1] += subdivide_value - 1;
			current_triangle[2] += 1;
		}
		if (alternator == true) {

			if (current_triangle[0] != 1 && current_triangle[0] != 2 && (current_triangle[0] - prev_sv) % subdivide_value == 0) {

				current_triangle[0] += 1;
				current_triangle[2] += 1;

				if (current_triangle[1] % subdivide_value == (subdivide_value - 2)) {
					current_triangle[1] -= subdivide_value - 3;
				}
			}
			else {

				current_triangle[1] -= subdivide_value - 2; //-3 ???
			}
		}

		indices.push_back(current_triangle[0]);
		indices.push_back(current_triangle[1]);
		indices.push_back(current_triangle[2]);

		alternator = !alternator;
	}
}

std::vector<GrayPixel> Terrain::process_heightmap(const char* filePath) {

	std::ifstream file(filePath, std::ios::binary);
	char header[54];
	file.read(header, 54);

	int width = *(int*)&header[18];
	int height = *(int*)&header[22];
	image_size = width;

	file.seekg(*(int*)&header[10], std::ios::beg);

	std::vector<GrayPixel> pixelValues;

	for (int i = 0; i < 4 * (width * height); i++) {
		GrayPixel pixel;
		file.read(reinterpret_cast<char*>(&pixel), sizeof(GrayPixel));

		if (i % 4 == 1) {
			pixelValues.push_back(pixel);
		}
	}

	file.close();
	return pixelValues;
}