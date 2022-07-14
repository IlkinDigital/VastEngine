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

    String Clock::DateTime()
    {
        time_t currTime;
        tm* currTm;
        char dateTime[50];

        time(&currTime);
        currTm = localtime(&currTime);

        strftime(dateTime, 50, "%m-%d-%y_%H-%M-%S", currTm);

        return dateTime;
    }

    String Clock::DateTimeDesc()
    {
        time_t currTime;
        tm* currTm;
        char dateTime[50];

        time(&currTime);
        currTm = localtime(&currTime);

        strftime(dateTime, 50, "%y-%m-%d_%H-%M-%S", currTm);

        return dateTime;
    }

}