#include "face.h"

uint32_t generate_face_info(
    glm::uvec3 position,
    glm::uvec2 texture,
    Direction dir,
    TextureRotation rot
)
{
    return (texture.y << 24) | (texture.x << 23) | (rot << 21) | (dir << 18) |
           (position.z << 12) | (position.y << 6) | position.x;
}

