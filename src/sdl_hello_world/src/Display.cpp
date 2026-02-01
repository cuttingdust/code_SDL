#include "Display.h"


#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <iostream>

class Display::PImpl
{
public:
    PImpl(Display *owenr);
    ~PImpl() = default;

public:
    Display      *owenr_    = nullptr;
    SDL_Window   *windows_  = nullptr;
    SDL_GLContext ctx_      = nullptr;
    bool          isClosed_ = false;
};

Display::PImpl::PImpl(Display *owenr) : owenr_(owenr)
{
}

Display::Display(int width, int height, const std::string_view &title) : impl_(std::make_unique<Display::PImpl>(this))
{
    SDL_Init(SDL_INIT_EVERYTHING);


    /// 设置 OpenGL 属性
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    impl_->windows_ = SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
                                       SDL_WINDOW_OPENGL);
    if (!impl_->windows_)
    {
        std::cerr << "窗口创建失败: " << SDL_GetError() << std::endl;
        SDL_Quit();
    }


    impl_->ctx_ = SDL_GL_CreateContext(impl_->windows_);
    if (!impl_->ctx_)
    {
        std::cerr << "OpenGL上下文创建失败: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(impl_->windows_);
        SDL_Quit();
    }

    /// 初始化 GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "GLAD初始化失败" << std::endl;
        SDL_GL_DeleteContext(impl_->ctx_);
        SDL_DestroyWindow(impl_->windows_);
        SDL_Quit();
    }

    impl_->isClosed_ = false;
}

Display::~Display()
{
    SDL_GL_DeleteContext(impl_->ctx_);
    SDL_DestroyWindow(impl_->windows_);
    SDL_Quit();
}

void Display::update()
{
    SDL_GL_SwapWindow(impl_->windows_);

    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            impl_->isClosed_ = true;
        }
    }
}

bool Display::isClosed() const
{
    return impl_->isClosed_;
}

void Display::clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
