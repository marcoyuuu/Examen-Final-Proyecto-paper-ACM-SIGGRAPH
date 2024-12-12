#include "Scene.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>

Scene::Scene() 
    : spotlight(glm::vec3(0.0f, 12.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.0f,-1.0f,0.0f)),
      skyboxVAO(0), skyboxVBO(0)
{
}

Scene::~Scene()
{
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
}

void Scene::Setup()
{
    // Setup skybox
    std::vector<std::string> faces = {
        "assets/textures/skybox/right.jpg",
        "assets/textures/skybox/left.jpg",
        "assets/textures/skybox/top.jpg",
        "assets/textures/skybox/bottom.jpg",
        "assets/textures/skybox/front.jpg",
        "assets/textures/skybox/back.jpg"
    };
    skyboxTexture = loadCubemap(faces);
    createSkybox();

    // Initialize lighthouse
    lighthouse = std::make_unique<Lighthouse>();
    lighthouse->Setup();

    // Initialize ground plane
    groundPlane.Setup();

    // Setup lights
    // Directional Light (like the sun)
    dirLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    dirLight.ambient = glm::vec3(0.1f,0.1f,0.1f);
    dirLight.diffuse = glm::vec3(0.5f,0.5f,0.5f);
    dirLight.specular = glm::vec3(1.0f,1.0f,1.0f);

    // Point Lights
    pointLights.push_back(PointLightData{
        glm::vec3(10.0f,5.0f,10.0f),
        glm::vec3(0.05f),
        glm::vec3(0.8f,0.8f,0.7f),
        glm::vec3(1.0f),
        1.0f,0.09f,0.032f
    });

    pointLights.push_back(PointLightData{
        glm::vec3(-10.0f,10.0f,-10.0f),
        glm::vec3(0.05f),
        glm::vec3(0.7f,0.3f,0.3f),
        glm::vec3(1.0f),
        1.0f,0.09f,0.032f
    });

    pointLights.push_back(PointLightData{
        glm::vec3(0.0f,20.0f,0.0f),
        glm::vec3(0.05f),
        glm::vec3(0.3f,0.7f,0.9f),
        glm::vec3(1.0f),
        1.0f,0.09f,0.032f
    });
}

bool Scene::isSphereInFrustum(const glm::vec3 &center, float radius, const glm::mat4 &vpMatrix) const
{
    glm::vec4 planes[6];
    // Extract frustum planes
    planes[0] = vpMatrix[0] + vpMatrix[3]; // Left
    planes[1] = vpMatrix[3] - vpMatrix[0]; // Right
    planes[2] = vpMatrix[1] + vpMatrix[3]; // Bottom
    planes[3] = vpMatrix[3] - vpMatrix[1]; // Top
    planes[4] = vpMatrix[2] + vpMatrix[3]; // Near
    planes[5] = vpMatrix[3] - vpMatrix[2]; // Far

    // Normalize
    for(auto &plane : planes)
    {
        float length = glm::length(glm::vec3(plane));
        plane /= length;
    }

    // Test sphere against planes
    for(const auto& plane : planes)
    {
        float distance = glm::dot(glm::vec3(plane), center) + plane.w;
        if(distance < -radius)
            return false;
    }
    return true;
}

unsigned int Scene::loadCubemap(const std::vector<std::string>& faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width,height,nrChannels;
    stbi_set_flip_vertically_on_load(false);
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width,&height,&nrChannels,0);
        if(data)
        {
            GLenum format = GL_RGB;
            if (nrChannels == 1) format = GL_RED;
            else if(nrChannels==3) format=GL_RGB;
            else if(nrChannels==4) format=GL_RGBA;

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,format,width,height,0,format,GL_UNSIGNED_BYTE,data);
            stbi_image_free(data);
        }
        else
        {
            std::cerr<<"Cubemap texture failed to load at path: "<<faces[i]<<std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    return textureID;
}

void Scene::createSkybox()
{
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices),skyboxVertices,GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glBindVertexArray(0);
}

void Scene::Render(Shader &shader, const Camera &camera, Shader &skyboxShader)
{
    // === Render Skybox First ===
    glDepthFunc(GL_LEQUAL);
    skyboxShader.use();
    glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.GetViewMatrix()));
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 1000.0f);
    skyboxShader.setMat4("view", skyboxView);
    skyboxShader.setMat4("projection", projection);
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    skyboxShader.setInt("skybox",0);
    glDrawArrays(GL_TRIANGLES,0,36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);

    // === Set Up Lighting and Material Uniforms for main scene shader ===
    shader.use();
    shader.setMat4("view", camera.GetViewMatrix());
    shader.setMat4("projection", projection);
    shader.setVec3("viewPos", camera.Position);

    // SpotLight parameters (from the lighthouse):
    shader.setVec3("spotLight.position", spotlight.getPosition());
    shader.setVec3("spotLight.direction", spotlight.getDirection());
    shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
    shader.setVec3("spotLight.ambient", glm::vec3(0.1f));
    shader.setVec3("spotLight.diffuse", glm::vec3(0.8f));
    shader.setVec3("spotLight.specular", glm::vec3(1.0f));
    shader.setFloat("spotLight.constant",1.0f);
    shader.setFloat("spotLight.linear",0.09f);
    shader.setFloat("spotLight.quadratic",0.032f);

    // Directional Light parameters
    shader.setVec3("dirLight.direction", dirLight.direction);
    shader.setVec3("dirLight.ambient", dirLight.ambient);
    shader.setVec3("dirLight.diffuse", dirLight.diffuse);
    shader.setVec3("dirLight.specular", dirLight.specular);

    // Point Lights
    shader.setInt("numPointLights",(int)pointLights.size());
    for (int i=0; i<(int)pointLights.size(); i++){
        std::string base="pointLights["+std::to_string(i)+"]";
        shader.setVec3((base+".position").c_str(), pointLights[i].position);
        shader.setVec3((base+".ambient").c_str(), pointLights[i].ambient);
        shader.setVec3((base+".diffuse").c_str(), pointLights[i].diffuse);
        shader.setVec3((base+".specular").c_str(), pointLights[i].specular);
        shader.setFloat((base+".constant").c_str(), pointLights[i].constant);
        shader.setFloat((base+".linear").c_str(), pointLights[i].linear);
        shader.setFloat((base+".quadratic").c_str(), pointLights[i].quadratic);
    }

    float currentTime = (float)glfwGetTime();
    glm::mat4 vpMatrix = projection * camera.GetViewMatrix();
    glm::vec3 lighthouseCenter = glm::vec3(0.0f,5.0f,0.0f);
    float lighthouseRadius=10.0f;
    if (isSphereInFrustum(lighthouseCenter,lighthouseRadius,vpMatrix)){
        lighthouse->Render(shader, currentTime);
    }

    groundPlane.draw(shader);

    for (const auto &mesh : meshes){
        mesh->Draw(shader);
    }
}
