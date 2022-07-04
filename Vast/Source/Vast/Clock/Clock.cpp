#include "vastpch.h"
#include "Clock.h"

#include <GLFW/glfw3.h>

namespace Vast {

    float Clock::EpochSeconds()
    {
        return (float)glfwGetTime(); // TODO: Make platform independent
    }

    float Clock::EpochMilliseconds()
    {
        return (float)(glfwGetTime() * 1000.0);
    }

}