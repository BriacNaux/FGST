#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include <glew.h>
#include <glfw3.h>

#include "Camera.h"
#include "Texture.h"
#include "Primitives.h"
#include "Mesh.h"
#include "Model.h"
#include "Terrain.h"
#include "Scene.h"
#include <iostream>

#include <fstream>
#include <filesystem>
#include <vector>

//NOTES FOR MYSELF ------------------------------------------------------------------------------
//Camera yaw or pitch or smth ne revient pas normal quand re en edit mode
//CREER UN TOOL QUI PERMET DE SLIDER DANS LES ARRAY QUAND MEME ... Tool de traitement d'indces ?
//Algorithme d'anti crenelage du terrain
//Faire une class light + fix la light
//Collisions
//Tool poiur mieux gerer les shaders et leur glsl associées
//Zoom plus rapide
//rearragner les variables et creer plus de fonctions dans le main
//Bug de regeneration quand appui deux fois sur generate forest
//-----------------------------------------------------------------------------------------------
//Declare functions//
void searchForFiles();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, Scene &m_scene);

//Setup const//-------------------------------------------------------
//Global
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
float bg_color[] = { 1.0f,1.0f,1.0f, 1.0f };
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;
bool lineMode = false;
bool editMode = true;
//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 50.0f));
const float cameraSpeed = 0.05f;
//Mouse
double xcenter, ycenter;
bool firstMouse = true;
bool mouseRot = true;
glm::vec3 lastRot;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
//Terrain
std::vector<const char*> heightmaps;
const char* heightmap;
float terrainColorRGB[] = { 1.0f,0.2f,0.0f, 1.0f };
float max_height = 2;
float size = 35;
float prev_mh = max_height;
float prev_s = size;
//Forest
int trees_nb = 7;


