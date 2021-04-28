#include "spdlog/spdlog.h"
#include <fmt/format.h>

#include "entry_p.h"

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#   if ENTRY_CONFIG_USE_WAYLAND
#       include <wayland-egl.h>
#   endif
#elif BX_PLATFORM_WINDOWS
#   define SDL_MAIN_HANDLED
#endif

#include "SDL.h"

BX_PRAGMA_DIAGNOSTIC_PUSH()
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG("-Wextern-c-compat")
#include "SDL_syswm.h"
BX_PRAGMA_DIAGNOSTIC_POP()

#include <bgfx/platform.h>
#if defined(None) // X11 defines this...
#   undef None
#endif // defined(None)

#include <bx/thread.h>
#include <bx/handlealloc.h>
// #include <tinystl/allocator.h>
#include <tinystl/string.h>



#include "SDL_net.h"
// #include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include <entt/entt.hpp>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp> //glm::pi<float>()
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp> // glm::to_string



// #include "ozz/animation/runtime/animation.h"
// #include "ozz/animation/runtime/skeleton.h"
// #include "ozz/animation/runtime/sampling_job.h"
// #include "ozz/base/containers/vector.h"
// #include "ozz/base/maths/soa_transform.h"

// #include "ozz/base/maths/box.h"
// #include "ozz/base/maths/simd_quaternion.h"
// #include "ozz/base/memory/allocator.h"
// #include "ozz/animation/runtime/track.h"
// #include "ozz/animation/offline/raw_skeleton.h"
// #include "ozz/animation/runtime/local_to_model_job.h"
// #include "ozz/base/io/archive.h"
// #include "ozz/base/io/stream.h"
// #include "ozz/base/log.h"
// #include "ozz/base/maths/math_ex.h"
// #include "ozz/base/maths/simd_math.h"
// #include "ozz/base/maths/vec_float.h"
// #include "ozz/base/io/archive_traits.h"
// #include "ozz/base/platform.h"
// #include "ozz/base/containers/vector_archive.h"
// #include "ozz/base/maths/math_archive.h"
// #include "ozz/base/maths/simd_math_archive.h"
// #include "ozz/geometry/runtime/skinning_job.h"







template <>
struct fmt::formatter<tinystl::string>
{
    constexpr auto parse(format_parse_context& ctx)
    {
        auto it = ctx.begin(), end = ctx.end();
        while (it != end)
            *it++;
        return it;
    }
    template <typename FormatContext> auto format(const tinystl::string& p, FormatContext& ctx)
    {
        return format_to(ctx.out(), "\"{}\"", p.c_str());
    }
};


// bool LoadSkeleton(const char* _filename, ozz::animation::Skeleton* _skeleton)
// {
//     assert(_filename && _skeleton);
//     spdlog::debug("Loading skeleton archive {}.", _filename);
//     ozz::io::File file(_filename, "rb");
//     if (!file.opened()) {
//         spdlog::error("Failed to open skeleton file {}.", _filename);
//         return false;
//     }
//     ozz::io::IArchive archive(&file);
//     if (!archive.TestTag<ozz::animation::Skeleton>()) {
//         spdlog::error("Failed to load skeleton instance from file {}.", _filename);
//         return false;
//     }

//     // Once the tag is validated, reading cannot fail.
//     archive >> *_skeleton;
//     return true;
// }


// bool LoadAnimation(const char* _filename,
//                    ozz::animation::Animation* _animation)
// {
//     assert(_filename && _animation);
//     spdlog::debug("Loading animation archive: {}.", _filename);
//     ozz::io::File file(_filename, "rb");
//     if (!file.opened()) {
//         spdlog::error("Failed to open animation file {}.", _filename);
//         return false;
//     }
//     ozz::io::IArchive archive(&file);
//     if (!archive.TestTag<ozz::animation::Animation>()) {
//         spdlog::error("Failed to load animation instance from file {}.", _filename);
//         return false;
//     }

//     // Once the tag is validated, reading cannot fail.
//     archive >> *_animation;
//     return true;
// }


