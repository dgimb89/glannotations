#pragma once

#include <glannotations/glannotations_api.h>
#include <glannotations/Common/DirtyFlagObject.h>
#include <glannotations/Positioning/SpaceObject.h>

#include <globjects/base/ref_ptr.h>
#include <unordered_map>

namespace glannotations {
	class GLANNOTATIONS_API AnnotationSpace : public DirtyFlagObject {
	public:
		void addSpaceObjects(std::initializer_list<globjects::ref_ptr<SpaceObject> > spaceObjects);
		void clear();

		const globjects::ref_ptr<glannotations::SpaceObject> getSpaceObjectAtUID(size_t uid) const;
		const std::unordered_map<size_t, globjects::ref_ptr<glannotations::SpaceObject> > &getSpaceObjects() const;

	private:
		std::unordered_map<size_t,  globjects::ref_ptr<SpaceObject> > m_spaceObjects;
	};
}