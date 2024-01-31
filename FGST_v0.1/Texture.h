#pragma once

#include "Shader.h"
#include "SOIL2.h"

class Texture {
private :
	GLenum ID;

	unsigned int type;

	int textureWidth;
	int textureHeight;


public :
	Texture(const char* texture_file, GLenum type) : type(type) {

		//The upside down effect has to be resolved maybe by a soil function
		unsigned char* image = SOIL_load_image(texture_file, &textureWidth, &textureHeight, NULL, SOIL_LOAD_RGBA);


		glGenTextures(1, &ID);
		//glActiveTexture(texture_unit); //Active le texture unit 0 pour pouvoir y loger notre texture (c'est une sorte de binding)
		glBindTexture(type, ID);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (ID) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "ERROR::TEXTURE_INIT_FAILED" << std::endl;
		}

		SOIL_free_image_data(image);
	}

	~Texture() {}

	void use(GLenum texture_unit) {
		glActiveTexture(GL_TEXTURE0 + texture_unit);
		glBindTexture(type, ID);
	}

	GLenum getTextureID() {
		return ID;
	}
}; 