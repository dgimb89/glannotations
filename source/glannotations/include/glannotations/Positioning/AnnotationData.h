#pragma once

#include <glannotations/glannotations_api.h>
#include <glannotations/Common/DirtyFlagObject.h>

#include <string>

namespace glannotations {
	class GLANNOTATIONS_API AnnotationData : glannotations::DirtyFlagObject {
	public:
		AnnotationData(std::string value);

		void setValue(std::string value);
		const std::string &getValue() const;

	private:
		std::string m_value;
	};
}