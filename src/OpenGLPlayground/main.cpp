#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "stb_image.h"

#include "Shader.h"

#include <iostream>

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
	GLFWwindow* window = glfwCreateWindow(800, 600, "opengl-getting-started", NULL, NULL);
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

	// Build and compile our shader program
	// ------------------------------------
	Shader shader("shader.vs", "shader.fs");

	// Setup up vertex data (an buffers) and configure vertex attributes
	// -----------------------------------------------------------------

	// Vertices in normalized device coordinated (NDC).
	// OpenGL only processes 3D coordinates when they're in a specific range between -1.0 and 1.0 on all 3 axes (x, y and z).
	// This range is called the normalized device coordinates range and only coordinates within this range will be depicted on the screen.
	// All other coordinated outside of the range will be discarded.
	// Texture coordinates are used to map a 2D texture to the object, in this case the triangle,
	// and range from 0 to 1 on the x and y axis, relative to the object we want to draw.
	float vertices[] = {
		// positions		  // colors			  // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,    // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,    // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,    // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f     // top left
	};

	// Drawing other shapes that triangles, but with the basis of a triangle as OpenGL prefers triangles.
	// By utilizing indices (known as indexed drawing) and element buffer objects (EBO), we can combine to triangles to draw a rectangle,
	// but without the overhead of drawing the same points multiple times.
	unsigned int indices[] = {
		0, 1, 3,  // first triangle
		1, 2, 3   // second triangle
	};

	// All OpenGL buffer objects have a unique ID corresponding to that specific buffer, in this case:
	//		- vertex array object (VAO)
	//		- vertex buffer object (VBO)
	//		- element buffer object (EBO)
	unsigned int VAO, VBO, EBO;

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
	glGenBuffers(1, &EBO);

	// Bind vertex array object (VAO) first, then bind and set vertex buffer object(s) (VBO), and then configure vertex attribute(s).
	glBindVertexArray(VAO);

	// The buffer type of a vertex buffer object is GL_ARRAY_BUFFER.
	// Bind the vertex buffer to the GL_ARRAY_BUFFER target.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Now, all buffer calls we make on the target GL_ARRAY_BUFFER will be used to configure the bound buffer, which in this case is VBO.
	// Copy the vertices data to the buffer, meaning that we copy the data to the memory of the graphics card.
	//	GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
	//	GL_STATIC_DRAW: the data is set only once and used many times.
	//	GL_DYNAMIC_DRAW : the data is changed a lot and used many times.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Bind EBO.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	// Now that we told OpenGL how to interpret the vertex data, we also need to enable the vertex attribute,
	// giving the vertex attribute location as the argument (vertex attributes are disabled by default).
	glEnableVertexAttribArray(0);

	// Color attribute.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture coordinates attribute.
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Generating textures
	// -------------------

	// Configure stb_image.h to flie the y-axis when loading images.
	// OpenGL expectes the 0.0 coordinate on the y-aixs to be on the bottom side of the image, but images usually have 0.0 at the top of the y-axis.
	stbi_set_flip_vertically_on_load(true);

	// Generate an OpenGL texture object.
	unsigned int texture1, texture2;

	// Generate and configure texture1
	// -------------------------------
	glGenTextures(1, &texture1);

	// Bind the created texture to the GL_TEXTURE_2D target.
	glBindTexture(GL_TEXTURE_2D, texture1);

	// Set the texture wrapping options (on the currently bound texture object).
	// - 1st argument specifies the target, in this case, as we're working with a 2D texture, the target is GL_TEXTURE_2D.
	// - 2nd argument specifies what option we want to set and for which texture axis (S/T is basically U/V)
	// - 3rd argument specifies the wrapping mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// If the above glTexParameteri has been set to use wrap mode GL_CLAMP_TO_BORDER, we'd need to specify a border color, with the below 2 lines.
	//float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Set the texture filtering method (on the currently bound texture object).
	// GL_NEAREST (also known as nearest neighbor or point filtering) is the default filtering method of OpenGL.
	// - With GL_NEAREST, OpenGL selected the texture pixel (also known as a texel) that center us closest to on the texture coordinate.
	// - With GL_LINEAR (also known as (bi)linear filtering), OpenGL takes an interpreted value from the texture coordinates neighboring texels,
	//	 approximating a color between the texels.
	// GL_LINEAR usually produces a more realistic output, whereas GL_NEAREST produces a more 8-bit look (more pixelated look).
	// Texture filtering can be set individually for magnifying operations (when scaling up) or minifying operations (when scaling down).
	// This allows us to differentiate the method used when scaling down (GL_TEXTURE_MIN_FILTER) as opposed to scaling up (GL_TEXTURE_MAG_FILTER).
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load a texture image using stb_image.h
	int width, height, nrChannels;

	// The function stbi_load (from stb_image.h library) first  takes an image (container.jpg) and provides us with the width, height and number of color channels.
	// Width and height is used to later generate textures.
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		// Once the texture has been created and bound, we can generate a texture with the loaded image data using glTexImage2D.
		// - 1st argument specifies the texture target, generating a texture on the currently bound texture object on the target, in this case GL_TEXTURE_2D and the texture created using glGenTextures.
		// - 2nd argument specifies the mipmap level for which we want to create a texture for, if you may want to set each mipmap level manually.
		// - 3rd argument specifies what kind of format we want to store the texture in, in OpenGL. The image only has RGB values, so we'll store the texture with RGB values as well.
		// - 4th and 5th arguments specifies the width and the height of the image.
		// - 6th argument should always be hardcoded as 0 (legacy stuff).
		// - 7th and 0th arguments specifies the format and the data type of the source image. As we've loaded the image with RGB values and stored them as chars (bytes), we'll pass in the correponding values (GL_RGB and GL_UNSIGNED_BYTE).
		// - 8th argument specifies the actual image (the data we loaded).
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		// If we somehow failed to load the image.
		std::cout << "Failed to load texture" << std::endl;
	}

	// Wants the texture and its corresponding mipmaps have been generated, we free the image memory using stbi_image_free
	stbi_image_free(data);

	// Generate and configure texture2
	// -------------------------------

	glGenTextures(1, &texture2);
	// Bind other texture.
	glBindTexture(GL_TEXTURE_2D, texture2);

	// Set wrapping.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// The function stbi_load (from stb_image.h library) first  takes an image (container.jpg) and provides us with the width, height and number of color channels.
	// Width and height is used to later generate textures.
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// As awesomeface.png has transparency and thus an alpha channel, we need to tell OpenGL that the data type is of GL_RGBA.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		// If we somehow failed to load the image.
		std::cout << "Failed to load texture" << std::endl;
	}

	// Wants the texture and its corresponding mipmaps have been generated, we free the image memory using stbi_image_free
	stbi_image_free(data);

	shader.use();

	// Tell OpenGL which texture unit each shader sampler belongs to.
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	// Bind element buffer (EBO) and copy the indices data.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
		//float timeValue = glfwGetTime();
		//float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

		// Bind texture object.
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

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
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		// Draw based on element buffer object (EBO)
		// NOTE: glDrawelements is only relevant when an element buffer object (EBO) is in play.
		// - 1st argument specifices the primitive shape that'll be our basis
		// - 2nd argument specifies the count or number of elements we'd like to draw. As we have 6 indices, we want to draw 6 vertices.
		// - 3rd argument specifies  the type of indicies which is of type GL_UNSIGNED_INT
		// - 4th argument allows us to specify an offset in the element buffer (EBO)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		//glBindVertexArray(0);

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
	// Set the dimensions of the OpenGL viewport (the size of the rendering window created with GLFW).
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