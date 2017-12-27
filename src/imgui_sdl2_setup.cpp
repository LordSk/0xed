#include "imgui_sdl2_setup.h"
#include "imgui.h"
#include <SDL2/SDL_scancode.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define TIME_MILLI() (clock() / (CLOCKS_PER_SEC / 1000))

struct ImGuiGLSetup
{
    ImGuiContext* pContext = nullptr;
    GLuint shaderProgram = 0;
    GLint shaderViewUni = -1;
    GLint shaderTextureUni = -1;
    GLuint shaderVertexBuff = 0;
    GLuint shaderElementsBuff = 0;
    GLuint shaderVao = 0;
    f32 viewMatrix[16];
    clock_t lastFrameTime;
};

GLuint glMakeShader(GLenum type, const char* pFileBuff, i32 fileSize)
{
    GLuint shader = glCreateShader(type);

    // compile
    glShaderSource(shader, 1, &pFileBuff, &fileSize);
    glCompileShader(shader);

    // check result
    GLint compileResult = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

    if(!compileResult) {
        int logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        char* logBuff = (char*)malloc(logLength);
        glGetShaderInfoLog(shader, logLength, NULL, logBuff);
        LOG("Error [shader compilation]: %s", logBuff);
        free(logBuff);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

static void renderUI(ImDrawData* pDrawData)
{
    ImGuiIO& io = ImGui::GetIO();
    i32 fb_width = (i32)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    i32 fb_height = (i32)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if (fb_width == 0 || fb_height == 0)
        return;
    pDrawData->ScaleClipRects(io.DisplayFramebufferScale);

    // Backup GL state
    GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
    GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    GLint last_array_buffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
    GLint last_element_array_buffer; glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING,
                                                   &last_element_array_buffer);
    GLint last_vertex_array; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
    GLint last_blend_src; glGetIntegerv(GL_BLEND_SRC, &last_blend_src);
    GLint last_blend_dst; glGetIntegerv(GL_BLEND_DST, &last_blend_dst);
    GLint last_blend_equation_rgb; glGetIntegerv(GL_BLEND_EQUATION_RGB, &last_blend_equation_rgb);
    GLint last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &last_blend_equation_alpha);
    GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
    GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
    GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
    GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
    GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

    // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glActiveTexture(GL_TEXTURE0);

    // Setup viewport, orthographic projection matrix
    const ImGuiGLSetup& ui = *(ImGuiGLSetup*)io.UserData;
    glUseProgram(ui.shaderProgram);
    glUniformMatrix4fv(ui.shaderViewUni, 1, GL_FALSE, ui.viewMatrix);
    glUniform1i(ui.shaderViewUni, 0);

    glBindVertexArray(ui.shaderVao);

    for(i32 n = 0; n < pDrawData->CmdListsCount; ++n) {
        const ImDrawList* cmd_list = pDrawData->CmdLists[n];
        const ImDrawIdx* idx_buffer_offset = 0;

        glBindBuffer(GL_ARRAY_BUFFER, ui.shaderVertexBuff);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.size() * sizeof(ImDrawVert),
                     (GLvoid*)&cmd_list->VtxBuffer.front(), GL_STREAM_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ui.shaderElementsBuff);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx),
                     (GLvoid*)&cmd_list->IdxBuffer.front(), GL_STREAM_DRAW);

        for(const ImDrawCmd* pcmd = cmd_list->CmdBuffer.begin();
            pcmd != cmd_list->CmdBuffer.end(); pcmd++) {
            if(pcmd->UserCallback) {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else {
                glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w),
                          (int)(pcmd->ClipRect.z - pcmd->ClipRect.x),
                          (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT,
                               idx_buffer_offset);
            }
            idx_buffer_offset += pcmd->ElemCount;
        }
    }

    // Restore modified GL state
    glUseProgram(last_program);
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glBindVertexArray(last_vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
    glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
    glBlendFunc(last_blend_src, last_blend_dst);
    if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
    if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
    if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
    if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
    glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
}

ImFont* imguiLoadFont(const char* path, i32 sizePx)
{
    ImFontConfig config;
    config.OversampleH = 4;
    config.OversampleV = 4;

    ImGuiIO& io = ImGui::GetIO();
    ImFont* font = io.Fonts->AddFontFromFileTTF(path, sizePx, &config);
    assert(font);

    u8* pFontPixels;
    i32 fontTexWidth, fontTexHeight;
    io.Fonts->GetTexDataAsRGBA32(&pFontPixels, &fontTexWidth, &fontTexHeight);

    GLuint oldTex = (GLuint)(intptr_t)io.Fonts->TexID;
    glDeleteTextures(1, &oldTex);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        fontTexWidth,
        fontTexHeight,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        pFontPixels
    );
    glBindTexture(GL_TEXTURE_2D, 0);

    io.Fonts->SetTexID((void*)(intptr_t)texture);
    return font;
}

