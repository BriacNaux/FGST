#pragma once
#include "Mesh.h"
#include <random>

class Stem : public Mesh {
private:

    //Construction variables
    glm::vec3 meristemPoint = glm::vec3(0.0f);
    glm::vec3 lastPoint;

    //for construction lines render mode
    std::vector<float> vertices;
    std::vector<int>  indices;

    //For full render mode
    std::vector<float> fullVertices;
    std::vector<int>  fullIndices;
    unsigned int fullVAO;
    unsigned int fullEBO;

    std::vector<float> ringsRadius;

    //Stem matrix is use to determine buds position
    glm::mat4 StemMatrix; 

    //can be choosen by user
    unsigned int nbrOfNodes;
    float internodeLength;
    float meristemBending;

    int generation; //Is not equal to biological age !!

    int NbrPreviousStems;
    std::vector<int> path;

    bool stopped = false;
    bool dead = false;
    

public:
    Stem(int m_generation, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, unsigned int m_nbrOfNodes, float m_internodeLength, float meristemBending, std::vector<int> m_path = {});

    ~Stem() {}

    void grow();

    void secondaryGrowth();

    void addNode();

    void addOctogone(glm::vec3 meristem_point);

    void initBuffers();

    void bindBuffers();

    void fullBindBuffers();

    //Grow func is the only thing that change from render func of mesh
    void render(Shader shader, bool line_mode, glm::mat4 model_matrix = glm::mat4(1.0f));

    void updateMatrix(glm::mat4 model_matrix = glm::mat4(1.0f));

    //Getters
    int getStemIndicesSize() {
        return indices.size();
    }
    
    float getInternodeLength() {
        return internodeLength;
    }

    unsigned int getNbrOfNodes() {
        return indices.size()/2;
    }

    glm::vec3 getMeristemPoint() {
        return meristemPoint;
    }

    glm::mat4 getStemMatrix() {
        return StemMatrix;
    }

    int getGeneration() {
        return generation;
    }

    std::vector<int> getPath() {
        return path;
    }

    bool getStatus() {
        return dead;
    }


    void setStopped(bool newState) {
        stopped = newState;
    }

    void setDead(bool newState) {
        dead = newState;
    }
};