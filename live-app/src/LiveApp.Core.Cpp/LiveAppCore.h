#pragma once

#include "PipelineManager.h"

namespace LiveAppCore {
    public delegate void FrameReadyHandler(System::String^ id);
    public delegate void AudioLevelHandler(System::String^ id, double level);

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
        void SetSourceVolume(System::String^ id, double volume);
        void SetSourceMute(System::String^ id, bool mute);

        enum class TransitionType {
            Cut,
            Fade,
            Wipe
        };
        void SetTransitionType(TransitionType type);
        void Transition();
        void StartRecording(System::String^ filePath);
        void StopRecording();
        void UpdateSourceProperties(System::String^ id, System::String^ deviceName, System::String^ capability);

        enum class FilterType {
            Grayscale
        };
        void AddFilter(System::String^ id, FilterType type);
        void RemoveFilter(System::String^ id);

        void SetTextOverlay(System::String^ text, System::String^ font, int size, int color, int x, int y);

        void StartVirtualCamera();
        void StopVirtualCamera();

        array<System::Byte, 2>^ GenerateTransitionPreview(TransitionType type, int% width, int% height);
        array<System::Byte>^ GetLatestFrame(System::String^ id, int% width, int% height);

        System::Collections::Generic::List<System::String^>^ GetVideoDevices();
        System::Collections::Generic::List<System::String^>^ GetDeviceCapabilities(System::String^ deviceName);

        static System::String^ GetGStreamerVersion();

        event FrameReadyHandler^ OnFrameReady;
        event AudioLevelHandler^ OnAudioLevel;

    private:
        void FrameReady(const std::string& id);
        void AudioLevel(const std::string& id, double level);
        PipelineManager* pipelineManager;
    };
}
