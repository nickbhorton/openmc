#include "block.h"

Block::Block(
    std::array<std::array<uint32_t, 2>, 6> face_textures,
    std::array<TextureRotation, 6> face_orientations
)
    : face_textures(face_textures), face_orientations(face_orientations)
{
}

uint32_t Block::get_face(std::array<uint32_t, 3> position, Direction d) const
{
    return generate_face_info(
        position,
        face_textures[d],
        d,
        face_orientations[d]
    );
}
