#ifndef GLAT_PREPROCESSOR_DISTANCE_FIELD_GENERATOR_H
#define GLAT_PREPROCESSOR_DISTANCE_FIELD_GENERATOR_H

#include <string>

namespace glat {
	namespace preprocessor {
		class DistanceFieldGenerator
		{
		public:
			typedef int Image;
			DistanceFieldGenerator();
			DistanceFieldGenerator(Image image);
			DistanceFieldGenerator(std::string fileName);
			void LoadImage(std::string fileName);
			void LoadImage(Image image);
			const Image& distanceTransform();
			void saveDistanceTransform(std::string fileName);

		protected:
			inline bool isDirty();
			inline bool setDirty(bool dirty);
			bool m_dirty;

		};
	}
}

#endif // !GLAT_PREPROCESSOR_DISTANCE_FIELD_GENERATOR_H
