#pragma once

#include "PipelineManager.h"

namespace LiveAppCore {
    public ref class CoreFunctions {
    public:
        CoreFunctions();
        ~CoreFunctions();
        !CoreFunctions();

        void StartWebcam();
        void StopWebcam();

        static System::String^ GetGStreamerVersion();

    private:
        PipelineManager* pipelineManager;
    };
}
