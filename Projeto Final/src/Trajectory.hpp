#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <glm/glm.hpp>

class Trajectory {
private:
    glm::vec3* points;
    int count;
public:
    Trajectory(glm::vec3* pts, int n) : points(pts), count(n) {}
    glm::vec3 getPosition(float time) {
        int i = ((int)time) % count;
        int j = (i + 1) % count;
        float t = time - (int)time;
        return (1.0f - t) * points[i] + t * points[j];
    }
};

#endif