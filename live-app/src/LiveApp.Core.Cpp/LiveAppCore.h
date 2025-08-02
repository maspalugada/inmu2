#pragma once

#ifdef LIVEAPPCORECPP_EXPORTS
#define LIVEAPP_API __declspec(dllexport)
#else
#define LIVEAPP_API __declspec(dllimport)
#endif

namespace LiveAppCore {
    public ref class CoreFunctions {
    public:
        static System::String^ GetGStreamerVersion();
    };
}
