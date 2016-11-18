/*

http://www.opengl-tutorial.org/beginners-tutorials/
https://github.com/opengl-tutorials/ogl

*/

#include "GLFWExample.h"
#include "GLSquare.h"

GLFWExample glfw_example;

int main(void)
{
	glfw_example.init();

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	const GLfloat triangle_vertices[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.5f,  1.0f, 0.0f,
	};

	//const GLfloat square_vertices[] = {
	//	0.0f, 0.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	1.0f, 1.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f,
	//};

	GLSquare my_square(glm::vec3(0.5f, 0.5f, 0.0f), 0.3f, 0.5);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);


	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		GLuint MatrixID = glGetUniformLocation(programID, "MVP");
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 0.1f, 100.0f);
		//glm::mat4 Projection = glm::ortho(-1.0f,1.0f,-1.0f,1.0f,0.0f,100.0f); // In world coordinates

		// Camera matrix
		glm::mat4 View = glm::lookAt(
			glm::vec3(0.5, 0.5, 3), // Camera is at (4,3,3), in World Space
			glm::vec3(0.5, 0.5, 0), // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

		glm::mat4 Model = glm::mat4(1.0f);
		glm::mat4 MVP = Projection * View * Model;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_LINE_LOOP, 0, 3); // 6 vertices

		my_square.rotateCenteredZAxis(1);
		my_square.drawLineLoop(MatrixID, Projection * View);

		glDisableVertexAttribArray(0);

		glfw_example.swapBuffers();

	} // Check if the ESC key was pressed or the window was closed
	while (glfw_example.getKeyPressed(GLFW_KEY_ESCAPE) &&	glfw_example.getWindowShouldClose());

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
