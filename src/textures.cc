#include "textures.hh"
#include "lib/obj.hh"
#include <GL/gl.h>
#include <iostream>


std::string textures_path = "textures/";

unsigned int loadSkybox() {
  std::vector<std::string> faces = {"blue.tga", "blue.tga", "blue.tga",
                                    "blue.tga", "blue.tga", "blue.tga"};

  return loadCubemap(faces);
}

unsigned int loadCubemap(std::vector<std::string> faces) {
  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  int width, height, nrChannels;
  for (unsigned int i = 0; i < faces.size(); i++) {
    std::string fullPath = textures_path;
    fullPath = fullPath.append(faces[i].c_str());
    void *data = read_tga(fullPath.c_str(), &width, &height, &nrChannels);
    if (data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
                   0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else {
      std::cout << "Cubemap tex failed to load at path: " << fullPath
                << std::endl;
      return -1;
    }
    free(data);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return textureID;
}