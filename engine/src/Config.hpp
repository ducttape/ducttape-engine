
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_CONFIG
#define DUCTTAPE_ENGINE_CONFIG

#define QT_USE_QSTRINGBUILDER

#define MYGUI_DONT_USE_OBSOLETE

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define DUCTTAPE_VERSION STR(DUCTTAPE_VERSION_MAJOR) "." STR(DUCTTAPE_VERSION_MINOR) "." STR(DUCTTAPE_VERSION_PATCH)

// The following definitions are kindly taken from SFML

// Identify the operating system
#if defined(_WIN32) || defined(__WIN32__)
    // Windows
    #define DUCTTAPE_SYSTEM_WINDOWS
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif

#elif defined(linux) || defined(__linux)
    // Linux
    #define DUCTTAPE_SYSTEM_LINUX

#elif defined(__APPLE__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh)
    // MacOS
    #define DUCTTAPE_SYSTEM_MACOS

#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
    // FreeBSD
    #define DUCTTAPE_SYSTEM_FREEBSD

#else
    // Unsupported system
    #error This operating system is not supported by Ducttape Engine library
#endif


// Define a portable debug macro
#if !defined(NDEBUG)
    #define DUCTTAPE_ENGINE_DEBUG
#endif


// Define portable import / export macros
#if !defined(DUCTTAPE_STATIC)

    #if defined(DUCTTAPE_SYSTEM_WINDOWS)

        #ifdef ducttape_EXPORTS // Automatic definition by CMake when building dlls.
            // From DLL side, we must export
            #define DUCTTAPE_API __declspec(dllexport)
        #else
            // From client application side, we must import
            #define DUCTTAPE_API __declspec(dllimport)
        #endif

        // For Visual C++ compilers, we also need to turn off this annoying C4251 warning.
        // You can read lots ot different things about it, but the point is the code will
        // just work fine, and so the simplest way to get rid of this warning is to disable it.
        #ifdef _MSC_VER
            #pragma warning(disable : 4251)
        #endif

    #else

        #define DUCTTAPE_API __attribute__ ((visibility ("default")))

    #endif

#else

    // Static build doesn't need these export macros
    #define DUCTTAPE_API

#endif

#endif
