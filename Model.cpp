
#include "Window.h"
#include "Model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



Model::Model(string const &path, bool gamma) : gammaCorrection(gamma)
{
   loadModel(path);
}

Model::Model(string const &path, GLint shader, bool gamma) : gammaCorrection(gamma)
{
   this->shader = shader;
   loadModel(path);
}

// draws the model, and thus all its meshe
/*old function
void Model::draw(glm::mat4 M)
{
   shader->use();
   shader->setMat4("projection", Window::P);
   shader->setMat4("view", Window::V);
   shader->setMat4("model", M);
   for (unsigned int i = 0; i < meshes.size(); i++)
      meshes[i].Draw(this->shader);
}
*/
void Model::draw(glm::mat4 M)
{
   glUseProgram(this->shader);
   glUniformMatrix4fv(glGetUniformLocation(this->shader, "projection"), 1, GL_FALSE, &Window::P[0][0]);
   glUniformMatrix4fv(glGetUniformLocation(this->shader, "view"), 1, GL_FALSE, &Window::V[0][0]);
   glUniformMatrix4fv(glGetUniformLocation(this->shader, "model"), 1, GL_FALSE, &M[0][0]);
   for (unsigned int i = 0; i < meshes.size(); i++)
      meshes[i].Draw(this->shader);
}
// draws the model, and thus all its meshes
void Model::Draw(glm::mat4 M, GLint shader)
{
   glUseProgram(shader);
   glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &Window::P[0][0]);
   glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &Window::V[0][0]);
   glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &M[0][0]);
   //
   for (unsigned int i = 0; i < meshes.size(); i++)
      meshes[i].Draw(shader);
}
// draw with BB:
void Model::DrawWBB(GLint shaderOBJ, GLint shaderBB, glm::mat4 obj2World) {
   glUseProgram(shaderBB);
   //glUniformMatrix4fv(glGetUniformLocation(this->shader, "projection"), 1, GL_FALSE, &Window::P[0][0]);
   //glUniformMatrix4fv(glGetUniformLocation(this->shader, "view"), 1, GL_FALSE, &Window::V[0][0]);
   glUniformMatrix4fv(glGetUniformLocation(shaderBB, "model"), 1, GL_FALSE, &obj2World[0][0]);
   
   modelBB = meshes[0].Draw(this->shader, shaderBB, obj2World);
   //std::cout << "Testing: bb2World[3].x of this mesh is: " << modelBB.bb2World[3].x << "\n";

   //Collision Detection: load Bounding Box info into static Window::vector<vec3>s
   // save min/Max for collision check
   Window::otherMin.push_back(modelBB.bbMin);
   Window::otherMax.push_back(modelBB.bbMax);
   /*
   Window::otherMin[Window::boxID].x = modelBB.bbMin.x;
   Window::otherMin[Window::boxID].y = modelBB.bbMin.y;
   Window::otherMin[Window::boxID].z = modelBB.bbMin.z;

   Window::otherMax[Window::boxID].x = modelBB.bbMax.x;
   Window::otherMax[Window::boxID].y = modelBB.bbMax.y;
   Window::otherMax[Window::boxID].z = modelBB.bbMax.z;

   Window::boxID++;
   */
}

void Model::update() {}

// Loads model using Assimp to import scene data

inline void Model::loadModel(string const & path)
{
   
   Assimp::Importer importer;
   const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
   // check for errors
   if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
   {
      cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
      return;
   }
   // retrieve the filepath directory
   directory = path.substr(0, path.find_last_of('/'));

   // process ASSIMP root node recursively
   processNode(scene->mRootNode, scene);
}

// processes a node recursively including children nodes

inline void Model::processNode(aiNode * node, const aiScene * scene)
{
   // process each mesh at the current node
   for (unsigned int i = 0; i < node->mNumMeshes; i++)
   {
      // the node object only contains indices to index the actual objects in the scene. 
      // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(processMesh(mesh, scene));
   }
   // recursively process each of the children nodes
   for (unsigned int i = 0; i < node->mNumChildren; i++)
   {
      processNode(node->mChildren[i], scene);
   }

}

inline Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)
{
   vector<Vertex> vertices;
   vector<unsigned int> indices;
   vector<Texture> textures;


   for (unsigned int i = 0; i < mesh->mNumVertices; i++)
   {
      Vertex vertex;
      glm::vec3 vector; // Assimp uses its own vector class, so temp declared here
      vector.x = mesh->mVertices[i].x;
      vector.y = mesh->mVertices[i].y;
      vector.z = mesh->mVertices[i].z;
      vertex.Position = vector;
      // normals
      vector.x = mesh->mNormals[i].x;
      vector.y = mesh->mNormals[i].y;
      vector.z = mesh->mNormals[i].z;
      vertex.Normal = vector;
      // texture coordinates
      if (mesh->mTextureCoords[0]) // texture coord existence check
      {
         glm::vec2 vec;
         //  we always take the first set of texture coords, in the case there are more (up to 8)
         vec.x = mesh->mTextureCoords[0][i].x;
         vec.y = mesh->mTextureCoords[0][i].y;
         vertex.TexCoords = vec;
      }
      else
         vertex.TexCoords = glm::vec2(0.0f, 0.0f);
      vector.x = mesh->mTangents[i].x;
      vector.y = mesh->mTangents[i].y;
      vector.z = mesh->mTangents[i].z;
      vertex.Tangent = vector;
      vector.x = mesh->mBitangents[i].x;
      vector.y = mesh->mBitangents[i].y;
      vector.z = mesh->mBitangents[i].z;
      vertex.Bitangent = vector;
      vertices.push_back(vertex);
   }
   // mesh faces, get indices
   for (unsigned int i = 0; i < mesh->mNumFaces; i++)
   {
      aiFace face = mesh->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++)
         indices.push_back(face.mIndices[j]);
   }
   // process materials
   aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

   // 1. diffuse maps
   vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
   textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
   // 2. specular maps
   vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
   textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());


   // return a mesh object created from the extracted mesh data
   return Mesh(vertices, indices, textures);
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.

inline vector<Texture> Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, string typeName)
{
   vector<Texture> textures;
   for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
   {
      aiString str;
      mat->GetTexture(type, i, &str);
      // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
      bool skip = false;
      for (unsigned int j = 0; j < textures_loaded.size(); j++)
      {
         if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
         {
            textures.push_back(textures_loaded[j]);
            skip = true; // a texture with the same filepath has already been loaded
            break;
         }
      }
      if (!skip)
      {   // if texture hasn't been loaded already, load it
         Texture texture;
         texture.id = TextureFromFile(str.C_Str(), this->directory);
         texture.type = typeName;
         texture.path = str.C_Str();
         textures.push_back(texture);
         textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
      }
   }
   return textures;
}

inline unsigned int TextureFromFile(const char * path, const string & directory, bool gamma)
{
   string filename = string(path);
   filename = directory + '/' + filename;

   unsigned int textureID;
   glGenTextures(1, &textureID);

   int width, height, nrComponents;
   unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
   if (data)
   {
      GLenum format;
      if (nrComponents == 1)
         format = GL_RED;
      else if (nrComponents == 3)
         format = GL_RGB;
      else if (nrComponents == 4)
         format = GL_RGBA;

      glBindTexture(GL_TEXTURE_2D, textureID);
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      stbi_image_free(data);
   }
   else
   {
      std::cout << "Texture failed to load at path: " << path << std::endl;
      stbi_image_free(data);
   }

   return textureID;
}