namespace entry
{
    ///
    static void* sdlNativeWindowHandle(SDL_Window* _window)
    {
        SDL_SysWMinfo wmi;
        SDL_VERSION(&wmi.version);
        if (!SDL_GetWindowWMInfo(_window, &wmi) )
        {
            return NULL;
        }

#   if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#       if ENTRY_CONFIG_USE_WAYLAND
        wl_egl_window *win_impl = (wl_egl_window*)SDL_GetWindowData(_window, "wl_egl_window");
        if(!win_impl)
        {
            int width, height;
            SDL_GetWindowSize(_window, &width, &height);
            struct wl_surface* surface = wmi.info.wl.surface;
            if(!surface)
                return nullptr;
            win_impl = wl_egl_window_create(surface, width, height);
            SDL_SetWindowData(_window, "wl_egl_window", win_impl);
        }
        return (void*)(uintptr_t)win_impl;
#       else
        return (void*)wmi.info.x11.window;
#       endif
#   elif BX_PLATFORM_OSX
        return wmi.info.cocoa.window;
#   elif BX_PLATFORM_WINDOWS
        return wmi.info.win.window;
#   endif // BX_PLATFORM_
    }

    inline bool sdlSetWindow(SDL_Window* _window)
    {
        SDL_SysWMinfo wmi;
        SDL_VERSION(&wmi.version);
        if (!SDL_GetWindowWMInfo(_window, &wmi) )
        {
            return false;
        }

        bgfx::PlatformData pd;
#   if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#       if ENTRY_CONFIG_USE_WAYLAND
        pd.ndt          = wmi.info.wl.display;
#       else
        pd.ndt          = wmi.info.x11.display;
#       endif
#   elif BX_PLATFORM_OSX
        pd.ndt          = NULL;
#   elif BX_PLATFORM_WINDOWS
        pd.ndt          = NULL;
#   endif // BX_PLATFORM_
        pd.nwh          = sdlNativeWindowHandle(_window);

        pd.context      = NULL;
        pd.backBuffer   = NULL;
        pd.backBufferDS = NULL;
        bgfx::setPlatformData(pd);

        return true;
    }

    static void sdlDestroyWindow(SDL_Window* _window)
    {
        if(!_window)
            return;
#   if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#       if ENTRY_CONFIG_USE_WAYLAND
        wl_egl_window *win_impl = (wl_egl_window*)SDL_GetWindowData(_window, "wl_egl_window");
        if(win_impl)
        {
            SDL_SetWindowData(_window, "wl_egl_window", nullptr);
            wl_egl_window_destroy(win_impl);
        }
#       endif
#   endif
        SDL_DestroyWindow(_window);
    }

    struct MainThreadEntry
    {
        static int32_t threadFunc(bx::Thread* _thread, void* _userData);
    };

    struct Msg
    {
        Msg()
            : m_x(0)
            , m_y(0)
            , m_width(0)
            , m_height(0)
            , m_flags(0)
        {
        }

        int32_t  m_x;
        int32_t  m_y;
        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_flags;
    };

    static uint32_t s_userEventStart;

    enum SDL_USER_WINDOW
    {
        SDL_USER_WINDOW_DESTROY,

        SDL_USER_WINDOW_SET_FLAGS,
        SDL_USER_WINDOW_SET_POS,
        SDL_USER_WINDOW_SET_SIZE,
        SDL_USER_WINDOW_TOGGLE_FRAME,
        SDL_USER_WINDOW_TOGGLE_FULL_SCREEN,
        // SDL_USER_WINDOW_MOUSE_LOCK,
    };

    static void sdlPostEvent(SDL_USER_WINDOW _type, WindowHandle _handle, Msg* _msg = NULL, uint32_t _code = 0)
    {
        SDL_Event event;
        SDL_UserEvent& uev = event.user;
        uev.type = s_userEventStart + _type;

        union { void* p; WindowHandle h; } cast;
        cast.h = _handle;
        uev.data1 = cast.p;

        uev.data2 = _msg;
        uev.code = _code;
        SDL_PushEvent(&event);
    }

    static WindowHandle getWindowHandle(const SDL_UserEvent& _uev)
    {
        union { void* p; WindowHandle h; } cast;
        cast.p = _uev.data1;
        return cast.h;
    }

    struct Context
    {
        Context()
            : m_width(ENTRY_DEFAULT_WIDTH)
            , m_height(ENTRY_DEFAULT_HEIGHT)
        {
            spdlog::set_level(spdlog::level::debug);
        }

