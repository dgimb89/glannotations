#pragma once

#include <glannotations/glannotations_api.h>
#include <glannotations/Styles/Styling.h>

#include <globjects/base/ref_ptr.h>

#include <string>
#include <unordered_map>

namespace glannotations {

	struct GLANNOTATIONS_API CommonConfigData {
		std::string technique = "";
		std::string type = "";
		std::string contentDataKey = "";
		float lineHeight = 0.f;
		unsigned short priority = 0;
		std::vector<globjects::ref_ptr<glannotations::Styling> > stylings;
	};

	struct GLANNOTATIONS_API AnnotationTechniqueConfig : public CommonConfigData {
		std::vector< std::pair<std::string, float> > critera;
		std::vector< std::pair<std::string, std::string > > additionalParams;
	};

	struct GLANNOTATIONS_API AnnotationClassConfig : public globjects::Referenced, public CommonConfigData {
		std::vector<AnnotationTechniqueConfig> techniques;
	};

	class GLANNOTATIONS_API ConfigBuilder {
	public:
		/*!
		 *	\brief		Builds config structs for all described classes in file at configPath (relative path to data folder)
		 */
		ConfigBuilder(std::string configPath);

		/*!
		 *	\returns	All generated annotation class configs as Map: ClassIdentifier => ClassConfig
		 */
		const std::unordered_map<std::string, globjects::ref_ptr<glannotations::AnnotationClassConfig> >& getAnnotationClassConfigs() const;
		/*!
		 *	\returns	ClassConfig for AnnotationClass with identifier "name"
		 *	\throws		Out-of-range Runtime Exception if Config for Class identifier "name" does not exist
		 */
		globjects::ref_ptr<glannotations::AnnotationClassConfig> getAnnotationClassConfig(std::string name) const;
		bool hasAnnotationClassConfig(std::string name) const;

	private:
		std::unordered_map<std::string, globjects::ref_ptr<glannotations::AnnotationClassConfig> > m_classConfigs;
	};
};