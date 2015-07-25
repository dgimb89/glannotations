#pragma once

#include <glannotations/glannotations_api.h>
#include <glannotations/Common/DirtyFlagObject.h>

#include <string>
#include <unordered_map>

namespace glannotations {
	class GLANNOTATIONS_API AnnotationData : public glannotations::DirtyFlagObject {
	public:
		void clear();
		void set(std::string key, std::string value);
		/*!
		 *	\brief		Accessing the Data Value will flag the AnnotationData instance as non-dirty, thus make sure to update everything 
		 *	\returns	
		 */
		const std::string &get(std::string key) const;

	private:
		std::unordered_map<std::string, std::string> m_dataMap;
	};
}