ImGuiGLSetup* imguiInit(u32 width, u32 height)
{
    ImGuiGLSetup* ims = (ImGuiGLSetup*)malloc(sizeof(ImGuiGLSetup));

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = width;
    io.DisplaySize.y = height;
    io.IniFilename = "imgui.ini";
    io.RenderDrawListsFn = renderUI;
    io.UserData = ims;

    u8* pFontPixels;
    i32 fontTexWidth, fontTexHeight;
    io.Fonts->GetTexDataAsRGBA32(&pFontPixels, &fontTexWidth, &fontTexHeight);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        fontTexWidth,
        fontTexHeight,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        pFontPixels
    );
    glBindTexture(GL_TEXTURE_2D, 0);

    io.Fonts->SetTexID((void*)(intptr_t)texture);
    ims->lastFrameTime = TIME_MILLI();

    // ui shader
    constexpr const char* vertexShader = ""
        "#version 330 core\n"
        "layout(location = 0) in vec2 position;\n"
        "layout(location = 1) in vec2 uv;\n"
        "layout(location = 2) in vec4 color;\n"
        "uniform mat4 uViewMatrix;\n"

        "out vec2 vert_uv;\n"
        "out vec4 vert_color;\n"

        "void main() {\n"
        "	vert_uv = uv;\n"
        "	vert_color = color;\n"
        "	gl_Position = uViewMatrix * vec4(position, 0.0, 1.0);\n"
        "}";

    constexpr const char* fragmentShader = "\
        #version 330 core\n\
        uniform sampler2D uTextureData;\n\
        \
        in vec2 vert_uv;\n\
        in vec4 vert_color;\n\
        out vec4 fragmentColor;\n\
        \
        void main()\n\
        {\n\
            fragmentColor = texture(uTextureData, vert_uv) * vert_color;\n\
        }";

    u32 vertexShaderLen = strlen(vertexShader);
    u32 fragmentShaderLen = strlen(fragmentShader);

    GLuint vsId = glMakeShader(GL_VERTEX_SHADER, vertexShader, vertexShaderLen);
    GLuint fsId = glMakeShader(GL_FRAGMENT_SHADER, fragmentShader, fragmentShaderLen);
    if(!vsId || !fsId) {
        return false;
    }

    ims->shaderProgram = glCreateProgram();
    glAttachShader(ims->shaderProgram, vsId);
    glAttachShader(ims->shaderProgram, fsId);
    glLinkProgram(ims->shaderProgram);

    // check
    GLint linkResult = GL_FALSE;
    glGetProgramiv(ims->shaderProgram, GL_LINK_STATUS, &linkResult);

    if(!linkResult) {
        int logLength = 0;
        glGetProgramiv(ims->shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
        char* logBuff =  (char*)malloc(logLength);
        glGetProgramInfoLog(ims->shaderProgram, logLength, NULL, logBuff);
        LOG("Error [program link]: %s", logBuff);
        free(logBuff);
        glDeleteProgram(ims->shaderProgram);

        free(ims);
        return 0;
    }

    if(!ims->shaderProgram) {
        free(ims);
        return 0;
    }

    ims->shaderViewUni = glGetUniformLocation(ims->shaderProgram, "uViewMatrix");
    ims->shaderTextureUni = glGetUniformLocation(ims->shaderProgram, "uTextureData");

    glGenBuffers(1, &ims->shaderVertexBuff);
    glGenBuffers(1, &ims->shaderElementsBuff);

    glGenVertexArrays(1, &ims->shaderVao);
    glBindVertexArray(ims->shaderVao);
    glBindBuffer(GL_ARRAY_BUFFER, ims->shaderVertexBuff);

    enum Location {
        POSITION = 0,
        UV = 1,
        COLOR = 2
    };

    glEnableVertexAttribArray(Location::POSITION);
    glEnableVertexAttribArray(Location::UV);
    glEnableVertexAttribArray(Location::COLOR);

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
    glVertexAttribPointer(Location::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert),
                          (GLvoid*)OFFSETOF(ImDrawVert, pos));
    glVertexAttribPointer(Location::UV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert),
                          (GLvoid*)OFFSETOF(ImDrawVert, uv));
    glVertexAttribPointer(Location::COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert),
                          (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF

    io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
    io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
    io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
    io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
    io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
    io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
    io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
    io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;
    io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
    io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
    io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
    io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
    io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
    io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;

    return ims;
}

