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
        pipelineManager = new PipelineManager(
            [this](const std::string& id) { this->FrameReady(id); },
            [this](const std::string& id, double level) { this->AudioLevel(id, level); }
        );
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

    void CoreFunctions::UpdateSourcePosition(System::String^ id, int x, int y) {
        if (pipelineManager) {
            pipelineManager->UpdateSourcePosition(msclr::interop::marshal_as<std::string>(id), x, y);
        }
    }

    void CoreFunctions::SetSourceVolume(System::String^ id, double volume) {
        if (pipelineManager) {
            pipelineManager->SetSourceVolume(msclr::interop::marshal_as<std::string>(id), volume);
        }
    }

    void CoreFunctions::SetSourceMute(System::String^ id, bool mute) {
        if (pipelineManager) {
            pipelineManager->SetSourceMute(msclr::interop::marshal_as<std::string>(id), mute);
        }
    }

    void CoreFunctions::SetTransitionType(TransitionType type) {
        if (pipelineManager) {
            pipelineManager->SetTransitionType((PipelineManager::TransitionType)type);
        }
    }

    void CoreFunctions::Transition() {
        if (pipelineManager) {
            pipelineManager->Transition();
        }
    }

    void CoreFunctions::StartRecording(System::String^ filePath) {
        if (pipelineManager) {
            msclr::interop::marshal_context context;
            const char* path = context.marshal_as<const char*>(filePath);
            pipelineManager->StartRecording(path);
        }
    }

    void CoreFunctions::StopRecording() {
        if (pipelineManager) {
            pipelineManager->StopRecording();
        }
    }

    void CoreFunctions::UpdateSourceProperties(System::String^ id, System::String^ deviceName, System::String^ capability) {
        if (pipelineManager) {
            pipelineManager->UpdateSourceProperties(
                msclr::interop::marshal_as<std::string>(id),
                msclr::interop::marshal_as<std::string>(deviceName),
                msclr::interop::marshal_as<std::string>(capability)
            );
        }
    }

    void CoreFunctions::AddFilter(System::String^ id, FilterType type) {
        if (pipelineManager) {
            pipelineManager->AddFilter(msclr::interop::marshal_as<std::string>(id), (PipelineManager::FilterType)type);
        }
    }

    void CoreFunctions::RemoveFilter(System::String^ id) {
        if (pipelineManager) {
            pipelineManager->RemoveFilter(msclr::interop::marshal_as<std::string>(id));
        }
    }

    void CoreFunctions::SetTextOverlay(System::String^ text, System::String^ font, int size, int color, int x, int y) {
        if (pipelineManager) {
            pipelineManager->SetTextOverlay(
                msclr::interop::marshal_as<std::string>(text),
                msclr::interop::marshal_as<std::string>(font),
                size,
                color,
                x,
                y
            );
        }
    }

    array<System::Byte, 2>^ CoreFunctions::GenerateTransitionPreview(TransitionType type, int% width, int% height) {
        if (pipelineManager) {
            int w, h;
            std::vector<std::vector<guint8>> frames = pipelineManager->GenerateTransitionPreview((PipelineManager::TransitionType)type, w, h);
            width = w;
            height = h;

            if (frames.empty()) {
                return nullptr;
            }

            int frameSize = frames[0].size();
            array<System::Byte, 2>^ result = gcnew array<System::Byte, 2>(frames.size(), frameSize);
            for (int i = 0; i < frames.size(); ++i) {
                for (int j = 0; j < frameSize; ++j) {
                    result[i, j] = frames[i][j];
                }
            }
            return result;
        }
        return nullptr;
    }

    void CoreFunctions::FrameReady(const std::string& id) {
        OnFrameReady(msclr::interop::marshal_as<System::String^>(id));
    }

    void CoreFunctions::AudioLevel(const std::string& id, double level) {
        OnAudioLevel(msclr::interop::marshal_as<System::String^>(id), level);
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

    System::Collections::Generic::List<System::String^>^ CoreFunctions::GetVideoDevices() {
        if (pipelineManager) {
            std::vector<std::string> devices = pipelineManager->GetVideoDevices();
            System::Collections::Generic::List<System::String^>^ result = gcnew System::Collections::Generic::List<System::String^>();
            for (const auto& device : devices) {
                result->Add(msclr::interop::marshal_as<System::String^>(device));
            }
            return result;
        }
        return nullptr;
    }

    System::Collections::Generic::List<System::String^>^ CoreFunctions::GetDeviceCapabilities(System::String^ deviceName) {
        if (pipelineManager) {
            std::vector<std::string> capabilities = pipelineManager->GetDeviceCapabilities(msclr::interop::marshal_as<std::string>(deviceName));
            System::Collections::Generic::List<System::String^>^ result = gcnew System::Collections::Generic::List<System::String^>();
            for (const auto& cap : capabilities) {
                result->Add(msclr::interop::marshal_as<System::String^>(cap));
            }
            return result;
        }
        return nullptr;
    }
}