        int run()
        {
            spdlog::info("Starting BGFX");
            spdlog::info("Compiler:" BX_COMPILER_NAME " / " BX_CPU_NAME "-" BX_ARCH_NAME " / " BX_PLATFORM_NAME);

            SDL_Init(SDL_INIT_EVERYTHING);
// ===================================
            // ========== sdl2 net ========================

    // int result = SDLNet_Init();

    // if (result)
    // {
    //     spdlog::error("SDLNet_Init: {}", SDLNet_GetError());
    //     // return result;
    // }

    // ========== sdl2 mixer ========================
    // int mixerFlags = MIX_INIT_OGG |
    //             MIX_INIT_MP3 |
    //             MIX_INIT_OPUS |
    //             MIX_INIT_FLAC |
    //             MIX_INIT_MID;
    // result = Mix_Init(mixerFlags);
    // if (!(result & mixerFlags))
    // {
    //     spdlog::error("Mix_Init: Failed to init required ogg and mod support!");
    //     spdlog::error("Mix_Init: {}", Mix_GetError());
    // }

    // if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    // {
    //     spdlog::error("Mix_OpenAudio failed");
    //     // return 1;
    // }

    // music = Mix_LoadMUS("../res/audio/links_2_3_4.mp3");
    // if (music == NULL)
    // {
    //  spdlog::error("Failed to load music");
    //  // return 1;
    // }
    // Mix_PlayMusic(music, -1);
    // ========== sdl2 ttf ========================
    // if (TTF_Init() == -1)
    // {
    //     spdlog::error("TTF_Init error: {}", TTF_GetError());
    //     // return 1;
    // }

    // auto f = TTF_OpenFont("../res/fonts/SourceCodePro-Regular.ttf", 24);
    // if (!f)
    // {
    //     spdlog::error("TTF_OpenFont error: {}", TTF_GetError());
    // }
    // SDL_Color color = {255, 255, 255};
    // SDL_Surface* text_surf = TTF_RenderUTF8_Blended(f, "Hello world", color);
    // if (!text_surf)
    // {
    //     spdlog::error("TTF_RenderUTF8_Blended error: {}", TTF_GetError());
    //     // return NULL;
    // }
    // spdlog::debug("Hello world text_surf {}x{}", text_surf->w, text_surf->h);

    // TTF_CloseFont(f);
    // ========================== EnTT and glm ================================
    struct TransformComponent
    {
        glm::mat4 Transform;
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4& transform) : Transform(transform) {}
        operator glm::mat4&() { return Transform; }
        operator const glm::mat4&() const { return Transform; }
    };

    struct MeshComponent
    {
        int mesh_id;
        MeshComponent() = default;
        MeshComponent(const MeshComponent&) = default;
        MeshComponent(const int id) : mesh_id(id) {}
    };

    TransformComponent transform;
    MeshComponent mc;

