#include <glat/AbstractPrimitiveRenderer.h>

#include <glat/Outline.h>
#include <glat/BumpMap.h>

void glat::AbstractPrimitiveRenderer::setupOutline(const Styling* outline) {
	if (outline == nullptr) return;
	const Style::Outline* outlineStyle = reinterpret_cast<const Style::Outline*>(outline);
	m_drawingPrimitive->setOutline(outlineStyle->getWidth()*0.1f, outlineStyle->getColor());
}


void glat::AbstractPrimitiveRenderer::setupBumpMap(const Styling* bumpMap) {
	if (bumpMap == nullptr) return;
	const Style::BumpMap* bumpMapStyle = reinterpret_cast<const Style::BumpMap*>(bumpMap);
	m_drawingPrimitive->setBumpMap(bumpMapStyle->getIntensity());
}
