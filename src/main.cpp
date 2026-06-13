#include "shader.h"
#include "texture.h"
#include "camera.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <sstream>
#include <iomanip>


/* Called automatically when the window size changes.
 We tell OpenGL about the new drawing area so things are not stretched or clipped.*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
        glViewport(0, 0, width, height);
}  

constexpr unsigned int scr_WIDTH = 800;
constexpr unsigned int scr_HEIGHT = 600;

// Simple input handler checked every frame: Pressing ESC will ask the window to close.
float deltaTime = 0.0f;//Time between the current frame and the last frame
float lastFrame = 0.0f;//Time of the last frame.
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//Lighting: 
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
        glfwSetWindowShouldClose(window, true); 

    
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}


float lastX = scr_WIDTH / 2.0f;
float lastY = scr_HEIGHT / 2.0f;
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xposn, double yposn)
{
    float xpos = static_cast<float>(xposn);
    float ypos = static_cast<float>(yposn);

    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float offsetX = xpos - lastX;
    float offsetY = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(offsetX, offsetY);
} 

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

int main() 
{
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return -1;
    }
    
    // OpenGL 4.6 core profile.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    
    // Created a window with an OpenGL context.
    GLFWwindow* window = glfwCreateWindow(scr_WIDTH, scr_HEIGHT, "Introduction", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    /* Make the OpenGL context of the window current on this thread,
     and register the resize callback so viewport updates automatically.*/
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // Mouse Inputs:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Print the OpenGL version to confirm everything initialized properly.
    std::cout<<"OpenGL Version:- "<<glGetString(GL_VERSION)<<std::endl;

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout<<"Max number of the vertex attributes supported: "<<nrAttributes<<std::endl;

    glEnable(GL_DEPTH_TEST);
    /* Vertex data layout:
       Each vertex has 3 floats for position (x,y,z) followed by 3 floats for color (r,g,b).
       Coordinates are in normalized device space after the vertex shader: x and y range from -1 to 1.*/
    float vertices[] = {
        // FRONT FACE
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

        // BACK FACE
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        // LEFT FACE
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        // RIGHT FACE
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        // BOTTOM FACE
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        // TOP FACE
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    
    unsigned int indices[] = {
        // FRONT
        0,1,2,
        3,4,5,

        // BACK
        6,7,8,
        9,10,11,

        // LEFT
        12,13,14,
        15,16,17,

        // RIGHT
        18,19,20,
        21,22,23,

        // TOP
        24,25,26,
        27,28,29,

        // BOTTOM
        30,31,32,
        33,34,35
    };

    glm::vec3 cubePos[]= {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    /* Generate and bind a VAO (stores vertex attribute settings) and VBO (stores vertex data).
     VAO remembers the calls to glVertexAttribPointer so we don't have to set them each frame.*/
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    //Vertices: size = 3 stride = 8 floats(for rectangle); offset = 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    //Texture: size = 3    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
    /* uncomment this call to draw in wireframe polygons.
     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/

    // Creating the light source object by making a new VAO:-
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    //Shader location: 
    shaderProgram shader("./assets/shaders/shader.vert",
                         "./assets/shaders/shader.frag");

    //Lightshader location:
    shaderProgram lightCubeShader(
                        "./assets/shaders/shader.vert",
                        "./assets/shaders/lightsrc.frag");

    //Texture location: 
    textureProgram container("assets/textures/container.png");
    textureProgram IGT("assets/textures/igotthis.jpg");     

    //FPS Counter: 
    double previousTime = glfwGetTime();
    int frameCount = 0;
    
    while (!glfwWindowShouldClose(window)) 
    {
        double currentTime = glfwGetTime();
        frameCount++;

        if (currentTime - previousTime >= 1.0)
        {
            double fps = double(frameCount) / (currentTime - previousTime);

            std::stringstream ss;
            ss << "My Basic Render (Press ESC to exit :) )| FPS: " << std::fixed << std::setprecision(2) << fps;

            glfwSetWindowTitle(window, ss.str().c_str());

            frameCount = 0;
            previousTime = currentTime;
        }

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);
        
        glClearColor(0.01f, 0.001f, 0.0001f, 1.0f); // background color
        glClear(GL_COLOR_BUFFER_BIT);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shader.use();
        float timeVal = (float)glfwGetTime(); 
        shader.setFloat("time", timeVal);
        
        //Color of the light src. and the obj.:
        glUniform3f(glGetUniformLocation(shader.program, "objColor"), 0.13f, 0.99f, 0.64f);
        glUniform3f(glGetUniformLocation(shader.program,"lightsrcColor"), 0.3f, 0.77f, 0.11f);
        glUniform3f(glGetUniformLocation(shader.program,"lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(shader.program,"viewPos"), camera.Position.x, camera.Position.y, 
                                                                    camera.Position.z);        

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        
        int resLoc = glGetUniformLocation(shader.program, "resolution");
        glUniform2f(resLoc, (float)width, (float)height);
        
        container.bind(GL_TEXTURE0);
        IGT.bind(GL_TEXTURE1);
        glUniform1i(glGetUniformLocation(shader.program, "container"),0);
        glUniform1i(glGetUniformLocation(shader.program, "IGT"), 1);         
        
        
        //MVP starts here: 
        // 1)Model View Projection Matrix Calculation:
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.25f, 0.75f)); // Eg: rotate around x axis-> yz plane rotates.
        int modelLoc = glGetUniformLocation(shader.program, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        // 2)View Matrix Projection Calculation:
        glm::mat4 view = camera.GetViewMatrix();
        int viewLoc = glGetUniformLocation(shader.program, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        
        // 3) Projection Matrix Calculation:
        glm::mat4 proj;
        proj = glm::perspective(glm::radians(camera.Zoom), 
                                                        (float)scr_WIDTH/(float)scr_HEIGHT
                                                        , 0.1f, 100.0f);
        int projLoc = glGetUniformLocation(shader.program, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
        
        
        glBindVertexArray(VAO); /* seeing as we only have a single VAO there's no need to bind it every time,
        but we'll do so to keep things a bit more organized*/

        for (size_t i = 0; i < 10; i++){
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePos[i]); 
            float angle = 20.0f * i;
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f)); 
            int modelLoc = glGetUniformLocation(shader.program, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            //two triangles withand without the unique positions to form quadrilateral.
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); 
        }
        lightCubeShader.use();

        glUniformMatrix4fv(glGetUniformLocation(lightCubeShader.program, "view"),
                            1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(lightCubeShader.program, "projection"),
                            1, GL_FALSE, glm::value_ptr(proj));

        glm::mat4 lightSrc = glm::mat4(1.0f);
        lightSrc = glm::translate(lightSrc, lightPos);
        lightSrc = glm::scale(lightSrc, glm::vec3(0.2f));         

        glUniformMatrix4fv(glGetUniformLocation(lightCubeShader.program, "model"),
                            1, GL_FALSE, glm::value_ptr(lightSrc));
        glBindVertexArray(lightVAO);

        glDrawElements(
            GL_TRIANGLES,
            36,
            GL_UNSIGNED_INT,
            0
        );

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Clean up GPU resources and close the window properly.
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader.program);
    
    glfwTerminate();
    return 0;
}

