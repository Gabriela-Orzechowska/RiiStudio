#include <algorithm>
#include <core/3d/gl.hpp>
#include <librii/gl/Compiler.hpp>
#include <librii/gl/EnumConverter.hpp>
#include <librii/glhelper/UBOBuilder.hpp>
#include <librii/mtx/TexMtx.hpp>
#include <plugins/gc/Export/IndexedPolygon.hpp>
#include <plugins/gc/Export/Material.hpp>

namespace libcube {

std::pair<std::string, std::string> IGCMaterial::generateShaders() const {
  auto result = librii::gl::compileShader(getMaterialData(), getName());

  assert(result);
  if (!result) {
    return {"Invalid", "Invalid"};
  }

  if (!applyCacheAgain)
    cachedPixelShader = result->fragment + "\n\n // End of shader";
  return {result->vertex, result->fragment};
}

glm::mat4x4 GCMaterialData::TexMatrix::compute(const glm::mat4& mdl,
                                               const glm::mat4& mvp) const {
  auto texsrt =
      librii::mtx::computeTexSrt(scale, rotate, translate, transformModel);
  return librii::mtx::computeTexMtx(mdl, mvp, texsrt, method, option);
}
void IGCMaterial::setMegaState(librii::gfx::MegaState& state) const {
  librii::gl::translateGfxMegaState(state, getMaterialData());
}
} // namespace libcube
