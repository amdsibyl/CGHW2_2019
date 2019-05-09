#include "StaticMesh.h"
#include "tiny_obj_loader.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

StaticMesh::StaticMesh()
    : m_uv(false), m_normal(false)
{

}

StaticMesh StaticMesh::LoadMesh(const std::string &filename, const bool flat_mode)
{

    std::vector<tinyobj::shape_t> shapes;
	{
		std::vector<tinyobj::material_t> materials;
		std::string error_string;
		if (!tinyobj::LoadObj(shapes, materials, error_string, filename.c_str())) {
			// GG
		}

        /*
		if (shapes.size() == 0)
			GG

		if (shapes[0].mesh.texcoords.size() == 0 || shapes[0].mesh.normals.size() == 0)
			GG*/
	}

    StaticMesh ret;
    glGenVertexArrays(1, &ret.vao);
    glBindVertexArray(ret.vao);

    glGenBuffers(3, ret.vbo);
    glGenBuffers(1, &ret.ibo);

    if(flat_mode == false){
        // Upload postion array
        glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.positions.size(),
            shapes[0].mesh.positions.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
        if (shapes[0].mesh.texcoords.size() > 0) {
            // Upload texCoord array
            glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[1]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.texcoords.size(),
                shapes[0].mesh.texcoords.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
            ret.m_uv = true;
        }
        if (shapes[0].mesh.normals.size() > 0) {
            // Upload normal array
            glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[2]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.normals.size(),
                shapes[0].mesh.normals.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
            ret.m_normal = true;
        }

        // Setup index buffer for glDrawElements
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret.ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * shapes[0].mesh.indices.size(),
            shapes[0].mesh.indices.data(), GL_STATIC_DRAW);

        ret.numIndices = shapes[0].mesh.indices.size();
    }
    else{
        // Upload postion array
        glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.positions.size(),
            shapes[0].mesh.positions.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
        if (shapes[0].mesh.texcoords.size() > 0) {
            // Upload texCoord array
            glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[1]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.texcoords.size(),
                shapes[0].mesh.texcoords.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
            ret.m_uv = true;
        }
        if (shapes[0].mesh.normals.size() > 0) {
            std::vector<glm::vec3> pos_;
            std::vector<float> pos;
            std::vector<float> uv;
            std::vector<float> norm;
            std::vector<unsigned int> ind;
            glm::vec3 tmp;
            for(unsigned int i=0;i < shapes[0].mesh.indices.size(); i++){
                for(int j=0;j<3;j++) tmp[j] = shapes[0].mesh.positions[shapes[0].mesh.indices[i] * 3 + j];
                pos_.push_back(tmp);
                uv.push_back(shapes[0].mesh.texcoords[shapes[0].mesh.indices[i] * 2]);
                uv.push_back(shapes[0].mesh.texcoords[shapes[0].mesh.indices[i] * 2 + 1]);
                ind.push_back(i);
            }

            glm::vec3 v0, v1;
            for(int i=0; i < pos_.size(); i+=3){

                v0 = pos_[i+1] - pos_[i];
                v1 = pos_[i+2] - pos_[i];

                glm::vec3 flat_n = glm::normalize(glm::cross(v0, v1));
                for(int j=0;j<3;j++){
                    for(int k=0;k<3;k++){
                        norm.push_back(flat_n[k]);
                        pos.push_back(pos_[i+j][k]);
                    }
                }
            }
            // Upload postion array
            glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * pos.size(),
                pos.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

            if (shapes[0].mesh.texcoords.size() > 0) {
                // Upload texCoord array
                glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[1]);
                glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * uv.size(),
                    uv.data(), GL_STATIC_DRAW);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
                ret.m_uv = true;
            }

            // Upload normal array
            glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[2]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * norm.size(),
                norm.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
            ret.m_normal = true;

            // Setup index buffer for glDrawElements
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret.ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * ind.size(),
                ind.data(), GL_STATIC_DRAW);

            ret.numIndices = ind.size();
        } // end of normal
        else{
            // Setup index buffer for glDrawElements
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret.ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * shapes[0].mesh.indices.size(),
                shapes[0].mesh.indices.data(), GL_STATIC_DRAW);
            ret.numIndices = shapes[0].mesh.indices.size();
        }

    } // end of else
    
    glBindVertexArray(0);
    return ret;
}

void StaticMesh::release()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(3, vbo);
    glDeleteBuffers(1, &ibo);

}

void StaticMesh::draw()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
}

bool StaticMesh::hasNormal() const
{
	return m_normal;
}

bool StaticMesh::hasUV() const 
{
	return m_uv;
}
