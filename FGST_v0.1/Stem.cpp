#include "Stem.h"


//Constructor
Stem::Stem(int m_generation, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, unsigned int m_nbrOfNodes, float m_internodeLength, float m_meristemBending, std::vector<int> m_path)
    : Mesh(position, rotation, scale) {

    initBuffers();

    path = m_path;
    generation = m_generation; //Init here bc seems to not work in the init list :/
    nbrOfNodes = m_nbrOfNodes;
    internodeLength = m_internodeLength;
    meristemBending = m_meristemBending;

    addNode();
    addNode();

    //reset indices bc addNode interfered but we don't want to (dirty)
    indices = { 0,1 };
    

}


void Stem::grow() {
    if (indices.size() / 2 < nbrOfNodes) {

        //Update the meristem
        meristemPoint.x = meristemPoint.x + ((float(rand()) / float(RAND_MAX)) * (meristemBending - (-meristemBending))) + (-meristemBending); //Ici natural orientation variable ? FONDAMENTAL
        meristemPoint.y = meristemPoint.y + 0.01;
        meristemPoint.z = meristemPoint.z + ((float(rand()) / float(RAND_MAX)) * (meristemBending - (-meristemBending))) + (-meristemBending);

        //Store the new meristem coordinates in the last point of the stem mesh
        vertices[vertices.size() - 8] = meristemPoint.x;
        vertices[vertices.size() - 7] = meristemPoint.y;
        vertices[vertices.size() - 6] = meristemPoint.z;

        //Store the new octogone by erasing the precedent
        //We see taht we use two different technique to draw current time meristem for line and full mode, replacing and delating / adding
        if (fullVertices.empty() != true) {
            for (int i = 0; i < 64; i++)
                fullVertices.pop_back();

            for (int i = 0; i < 48; i++)
                fullIndices.pop_back();
        }
        addOctogone(meristemPoint);
        
        //create a vector between the meristem and the previous point of the stem mesh
        glm::vec3 meristemSeg = meristemPoint - lastPoint;

        //la valeur 1 correspond a la longueur de chaque entrenoeud
        if (glm::abs(glm::length(meristemSeg) - internodeLength) < 0.1f) {
            addNode();
        }
    }
}

void Stem::addNode() {
    
    //Add vertex to the primitive
    vertices.push_back(meristemPoint.x);
    vertices.push_back(meristemPoint.y);
    vertices.push_back(meristemPoint.z);

    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    //Add index to the primitive
    if (indices.empty()) { //Cases when it's empy
        indices.push_back(0);
        indices.push_back(1);
    }
    else {
        indices.push_back(indices.back());
        indices.push_back(indices.back() + 1);
    }
    lastPoint = meristemPoint;

    //Full mode octogone push_back
    addOctogone(meristemPoint);
 
}

void Stem::addOctogone(glm::vec3 meristem_point) {

    ringsRadius.push_back(0.01); //add a new radius in the vector

    //Create the ring of the node
    for (int i = 0; i < 8; i++) {
        float angle = glm::two_pi<float>() * i / (static_cast<float>(8));

        fullVertices.push_back(meristemPoint.x + (cos(angle) * 0.01));
        fullVertices.push_back(meristemPoint.y);
        fullVertices.push_back(meristemPoint.z + (sin(angle) * 0.01));

        fullVertices.push_back(0.0f);
        fullVertices.push_back(0.0f);

        fullVertices.push_back(0.0f);
        fullVertices.push_back(0.0f);
        fullVertices.push_back(0.0f);

    }   

    //Compute indices
    int tempIndex = ((vertices.size() / 8)-2) * 8; 

    fullIndices.push_back(tempIndex);

        for (int i = 0; i < 8; i++) {

            if (i != 7) {
                tempIndex += 1;
                fullIndices.push_back(tempIndex);

                tempIndex += 7;
                fullIndices.push_back(tempIndex);

                tempIndex -= 7;
                fullIndices.push_back(tempIndex);

                tempIndex += 7;
                fullIndices.push_back(tempIndex);

                tempIndex += 1;
                fullIndices.push_back(tempIndex);

                tempIndex -= 8;
                fullIndices.push_back(tempIndex);
            }
            else {
                tempIndex -= 7;
                fullIndices.push_back(tempIndex);

                tempIndex += 15;
                fullIndices.push_back(tempIndex);

                tempIndex -= 15;
                fullIndices.push_back(tempIndex);

                tempIndex += 15;
                fullIndices.push_back(tempIndex);

                tempIndex -= 7;
                fullIndices.push_back(tempIndex);
            }
        }
}

