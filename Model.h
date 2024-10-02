#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include <json/json.h>
#include "Mesh.h"

using json = nlohmann::json;

class Model
{
public:
    // Constructor that loads in a model from a file
    Model(const char* file);

    // Drawing the model
    void Draw(Shader& shader, Camera& camera);

    // Transformation methods
    void setPosition(const glm::vec3& position);
    void setRotation(const glm::vec3& rotation);   // Rotation as Euler angles in degrees
    void setScale(const glm::vec3& scale);

    // Methods to incrementally rotate the model
    void rotate(const glm::vec3& rotation);        // Rotation as Euler angles in degrees

private:
    // Variables for easy access
    const char* file;
    std::vector<unsigned char> data;
    json JSON;

    // All the meshes and their associated transformations
    std::vector<Mesh> meshes;
    std::vector<glm::vec3> translationsMeshes;
    std::vector<glm::quat> rotationsMeshes;
    std::vector<glm::vec3> scalesMeshes;
    std::vector<glm::mat4> matricesMeshes;

    // Model-wide transformations
    glm::vec3 modelPosition;
    glm::vec3 modelRotation;   // Stored as Euler angles (degrees)
    glm::vec3 modelScale;

    // Prevents textures from being loaded twice
    std::vector<std::string> loadedTexName;
    std::vector<Texture> loadedTex;

    // Mesh loading functions
    void loadMesh(unsigned int indMesh);
    void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

    // Gets the binary data from a file
    std::vector<unsigned char> getData();
    std::vector<float> getFloats(json accessor);
    std::vector<GLuint> getIndices(json accessor);
    std::vector<Texture> getTextures();

    // Assembles all the floats into vertices
    std::vector<Vertex> assembleVertices(
        std::vector<glm::vec3> positions,
        std::vector<glm::vec3> normals,
        std::vector<glm::vec2> texUVs);

    // Group floats into vectors for vertices
    std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
    std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
    std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
};

#endif
