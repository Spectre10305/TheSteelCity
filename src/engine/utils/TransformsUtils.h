#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace nothing
{

    // =================================================


    inline glm::quat EulerToQuaternion(glm::vec3 eulerDegrees)
    {

        return glm::quat(glm::radians(eulerDegrees));

    }


    // =================================================


    inline glm::vec3 QuaternionToEuler(glm::quat rotation)
    {

        return glm::degrees(glm::eulerAngles(rotation));

    }


    // =================================================

}