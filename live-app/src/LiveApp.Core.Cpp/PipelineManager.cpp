#include "PipelineManager.h"

PipelineManager::PipelineManager(FrameCallback callback) : pipeline(nullptr), frameCallback(callback), frameWidth(0), frameHeight(0) {
    gst_init(nullptr, nullptr);
}

PipelineManager::~PipelineManager() {
    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
    }
}

GstFlowReturn PipelineManager::OnNewSample(GstElement* sink, PipelineManager* data) {
    GstSample* sample = gst_app_sink_pull_sample(GST_APP_SINK(sink));
    if (sample) {
        GstBuffer* buffer = gst_sample_get_buffer(sample);
        GstMapInfo map;
        if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {
            GstCaps* caps = gst_sample_get_caps(sample);
            GstStructure* s = gst_caps_get_structure(caps, 0);
            int width, height;
            gst_structure_get_int(s, "width", &width);
            gst_structure_get_int(s, "height", &height);

            {
                std::lock_guard<std::mutex> lock(data->frameMutex);
                data->frameWidth = width;
                data->frameHeight = height;
                data->frameBuffer.assign(map.data, map.data + map.size);
            }

            if (data->frameCallback) {
                data->frameCallback();
            }
            gst_buffer_unmap(buffer, &map);
        }
        gst_sample_unref(sample);
    }
    return GST_FLOW_OK;
}

void PipelineManager::GetLatestFrame(std::vector<guint8>& buffer, int& width, int& height) {
    std::lock_guard<std::mutex> lock(frameMutex);
    buffer = frameBuffer;
    width = frameWidth;
    height = frameHeight;
}

bool PipelineManager::CreateWebcamPipeline() {
    const char* pipeline_str = "ksvideosrc ! videoconvert ! video/x-raw,format=BGRx ! appsink name=sink";
    pipeline = gst_parse_launch(pipeline_str, nullptr);

    if (!pipeline) {
        return false;
    }

    GstElement* sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");
    if (sink) {
        g_object_set(sink, "emit-signals", TRUE, "sync", FALSE, NULL);
        g_signal_connect(sink, "new-sample", G_CALLBACK(OnNewSample), this);
        gst_object_unref(sink);
    }

    return true;
}

bool PipelineManager::CreateScreenCapturePipeline() {
    const char* pipeline_str = "gdiscreencapsrc ! videoconvert ! video/x-raw,format=BGRx ! appsink name=sink";
    pipeline = gst_parse_launch(pipeline_str, nullptr);

    if (!pipeline) {
        return false;
    }

    GstElement* sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");
    if (sink) {
        g_object_set(sink, "emit-signals", TRUE, "sync", FALSE, NULL);
        g_signal_connect(sink, "new-sample", G_CALLBACK(OnNewSample), this);
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
