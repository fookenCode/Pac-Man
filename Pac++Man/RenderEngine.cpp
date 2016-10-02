#include "RenderEngine.h"
#include <memory.h>
void RenderEngine::InitializeEngine(int bufferSize) {
    if (presentBuffer != nullptr) {
        delete[] presentBuffer;
    }
    if (backBuffer != nullptr) {
        delete[] backBuffer;
    }
    mBufferSize = bufferSize;
    presentBuffer = new char[bufferSize];
    backBuffer = new char[bufferSize];
}

void RenderEngine::AddEntity(Entity &entity) {

}

// void AddNonEntity(NonEntity &nentity);

void RenderEngine::PrepareBuffer() {
    memset(backBuffer, 0, sizeof(char)* mBufferSize);
}

void RenderEngine::Present() {

}