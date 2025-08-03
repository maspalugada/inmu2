#pragma once

#include "PipelineManager.h"

namespace LiveAppCore {
    public delegate void FrameReadyHandler();

    public ref class CoreFunctions {
    public:
        CoreFunctions();
        ~CoreFunctions();
        !CoreFunctions();

        void StartWebcam();
        void StopWebcam();
        array<System::Byte>^ GetLatestFrame(int% width, int% height);

        static System::String^ GetGStreamerVersion();

        event FrameReadyHandler^ OnFrameReady;

    private:
        void FrameReady();
        PipelineManager* pipelineManager;
    };
}
