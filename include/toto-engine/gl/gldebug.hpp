#pragma once
#include <glad/glad.h>
#include <string>

namespace toto {

class GLDebug {
public:
    static void pushGroup(const std::string& message) {
        glad_glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, message.c_str());
    }
    static void popGroup() { glad_glPopDebugGroup(); }
};

} // namespace toto
