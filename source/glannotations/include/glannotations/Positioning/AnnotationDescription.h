#pragma once

#include <glannotations/glannotations_api.h>
#include <glannotations/Common/DirtyFlagObject.h>
#include <glannotations/Positioning/SpaceObject.h>

#include <globjects/base/ref_ptr.h>

#include <utility>
#include <tuple>
#include <map>

namespace glannotations {
	class GLANNOTATIONS_API AnnotationDescription : public DirtyFlagObject {
	public:
		typedef std::pair<globjects::ref_ptr<SpaceObject>, std::string> AnnotationMapping;

		void addAnnotatedObjects(std::initializer_list< std::tuple<size_t, size_t, std::string> > annotatedObjects);
		void addAnnotatedObject(size_t priority, size_t uid, std::string dataKey);
		void clearAnnotatedObjects();

		/// todo: constraint (scene + annotation) definition

	private:
		std::multimap<size_t,  AnnotationMapping> m_annotationMappings;
	};
}