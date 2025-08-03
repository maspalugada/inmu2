#include "LiveAppCore.h"
#include <gst/gst.h>
#include <msclr/marshal.h>

namespace LiveAppCore {

    CoreFunctions::CoreFunctions() {
        pipelineManager = new PipelineManager([this]() { this->FrameReady(); });
    }

    CoreFunctions::~CoreFunctions() {
        this->!CoreFunctions();
    }

    CoreFunctions::!CoreFunctions() {
        delete pipelineManager;
    }

    void CoreFunctions::StartWebcam() {
        if (pipelineManager) {
            if (pipelineManager->CreatePipeline()) {
                pipelineManager->StartPipeline();
            }
        }
    }

    void CoreFunctions::StopWebcam() {
        if (pipelineManager) {
            pipelineManager->StopPipeline();
        }
    }

    void CoreFunctions::FrameReady() {
        OnFrameReady();
    }

    array<System::Byte>^ CoreFunctions::GetLatestFrame(int% width, int% height) {
        std::vector<guint8> frameBuffer;
        int w, h;
        pipelineManager->GetLatestFrame(frameBuffer, w, h);

        width = w;
        height = h;

        if (frameBuffer.empty()) {
            return nullptr;
        }

        array<System::Byte>^ byteArray = gcnew array<System::Byte>(frameBuffer.size());
        System::Runtime::InteropServices::Marshal::Copy((System::IntPtr)frameBuffer.data(), byteArray, 0, frameBuffer.size());
        return byteArray;
    }

    System::String^ CoreFunctions::GetGStreamerVersion() {
        guint major, minor, micro, nano;
        gst_version(&major, &minor, &micro, &nano);
        return System::String::Format("GStreamer version: {0}.{1}.{2}.{3}", major, minor, micro, nano);
    }
}
