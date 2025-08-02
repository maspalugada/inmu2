#include "PipelineManager.h"

PipelineManager::PipelineManager() : pipeline(nullptr) {
    gst_init(nullptr, nullptr);
}

PipelineManager::~PipelineManager() {
    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
    }
}

bool PipelineManager::CreatePipeline() {
    // This pipeline captures from the default webcam, converts the video to a format WPF can use,
    // and sends it to an appsink element so we can grab the frames.
    const char* pipeline_str = "ksvideosrc ! videoconvert ! video/x-raw,format=BGRx ! appsink name=sink";
    pipeline = gst_parse_launch(pipeline_str, nullptr);

    if (!pipeline) {
        return false;
    }

    GstElement* sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");
    if (sink) {
        g_object_set(sink, "emit-signals", TRUE, "sync", FALSE, NULL);
        // g_signal_connect(sink, "new-sample", G_CALLBACK(OnNewSample), this);
        gst_object_unref(sink);
    }

    return true;
}

void PipelineManager::StartPipeline() {
    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_PLAYING);
    }
}

void PipelineManager::StopPipeline() {
    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_NULL);
    }
}
