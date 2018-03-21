#ifndef MESH_H
#define MESH_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

struct Vertex {
   glm::vec3 Position;
   glm::vec3 Normal;
   glm::vec2 TexCoords;
   glm::vec3 Tangent;
   glm::vec3 Bitangent;
};

struct Texture {
   unsigned int id;
   string type; //diffuse or specular
   string path;
};

struct bbInfo {
   glm::mat4 bb2World;
   glm::vec3 bbSize;
   glm::vec3 bbCenter;
   glm::vec3 bbMin;
   glm::vec3 bbMax;
};

class Mesh {
public:
   vector<Vertex> vertices;
   vector<unsigned int> indices;
   vector<Texture> textures;
   unsigned int VAO;
   bbInfo meshBB;

   //Draw Bounding Box for mesh.
   void drawBBox(GLint bbShader, glm::mat4 obj2World) {

      if (this->vertices.size() == 0) {
         //std::cout << "vertices # = 0 \n";
         return;
      }
      
      // 1x1x1 Box, centered on origin
      GLfloat boxVertices[] = {
         -0.5, -0.5, -0.5, 1.0,
         0.5, -0.5, -0.5, 1.0,
         0.5,  0.5, -0.5, 1.0,
         -0.5,  0.5, -0.5, 1.0,
         -0.5, -0.5,  0.5, 1.0,
         0.5, -0.5,  0.5, 1.0,
         0.5,  0.5,  0.5, 1.0,
         -0.5,  0.5,  0.5, 1.0,
      };
      GLuint vboVertices;
      glGenBuffers(1, &vboVertices);
      glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
      glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, 0);

      GLushort boxElements[] = {
         0, 1, 2, 3,
         4, 5, 6, 7,
         0, 4, 1, 5, 2, 6, 3, 7
      };
      GLuint iboElements;
      glGenBuffers(1, &iboElements);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboElements);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boxElements), boxElements, GL_STATIC_DRAW);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

      // Collect min/max coords
      GLfloat minX, maxX, minY, maxY, minZ, maxZ;
      minX = maxX = this->vertices[0].Position.x;
      minY = maxY = this->vertices[0].Position.y;
      minZ = maxZ = this->vertices[0].Position.z;
      for (int i = 0; i < this->vertices.size(); i++) {
         if (this->vertices[i].Position.x < minX) minX = this->vertices[i].Position.x;
         if (this->vertices[i].Position.x > maxX) maxX = this->vertices[i].Position.x;
         if (this->vertices[i].Position.y < minY) minY = this->vertices[i].Position.y;
         if (this->vertices[i].Position.y > maxY) maxY = this->vertices[i].Position.y;
         if (this->vertices[i].Position.z < minZ) minZ = this->vertices[i].Position.z;
         if (this->vertices[i].Position.z > maxZ) maxZ = this->vertices[i].Position.z;
      }

      //Drawing Box:
      glm::vec3 size = glm::vec3(maxX - minX, maxY - minY, maxZ - minZ);
      glm::vec3 center = glm::vec3((minX + maxX) / 2, (minY + maxY) / 2, (minZ + maxZ) / 2);
      glm::mat4 transform = glm::translate(glm::mat4(1.0), center) * glm::scale(glm::mat4(1.0), size);

      // Apply object's transformation matrix
      glm::mat4 bbToWorld = obj2World * transform;





      glUseProgram(bbShader);
      glUniformMatrix4fv(glGetUniformLocation(bbShader, "model"), 1, GL_FALSE, &bbToWorld[0][0]);
      //glUniformMatrix4fv(glGetUniformLocation(bbShader, "view"), 1, GL_FALSE, &Window::V[0][0]);
      //glUniformMatrix4fv(glGetUniformLocation(bbShader, "projection"), 1, GL_FALSE, &Window::P[0][0]);

      glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
      glEnableVertexAttribArray(0); //attribute v Coords
      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0 );

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboElements);
      glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
      glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4 * sizeof(GLushort)));
      glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

      glDisableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);

      glDeleteBuffers(1, &vboVertices);
      glDeleteBuffers(1, &iboElements);
   
      /*
      bbInfo returnBBInfo;
      returnBBInfo.bb2World = bbToWorld;
      returnBBInfo.bbSize = size;
      returnBBInfo.bbCenter = center;
      returnBBInfo.bbMin.x = minX;
      returnBBInfo.bbMin.y = minY;
      returnBBInfo.bbMin.z = minZ;
      returnBBInfo.bbMax.x = maxX;
      returnBBInfo.bbMax.y = maxY;
      returnBBInfo.bbMax.z = maxZ;
      */
      meshBB.bb2World = bbToWorld;
      meshBB.bbSize = size;
      meshBB.bbCenter = center;
      meshBB.bbMin.x = minX;
      meshBB.bbMin.y = minY;
      meshBB.bbMin.z = minZ;
      meshBB.bbMax.x = maxX;
      meshBB.bbMax.y = maxY;
      meshBB.bbMax.z = maxZ;

      return;




   }


   // Constructor
   Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
      this->vertices = vertices;
      this->indices = indices;
      this->textures = textures;

      setupMesh();
   }



   void Draw(GLint shader) {

      //drawBBox();
      // texture binding;
      unsigned int diffuseNr = 1;
      unsigned int specularNr = 1;
      unsigned int normalNr = 1;
      unsigned int heightNr = 1;
      for (unsigned int i = 0; i < textures.size(); i++) { 
            glActiveTexture(GL_TEXTURE0 + i);

            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse") {
               number = std::to_string(diffuseNr++);
            }
            else if (name == "texture_specular"){
              number = std::to_string(specularNr++);
            }
            else if (name == "texture_normal") {
               number = std::to_string(normalNr++);
           }
           else if (name == "texture_height") {
              number = std::to_string(heightNr++);
           }

            //bind texture map
            glUniform1i(glGetUniformLocation(shader, (name + number).c_str()), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);

      glActiveTexture(GL_TEXTURE0);
   }

   bbInfo Draw(GLint shader, GLint BBshader, glm::mat4 obj2World) {

      glLineWidth(4);
      drawBBox(BBshader, obj2World);
      glLineWidth(1);
      glUseProgram(shader);
      // texture binding;
      unsigned int diffuseNr = 1;
      unsigned int specularNr = 1;
      unsigned int normalNr = 1;
      unsigned int heightNr = 1;
      for (unsigned int i = 0; i < textures.size(); i++) { //unknown # of texture maps, loop through all
         glActiveTexture(GL_TEXTURE0 + i);

         string number;
         string name = textures[i].type;
         if (name == "texture_diffuse") {
            number = std::to_string(diffuseNr++);
         }
         else if (name == "texture_specular") {
            number = std::to_string(specularNr++);
         }
         else if (name == "texture_normal") {
            number = std::to_string(normalNr++);
         }
         else if (name == "texture_height") {
            number = std::to_string(heightNr++);
         }

         //bind texture map
         glUniform1i(glGetUniformLocation(shader, (name + number).c_str()), i);
         glBindTexture(GL_TEXTURE_2D, textures[i].id);
      }

      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);

      glActiveTexture(GL_TEXTURE0);

      return meshBB;
   }


private:
   unsigned int VBO, EBO;

   void setupMesh() {
      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);

      glBindVertexArray(VAO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
      glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

      // set the vertex attribute pointers
      // vertex positions
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
      // vertex normals
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
      // vertex texture coords
      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
      // vertex tan
      glEnableVertexAttribArray(3);
      glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
      // vertex bitan
      glEnableVertexAttribArray(4);
      glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

      glBindVertexArray(0);
   }

};














#endif