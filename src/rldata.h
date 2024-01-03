//
// Created by Alether on 12/7/2023.
//

#ifndef HELLO_C_RLDATA_H
#define HELLO_C_RLDATA_H

#include<iostream>
#include<raylib.h>
#include<memory>

// Wrap raylib data models in RAII
// Cannot be copied because ResourceHandle contains owning pointers
// Shared ownership can be achieved with shared_ptr<T>
template<typename Resource, typename DestroyFunction>
class ResourceHandle {
public:
    // Owning
    Resource d;

    ResourceHandle() : d(DestroyFunction::reset()) {}

    ResourceHandle(Resource resource) : d(resource) {}

    // no copy allowed,  otherwise dtor will screw us in std::vector
    ResourceHandle(const ResourceHandle &) = delete;

    // no copy assignment allowed, otherwise dtor will screw us in std::vector
    ResourceHandle &operator=(const ResourceHandle &) = delete;

    // allow move ctor
    ResourceHandle(ResourceHandle &&other) noexcept {
        *this = std::move(other);
    }

    // allow move assignment
    ResourceHandle &operator=(ResourceHandle &&other) noexcept {
        if (this == &other) return *this;
        this->d = other.d;
        other.d = Resource{DestroyFunction::reset()}; // destroy the old one
        return *this;
    }

    ~ResourceHandle() {
        DestroyFunction::destroy(this->d);
    }
};

struct FontDestroy {
    static Font reset() {
        return Font{0};
    }

    static void destroy(Font &font) {
        if (IsFontReady(font)) {
            UnloadFont(font);
        }
    }
};

struct ModelDestroy {
    static Model reset() {
        return Model{0};
    }

    static void destroy(Model &model) {
        if (IsModelReady(model)) {
            UnloadModel(model);
        }
    }
};

struct RenderTexture2DDestroy {
    static RenderTexture2D reset() {
        return RenderTexture2D{0};
    }

    static void destroy(RenderTexture2D &renderTexture2D) {
        if (IsRenderTextureReady(renderTexture2D)) {
            UnloadRenderTexture(renderTexture2D);
        }
    }
};

struct ShaderDestroy {
    static Shader reset() {
        return Shader{0};
    }

    static void destroy(Shader &shader) {
        if (IsShaderReady(shader)) {
            UnloadShader(shader);
        }
    }
};

using RlFont = ResourceHandle<Font, FontDestroy>;
using RlModel = ResourceHandle<Model, ModelDestroy>;
using RlRenderTexture2D = ResourceHandle<RenderTexture2D, RenderTexture2DDestroy>;
using RlShader = ResourceHandle<Shader, ShaderDestroy>;

#endif //HELLO_C_RLDATA_H
