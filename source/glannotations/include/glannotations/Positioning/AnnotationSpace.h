#pragma once

#include <glannotations/glannotations_api.h>
#include <glannotations/Common/DirtyFlagObject.h>
#include <glannotations/Positioning/SpaceObject.h>

#include <globjects/base/ref_ptr.h>
#include <unordered_map>

namespace glannotations {
	class GLANNOTATIONS_API AnnotationSpace : public DirtyFlagObject {
	public:
		typedef std::vector<globjects::ref_ptr<SpaceObject> > AnnotationClass_t;
		typedef std::unordered_map<std::string,  AnnotationClass_t> AnnotationSpace_t;

		AnnotationSpace();
		void addSpaceObjects(std::initializer_list<globjects::ref_ptr<SpaceObject> > spaceObjects);
		void clear();

		/*!
		 *	\returns	All SpaceObjects for given annotationClass-Identifier
		 */
		const AnnotationClass_t& getSpaceObjectsForClass(std::string annotationClass);

		/*!
		*	\returns	Returns whole annotation space
		*/
		const AnnotationSpace_t &getSpaceObjects() const;

		/*!
		 *	\brief		SpaceObjects are identified by automatically generated UID on creation. This can be used to access a SpaceObject directly.
		 *	\returns	SpaceObject with given UID if exists, nullptr otherwise
		 */
		const globjects::ref_ptr<glannotations::SpaceObject> getSpaceObjectWithUID(size_t uid) const;

	private:
		AnnotationSpace_t m_spaceObjects;
	};
}