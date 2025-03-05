#include "face.h"

uint32_t generate_face_info(
    std::array<uint32_t, 3> position,
    std::array<uint32_t, 2> texture,
    Direction dir,
    TextureRotation rot
)
{
    return (texture[1] << 25) | (texture[0] << 23) | (rot << 21) | (dir << 18) |
           (position[2] << 12) | (position[1] << 6) | position[0];
}