    entt::registry m_Registry;
    entt::entity entity = m_Registry.create();
    auto transf = m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));
    auto x = m_Registry.emplace<MeshComponent>(entity, mc);
    auto v = m_Registry.view<TransformComponent, MeshComponent>();
    for (auto entity: v)
    {
        auto [ttt, mmm] = v.get<TransformComponent, MeshComponent>(entity);
        spdlog::debug("Entity: {}, transform: {}, mesh_id: {}", (int) entity, glm::to_string(ttt.Transform).c_str(), (int) mmm.mesh_id);
    }
 //    // ============================= OZZ ==================================================

    tinystl::string skeleton = "../res/models/skeleton.ozz";
    tinystl::string animation = "../res/models/animation.ozz";
    // tinystl::string mesh = "../res/models/mesh.ozz";

    // ozz::animation::Skeleton m_skeleton;
    // ozz::animation::Animation m_animation;

    // ozz::vector<dvigl::Mesh> m_meshes;

    // // Reading skeleton.
    // if (!LoadSkeleton(skeleton.c_str(), &m_skeleton)) {
    //     spdlog::error("Failed to load skeleton {}", skeleton);
    // }
    // spdlog::debug("Loaded skeleton from {}", skeleton);

    // // Reading animation.
    // if (!LoadAnimation(animation.c_str(), &m_animation)) {
    //     spdlog::error("Failed to load animation {}", animation);
    // }

 //    // Reading skinned meshes.
 //    if (!LoadMeshes(mesh.c_str(), &m_meshes)) {
 //      // return false;
 //    //  return 1;
 //    }

 //    // =====================================================================================


            m_windowAlloc.alloc();
            m_window = SDL_CreateWindow("bgfx"
                            , SDL_WINDOWPOS_UNDEFINED
                            , SDL_WINDOWPOS_UNDEFINED
                            , m_width
                            , m_height
                            , SDL_WINDOW_SHOWN
                            | SDL_WINDOW_RESIZABLE
                            );

            // SDL_Surface* icon = IMG_Load("../res/icons/icon.png");
            // SDL_SetWindowIcon(m_window, icon);


            m_flags = 0
                | ENTRY_WINDOW_FLAG_ASPECT_RATIO
                | ENTRY_WINDOW_FLAG_FRAME
                ;

            s_userEventStart = SDL_RegisterEvents(7);

            sdlSetWindow(m_window);
            bgfx::renderFrame();

            m_thread.init(MainThreadEntry::threadFunc, &m_mte);

            // Force window resolution...
            WindowHandle defaultWindow = { 0 };
            // setWindowSize(defaultWindow, m_width, m_height, true);

            bool exit = false;
            SDL_Event event;
            while (!exit)
            {
                bgfx::renderFrame();

                while (SDL_PollEvent(&event) )
                {
                    switch (event.type)
                    {
                    case SDL_QUIT:
                        m_eventQueue.postExitEvent();
                        exit = true;
                        break;

                    case SDL_WINDOWEVENT:
                        {
                            const SDL_WindowEvent& wev = event.window;
                            switch (wev.event)
                            {
                            case SDL_WINDOWEVENT_RESIZED:
                            case SDL_WINDOWEVENT_SIZE_CHANGED:
                                {
                                    WindowHandle handle = findHandle(wev.windowID);
                                    setWindowSize(handle, wev.data1, wev.data2);
                                }
                                break;

                            // case SDL_WINDOWEVENT_SHOWN:
                            // case SDL_WINDOWEVENT_HIDDEN:
                            // case SDL_WINDOWEVENT_EXPOSED:
                            // case SDL_WINDOWEVENT_MOVED:
                            // case SDL_WINDOWEVENT_MINIMIZED:
                            // case SDL_WINDOWEVENT_MAXIMIZED:
                            // case SDL_WINDOWEVENT_RESTORED:
                            // case SDL_WINDOWEVENT_ENTER:
                            // case SDL_WINDOWEVENT_LEAVE:
                            // case SDL_WINDOWEVENT_FOCUS_GAINED:
                            // case SDL_WINDOWEVENT_FOCUS_LOST:
                            //     break;

                            case SDL_WINDOWEVENT_CLOSE:
                                {
                                    WindowHandle handle = findHandle(wev.windowID);
                                    if (0 == handle.idx)
                                    {
                                        m_eventQueue.postExitEvent();
                                        exit = true;
                                    }
                                }
                                break;
                            }
                        }
                        break;


                    default:
                        break;
                    }
                }

                auto keystates = SDL_GetKeyboardState(NULL);
                if (keystates[SDL_SCANCODE_AC_HOME] || keystates[SDL_SCANCODE_Q])
                {
                    m_eventQueue.postExitEvent();
                    exit = true;
                }
            }

            while (bgfx::RenderFrame::NoContext != bgfx::renderFrame() ) {};
            m_thread.shutdown();

            sdlDestroyWindow(m_window);

            // Mix_FreeMusic(music);
            // IMG_Quit();
            // TTF_Quit();
            // SDLNet_Quit();

            SDL_Quit();

            return m_thread.getExitCode();
        }

        WindowHandle findHandle(uint32_t _windowId)
        {
            SDL_Window* window = SDL_GetWindowFromID(_windowId);
            return findHandle(window);
        }

        WindowHandle findHandle(SDL_Window* _window)
        {
            bx::MutexScope scope(m_lock);
            for (uint32_t ii = 0, num = m_windowAlloc.getNumHandles(); ii < num; ++ii)
            {
                uint16_t idx = m_windowAlloc.getHandleAt(ii);
                if (_window == m_window)
                {
                    WindowHandle handle = { idx };
                    return handle;
                }
            }

            WindowHandle invalid = { UINT16_MAX };
            return invalid;
        }

        void setWindowSize(WindowHandle _handle, uint32_t _width, uint32_t _height, bool _force = false)
        {
            if (_width  != m_width
            ||  _height != m_height
            ||  _force)
            {
                m_width  = _width;
                m_height = _height;

                SDL_SetWindowSize(m_window, m_width, m_height);
                m_eventQueue.postSizeEvent(_handle, m_width, m_height);
            }
        }

        MainThreadEntry m_mte;
        bx::Thread m_thread;

        EventQueue m_eventQueue;
        bx::Mutex m_lock;

        bx::HandleAllocT<ENTRY_CONFIG_MAX_WINDOWS> m_windowAlloc;
        SDL_Window* m_window;
        uint32_t m_flags;

        uint32_t m_width;
        uint32_t m_height;

        Mix_Music* music;
    };

    static Context s_ctx;

    const Event* poll()
    {
        return s_ctx.m_eventQueue.poll();
    }

    void release(const Event* _event)
    {
        s_ctx.m_eventQueue.release(_event);
    }

    // void setWindowSize(WindowHandle _handle, uint32_t _width, uint32_t _height)
    // {
    //     Msg* msg = new Msg;
    //     msg->m_width  = _width;
    //     msg->m_height = _height;

    //     sdlPostEvent(SDL_USER_WINDOW_SET_SIZE, _handle, msg);
    // }

    int32_t MainThreadEntry::threadFunc(bx::Thread* _thread, void* _userData)
    {
        BX_UNUSED(_thread);

        MainThreadEntry* self = (MainThreadEntry*)_userData;
        int32_t result = main();

        SDL_Event event;
        SDL_QuitEvent& qev = event.quit;
        qev.type = SDL_QUIT;
        SDL_PushEvent(&event);
        return result;
    }

} // namespace entry












