#define WIDTH 800
#define HEIGHT 600

//#include "graph.cpp"

/*
GLuint loadTextures(char const* path) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_uc* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        switch (nrComponents) {
        case 1: format = GL_RED; break;
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default: format = 0;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    }
    else std::cout << "Failed to load texture:" << std::endl << path << std::endl;

    stbi_image_free(data);

    return textureID;
}

*/
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };

class Smoke {
    GLuint posBuf[2], velBuf[2];
    GLuint particleArray[2];
    GLuint feedback[2], initVel, startTime[2];
    GLuint drawBuf = 1;
    GLuint renderSub, updateSub;

    int nParticles = 500;

    GLuint smokeTexture;

    ShaderSmoke shader = ShaderSmoke("shaders/smoke.vs", "shaders/smoke.fs");
public:
    Smoke() {
        // Generate the buffers
        glGenBuffers(2, posBuf);    // position buffers
        glGenBuffers(2, velBuf);    // velocity buffers
        glGenBuffers(2, startTime); // Start time buffers
        glGenBuffers(1, &initVel);  // Initial velocity buffer (never changes, only need one)

         // Allocate space for all buffers
        int size = nParticles * 3 * sizeof(GLfloat);
        glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
        glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
        glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
        glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
        glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
        glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
        glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
        glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
        glBindBuffer(GL_ARRAY_BUFFER, initVel);
        glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, startTime[0]);
        glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_DYNAMIC_COPY);
        glBindBuffer(GL_ARRAY_BUFFER, startTime[1]);
        glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_DYNAMIC_COPY);

        // Fill the first velocity buffer with random velocities
        glm::vec3 v(0.0f);
        float velocity, theta, phi;
        GLfloat* data = new GLfloat[nParticles * 3];

        for (unsigned int i = 0; i < nParticles; i++) {
            theta = glm::mix(0.0f, glm::pi<float>() / 3.0f, randFloat());
            phi = glm::mix(0.0f, glm::two_pi<float>(), randFloat());

            v.x = sinf(theta) * cosf(phi);
            v.y = cosf(theta);
            v.z = sinf(theta) * sinf(phi);

            velocity = glm::mix(0.1f, 0.3f, randFloat());
            v = glm::normalize(v) * velocity;

            data[3 * i] = v.x;
            data[3 * i + 1] = v.y;
            data[3 * i + 2] = v.z;
        }

        glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
        glBindBuffer(GL_ARRAY_BUFFER, initVel);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

        // Fill the first start time buffer
        GLfloat* tdata = new GLfloat[nParticles];
        float ttime = 0.0f;
        float rate = 0.2f;
        for (int i = 0; i < nParticles; i++) {
            tdata[i] = ttime;
            ttime += rate;
        }
        glBindBuffer(GL_ARRAY_BUFFER, startTime[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Create vertex arrays for each set of buffers
        glGenVertexArrays(2, particleArray);

        // Set up particle array 0
        glBindVertexArray(particleArray[0]);
        glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, startTime[0]);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, initVel);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(3);

        // Set up particle array 1
        glBindVertexArray(particleArray[1]);
        glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, startTime[1]);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, initVel);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(3);

        glBindVertexArray(0);

        // Setup the feedback objects
        glGenTransformFeedbacks(2, feedback);

        // Transform feedback 0
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, startTime[0]);

        // Transform feedback 1
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, startTime[1]);

        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

        GLint value;
        glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_BUFFERS, &value);
        printf("MAX_TRANSFORM_FEEDBACK_BUFFERS = %d\n", value);

        const char* outputNames[] = { "Position", "Velocity", "StartTime" };
        glTransformFeedbackVaryings(shader.Program, 3, outputNames, GL_SEPARATE_ATTRIBS);
        shader.Link();

        smokeTexture = loadTexture("textures/smoke.png");
        //float T = glfwGetTime();

        delete[] data;
        delete[] tdata;
    }

    void Draw() {
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
        glm::mat4 model(1.0f);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, smokeTexture);

        shader.Use();

        renderSub = glGetSubroutineIndex(shader.Program, GL_VERTEX_SHADER, "render");
        updateSub = glGetSubroutineIndex(shader.Program, GL_VERTEX_SHADER, "update");

        glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &updateSub);

        shader.setInt("texture1", 0);
        shader.setFloat("ParticleLifetime", 7.0f);
        glm::vec3 Accel(0.0f, 0.1f, 0.0f);
        shader.setVec3("Accel", Accel);
        shader.setFloat("MinParticleSize", 10.0f);
        shader.setFloat("MaxParticleSize", 200.0f);

        shader.setFloat("Time", glfwGetTime());
        shader.setFloat("H", glfwGetTime() - T);
        T = glfwGetTime();


        glEnable(GL_PROGRAM_POINT_SIZE);
        glPointSize(10.0);

        glEnable(GL_RASTERIZER_DISCARD);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
        glBeginTransformFeedback(GL_POINTS);
        glBindVertexArray(particleArray[1 - drawBuf]);
        glDrawArrays(GL_POINTS, 0, nParticles);
        glEndTransformFeedback();

        glDisable(GL_RASTERIZER_DISCARD);

        // Render pass
        glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &renderSub);

        model = glm::mat4(1.0f);
        glm::vec3 la(2.0f, 0.0f, 2.0f);
        model = glm::translate(model, la);
        glm::mat4 mv = view * model;
        shader.setMat4("MVP", projection * mv);

        glBindVertexArray(particleArray[drawBuf]);
        glDrawTransformFeedback(GL_POINTS, feedback[drawBuf]);

        // swap buffers
        drawBuf = 1 - drawBuf;

        glBindVertexArray(0);

        glEnable(GL_DEPTH_TEST);
    }

    ~Smoke() {
        //delete[] data;
        //delete[] tdata;
        glDeleteBuffers(1, &initVel);
        glDeleteBuffers(2, posBuf);
        glDeleteBuffers(2, velBuf);    // velocity buffers
        glDeleteBuffers(2, startTime);

        //glDeleteBuffers(2, &startTime); 
        //glDeleteVertexArrays(1, &particles);
    }
};
