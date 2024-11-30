#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "Shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	// Initialize and configure GLFW
	// -----------------------------
	glfwInit();

	// Tell GLFW which version of OpenGL we want to use.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Tell GLFW which OpenGL we want to use (should always be core profile).
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// GLFW window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGLPlayground", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Tell GLFW to make the context of our window the main context on the current thread.
	glfwMakeContextCurrent(window);
	
	// Tell GLFW to register the framebuffer callback to handle window resizing.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Load all OpenGL function pointers for GLAD
	// ------------------------------------------
	// Loads all OpenGL function pointers based on the version we told it to use (in this instance OpenGL v3.3).
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Set the dimensions of the OpenGL viewport (the size of the rendering window created with GLFW).
	glViewport(0 ,0, 800, 600);

	// Build and compile our shader program
	// ------------------------------------
	Shader shader("shader.vs", "shader.fs");

	//// Instatiate the vertex shader using glCreateShader, again references by an ID, in this case int vertexShader.
	//unsigned int vertexShader;

	//// We provide the type of shader taht we want, in this case a vertex shader defined by GL_VERTEX_SHADER.
	//vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//// Attach the shader source code and compile.
	////	Arg 1: The object to compile - our vertex shader object
	////	Arg 2: How many strings we're passing in as source code
	////	Arg 3: The actual source code to compile
	//glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//glCompileShader(vertexShader);

	//// Check if the compilation was successfull.
	//int glShaderSuccess;
	//char glShaderInfoLog[512];
	//glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &glShaderSuccess);
	//if (!glShaderSuccess)
	//{
	//	// If shader compilation was not successfull, we can retrieve the error message using glGetShaderInfoLog.
	//	glGetShaderInfoLog(vertexShader, 512, NULL, glShaderInfoLog);
	//	std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << glShaderInfoLog << std::endl;
	//}

	//// Exactly the same as a vertex shader, except for the glCreateShader function call,
	//// which instead of GL_VERTEX_SHADER takes GL_FRAGMENT_SHADER.
	//unsigned int fragmentShader;
	//fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//glCompileShader(fragmentShader);
	//glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &glShaderSuccess);
	//if (!glShaderSuccess)
	//{
	//	glGetShaderInfoLog(fragmentShader, 512, NULL, glShaderInfoLog);
	//	std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << glShaderInfoLog << std::endl;
	//}

	//// A shader program object is the final linjed version of multiple shaders combined.
	//// To use the shaders we've created, we have to link them to a shader pgoram object, and active this shader program when rendering objects.
	//// When linking shaders into the program, it links the output ofr each shader to the inputs of the next shader.
	//// Creating a program object is the same as creating many other OpenGL object, as is references by an ID, in this case int shaderProgram.
	//unsigned int shaderProgram;
	//shaderProgram = glCreateProgram();

	//// Attach the shaders to the shader program.
	//glAttachShader(shaderProgram, vertexShader);
	//glAttachShader(shaderProgram, fragmentShader);

	//// Link the shaders together with glLinkProgram.
	//glLinkProgram(shaderProgram);

	//// Verify that linking the shader program was successfully, much in the same way as compiling the individual shaders.
	//glGetProgramiv(shaderProgram, GL_LINK_STATUS, &glShaderSuccess);
	//if (!glShaderSuccess)
	//{
	//	glGetProgramInfoLog(shaderProgram, 512, NULL, glShaderInfoLog);
	//	std::cout << "ERROR::SHADER::LINKING::ERROR\n" << glShaderInfoLog << std::endl;
	//}

	//// Once the shader program has been linked, the shaders themselves aren't needed anymore
	//// and therefore should be deleted.
	//glDeleteShader(vertexShader);
	//glDeleteShader(fragmentShader);

	// Setup up vertex data (an buffers) and configure vertex attributes
	// -----------------------------------------------------------------

	// Vertices in normalized device coordinated (NDC).
	// OpenGL only processes 3D coordinates when they're in a specific range between -1.0 and 1.0 on all 3 axes (x, y and z).
	// This range is called the normalized device coordinates range and only coordinates within this range will be depicted on the screen.
	// All other coordinated outside of the range will be discarded.
	float vertices[] = {
		// positions		  // colors
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f
	};

	// Drawing other shapes that triangles, but with the basis of a triangle as OpenGL prefers triangles.
	// By utilizing indices (known af indexed drawing) and element buffer objects (EBO), we can combine to triangles to draw a rectangle,
	// but without the overhead of drawing the same points multiple times.
	// float vertices[] = {
    //		* first triangle *
	//		0.5f, 0.5f, 0.0f,  // top right
	//		0.5f, -0.5f, 0.0f,  // bottom right - drawn twice
	//		-0.5f, 0.5f, 0.0f,  // top left - drawn twice
	//		* second triangle *
	//		0.5f, -0.5f, 0.0f,  // bottom right - drawn twice
	//		-0.5f, -0.5f, 0.0f,  // bottom left
	//		-0.5f, 0.5f, 0.0f   // top left - drawn twice
    // };
	/*unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};*/

	// All OpenGL buffer objects have a unique ID corresponding to that specific buffer, in this case:
	//		- vertex array object (VAO)
	//		- vertex buffer object (VBO)
	//		- element buffer object (EBO)
	unsigned int VAO, VBO;
	//unsigned int EBO;

	// Instantiate the vertex array object.
	// A vertex array object can be bound just like a vert buffer. All subsequent vertex attribute calls (glVertexAttribPointer, glEnableVertexAttribArray)
	// will be stored inside the vertex array object. This has the advantage that we only have to make these calls once, when configuring vertex attribute pointers.
	// Whenever we want to draw the object in the future, all we have to do is to bind the corresponding VAO.
	// A vertex array object stores the following:
	//		calls to glEnableVertexAttribArray or glDisplayVertexAttribArray
	//		vertex attribute configuration via glVertexAttribPointer
	//		vertex buffer objects associated with the vertex attributes by calls to glVertexAttribPointer
	glGenVertexArrays(1, &VAO);

	// Instantiate the vertex buffer object in OpenGL: Beef was here, and she loves you <3
	// A vertex buffer is one of OpenGL's "objects", hence the abbreviation VBO (vertex buffer object).
	glGenBuffers(1, &VBO);

	// Instatiate the element buffer object.
	//glGenBuffers(1, &EBO);

	// Bind vertex array object (VAO) first, then bind and set vertex buffer object(s) (VBO), and then configure vertex attribute(s).
	glBindVertexArray(VAO);

	// All OpenGL buffer objects have a corresponding buffer type. The buffer type of a vertex buffer object is GL_ARRAY_BUFFER.
	// Bind the vertex buffer to the GL_ARRAY_BUFFER target.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Now, all buffer calls we make on the target GL_ARRAY_BUFFER will be used to configure the bound buffer, which in this case is VBO.
	// Copy the vertices data to the buffer, meaning that we copy the data to the memory of the graphics card.
	//	GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
	//	GL_STATIC_DRAW: the data is set only once and used many times.
	//	GL_DYNAMIC_DRAW : the data is changed a lot and used many times.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Bind element buffer (EBO) and copy the indices data.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Linking vertex attributes.
	// We now added the vertex data to the GPU memory, as well as instructed the GPU how to handle the vertex data within a vertex and fragment shader.
	// However, OpenGL does not know how the vertex data in memory should be interpreted, and how to connect the vertex data to vertex shader's attributes.
	// We need to explicitly tell OpenGL how to interpret the vertex data.

	// - 1st argument specifies which vertex attribute we want to configure. This is directly related to the location of the position vertex attribute
	//		in the vertex shader (layout (location = 0). This sets the location of the vertex attribute to 0 and since we want to pass data to this vertex attribute,
	//		we pass in 0.
	// - 2nd argument specifies the size of the vertex attribute. Since it's a vec3 vertex attribute, it's composed on 3 values, so we pass in 3.
	// - 3rd argument specifies the type of the data. In this case, the type if floats, hence the GL_FLOAT (a vec* in GLSL consists of floating point values).
	// - 4th argument specifies if we want the data normalized. If we're inputting data types int or byte, and we've set this to GL_TRUE, the integer
	//		data is normalized to 0 (or -1 for signed data) and 1 when converted to float. No relevant in this case.
	// - 5th argument is known as the stride and tells us the space between consecutive vertex attributes. As the next set of position data is located exactly 
	//		3 times the size of float away, we specify that value as the stride.
	// - 6th argument specifies the offset as in where in the buffer the position data begins. The offset is of type void* (hence the weird cast).
	//		Since the position data is at the start of the data array, this value is just 0 - there's no offset.
	
	// Position attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	// Now that we told OpenGL how to interpret the vertex data, we also need to enable the vertex attribute,
	// giving the vertex attribute location as the argument (vertex attributes are disabled by default).
	glEnableVertexAttribArray(0);

	// Color attribute.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Render loop - continue to run until GLFW has been instructed to close.
	while (!glfwWindowShouldClose(window))
	{
		// Handle input
		// ------------
		processInput(window);

		// Render
		// ------

		// Define the color we want to clear the buffer with.
		glClearColor(0.f, 0.3f, 0.3f, 1.0f);

		// Clear the buffer (the specific buffer we're clearing is defined by GL_COLOR_BUFFER_BIT).
		// Whenever we call glClear and clear the color buffer, the entire color buffer will be filled with the color as configured by glClearColor.
		glClear(GL_COLOR_BUFFER_BIT);
		
		// Playing with uniform data type in conjunction with the fragment shader.
		// Vertex attributes of type uniform are global for the specific shader program, meaning that they are accessible from any shader at any stage in the shader program.
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

		shader.use();

		// Before we can set the color on the uniform type, we need to find it within our shader program.
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

		// Tell OpenGL to use the shader program when we just created, when rendering.
		//glUseProgram(shaderProgram);

		// Set the color within the shader program, after we've told OpenGL to use the shader program in question.
		// Is essential to tell OpenGL to use the shader program, before we can update the uniform value, as we're settings a uniform on the currently active shader program.
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		// Optionally we can tell OpenGL that we want it to draw in wireframe mode, showing us the lines between the vertices,
		// instead of filling the drawn object with a solid color.
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // To reverse wireframe mode back to solid.

		// Bind out VAO (the triangle information)
		glBindVertexArray(VAO);

		// Draw based on vertex buffer object (VBO).
		// NOTE: The pure glDrawArrays is only relevant when no element buffer object (EBO) is in play.
		// - 1st argument specifices the primitive shape that'll be our basis
		// - 2nd argument specifices the starting index of the vertex array ew'd like to draw
		// - 3rd argument specifies how many vertices we want to draw (render 1 triangle from our data, which is 3 vertices long)
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Draw based on element buffer object (EBO)
		// NOTE: glDrawelements is only relevant when an element buffer object (EBO) is in play.
		// - 1st argument specifices the primitive shape that'll be our basis
		// - 2nd argument specifies the count or number of elements we'd like to draw. As we have 6 indices, we want to draw 6 vertices.
		// - 3rd argument specifies  the type of indicies which is of type GL_UNSIGNED_INT
		// - 4th argument allows us to specify an offset in the element buffer (EBO)
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glBindVertexArray(0);

		// Swap buffers and poll IO event (keys pressed/released, mouse moved, etc.)
		// -------------------------------------------------------------------------
		
		// When an application draws in a single buffer the resulting image may display flickering issues.
		// This is because the resulting output image is not drawn in an instant, but drawn pixel by pixel and usually from left to right and top to bottom.
		// Because this image is not displayed at an instant to the user while still being rendered to, the result may contain artifacts.
		// To circumvent these issues, windowing applications apply a double buffer for rendering.
		// The front buffer contains the final output image that is shown at the screen, while all the rendering commands draw to the back buffer.
		// As soon as all the rendering commands are finished we swap the back buffer to the front buffer so the image can be displayed
		// without still being rendered to, removing all the aforementioned artifacts.
		glfwSwapBuffers(window);

		// Checks if any events are triggered (keyboard input or mouse movement events).
		glfwPollEvents();
	}

	// Clean/delete all of GLFW's resources that were allocated.
	glfwTerminate();

	return 0;
}

// Callback function to register on the window, so whenever the window is resized,
// we register the new width and height on the OpenGL viewport.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Process input on each render iteration in the render loop.
void processInput(GLFWwindow* window)
{
	// If the ESC key is being pressed, tell GLFW to close the window (will exit the render loop).
	// glfwGetKey will return GLFW_PRESS is the key is currently being pressed, while returning GLFW_RELEASE if it's not.
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}