#include "block.h"

Block::Block(
    uint32_t id,
    std::array<uint32_t, 6> face_textures,
    std::array<TextureRotation, 6> face_orientations
)
    : face_textures(face_textures), face_orientations(face_orientations), id{id}
{
}

Block::Block(
    uint32_t id,
    uint32_t face_texture,
    std::array<TextureRotation, 6> face_orientations
)
    : face_textures({
          face_texture,
          face_texture,
          face_texture,
          face_texture,
          face_texture,
          face_texture,
      }),
      face_orientations(face_orientations), id{id}
{
}

uint32_t Block::get_face(std::array<uint32_t, 3> position, Direction d) const
{
    return generate_face_info(
        position,
        {face_textures[d] / g_texture_square_width,
         face_textures[d] % g_texture_square_width},
        d,
        face_orientations[d]
    );
}

uint32_t Block::get_id() const { return id; }
