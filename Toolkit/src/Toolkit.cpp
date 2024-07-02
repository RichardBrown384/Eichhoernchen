#include "Toolkit/Toolkit.h"

#include <SDL3/SDL.h>
#include <array>

namespace rbrown::toolkit {

template<typename T>
constexpr auto U(const T& v) { return std::to_underlying(v); }

consteval auto ConstructScanCodeMapping() {
    std::array<ScanCode, SDL_NUM_SCANCODES> mapping {};

    mapping.fill(ScanCode::CODE_UNKNOWN);

    mapping[SDL_SCANCODE_A] = ScanCode::CODE_A;
    mapping[SDL_SCANCODE_B] = ScanCode::CODE_B;
    mapping[SDL_SCANCODE_C] = ScanCode::CODE_C;
    mapping[SDL_SCANCODE_D] = ScanCode::CODE_D;
    mapping[SDL_SCANCODE_E] = ScanCode::CODE_E;
    mapping[SDL_SCANCODE_F] = ScanCode::CODE_F;
    mapping[SDL_SCANCODE_G] = ScanCode::CODE_G;
    mapping[SDL_SCANCODE_H] = ScanCode::CODE_H;
    mapping[SDL_SCANCODE_I] = ScanCode::CODE_I;
    mapping[SDL_SCANCODE_J] = ScanCode::CODE_J;
    mapping[SDL_SCANCODE_K] = ScanCode::CODE_K;
    mapping[SDL_SCANCODE_L] = ScanCode::CODE_L;
    mapping[SDL_SCANCODE_M] = ScanCode::CODE_M;
    mapping[SDL_SCANCODE_N] = ScanCode::CODE_N;
    mapping[SDL_SCANCODE_O] = ScanCode::CODE_O;
    mapping[SDL_SCANCODE_P] = ScanCode::CODE_P;
    mapping[SDL_SCANCODE_Q] = ScanCode::CODE_Q;
    mapping[SDL_SCANCODE_R] = ScanCode::CODE_R;
    mapping[SDL_SCANCODE_S] = ScanCode::CODE_S;
    mapping[SDL_SCANCODE_T] = ScanCode::CODE_T;
    mapping[SDL_SCANCODE_U] = ScanCode::CODE_U;
    mapping[SDL_SCANCODE_V] = ScanCode::CODE_V;
    mapping[SDL_SCANCODE_W] = ScanCode::CODE_W;
    mapping[SDL_SCANCODE_X] = ScanCode::CODE_X;
    mapping[SDL_SCANCODE_Y] = ScanCode::CODE_Y;
    mapping[SDL_SCANCODE_Z] = ScanCode::CODE_Z;

    mapping[SDL_SCANCODE_1] = ScanCode::CODE_1;
    mapping[SDL_SCANCODE_2] = ScanCode::CODE_2;
    mapping[SDL_SCANCODE_3] = ScanCode::CODE_3;
    mapping[SDL_SCANCODE_4] = ScanCode::CODE_4;
    mapping[SDL_SCANCODE_5] = ScanCode::CODE_5;
    mapping[SDL_SCANCODE_6] = ScanCode::CODE_6;
    mapping[SDL_SCANCODE_7] = ScanCode::CODE_7;
    mapping[SDL_SCANCODE_8] = ScanCode::CODE_8;
    mapping[SDL_SCANCODE_9] = ScanCode::CODE_9;
    mapping[SDL_SCANCODE_0] = ScanCode::CODE_0;

    mapping[SDL_SCANCODE_RETURN] = ScanCode::CODE_RETURN;
    mapping[SDL_SCANCODE_ESCAPE] = ScanCode::CODE_ESCAPE;
    mapping[SDL_SCANCODE_BACKSPACE] = ScanCode::CODE_BACKSPACE;
    mapping[SDL_SCANCODE_TAB] = ScanCode::CODE_TAB;
    mapping[SDL_SCANCODE_SPACE] = ScanCode::CODE_SPACE;

    mapping[SDL_SCANCODE_MINUS] = ScanCode::CODE_MINUS;
    mapping[SDL_SCANCODE_EQUALS] = ScanCode::CODE_EQUALS;
    mapping[SDL_SCANCODE_LEFTBRACKET] = ScanCode::CODE_LEFTBRACKET;
    mapping[SDL_SCANCODE_RIGHTBRACKET] = ScanCode::CODE_RIGHTBRACKET;
    mapping[SDL_SCANCODE_BACKSLASH] = ScanCode::CODE_BACKSLASH;
    mapping[SDL_SCANCODE_SEMICOLON] = ScanCode::CODE_SEMICOLON;
    mapping[SDL_SCANCODE_APOSTROPHE] = ScanCode::CODE_APOSTROPHE;
    mapping[SDL_SCANCODE_GRAVE] = ScanCode::CODE_GRAVE;
    mapping[SDL_SCANCODE_COMMA] = ScanCode::CODE_COMMA;
    mapping[SDL_SCANCODE_PERIOD] = ScanCode::CODE_PERIOD;
    mapping[SDL_SCANCODE_SLASH] = ScanCode::CODE_SLASH;

    mapping[SDL_SCANCODE_CAPSLOCK] = ScanCode::CODE_CAPSLOCK;

    mapping[SDL_SCANCODE_F1] = ScanCode::CODE_F1;
    mapping[SDL_SCANCODE_F2] = ScanCode::CODE_F2;
    mapping[SDL_SCANCODE_F3] = ScanCode::CODE_F3;
    mapping[SDL_SCANCODE_F4] = ScanCode::CODE_F4;
    mapping[SDL_SCANCODE_F5] = ScanCode::CODE_F5;
    mapping[SDL_SCANCODE_F6] = ScanCode::CODE_F6;
    mapping[SDL_SCANCODE_F7] = ScanCode::CODE_F7;
    mapping[SDL_SCANCODE_F8] = ScanCode::CODE_F8;
    mapping[SDL_SCANCODE_F9] = ScanCode::CODE_F9;
    mapping[SDL_SCANCODE_F10] = ScanCode::CODE_F10;
    mapping[SDL_SCANCODE_F11] = ScanCode::CODE_F11;
    mapping[SDL_SCANCODE_F12] = ScanCode::CODE_F12;

    mapping[SDL_SCANCODE_PRINTSCREEN] = ScanCode::CODE_PRINTSCREEN;
    mapping[SDL_SCANCODE_SCROLLLOCK] = ScanCode::CODE_SCROLLLOCK;
    mapping[SDL_SCANCODE_PAUSE] = ScanCode::CODE_PAUSE;
    mapping[SDL_SCANCODE_INSERT] = ScanCode::CODE_INSERT;
    
    mapping[SDL_SCANCODE_HOME] = ScanCode::CODE_HOME;
    mapping[SDL_SCANCODE_PAGEUP] = ScanCode::CODE_PAGEUP;
    mapping[SDL_SCANCODE_DELETE] = ScanCode::CODE_DELETE;
    mapping[SDL_SCANCODE_END] = ScanCode::CODE_END;
    mapping[SDL_SCANCODE_PAGEDOWN] = ScanCode::CODE_PAGEDOWN;
    mapping[SDL_SCANCODE_RIGHT] = ScanCode::CODE_RIGHT;
    mapping[SDL_SCANCODE_LEFT] = ScanCode::CODE_LEFT;
    mapping[SDL_SCANCODE_DOWN] = ScanCode::CODE_DOWN;
    mapping[SDL_SCANCODE_UP] = ScanCode::CODE_UP;

    mapping[SDL_SCANCODE_NUMLOCKCLEAR] = ScanCode::CODE_NUMLOCKCLEAR,

    mapping[SDL_SCANCODE_KP_DIVIDE] = ScanCode::CODE_KP_DIVIDE;
    mapping[SDL_SCANCODE_KP_MULTIPLY] = ScanCode::CODE_KP_MULTIPLY;
    mapping[SDL_SCANCODE_KP_MINUS] = ScanCode::CODE_KP_MINUS;
    mapping[SDL_SCANCODE_KP_PLUS] = ScanCode::CODE_KP_PLUS;
    mapping[SDL_SCANCODE_KP_ENTER] = ScanCode::CODE_KP_ENTER;
    mapping[SDL_SCANCODE_KP_1] = ScanCode::CODE_KP_1;
    mapping[SDL_SCANCODE_KP_2] = ScanCode::CODE_KP_2;
    mapping[SDL_SCANCODE_KP_3] = ScanCode::CODE_KP_3;
    mapping[SDL_SCANCODE_KP_4] = ScanCode::CODE_KP_4;
    mapping[SDL_SCANCODE_KP_5] = ScanCode::CODE_KP_5;
    mapping[SDL_SCANCODE_KP_6] = ScanCode::CODE_KP_6;
    mapping[SDL_SCANCODE_KP_7] = ScanCode::CODE_KP_7;
    mapping[SDL_SCANCODE_KP_8] = ScanCode::CODE_KP_8;
    mapping[SDL_SCANCODE_KP_9] = ScanCode::CODE_KP_9;
    mapping[SDL_SCANCODE_KP_0] = ScanCode::CODE_KP_0;
    mapping[SDL_SCANCODE_KP_PERIOD] = ScanCode::CODE_KP_PERIOD;

    mapping[SDL_SCANCODE_LCTRL] = ScanCode::CODE_LCTRL;
    mapping[SDL_SCANCODE_LSHIFT] = ScanCode::CODE_LSHIFT;
    mapping[SDL_SCANCODE_LALT] = ScanCode::CODE_LALT;
    mapping[SDL_SCANCODE_LGUI] = ScanCode::CODE_LGUI;
    mapping[SDL_SCANCODE_RCTRL] = ScanCode::CODE_RCTRL;
    mapping[SDL_SCANCODE_RSHIFT] = ScanCode::CODE_RSHIFT;
    mapping[SDL_SCANCODE_RALT] = ScanCode::CODE_RALT;
    mapping[SDL_SCANCODE_RGUI] = ScanCode::CODE_RGUI;

    return mapping;
}

consteval auto ConstructInverseScanCodeMapping(const auto& mapping) {
    std::array<int, U(ScanCode::CODE_MAX)> inverse {};
    inverse.fill(SDL_SCANCODE_UNKNOWN);
    for (auto i = 0u; i < mapping.size(); ++i) {
        const auto scanCode = mapping[i];
        if (scanCode != ScanCode::CODE_UNKNOWN) {
            inverse[U(scanCode)] = i;
        }
    }
    return inverse;
}

constexpr auto SCANCODE_MAPPING = ConstructScanCodeMapping();
constexpr auto INVERSE_MAPPING = ConstructInverseScanCodeMapping(SCANCODE_MAPPING);

auto MapSDLScanCodeToToolkitScanCode(int sdlScanCode) -> ScanCode {
    if (sdlScanCode >= 0 && static_cast<uint32_t>(sdlScanCode) < SCANCODE_MAPPING.size()) {
        return SCANCODE_MAPPING[sdlScanCode];
    }
    return ScanCode::CODE_UNKNOWN;
}

auto MapToolkitScanCodeToSDLScanCode(ScanCode scanCode) -> int {
    const auto index = static_cast<uint32_t>(U(scanCode));
    if (index < INVERSE_MAPPING.size()) {
        return INVERSE_MAPPING[index];
    }
    return SDL_SCANCODE_UNKNOWN;
}

consteval auto ConstructMouseButtonMapping() {
    std::array<MouseButton, 8> mapping {};
    mapping.fill(MouseButton::UNKNOWN);
    mapping[SDL_BUTTON_LEFT] = MouseButton::LEFT;
    mapping[SDL_BUTTON_MIDDLE] = MouseButton::MIDDLE;
    mapping[SDL_BUTTON_RIGHT] = MouseButton::RIGHT;
    return mapping;
}

constexpr auto MOUSE_BUTTON_MAPPING = ConstructMouseButtonMapping();

auto MapSDLMouseButtonCodeToToolkitMouseButton(uint8_t sdlButtonCode) -> MouseButton {
    if (sdlButtonCode < MOUSE_BUTTON_MAPPING.size()) {
        return MOUSE_BUTTON_MAPPING[sdlButtonCode];
    }
    return MouseButton::UNKNOWN;
}

using WindowUniquePtr = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;
using TextureUniquePtr = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;
using RendererUniquePtr = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>;

template<typename ...Args>
auto CreateSDLWindow(Args&&... args) {
    return WindowUniquePtr(SDL_CreateWindow(std::forward<Args>(args)...), SDL_DestroyWindow);
}

template<typename ...Args>
auto CreateSDLTexture(Args&&... args) {
    return TextureUniquePtr(SDL_CreateTexture(std::forward<Args>(args)...), SDL_DestroyTexture);
}

template<typename ...Args>
auto CreateSDLRenderer(Args&&... args) {
    return RendererUniquePtr(SDL_CreateRenderer(std::forward<Args>(args)...), SDL_DestroyRenderer);
}

class WindowImpl final : public rbrown::toolkit::Window {
public:
    explicit WindowImpl(WindowUniquePtr w) : window(std::move(w)) {}
    ~WindowImpl() final = default;
    operator SDL_Window*() const { return window.get(); } // NOLINT
    auto SetSize(int w, int h) -> bool final {
        return SDL_SetWindowSize(*this, w, h) == 0;
    }
private:
    WindowUniquePtr window;
};

class TextureImpl final: public rbrown::toolkit::Texture {
public:
    TextureImpl(TextureUniquePtr t, int w, int h)
        : texture(std::move(t)), width {w}, height{h}, pixels{}, pitch{} {}
    ~TextureImpl() final = default;
    operator SDL_Texture*() const { return texture.get(); } // NOLINT
    auto LockTexture() -> bool final {
        if (SDL_LockTexture(*this, nullptr, reinterpret_cast<void**>(&pixels), &pitch)) {
            pixels = {};
            pitch = {};
            return false;
        }
        return true;
    }
    auto UnlockTexture() -> void final {
        SDL_UnlockTexture(*this);
        pixels = {};
        pitch = {};
    }
    [[nodiscard]] auto GetTextureRowAddress(int y) const -> uint32_t* {
        return reinterpret_cast<uint32_t*>(pixels + y * pitch);
    }
    auto SetPixel(int x, int y, uint32_t c) -> void final {
        if (pixels && x < width && y < height) [[likely]] {
            GetTextureRowAddress(y)[x] = c;
        }
    }
    auto GetPixel(int x, int y) -> uint32_t final {
        if (pixels && x < width && y < height) [[likely]] {
            return GetTextureRowAddress(y)[x];
        }
        return 0u;
    }
private:
    TextureUniquePtr texture;
    int width;
    int height;
    unsigned char* pixels;
    int pitch;
};

class RendererImpl final: public rbrown::toolkit::Renderer {
public:
    explicit RendererImpl(RendererUniquePtr r) : renderer(std::move(r)) {}
    ~RendererImpl() final = default;
    operator SDL_Renderer*() const { return renderer.get(); } // NOLINT
    auto RenderTexture(Texture& t, const FloatRect& r) -> bool override {
        const SDL_FRect source = { .x = r.x, .y = r.y, .w = r.w, .h = r.h };
        return SDL_RenderTexture(*this, dynamic_cast<TextureImpl&>(t), &source, nullptr) == 0;
    }
    auto Present() -> bool override {
        return SDL_RenderPresent(*this) == 0;
    }
private:
    RendererUniquePtr renderer;
};

auto CreateWindow(const char* title, int w, int h) -> std::unique_ptr<rbrown::toolkit::Window> {
    auto window = CreateSDLWindow(title, w, h, SDL_WINDOW_OPENGL);
    if (!window) {
        return {};
    }
    return std::make_unique<WindowImpl>(std::move(window));
}

auto CreateRenderer(Window& window) -> std::unique_ptr<rbrown::toolkit::Renderer> {
    auto renderer = CreateSDLRenderer(dynamic_cast<WindowImpl&>(window), nullptr);
    if (!renderer) {
        return {};
    }
    return std::make_unique<RendererImpl>(std::move(renderer));
}

auto CreateTexture(Renderer& renderer, int w, int h) -> std::unique_ptr<rbrown::toolkit::Texture> {
    auto texture = CreateSDLTexture(
        dynamic_cast<RendererImpl&>(renderer),
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        w,
        h);
    if (!texture) {
        return {};
    }
    SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_NONE);
    SDL_SetTextureScaleMode(texture.get(), SDL_SCALEMODE_NEAREST);
    return std::make_unique<TextureImpl>(std::move(texture), w, h);
}


