#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Node.h"


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

inline unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model : public Node
{
public:
   // Model data
   vector<Texture> textures_loaded;	// stores all the textures loaded so far
   vector<Mesh> meshes;
   string directory; // directory of obj, stored to pass to meshes
   bool gammaCorrection;
   //Merge change:
   GLint shader;
   // bounding box struct:
   bbInfo modelBB;



   /*  Functions   */
   // constructor, expects a filepath to a 3D model.

   Model(string const &path, bool gamma = false);
   Model(string const &path, GLint shader, bool gamma = false);

   // draws the model, and thus all its meshes
   void draw(glm::mat4 M);

   void Draw(glm::mat4 M, GLint shader);

   // draw with BB:
   void DrawWBB(GLint shaderOBJ, GLint shaderBB, glm::mat4 obj2World);
   /*
   void DrawWBB(GLint shaderOBJ, GLint shaderBB, glm::mat4 obj2World)
   {
      for (unsigned int i = 0; i < meshes.size(); i++)
         meshes[i].Draw(shaderOBJ, shaderBB, obj2World);
   }
   */
   void update();


private:
   /*  Functions   */
   // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
   void loadModel(string const &path);

   // processes a node recursively, includes child nodes (collects unprocessed meshes)
   void processNode(aiNode *node, const aiScene *scene);

   Mesh processMesh(aiMesh *mesh, const aiScene *scene);

   // checks all material textures of a given type and loads the textures if they're not loaded yet.
   // the required info is returned as a Texture struct.
   vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};




#endif