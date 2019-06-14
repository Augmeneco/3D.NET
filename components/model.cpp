#include <stdio.h>
#include <string.h>
#include <regex>
#include <GL/glew.h>
#include "../misc.h"
#include "model.h"

Model::Model()
{
    this->verticies = std::vector<std::array<float, 3>>();
}

void Model::loadOBJ(std::string& objText)
{
    std::vector<std::string> lines = explode(objText, '\n');
    objText.clear();

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<std::array<float, 3>> verticesTemp;
	std::vector<std::array<float, 2>> uvsTemp;
	std::vector<std::array<float, 3>> normalsTemp;
    auto iter = lines.begin();
	while (iter != lines.end())
    {
        //puts(iter->c_str());
		char lineHeaderChar[256];
		// read the first word of the line
		sscanf(iter->c_str(), "%s", &lineHeaderChar);
		//puts(lineHeaderChar);
		std::string lineHeader(lineHeaderChar);

		if (lineHeader == "v")
        {
			std::array<float, 3> vertex;
			sscanf(iter->c_str(), "v %f %f %f", &vertex[0], &vertex[1], &vertex[2]);
			verticesTemp.push_back(vertex);
		}
		else if (lineHeader == "vt")
        {
			std::array<float, 2> uv;
			sscanf(iter->c_str(), "vt %f %f", &uv[0], &uv[1]);
			//uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			uvsTemp.push_back(uv);
		}
		else if (lineHeader == "vn")
        {
			std::array<float, 3> normal;
			sscanf(iter->c_str(), "vn %f %f %f", &normal[0], &normal[1], &normal[2]);
			normalsTemp.push_back(normal);
		}
        if (lineHeader == "f")
        {
            std::regex pattern1("f (\\d+) (\\d+) (\\d+)");
            std::regex pattern2("f (\\d+)\\/(\\d+) (\\d+)\\/(\\d+) (\\d+)\\/(\\d+)");
            std::regex pattern3("f (\\d+)\\/(\\d+)\\/(\\d+) (\\d+)\\/(\\d+)\\/(\\d+) (\\d+)\\/(\\d+)\\/(\\d+)");
            std::regex pattern4("f (\\d+)\\/\\/(\\d+) (\\d+)\\/\\/(\\d+) (\\d+)\\/\\/(\\d+)");
            std::smatch match;

            if (std::regex_match(*iter, match, pattern1))
            {
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                sscanf(iter->c_str(), "f %d %d %d", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
            }
            else if (std::regex_match(*iter, match, pattern2))
            {
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                sscanf(iter->c_str(), "f %d/%d %d/%d %d/%d", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]);
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                uvIndices    .push_back(uvIndex[0]);
                uvIndices    .push_back(uvIndex[1]);
                uvIndices    .push_back(uvIndex[2]);
            }
            else if (std::regex_match(*iter, match, pattern3))
            {
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                sscanf(iter->c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                uvIndices    .push_back(uvIndex[0]);
                uvIndices    .push_back(uvIndex[1]);
                uvIndices    .push_back(uvIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
            }
            else if (std::regex_match(*iter, match, pattern4))
            {
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                sscanf(iter->c_str(), "f %d//%d %d//%d %d//%d", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                uvIndices    .push_back(uvIndex[0]);
                uvIndices    .push_back(uvIndex[1]);
                uvIndices    .push_back(uvIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
            }
		}
		else
        {
		}

        //auto iter_old = iter;
        iter++;
        //lines.erase(iter_old);
//        if (iter == lines.end())
//            break;
	}

	lines.clear();
	//lines.shrink_to_fit();

	// For each vertex of each triangle
	for(unsigned int i=0; i<vertexIndices.size(); i++){
		// Get the attributes thanks to the index
        unsigned int vertexIndex = vertexIndices[i];
		std::array<float, 3> vertex = verticesTemp[vertexIndex-1];
        this->verticies.push_back(vertex);

		if (uvsTemp.size() != 0)
        {
            unsigned int uvIndex = uvIndices[i];
            std::array<float, 2> uv = uvsTemp[uvIndex-1];
            		this->uvs.push_back(uv);
        }

        if (normalsTemp.size() != 0)
        {
            unsigned int normalIndex = normalIndices[i];
            std::array<float, 3> normal = normalsTemp[normalIndex-1];
            		this->normals.push_back(normal);
        }

		// Put the attributes in buffers

	}
}

void cube(float size, Transform pos)
{
    glBegin(GL_QUADS);
        glColor3f(0.0f,1.0f,0.0f);    // Color Blue
        glVertex3f( 0.25f, 0.25f,-0.25f);    // Top Right Of The Quad (Top)
        glVertex3f(-0.25f, 0.25f,-0.25f);    // Top Left Of The Quad (Top)
        glVertex3f(-0.25f, 0.25f, 0.25f);    // Bottom Left Of The Quad (Top)
        glVertex3f( 0.25f, 0.25f, 0.25f);    // Bottom Right Of The Quad (Top)
        glColor3f(1.0f,0.5f,0.0f);    // Color Orange
        glVertex3f( 0.25f,-0.25f, 0.25f);    // Top Right Of The Quad (Bottom)
        glVertex3f(-0.25f,-0.25f, 0.25f);    // Top Left Of The Quad (Bottom)
        glVertex3f(-0.25f,-0.25f,-0.25f);    // Bottom Left Of The Quad (Bottom)
        glVertex3f( 0.25f,-0.25f,-0.25f);    // Bottom Right Of The Quad (Bottom)
        glColor3f(1.0f,0.0f,0.0f);    // Color Red
        glVertex3f( 0.25f, 0.25f, 0.25f);    // Top Right Of The Quad (Front)
        glVertex3f(-0.25f, 0.25f, 0.25f);    // Top Left Of The Quad (Front)
        glVertex3f(-0.25f,-0.25f, 0.25f);    // Bottom Left Of The Quad (Front)
        glVertex3f( 0.25f,-0.25f, 0.25f);    // Bottom Right Of The Quad (Front)
        glColor3f(1.0f,1.0f,0.0f);    // Color Yellow
        glVertex3f( 0.25f,-0.25f,-0.25f);    // Top Right Of The Quad (Back)
        glVertex3f(-0.25f,-0.25f,-0.25f);    // Top Left Of The Quad (Back)
        glVertex3f(-0.25f, 0.25f,-0.25f);    // Bottom Left Of The Quad (Back)
        glVertex3f( 0.25f, 0.25f,-0.25f);    // Bottom Right Of The Quad (Back)
        glColor3f(0.0f,0.0f,1.0f);    // Color Blue
        glVertex3f(-0.25f, 0.25f, 0.25f);    // Top Right Of The Quad (Left)
        glVertex3f(-0.25f, 0.25f,-0.25f);    // Top Left Of The Quad (Left)
        glVertex3f(-0.25f,-0.25f,-0.25f);    // Bottom Left Of The Quad (Left)
        glVertex3f(-0.25f,-0.25f, 0.25f);    // Bottom Right Of The Quad (Left)
        glColor3f(1.0f,0.0f,1.0f);    // Color Violet
        glVertex3f( 0.25f, 0.25f,-0.25f);    // Top Right Of The Quad (Right)
        glVertex3f( 0.25f, 0.25f, 0.25f);    // Top Left Of The Quad (Right)
        glVertex3f( 0.25f,-0.25f, 0.25f);    // Bottom Left Of The Quad (Right)
        glVertex3f( 0.25f,-0.25f,-0.25f);    // Bottom Right Of The Quad (Right)
    glEnd();
}