/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////O    T    H   E   R         C    O    N    C    E    P     T     S  A    P    P    L    I     E   D//////////////// 
////////////////////////////////////////////////H    E    R    E////////////////////////////////////////////////////////////////////     
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// glm test: 
// glm::vec4 vec(0.0f, 0.0f, 0.0f, 1.0f); // x=1, y=0, z=0, a=1;
//glm::ortho(left, right, bottom, top, near, far);
//glm::perspective(fieldOfView, aspectRatio, near, far);
// glm::mat4 trans = glm::mat4(1.0f); //transformation starting with the identity matrix.
// trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f)); //translate to x=1, y=1, z=0
// trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f)); //rotation around the z-axis.
// trans = glm::scale(trans, glm::vec3(2.0f, 2.0f, 2.0f)); //scale the object to 0.5x times.
// vec = trans * vec ;
// std::cout << vec.x << "\t" << vec.y << "\t" <<vec.z << "\t" <<std::endl;
// unsigned int transformLoc = glGetUniformLocation(shader.program, "transform");
// glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));


/*float vertices[] = {
    //|<----pos(vertex)--->|<-----color---->|<----textCoords--->|
        // FRONT FACE
        0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,

        // BACK FACE
        0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,

        // LEFT FACE
        -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,

        // RIGHT FACE
        0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,

        0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,

        // TOP FACE
        0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,

        // BOTTOM FACE
        0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f
     };
// //Look-At Vector;
// glm::mat4 view;
// view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), //CameraPos
//                    glm::vec3(0.0f, 0.0f, 0.0f), //CameraTarget
//                    glm::vec3(0.0f, 1.0f, 0.0f));//Up axis

//Animation of camera revolving around x and z axis.
// const float radius = 10.0f ;
// float camX = sin((float)glfwGetTime()) * radius;
// float camZ = cos((float)glfwGetTime()) * radius;
// glm::mat4 view;
// view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), //CameraPos
//                    glm::vec3(0.0f, 0.0f, 0.0f), //CameraTarget
//                    glm::vec3(0.0f, 1.0f, 0.0f));//Up axis
// int viewLoc = glGetUniformLocation(shader.program, "view");
// glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
*/