void imguiDeinit(ImGuiGLSetup* ims)
{
    ImGui::GetIO().Fonts->Clear();
    free(ims);
    ims = 0;
}

void imguiUpdate(ImGuiGLSetup* ims, f64 delta)
{
    ImGuiIO& io = ImGui::GetIO();

    // update view matrix
    f32 left = 0;
    f32 right = io.DisplaySize.x;
    f32 top = 0;
    f32 bottom = io.DisplaySize.y;
    f32 nearPlane = -1.f;
    f32 farPlane = 1.f;
    memset(ims->viewMatrix, 0, sizeof(ims->viewMatrix));

    f32* mat = ims->viewMatrix;
    mat[15] = 1.f;
    mat[0] = 2.f / (right - left);
    mat[5] = 2.f / (top - bottom);
    mat[10] = -2.f / (farPlane - nearPlane);
    mat[12] = -((right + left) / (right - left));
    mat[13] = -((top + bottom) / (top - bottom));
    mat[14] = -((farPlane + nearPlane) / (farPlane - nearPlane));


    clock_t now = TIME_MILLI();
    io.DeltaTime = (now - ims->lastFrameTime) / 1000.f;
    ims->lastFrameTime = now;
    i32 mx, my;
    u32 mstate = SDL_GetMouseState(&mx, &my);
    io.MousePos = ImVec2((f32)mx, (f32)my);
    io.MouseDown[0] = mstate & SDL_BUTTON(SDL_BUTTON_LEFT);
    io.MouseDown[1] = mstate & SDL_BUTTON(SDL_BUTTON_RIGHT);
    ImGui::NewFrame();
}

void imguiHandleInput(ImGuiGLSetup* ims, const SDL_Event& event)
{
    ImGuiIO& io = ImGui::GetIO();

    if(event.type == SDL_MOUSEWHEEL) {
        io.MouseWheel = event.wheel.y;
        return;
    }

    if(event.type == SDL_KEYDOWN) {
        io.KeysDown[event.key.keysym.scancode] = true;

        if(event.key.keysym.scancode == SDL_SCANCODE_KP_ENTER) {
            io.KeysDown[io.KeyMap[ImGuiKey_Enter]] = true;
        }

        if(event.key.keysym.scancode == SDL_SCANCODE_LCTRL ||
           event.key.keysym.scancode == SDL_SCANCODE_RCTRL) {
            io.KeyCtrl = true;
        }
        if(event.key.keysym.scancode == SDL_SCANCODE_LSHIFT ||
           event.key.keysym.scancode == SDL_SCANCODE_RSHIFT) {
            io.KeyShift = true;
        }
        if(event.key.keysym.scancode == SDL_SCANCODE_LALT ||
           event.key.keysym.scancode == SDL_SCANCODE_RALT) {
            io.KeyAlt = true;
        }

        if(event.key.keysym.mod & KMOD_CTRL) {
            io.KeyCtrl = true;
        }
        if(event.key.keysym.mod & KMOD_ALT) {
            io.KeyAlt = true;
        }
        if(event.key.keysym.mod & KMOD_SHIFT) {
            io.KeyShift = true;
        }
        return;
    }

    if(event.type == SDL_KEYUP) {
        io.KeysDown[event.key.keysym.scancode] = false;

        if(event.key.keysym.scancode == SDL_SCANCODE_KP_ENTER) {
            io.KeysDown[io.KeyMap[ImGuiKey_Enter]] = false;
        }

        if(event.key.keysym.scancode == SDL_SCANCODE_LCTRL ||
           event.key.keysym.scancode == SDL_SCANCODE_RCTRL) {
            io.KeyCtrl = false;
        }
        if(event.key.keysym.scancode == SDL_SCANCODE_LSHIFT ||
           event.key.keysym.scancode == SDL_SCANCODE_RSHIFT) {
            io.KeyShift = false;
        }
        if(event.key.keysym.scancode == SDL_SCANCODE_LALT ||
           event.key.keysym.scancode == SDL_SCANCODE_RALT) {
            io.KeyAlt = false;
        }
        return;
    }

    if(event.type == SDL_TEXTINPUT) {
        io.AddInputCharactersUTF8(event.text.text);
        return;
    }
}
