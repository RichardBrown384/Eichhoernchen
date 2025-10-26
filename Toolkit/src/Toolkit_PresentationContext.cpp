#include "Toolkit/Toolkit.h"

namespace rbrown::toolkit {

class PresentationContextImpl final : public rbrown::toolkit::PresentationContext {
public:
    PresentationContextImpl(
        std::unique_ptr<Window> w,
        std::unique_ptr<Renderer> r,
        std::unique_ptr<Texture> t,
        float scaleFactor,
        float textureWidth,
        float textureHeight) :
        window{ std::move(w) },
        renderer{ std::move(r) },
        texture{ std::move(t) },
        textureRect{ .x = 0, .y =0, .w = textureWidth, .h = textureHeight },
        windowWidth{},
        windowHeight{},
        scaleFactor{ scaleFactor },
        complete{ false } {}
    ~PresentationContextImpl() final = default;
    auto SetRelativeMouseMode(bool) -> void final {
        window->SetRelativeMouseMode(true);
    }
    auto SetTextureSource(uint32_t x, uint32_t y, uint32_t w, uint32_t h) -> void final {
        textureRect.x = static_cast<float>(x);
        textureRect.y = static_cast<float>(y);
        textureRect.w = static_cast<float>(w);
        textureRect.h = static_cast<float>(h);
    }
    auto SetWindowSize(uint32_t width, uint32_t height) -> void final {
        windowWidth = width;
        windowHeight = height;
    }
    auto LockTexture() -> bool final { return texture->LockTexture(); }
    auto UnlockTexture() -> void final { texture->UnlockTexture(); }
    auto Present() -> void final {
        window->SetSize(
            static_cast<int>(scaleFactor * static_cast<float>(windowWidth)),
            static_cast<int>(scaleFactor * static_cast<float>(windowHeight))
        );
        renderer->RenderTexture(*texture, textureRect);
        renderer->Present();
    }
    auto SetPixel(uint32_t x, uint32_t y, uint32_t c) -> void final {
        texture->SetPixel(static_cast<int>(x), static_cast<int>(y), c);
    }
    auto GetPixel(uint32_t x, uint32_t y) -> uint32_t final {
        return texture->GetPixel(static_cast<int>(x), static_cast<int>(y));
    }
    auto StartFrame() -> void final { complete = false; }
    auto CompleteFrame() -> void final { complete = true; }
    [[nodiscard]] auto IsFrameComplete() const -> bool final { return complete; }
private:
    std::unique_ptr<Window> window;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Texture> texture;
    FloatRect textureRect;
    uint32_t windowWidth;
    uint32_t windowHeight;
    float scaleFactor;
    bool complete;
};


std::unique_ptr<PresentationContext> CreatePresentationContext(
    const char* title,
    int scaleFactor,
    int textureWidth,
    int textureHeight) {
    auto window = CreateWindow(title, 0, 0);
    if (!window) {
        return {};
    }
    auto renderer = CreateRenderer(*window);
    if (!renderer) {
        return {};
    }
    auto texture = CreateTexture(*renderer, textureWidth, textureHeight);
    if (!texture) {
        return {};
    }
    return std::make_unique<PresentationContextImpl>(
        std::move(window),
        std::move(renderer),
        std::move(texture),
        static_cast<float>(scaleFactor),
        static_cast<float>(textureWidth),
        static_cast<float>(textureHeight));
}
}