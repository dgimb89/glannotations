#pragma once

#include <glannotations/glannotations_api.h>
#include <glannotations/Common/DirtyFlagObject.h>
#include <glannotations/Positioning/SpaceObject.h>

#include <globjects/base/ref_ptr.h>

#include <string>
#include <initializer_list>
#include <utility>
#include <tuple>
#include <map>

namespace glannotations {
	class GLANNOTATIONS_API AnnotationDescription : public DirtyFlagObject {
	public:
		typedef std::pair<globjects::ref_ptr<SpaceObject>, std::string> AnnotationMapping;

		AnnotationDescription(/*config ,*/ std::string annotationClass);

		std::string getAnnotationClassIdentifier();
		void setAnnotationClassIdentifier(std::string annotationClass);

		/// todo: constraint (scene + annotation) definition

	protected:
		void parseConifg(/*config*/);

	private:
		std::string m_classIdentifier;
		/* config */
	};
}