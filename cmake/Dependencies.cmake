include(FetchContent)

FetchContent_Declare(glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw"
    GIT_TAG 3.4
)

set(GLFW_BUILD_DOCS FALSE)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(gl3w 
    GIT_REPOSITORY "https://github.com/skaslev/gl3w" 
    GIT_TAG "master"
)

cmake_policy(SET CMP0148 OLD)
FetchContent_MakeAvailable(gl3w)

FetchContent_Declare(glm
    GIT_REPOSITORY "https://github.com/g-truc/glm"
    GIT_TAG 1.0.1
)

FetchContent_MakeAvailable(glm)