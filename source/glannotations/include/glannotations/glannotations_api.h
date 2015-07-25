#pragma once

#ifdef _MSC_VER
#   define GLANNOTATIONS_API_EXPORT_DECLARATION __declspec(dllexport)
#   define GLANNOTATIONS_API_IMPORT_DECLARATION __declspec(dllimport)
#elif __GNUC__
#	define GLANNOTATIONS_API_EXPORT_DECLARATION __attribute__ ((visibility ("default")))
#	define GLANNOTATIONS_API_IMPORT_DECLARATION __attribute__ ((visibility ("default")))
#else
#   define GLANNOTATIONS_API_EXPORT_DECLARATION __attribute__ ((visibility ("default")))
#   define GLANNOTATIONS_API_IMPORT_DECLARATION __attribute__ ((visibility ("default")))
#endif

#ifndef GLANNOTATIONS_STATIC
#ifdef GLANNOTATIONS_EXPORTS
#   define GLANNOTATIONS_API GLANNOTATIONS_API_EXPORT_DECLARATION
#else
#   define GLANNOTATIONS_API GLANNOTATIONS_API_IMPORT_DECLARATION
#endif
#else
#   define GLANNOTATIONS_API
#endif

#ifdef N_DEBUG
#   define IF_DEBUG(statement)
#   define IF_NDEBUG(statement) statement
#else
#   define IF_DEBUG(statement) statement
#   define IF_NDEBUG(statement)
#endif // N_DEBUG

// http://stackoverflow.com/questions/18387640/how-to-deal-with-noexcept-in-visual-studio
#ifndef NOEXCEPT
#   ifdef _MSC_VER
#       define NOEXCEPT
#   else
#       define NOEXCEPT noexcept
#   endif
#endif

/**
 * \namespace GLANNOTATIONS
 *
 * \brief Contains all the core classes that ... functionality.
 *
 * TODO: Detailed documentation for GLANNOTATIONS here.
 */
