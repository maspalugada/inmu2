#pragma once

#include <gst/gst.h>

class PipelineManager {
public:
    PipelineManager();
    ~PipelineManager();

    bool CreatePipeline();
    void StartPipeline();
    void StopPipeline();

private:
    GstElement* pipeline;
};
