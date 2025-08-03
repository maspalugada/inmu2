#pragma once

#include "PipelineManager.h"

namespace LiveAppCore {
    public delegate void FrameReadyHandler(System::String^ id);

    public ref class CoreFunctions {
    public:
        CoreFunctions();
        ~CoreFunctions();
        !CoreFunctions();

        System::String^ CreateWebcamSource();
        System::String^ CreateScreenCaptureSource();
        System::String^ CreateVideoFileSource(System::String^ filePath);
        void StopSource(System::String^ id);
        void SetAsPreview(System::String^ id);
        void UpdateSourcePosition(System::String^ id, int x, int y);
        void Transition();
        array<System::Byte>^ GetLatestFrame(System::String^ id, int% width, int% height);

        static System::String^ GetGStreamerVersion();

        event FrameReadyHandler^ OnFrameReady;

    private:
        void FrameReady(const std::string& id);
        PipelineManager* pipelineManager;
    };
}
