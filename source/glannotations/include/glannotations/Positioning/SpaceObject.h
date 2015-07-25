#pragma once

#include <glannotations/glannotations_api.h>
#include <glannotations/Common/DirtyFlagObject.h>
#include <glannotations/Positioning/AnnotationData.h>

#include <globjects/base/ref_ptr.h>
#include <gloperate/painter/Camera.h>

#include <initializer_list>
#include <string>
#include <array>

namespace glannotations {
	class GLANNOTATIONS_API SpaceObject : public DirtyFlagObject {
	public:
		
		/*!
		 *	\brief		Sets all necessary data attributes for given SpaceObject
		 */
		void setData(const globjects::ref_ptr<glannotations::AnnotationData>& data);

		/*!
		 *	\returns	Data attributes map
		 */
		globjects::ref_ptr<glannotations::AnnotationData> getData() const;
		const std::string& getData(std::string dataKey);

		void setAnnotationClasses(std::initializer_list<std::string> annotationClasses);
		const std::vector<std::string>& getAnnotationClasses() const;

		size_t getUID() const;

		virtual bool isValid() const = 0;
		virtual double getVolume() const = 0;
		virtual double distanceToCamera(const gloperate::Camera& cam) const = 0;
		virtual glm::vec3 getCentroid() const = 0;
		/*!
		 *	\returns	Bounding Box via 4 vec3: LowerLeftFront, LowerRightFront, UpperLeftFront, LowerLeftBack
		 */
		virtual std::array<glm::vec3, 4> getBoundingBox() = 0;

	private:
		size_t generateUID() const;
		std::vector<std::string> m_annotationClasses;
		globjects::ref_ptr<glannotations::AnnotationData> m_data;
		mutable size_t m_uid = 0;
	};
}