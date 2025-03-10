#include "face.h"

uint32_t generate_face_info(
    std::array<uint32_t, 3> position,
    std::array<uint32_t, 2> texture,
    Direction dir,
    TextureRotation rot
)
{
    return (texture[1] << 25) | (texture[0] << 20) | (rot << 18) | (dir << 15) |
           (position[2] << 10) | (position[1] << 5) | position[0];
}
