#include "vastpch.h"
#include "Clock.h"

#include <GLFW/glfw3.h>

namespace Vast {

    float Clock::EpochSeconds()
    {
        return (float)glfwGetTime(); // TODO: Make platform independent
    }

}