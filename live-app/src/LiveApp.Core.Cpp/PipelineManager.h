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

    PipelineManager(FrameCallback callback);
    ~PipelineManager();

    bool CreateWebcamPipeline(const std::string& id);
    bool CreateScreenCapturePipeline(const std::string& id);
    bool CreateVideoFilePipeline(const std::string& id, const std::string& filePath);
    void StartPipeline(const std::string& id);
    void StopPipeline(const std::string& id);
    void SetAsPreview(const std::string& id);
    void UpdateSourcePosition(const std::string& id, int x, int y);
    void Transition();
    void GetLatestFrame(const std::string& id, std::vector<guint8>& buffer, int& width, int& height);

private:
    static GstFlowReturn OnNewSample(GstElement* sink, PipelineManager* manager);

    std::map<std::string, PipelineWrapper> pipelines;
    std::map<std::string, GstPad*> compositorPads;
    std::string previewId;
    GstElement* compositorPipeline = nullptr;
    FrameCallback frameCallback;
};
