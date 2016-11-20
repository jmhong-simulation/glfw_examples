/*

http://www.opengl-tutorial.org/beginners-tutorials/
https://github.com/opengl-tutorials/ogl

*/

#include "GLFWExample.h"
#include "GLSquare.h"
#include "GLLineSegments.h"

GLFWExample glfw_example;

int main(void)
{
	glfw_example.init();

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	GLSquare my_square(glm::vec3(0.5f, 0.5f, 0.0f), 0.2f, 0.1f);
	GLSquare my_square2(glm::vec3(0.9f, 0.5f, 0.0f), 0.1f, 0.2f);

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

		my_square.rotateCenteredZAxis(1);
		my_square.drawLineLoop(MatrixID, Projection * View);
		my_square2.drawLineLoop(MatrixID, Projection * View);

		// sensor lines
		std::vector<glm::vec3> sensor_lines;
		const glm::vec3 center = my_square.center_;
		const float radius = 1.0;
		for (int i = 0; i < 360; i += 10)
		{
			glm::vec4 end_pt = glm::vec4(radius*cos(glm::radians((float)i)), radius*-sin(glm::radians((float)i)), 0.0f, 0.0f);
			end_pt = my_square.model_matrix_ * end_pt;

			const glm::vec3 r = center + glm::vec3(end_pt.x, end_pt.y, end_pt.z);

			int flag;
			glm::vec3 col_pt;
			float t;

			my_square2.checkCollisionLoop(center, r, flag, t, col_pt);

			//TODO: clamp col_pt by r
			if (flag == 1)
			{
				sensor_lines.push_back(center);
				sensor_lines.push_back(col_pt);
			}
			else
			{
				sensor_lines.push_back(center);
				sensor_lines.push_back(r);
			}
		}

		GLLineSegments lines(sensor_lines);
		lines.center_ = my_square.center_;

		//lines.rotateCenteredZAxis(1);
		lines.drawLineLoop(MatrixID, Projection * View);

		glDisableVertexAttribArray(0);

		glfw_example.swapBuffers();

	} // Check if the ESC key was pressed or the window was closed
	while (glfw_example.getKeyPressed(GLFW_KEY_ESCAPE) &&	glfw_example.getWindowShouldClose());

	// Cleanup VBO
	//glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