//MAIN FUNCTION//------------------------------------------------------
int main() {

    //Init GLFW//
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Init Window//
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Forest Simulation feat Bernard Arnauld", NULL, NULL); //Retourne un pointeur de class GLFWwindow d'une nouvelle window
    if (window == NULL) //Check error
    {
        std::cout << "ERROR::GLFWWINDOW_CREATION_FAILED" << std::endl;
        glfwTerminate();
        return -1;
    }

    //Make window our current context (Init OpenGL context too)//
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glLineWidth(1.0f);
    glPointSize(2.0f);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    //Init GLEW//
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "ERROR::GLEW_INIT_FAILED" << "\n";
        glfwTerminate();
    }
    glEnable(GL_DEPTH_TEST);

    //init ImGUI//
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsLight();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 440");

    //Init files//
    searchForFiles();
    heightmap = heightmaps[0];

    //Set up the shader program with the shader function of the shader header file//
    //FIRST SHADER
    Shader colorOnlyShader("vertex_shader_2.glsl", "fragment_shader_2.glsl");

    colorOnlyShader.use();
    colorOnlyShader.setVec3("objectColor", glm::vec3(1.0f, 0.1f, 0.0f));
    colorOnlyShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    glUseProgram(0);

    //SECOND SHADER FOR LIGHT
    Shader lightSourceShader("vertex_shader_4.glsl", "fragment_shader_4.glsl");
    glm::vec3 lightPos = glm::vec3(-400.0f, -100.0f, 0.0f);
    Shader screenShader("framebuffers_screen_vs.glsl", "framebuffers_screen_fs.glsl");

    screenShader.use();
    screenShader.setInt("screenTexture", 0);

    // Config FrameBuffer//
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    //Primitive set and Meshes, Model and Scene declaration --------------------
    //Scene
    Scene simulation;
    
    //Light
    Primitive Pcube("cube");
    Mesh light(Pcube, lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f));

    //Others
    Mesh cube(Pcube, glm::vec3(-8.0f, 8.0f, -8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f));

    //GAME LOOP//
    while (!glfwWindowShouldClose(window)) {

        //Variables
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

       
        //Frame Buffer
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glEnable(GL_DEPTH_TEST);

        //Clear
        glClearColor(bg_color[0], bg_color[1], bg_color[2], bg_color[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // create camera view and projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
        glm::mat4 view = camera.GetViewMatrix();

        //sent cam view and projection to shaders------------------------------
        colorOnlyShader.sendUniforms(projection, view);
        lightSourceShader.sendUniforms(projection, view);

        lightSourceShader.setVec3("lightPos", light.getPosition());
        colorOnlyShader.setVec3("lightPos", light.getPosition());
        colorOnlyShader.setVec3("viewPos", camera.Position);
        
        //RENDER OBJECTS
        light.render(lightSourceShader, false);
        simulation.render(colorOnlyShader, lineMode);
        
        //Frame buffers
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        screenShader.use();

        //GUI rendering//
        //Setting the window
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImVec2(400.0f, 1000.0f));

        ImGui::Begin("PARAMETERS");
        ImGui::SeparatorText("TERRAIN CONFIGURATION");
       
        ImGui::BeginListBox(".", ImVec2(360.0f, 120.0f));
        if (ImGui::TreeNodeEx("heightmap bitmap image", ImGuiTreeNodeFlags_DefaultOpen)) {
            static int selected = 0;
            for (int n = 0; n < heightmaps.size(); n++)
            {
                if (ImGui::Selectable(heightmaps[n], selected == n)) {
                    selected = n;
                    heightmap = heightmaps[selected];
                }
            }
            ImGui::TreePop();
        }
        ImGui::EndListBox();

        ImGui::SliderFloat("Maximum height", &max_height, 0.0f, 20.0f);
        ImGui::SliderFloat("Size", &size, 1.0f, 80.0f);
        ImGui::ColorEdit3("Terrain Color",terrainColorRGB);

        if (ImGui::Button("(Re)generate Terrain", ImVec2(360, 30))) {
            
            std::string hmp(heightmap);
            std::string filePath = "heightmaps/" + hmp + ".bmp";
            const char* filePathC = filePath.c_str();
            simulation.addTerrain(filePathC, max_height, size);
        }

        ImGui::SeparatorText("FOREST CONFIGURATION");

        //ImGui::Text("Explanations");
        //ImGui::TextColored(ImVec4(0, 0.4, 0, 1), "Future mportant stuff about forest");

        ImGui::SliderInt("Trees number", &trees_nb, 1, 80);

        ImGui::TextColored(ImVec4(1.0f, 0.2f, 0, 1), "Ensure to have a terrain generated to start simulation ");
        if (ImGui::Button("Start Simulation", ImVec2(360, 30))) {
            
            if (simulation.getTerrainIndices() != 0) {
                simulation.addForest(trees_nb, 33, 77, simulation.getTerrainVertices());
            }
        }
        ImGui::End();

        //Render Window
        ImGui::SetNextWindowPos(ImVec2(400.0f, 0.0f));
        ImGui::SetNextWindowSize(ImVec2(1520.0f, 1000.0f));

        ImGui::Begin("VIEWPORT");
        ImGui::SeparatorText("VISUAL AND GRAPHICS CONFIGURATION");

        ImGui::ColorEdit3("Background color", bg_color, ImGuiColorEditFlags_NoInputs);
        ImGui::SameLine();
        
        ImGui::Checkbox("Enable line mode", &lineMode);
        ImGui::SameLine();

        if (ImGui::Button("Toggle game mode", ImVec2(210, 22))) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            simulation.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
            editMode = false;
        }
        ImGui::SameLine();

        if (editMode == false) 
            ImGui::TextColored(ImVec4(1, 0.2, 0, 1), "'CTRL + E' TO RETURN EDIT MODE");
        else ImGui::TextColored(ImVec4(0, 0.6, 0.2, 1), "OR 'CTRL + G'");
        ImGui::NewLine();

        ImGui::BeginChild("GameRender");
        ImVec2 wsize = ImGui::GetWindowSize();
        ImGui::Image((ImTextureID)textureColorbuffer, wsize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::EndChild();
        ImGui::End();
       
        //ImGui rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
       
        //Updates
        processInput(window, simulation);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwPollEvents();
        glfwSwapBuffers(window);
        
        //Update Terrain shape and color
        if (max_height != prev_mh || size != prev_s) {
            simulation.setTerrainScale(glm::vec3(size, max_height, size));
        }
        prev_mh = max_height;
        prev_s = size;

        colorOnlyShader.use(); 
        colorOnlyShader.setVec3("objectColor", glm::vec3(terrainColorRGB[0], terrainColorRGB[1], terrainColorRGB[2]));
        glUseProgram(0);
    }


    //ImGui deleting
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    //End program
    glfwTerminate();
    return 0;
}



//UTILS FUNCTIONS//
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, Scene &m_scene) {
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //Mode
    if (editMode == true && glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        m_scene.setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
        editMode = false;
    }

    if (editMode == false && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPos(window, SCR_WIDTH / 2, SCR_HEIGHT / 2);
        editMode = true;
        
        camera.setPosition(glm::vec3(0.0f,0.0f,50.0f));
        camera.reset();
    }

    //Rotate on right click
    if (editMode == true) {

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {

            if (mouseRot == true) {

                glfwGetCursorPos(window, &xcenter, &ycenter);
                mouseRot = false;
                lastRot = m_scene.getRotation();
            }

            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            float xoffset = xpos - xcenter;
            float yoffset = ypos - ycenter; 
            
            m_scene.setRotation(glm::vec3(lastRot.x + yoffset, lastRot.y + xoffset, 0.0f));
        }
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
        mouseRot = true;
    }
    
    //3D movment with ZQSD
    if (editMode == false) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    if (editMode == false) {
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void searchForFiles() {
    //Heightmaps :
    std::filesystem::path path = "heightmaps";
    std::vector<std::string> heightmapsStr;

    if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            heightmapsStr.push_back(entry.path().filename().string());
        }
    }

    for (int i = 0; i < heightmapsStr.size(); i++) {
        size_t newSize = heightmapsStr[i].size() - 4;
        std::string newString = heightmapsStr[i].substr(0, newSize);
        heightmaps.push_back(_strdup(newString.c_str()));
    }
}