__declspec(dllexport) int dvigl_init(int _argc, char const *_argv[])
{
    using namespace entry;
    return s_ctx.run();
}


namespace
{

class ExampleHelloWorld : public entry::AppI
{
public:
    ExampleHelloWorld() : entry::AppI() {}

    void init(uint32_t _width, uint32_t _height) override
    {
        m_width  = _width;
        m_height = _height;
        m_reset  = BGFX_RESET_VSYNC | BGFX_RESET_MSAA_X16 | BGFX_RESET_MAXANISOTROPY ;

        bgfx::Init init;
        // TODO: read from saved user config
        // init.type     = bgfx::RendererType::Direct3D9;
        // init.type     = bgfx::RendererType::Direct3D11;
        init.type     = bgfx::RendererType::Direct3D12;
        // init.type     = bgfx::RendererType::OpenGL;
        // init.type     = bgfx::RendererType::Vulkan;
        // init.vendorId = args.m_pciId;
        init.resolution.width  = m_width;
        init.resolution.height = m_height;
        init.resolution.reset  = m_reset;

        bgfx::init(init);

switch (bgfx::getRendererType())
{
    case (bgfx::RendererType::Direct3D9):
        spdlog::info("Renderer: Direct3D9");
        break;
    case (bgfx::RendererType::Direct3D11):
        spdlog::info("Renderer: Direct3D11");
        break;
    case (bgfx::RendererType::Direct3D12):
        spdlog::info("Renderer: Direct3D12");
        break;
    case (bgfx::RendererType::OpenGL):
        spdlog::info("Renderer: OpenGL");
        break;
    case (bgfx::RendererType::Vulkan):
        spdlog::info("Renderer: Vulkan");
        break;
    default:
        spdlog::info("Renderer: Metal????");
        break;
}
    spdlog::debug("test debug");
    spdlog::warn("test warning");
    spdlog::critical("test critical");
    spdlog::error("test error {}, {:.2f}", 42, 42.0f);

        m_debug = BGFX_DEBUG_STATS;
        bgfx::setDebug(m_debug);

        // Set view 0 clear state.
        bgfx::setViewClear(0
            , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
            , 0x303030ff
            , 1.0f
            , 0
            );
    }

    virtual int shutdown() override
    {
        bgfx::shutdown();
        return 0;
    }

    bool update() override
    {
        static int clearColor = 0x303030ff;
        clearColor += 1024 + 256;


        if (!entry::processEvents(m_width, m_height, m_debug, m_reset) )
        {
            bgfx::setViewClear(0
            , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
            , clearColor
            , 1.0f
            , 0
            );

            bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height) );
        // bgfx::dbgTextClear();
            bgfx::touch(0);
            bgfx::frame();

            return true;
        }

        return false;
    }

    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_debug;
    uint32_t m_reset;
};

} // namespace

int _main_()
{
    spdlog::set_level(spdlog::level::debug);
    ExampleHelloWorld app;
    return entry::runApp(&app);
}