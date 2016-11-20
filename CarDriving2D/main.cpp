/*

http://www.opengl-tutorial.org/beginners-tutorials/
https://github.com/opengl-tutorials/ogl

*/

#include "GLFWExample.h"
#include "GLSquare.h"
#include "GLLineSegments.h"
#include "SelfDrivingCar.h"

GLFWExample glfw_example;

int main(void)
{

	glfw_example.init();

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	SelfDrivingCar my_car;

	//TODO: extend to object list
	GLSquare my_square2;
	my_square2.update(glm::vec3(0.9f, 0.5f, 0.0f), 0.1f, 0.2f);

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

		// animate update
		my_car.car_body.rotateCenteredZAxis(1);
		my_car.updateSensor(my_square2);

		// draw
		my_car.car_body.drawLineLoop(MatrixID, Projection * View);
		my_square2.drawLineLoop(MatrixID, Projection * View);
		my_car.sensing_lines.drawLineLoop(MatrixID, Projection * View);

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
