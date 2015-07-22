#pragma once

#include <glannotations/glannotations_api.h>
#include <glannotations/Common/DirtyFlagObject.h>
#include <glannotations/Positioning/ConfigBuilder.h>
#include <glannotations/Positioning/SpaceObject.h>

#include <globjects/base/ref_ptr.h>

#include <string>
#include <initializer_list>
#include <utility>
#include <tuple>
#include <map>

namespace glannotations {
	class GLANNOTATIONS_API AnnotationDescription : public globjects::Referenced {
	public:
		typedef std::pair<globjects::ref_ptr<SpaceObject>, std::string> AnnotationMapping;

		AnnotationDescription(std::string annotationClass, const globjects::ref_ptr<AnnotationClassConfig>& config);

		/*!
		 *	\returns	Annotation class identifier
		 */
		const std::string& getAnnotationClassIdentifier() const;
		/*!
		 *	\returns	AnnotationClassConfig which describes content and visualization of this annotation class (will be used by Positioner)
		 */
		const globjects::ref_ptr<glannotations::AnnotationClassConfig> getAnnotationClassConfig() const;


		/// todo: constraint (scene + annotation) definition

	protected:
		void setAnnotationClassConfig(const globjects::ref_ptr<AnnotationClassConfig>& config);
		void setAnnotationClassIdentifier(std::string annotationClass);

	private:
		std::string m_classIdentifier;
		globjects::ref_ptr<AnnotationClassConfig> m_config;
	};
}