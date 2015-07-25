#pragma once

#include <glannotations/glannotations_api.h>
#include <glannotations/Styles/Styling.h>

#include <globjects/base/ref_ptr.h>

#include <string>
#include <unordered_map>

namespace glannotations {

	class InvalidConfigError : public std::runtime_error {
	public:
		InvalidConfigError(const char* m);
	};

	struct GLANNOTATIONS_API CommonConfigData {
		std::unordered_map<std::string, std::string> attr;
		std::vector<globjects::ref_ptr<glannotations::Styling> > stylings;

		std::string getAttribute(std::string attrKey) const;
		bool hasAttribute(std::string attrKey) const;
	};

	struct GLANNOTATIONS_API AnnotationTechniqueConfig : public CommonConfigData {
		std::vector< std::pair<std::string, float> > critera;
	};

	struct GLANNOTATIONS_API AnnotationClassConfig : public globjects::Referenced, public CommonConfigData {
		bool hasAttributeForTechnique(std::string attrKey, unsigned techniqueIndex) const;
		std::string getAttributeForTechnique(std::string attrKey, unsigned techniqueIndex) const;

		std::vector<const AnnotationTechniqueConfig> techniques;
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