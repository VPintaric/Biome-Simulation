#ifndef WINDOWCONSTANTS_H
#define WINDOWCONSTANTS_H

#include <string>


namespace WindowConst{
    const std::string WINDOW_TITLE = "Biome Simulator";
    const int WINDOW_WIDTH = 512;
    const int WINDOW_HEIGHT = 512;
    
    const std::string DEFAULT_SHADER_NAME = "default_shader";

    const glm::vec3 INIT_CAMERA_POS(0.f, 0.f, -500.f);
}

#endif /* WINDOWCONSTANTS_H */

