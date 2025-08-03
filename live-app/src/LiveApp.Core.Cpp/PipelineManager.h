#pragma once

#include <gst/gst.h>
#include <functional>
#include <vector>
#include <mutex>

class PipelineManager {
public:
    using FrameCallback = std::function<void()>;

    PipelineManager(FrameCallback callback);
    ~PipelineManager();

    bool CreateWebcamPipeline();
    bool CreateScreenCapturePipeline();
    bool CreateVideoFilePipeline(const std::string& filePath);
    void StartPipeline();
    void StopPipeline();

    void GetLatestFrame(std::vector<guint8>& buffer, int& width, int& height);

private:
    static GstFlowReturn OnNewSample(GstElement* sink, PipelineManager* data);

    GstElement* pipeline;
    FrameCallback frameCallback;

    std::vector<guint8> frameBuffer;
    int frameWidth;
    int frameHeight;
    std::mutex frameMutex;
};
