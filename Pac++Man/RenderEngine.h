#ifndef _RENDER_ENGINE_H_
#define _RENDER_ENGINE_H_

#include "Entity.h"

class RenderEngine {
private:
    char *presentBuffer, *backBuffer;
    int mBufferSize;
    RenderEngine(const RenderEngine &other) { }
    RenderEngine &operator=(const RenderEngine &other) { }
    RenderEngine() : presentBuffer(nullptr), backBuffer(nullptr) { }

    virtual ~RenderEngine() {
        if (presentBuffer != nullptr) {
            delete[] presentBuffer;
            presentBuffer = nullptr;
        }
        if (backBuffer != nullptr) {
            delete[] backBuffer;
            backBuffer = nullptr;
        }
    }

public:
    static RenderEngine &GetInstance() {
        static RenderEngine instance;
        return instance;
    }

    void InitializeEngine(int bufferSize = 0);
    void AddEntity(Entity &entity);

    void PrepareBuffer();
    void Present();
};

#endif //_RENDER_ENGINE_H_