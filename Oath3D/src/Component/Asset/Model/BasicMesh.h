#pragma once

#include "Common/Common.h"

namespace Oath {
	namespace Comp {

		class BasicMesh {
		public:
			void InitWithTexCoord(const Vector<GLfloat>& vertices, uint dim) {

				GLCall(glGenVertexArrays(1, &VAO));
				GLCall(glGenBuffers(1, &VBO));
				GLCall(glBindVertexArray(VAO));
				GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
				GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW));

				GLCall(glEnableVertexAttribArray(0));
				GLCall(glVertexAttribPointer(0, dim, GL_FLOAT, GL_FALSE, (dim + 2) * sizeof(GLfloat), 0));

				GLCall(glEnableVertexAttribArray(1));
				GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (dim + 2) * sizeof(GLfloat), (void*)(dim * sizeof(GLfloat))));

				size = (GLuint)((float)vertices.size() / (float)(dim + 2));
			}
			void Init(const Vector<GLfloat>& vertices, uint dim) {

				GLCall(glGenVertexArrays(1, &VAO));
				GLCall(glGenBuffers(1, &VBO));
				GLCall(glBindVertexArray(VAO));
				GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
				GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW));

				GLCall(glEnableVertexAttribArray(0));
				GLCall(glVertexAttribPointer(0, dim, GL_FLOAT, GL_FALSE, 0, 0));

				size = (GLuint)((float)vertices.size() / (float)dim);
			}

			void bind() const;
			void Draw() const;
			void release();

		protected:
			GLuint VAO = 0, VBO = 0, size = 0;
		};
	}
}
