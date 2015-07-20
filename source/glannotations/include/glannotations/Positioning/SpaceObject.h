#pragma once

#include <glannotations/glannotations_api.h>
#include <glannotations/Common/DirtyFlagObject.h>
#include <glannotations/Positioning/AnnotationData.h>

#include <globjects/base/ref_ptr.h>
#include <gloperate/painter/Camera.h>

#include <initializer_list>
#include <string>
#include <unordered_map>

namespace glannotations {
	class GLANNOTATIONS_API SpaceObject : public DirtyFlagObject {
	public:
		typedef std::unordered_map < std::string, globjects::ref_ptr<glannotations::AnnotationData> > AnnotationDataMap;
		
		/*!
		 *	\brief		Sets all necessary data attributes for given SpaceObject
		 */
		void setData(std::string key, globjects::ref_ptr<glannotations::AnnotationData> data);
		/*!
		 *	\brief		Removes all data attributes from SpaceObject
		 */
		void clearData();
		/*!
		 *	\returns	Data attributes map
		 */
		const SpaceObject::AnnotationDataMap &getData() const;

		void setAnnotationClasses(std::initializer_list<std::string> annotationClasses);
		const std::vector<std::string>& getAnnotationClasses() const;

		size_t getUID() const;

		virtual bool isValid() const = 0;
		virtual double getVolume() const = 0;
		virtual double distanceToCamera(const gloperate::Camera& cam) const = 0;
		virtual glm::vec3 getCentroid() const = 0;

	private:
		size_t generateUID() const;
		std::vector<std::string> m_annotationClasses;
		AnnotationDataMap m_data;
		mutable size_t m_uid = 0;
	};
}