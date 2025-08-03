#pragma once

#include <gst/gst.h>
#include <functional>
#include <vector>
#include <mutex>
#include <map>
#include <string>

class PipelineWrapper {
public:
    GstElement* pipeline = nullptr;
    std::vector<guint8> frameBuffer;
    int frameWidth = 0;
    int frameHeight = 0;
    int xpos = 0;
    int ypos = 0;
    std::mutex frameMutex;
};

class PipelineManager {
public:
    using FrameCallback = std::function<void(const std::string&)>;
    using AudioLevelCallback = std::function<void(const std::string&, double)>;

    PipelineManager(FrameCallback frameCallback, AudioLevelCallback audioLevelCallback);
    ~PipelineManager();

    bool CreateWebcamPipeline(const std::string& id);
    bool CreateScreenCapturePipeline(const std::string& id);
    bool CreateVideoFilePipeline(const std::string& id, const std::string& filePath);
    void StartPipeline(const std::string& id);
    void StopPipeline(const std::string& id);
    void SetAsPreview(const std::string& id);
    void UpdateSourcePosition(const std::string& id, int x, int y);
    void SetSourceVolume(const std::string& id, double volume);
    void SetSourceMute(const std::string& id, bool mute);
    void Transition();
    void StartRecording(const std::string& filePath);
    void StopRecording();
    void UpdateSourceProperties(const std::string& id, const std::string& deviceName, const std::string& capability);

    enum class FilterType {
        Grayscale
    };
    void AddFilter(const std::string& id, FilterType type);
    void RemoveFilter(const std::string& id);

    void GetLatestFrame(const std::string& id, std::vector<guint8>& buffer, int& width, int& height);
    std::vector<std::vector<guint8>> GenerateTransitionPreview(TransitionType type, int& width, int& height);
    std::vector<std::string> GetVideoDevices();
    std::vector<std::string> GetDeviceCapabilities(const std::string& deviceName);

private:
    static GstFlowReturn OnNewSample(GstElement* sink, PipelineManager* manager);

    std::map<std::string, PipelineWrapper> pipelines;
    enum class TransitionType {
        Cut,
        Fade,
        Wipe
    };

    std::map<std::string, GstPad*> compositorPads;
    std::string previewId;
    GstElement* compositorPipeline = nullptr;
    FrameCallback frameCallback;
    AudioLevelCallback audioLevelCallback;
    TransitionType currentTransition = TransitionType::Cut;
};