auto Init() -> bool { return !SDL_Init(SDL_INIT_VIDEO); }
auto Quit() -> void { SDL_Quit(); }

auto PollEvent(Event& e) -> bool {
    static thread_local SDL_Event sdlEvent;
    const auto result = SDL_PollEvent(&sdlEvent);
    e.type = EventType::UNUSED;
    switch (sdlEvent.type) {
        case SDL_EVENT_QUIT: {
            e.type = EventType::QUIT;
            break;
        }
        case SDL_EVENT_KEY_DOWN: {
            const auto scanCode = MapSDLScanCodeToToolkitScanCode(sdlEvent.key.keysym.scancode);
            if (scanCode != ScanCode::CODE_UNKNOWN) {
                e.type = EventType::KEY_DOWN;
                e.key.scanCode = scanCode;
                e.key.repeat = sdlEvent.key.repeat;
            }
            break;
        }
        case SDL_EVENT_KEY_UP: {
            const auto scanCode = MapSDLScanCodeToToolkitScanCode(sdlEvent.key.keysym.scancode);
            if (scanCode != ScanCode::CODE_UNKNOWN) {
                e.type = EventType::KEY_UP;
                e.key.scanCode = scanCode;
                e.key.repeat = sdlEvent.key.repeat;
            }
            break;
        }
        case SDL_EVENT_MOUSE_MOTION: {
            e.type = EventType::MOUSE_MOTION;
            e.motion.x = sdlEvent.motion.x;
            e.motion.y = sdlEvent.motion.y;
            e.motion.xrel = sdlEvent.motion.xrel;
            e.motion.yrel = sdlEvent.motion.yrel;
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            const auto mouseButton = MapSDLMouseButtonCodeToToolkitMouseButton(sdlEvent.button.button);
            if (mouseButton != MouseButton::UNKNOWN) {
                e.type = EventType::MOUSE_BUTTON_DOWN;
                e.button.button = mouseButton;
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            const auto mouseButton = MapSDLMouseButtonCodeToToolkitMouseButton(sdlEvent.button.button);
            if (mouseButton != MouseButton::UNKNOWN) {
                e.type = EventType::MOUSE_BUTTON_UP;
                e.button.button = mouseButton;
            }
            break;
        }
        default: {
            e.type = EventType::UNUSED;
            break;
        }
    }
    return result;
}

auto GetKeyboardState(ScanCode c) -> bool {
    int numKeys{};
    const auto sdlScanCode = MapToolkitScanCodeToSDLScanCode(c);
    const auto state = SDL_GetKeyboardState(&numKeys);
    if (state && (sdlScanCode < numKeys)) {
        return state[sdlScanCode];
    }
    return false;
}

auto SetRelativeMouseMode(bool enabled) -> bool {
    return SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE) == 0;
}

}
