#pragma once

#include <glannotations/glannotations_api.h>
#include <glannotations/Common/DirtyFlagObject.h>
#include <glannotations/Positioning/AnnotationData.h>

#include <globjects/base/ref_ptr.h>
#include <gloperate/painter/Camera.h>

#include <unordered_map>

namespace glannotations {
	class GLANNOTATIONS_API SpaceObject : public DirtyFlagObject {
	public:
		typedef std::unordered_map < std::string, globjects::ref_ptr<glannotations::AnnotationData> > AnnotationDataMap;
		
		void setData(std::string key, globjects::ref_ptr<glannotations::AnnotationData> data);
		void clearData();
		const SpaceObject::AnnotationDataMap &getData();

		void setUID(size_t uid);
		size_t getUID() const;

		virtual bool isValid() const = 0;
		virtual double getVolume() const = 0;
		virtual double distanceToCamera(const gloperate::Camera& cam) const = 0;
		virtual glm::vec3 getCentroid() const = 0;

		template <typename T>
		T min(T v1, T v2) const {
			return v1 < v2 ? v1 : v2;
		}

		template <typename T>
		T max(T v1, T v2) const {
			return v1 > v2 ? v1 : v2;
		}

	private:
		AnnotationDataMap m_data;
		size_t m_uid;
	};
}