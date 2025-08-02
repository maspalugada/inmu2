#include "LiveAppCore.h"
#include <gst/gst.h>

namespace LiveAppCore {

    CoreFunctions::CoreFunctions() {
        pipelineManager = new PipelineManager();
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

    System::String^ CoreFunctions::GetGStreamerVersion() {
        guint major, minor, micro, nano;
        gst_version(&major, &minor, &micro, &nano);
        return System::String::Format("GStreamer version: {0}.{1}.{2}.{3}", major, minor, micro, nano);
    }
}