void Stem::secondaryGrowth() {
    //For all the hexagones, increment by the growing factor
    for (int h = 0; h < indices.size() / 2; h++) {

        int hex = h * 64;

        for (int s = 0; s < 8; s++) {

            float angle = glm::two_pi<float>() * s / (static_cast<float>(8));

            fullVertices[hex] = vertices[h * 8] + (cos(angle) * ringsRadius[h]);

            fullVertices[hex + 2] = vertices[(h * 8) + 2] + (sin(angle) * ringsRadius[h]);

            hex += 8;
        }
        ringsRadius[h] += 0.00020; //growing factor
    }
}

void Stem::initBuffers() {

    //create VAO
    glGenVertexArrays(1, &VAO);

    //Create Buffers
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    bindBuffers();

    //Link vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //Texcoords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))); //Normals
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); //Debind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &fullVAO);
    glGenBuffers(1, &fullEBO);

    glBindVertexArray(fullVAO);

    fullBindBuffers();

    //Link vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //Texcoords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))); //Normals
    glEnableVertexAttribArray(2);
}

void Stem::bindBuffers() {
    //Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    //Bind VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
}

void Stem::fullBindBuffers() {
    //Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, fullVertices.size() * sizeof(float), fullVertices.data(), GL_STATIC_DRAW);
    //Bind VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fullEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, fullIndices.size() * sizeof(int), fullIndices.data(), GL_STATIC_DRAW);
}

//Grow func is the only thing that change from render func of mesh
void Stem::render(Shader shader, bool line_mode, glm::mat4 model_matrix) {

    grow();

    if (stopped == false)
        secondaryGrowth();

    updateMatrix(model_matrix);
    shader.use();
    shader.setMat4("MeshMatrix", MeshMatrix);

    if (line_mode == true) {

        //Line mode
        fullBindBuffers();
        glBindVertexArray(fullVAO);
        glDrawElements(GL_LINES, fullIndices.size(), GL_UNSIGNED_INT, 0);
    }
    else {
        fullBindBuffers();
        glBindVertexArray(fullVAO);

        glDrawElements(GL_TRIANGLES, fullIndices.size(), GL_UNSIGNED_INT, 0);

    }
   
    //Cleanup
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void Stem::updateMatrix(glm::mat4 model_matrix) {

    StemMatrix = glm::mat4(1.0f); //set mesh mat

    StemMatrix = glm::translate(StemMatrix, position); //object to this positon

    StemMatrix = glm::rotate(StemMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); //rotate x
    StemMatrix = glm::rotate(StemMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); //rotate y
    StemMatrix = glm::rotate(StemMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); //rotate z
    
    StemMatrix = glm::scale(StemMatrix, scale); //scale

    MeshMatrix = glm::mat4(1.0f); //set mesh mat

    MeshMatrix = glm::translate(MeshMatrix, position); //object to this positon

    MeshMatrix = glm::rotate(MeshMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); //rotate y
    MeshMatrix = glm::rotate(MeshMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); //rotate y
    MeshMatrix = glm::rotate(MeshMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); //rotate z

    MeshMatrix = glm::scale(MeshMatrix, scale); //scale

    MeshMatrix = model_matrix * MeshMatrix;

} 