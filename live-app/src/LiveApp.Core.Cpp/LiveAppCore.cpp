#include "LiveAppCore.h"
#include <gst/gst.h>
#include <msclr/marshal.h>
#include <msclr/marshal_cppstd.h>
#include <string>
#include <random>
#include <sstream>

namespace LiveAppCore {

    std::string GenerateUniqueId() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, 15);

        std::stringstream ss;
        for (int i = 0; i < 16; ++i) {
            ss << std::hex << distrib(gen);
        }
        return ss.str();
    }

    CoreFunctions::CoreFunctions() {
        pipelineManager = new PipelineManager([this](const std::string& id) { this->FrameReady(id); });
    }

    CoreFunctions::~CoreFunctions() {
        this->!CoreFunctions();
    }

    CoreFunctions::!CoreFunctions() {
        delete pipelineManager;
    }

    System::String^ CoreFunctions::CreateWebcamSource() {
        std::string id = GenerateUniqueId();
        if (pipelineManager && pipelineManager->CreateWebcamPipeline(id)) {
            pipelineManager->StartPipeline(id);
            return msclr::interop::marshal_as<System::String^>(id);
        }
        return nullptr;
    }

    System::String^ CoreFunctions::CreateScreenCaptureSource() {
        std::string id = GenerateUniqueId();
        if (pipelineManager && pipelineManager->CreateScreenCapturePipeline(id)) {
            pipelineManager->StartPipeline(id);
            return msclr::interop::marshal_as<System::String^>(id);
        }
        return nullptr;
    }

    System::String^ CoreFunctions::CreateVideoFileSource(System::String^ filePath) {
        std::string id = GenerateUniqueId();
        msclr::interop::marshal_context context;
        const char* path = context.marshal_as<const char*>(filePath);
        if (pipelineManager && pipelineManager->CreateVideoFilePipeline(id, path)) {
            pipelineManager->StartPipeline(id);
            return msclr::interop::marshal_as<System::String^>(id);
        }
        return nullptr;
    }

    void CoreFunctions::StopSource(System::String^ id) {
        if (pipelineManager) {
            pipelineManager->StopPipeline(msclr::interop::marshal_as<std::string>(id));
        }
    }

    void CoreFunctions::SetAsPreview(System::String^ id) {
        if (pipelineManager) {
            pipelineManager->SetAsPreview(msclr::interop::marshal_as<std::string>(id));
        }
    }

    void CoreFunctions::Transition() {
        if (pipelineManager) {
            pipelineManager->Transition();
        }
    }

    void CoreFunctions::FrameReady(const std::string& id) {
        OnFrameReady(msclr::interop::marshal_as<System::String^>(id));
    }

    array<System::Byte>^ CoreFunctions::GetLatestFrame(System::String^ id, int% width, int% height) {
        std::vector<guint8> frameBuffer;
        int w, h;
        pipelineManager->GetLatestFrame(msclr::interop::marshal_as<std::string>(id), frameBuffer, w, h);

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
