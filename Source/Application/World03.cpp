#include "World03.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#define INTERLEAVE

namespace nc
{
    bool World03::Initialize() {
        m_program = GET_RESOURCE(Program, "Shaders/unlit_color.prog");
        m_program->Use();


#ifdef INTERLEAVE
        // vertex data
        float vertexData[] = {  // x y z, r, g, b
        -0.8f, 0.8f, 0.0f, 1.0f, 0.5f, 1.0f,
        0.8f, 0.8f, 0.0f, 1.0f, 0.5f, 0.0f,
        0.8f, -0.8f, 0.0f, 1.0f, 0.5f, 1.0f,
        -0.8f, -0.8f, 0.0f, 1.0f, 0.5f, 0.0f
        };

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glBindVertexBuffer(0, vbo, 0, 6 * sizeof(GLfloat));

        // position
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(0, 0);

        // color
        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));
        glVertexAttribBinding(1, 0);
#else
        // vertex data
        float positionData[] = {  // x y z
        -0.8f, 0.8f, 0.0f,
        0.8f, 0.8f, 0.0f,
        0.8f, -0.8f, 0.0f,
        -0.8f, -0.8f, 0.0f
        };

        float colorData[] =
        {
        1.0f, 0.5f, 1.0f,
        1.0f, 0.5f, 0.0f,
        1.0f, 0.5f, 1.0f,
        1.0f, 0.5f, 0.0f
        };

        GLuint vbo[2];
        glGenBuffers(2, vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(positionData), positionData, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        // position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexBuffer(0, vbo[0], 0, 3 * sizeof(GLfloat));

        // color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexBuffer(1, vbo[1], 0, 3 * sizeof(GLfloat));

#endif

        //m_position.z = -10.0f;
        return true;
    }

    void World03::Shutdown() {
    }

    void World03::Update(float dt) {
        m_angle += 90 * dt;
        m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? m_speed * +dt * 2 : 0;
        m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? m_speed * -dt * 2 : 0;
        m_position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? m_speed * +dt * 2 : 0;
        m_position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? m_speed * -dt * 2 : 0;
        m_time += dt;
        
        // model matrix
        glm::mat4 position = glm::translate(glm::mat4{ 1 }, m_position);
        glm::mat4 rotation = glm::rotate(glm::mat4{ 1 }, glm::radians(m_angle), glm::vec3{ 0, 0, 1 });
        glm::mat4 model = position * rotation;
        GLint uniform = glGetUniformLocation(m_program->m_program, "model");
        glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(model));

        // view matrix
        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 4, 5 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 });
        uniform = glGetUniformLocation(m_program->m_program, "view");
        glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(view));

        // projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.01f, 100.0f);
        uniform = glGetUniformLocation(m_program->m_program, "projection");
        glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(projection));

    }

    void World03::Draw(Renderer& renderer) {
        // pre-render
        renderer.BeginFrame();

        //render
        glBindVertexArray(m_vao);
        glDrawArrays(GL_QUADS, 0, 4);


        // post-render
        renderer.EndFrame();
    }
}
