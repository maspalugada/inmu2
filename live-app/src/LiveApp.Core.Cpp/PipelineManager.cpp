#include "PipelineManager.h"

PipelineManager::PipelineManager(FrameCallback callback) : frameCallback(callback) {
    gst_init(nullptr, nullptr);
}

PipelineManager::~PipelineManager() {
    for (auto const& [id, val] : pipelines) {
        if (val.pipeline) {
            gst_element_set_state(val.pipeline, GST_STATE_NULL);
            gst_object_unref(val.pipeline);
        }
    }
}

GstFlowReturn PipelineManager::OnNewSample(GstElement* sink, PipelineManager* manager) {
    GstSample* sample = gst_app_sink_pull_sample(GST_APP_SINK(sink));
    if (sample) {
        const gchar* sink_name_char = gst_element_get_name(sink);
        std::string id(sink_name_char);

        auto it = manager->pipelines.find(id);
        if (it != manager->pipelines.end()) {
            PipelineWrapper& wrapper = it->second;
            GstBuffer* buffer = gst_sample_get_buffer(sample);
            GstMapInfo map;
            if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {
                GstCaps* caps = gst_sample_get_caps(sample);
                GstStructure* s = gst_caps_get_structure(caps, 0);
                int width, height;
                gst_structure_get_int(s, "width", &width);
                gst_structure_get_int(s, "height", &height);

                {
                    std::lock_guard<std::mutex> lock(wrapper.frameMutex);
                    wrapper.frameWidth = width;
                    wrapper.frameHeight = height;
                    wrapper.frameBuffer.assign(map.data, map.data + map.size);
                }

                if (manager->frameCallback) {
                    manager->frameCallback(id);
                }
                gst_buffer_unmap(buffer, &map);
            }
        }
        gst_sample_unref(sample);
    }
    return GST_FLOW_OK;
}

void PipelineManager::GetLatestFrame(const std::string& id, std::vector<guint8>& buffer, int& width, int& height) {
    auto it = pipelines.find(id);
    if (it != pipelines.end()) {
        PipelineWrapper& wrapper = it->second;
        std::lock_guard<std::mutex> lock(wrapper.frameMutex);
        buffer = wrapper.frameBuffer;
        width = wrapper.frameWidth;
        height = wrapper.frameHeight;
    }
}

bool PipelineManager::CreateWebcamPipeline(const std::string& id) {
    std::string pipeline_str = "ksvideosrc ! videoconvert ! video/x-raw,format=BGRx ! appsink name=" + id;
    pipelines[id].pipeline = gst_parse_launch(pipeline_str.c_str(), nullptr);
    if (!pipelines[id].pipeline) return false;

    GstElement* sink = gst_bin_get_by_name(GST_BIN(pipelines[id].pipeline), id.c_str());
    if (sink) {
        g_object_set(sink, "emit-signals", TRUE, "sync", FALSE, NULL);
        g_signal_connect(sink, "new-sample", G_CALLBACK(OnNewSample), this);
        gst_object_unref(sink);
    }
    return true;
}

bool PipelineManager::CreateScreenCapturePipeline(const std::string& id) {
    std::string pipeline_str = "gdiscreencapsrc ! videoconvert ! video/x-raw,format=BGRx ! appsink name=" + id;
    pipelines[id].pipeline = gst_parse_launch(pipeline_str.c_str(), nullptr);
    if (!pipelines[id].pipeline) return false;

    GstElement* sink = gst_bin_get_by_name(GST_BIN(pipelines[id].pipeline), id.c_str());
    if (sink) {
        g_object_set(sink, "emit-signals", TRUE, "sync", FALSE, NULL);
        g_signal_connect(sink, "new-sample", G_CALLBACK(OnNewSample), this);
        gst_object_unref(sink);
    }
    return true;
}

bool PipelineManager::CreateVideoFilePipeline(const std::string& id, const std::string& filePath) {
    std::string pipeline_str = "filesrc location=\"" + filePath + "\" ! decodebin ! videoconvert ! video/x-raw,format=BGRx ! appsink name=" + id;
    pipelines[id].pipeline = gst_parse_launch(pipeline_str.c_str(), nullptr);
    if (!pipelines[id].pipeline) return false;

    GstElement* sink = gst_bin_get_by_name(GST_BIN(pipelines[id].pipeline), id.c_str());
    if (sink) {
        g_object_set(sink, "emit-signals", TRUE, "sync", FALSE, NULL);
        g_signal_connect(sink, "new-sample", G_CALLBACK(OnNewSample), this);
        gst_object_unref(sink);
    }
    return true;
}

void PipelineManager::StartPipeline(const std::string& id) {
    auto it = pipelines.find(id);
    if (it != pipelines.end()) {
        gst_element_set_state(it->second.pipeline, GST_STATE_PLAYING);
    }
}

void PipelineManager::StopPipeline(const std::string& id) {
    auto it = pipelines.find(id);
    if (it != pipelines.end()) {
        gst_element_set_state(it->second.pipeline, GST_STATE_NULL);
    }
}

void PipelineManager::SetAsPreview(const std::string& id) {
    previewId = id;
}

void PipelineManager::UpdateSourcePosition(const std::string& id, int x, int y) {
    auto it = pipelines.find(id);
    if (it != pipelines.end()) {
        it->second.xpos = x;
        it->second.ypos = y;
    }
}

void PipelineManager::Transition() {
    if (compositorPipeline == nullptr) {
        std::string pipeline_str = "compositor name=comp ! videoconvert ! video/x-raw,format=BGRx ! appsink name=program";
        compositorPipeline = gst_parse_launch(pipeline_str.c_str(), nullptr);

        GstElement* sink = gst_bin_get_by_name(GST_BIN(compositorPipeline), "program");
        if (sink) {
            g_object_set(sink, "emit-signals", TRUE, "sync", FALSE, NULL);
            g_signal_connect(sink, "new-sample", G_CALLBACK(OnNewSample), this);
            gst_object_unref(sink);
        }
        gst_element_set_state(compositorPipeline, GST_STATE_PLAYING);
    }

    auto it = pipelines.find(previewId);
    if (it != pipelines.end()) {
        GstElement* compositor = gst_bin_get_by_name(GST_BIN(compositorPipeline), "comp");
        if (compositor) {
            GstPad* sinkpad = gst_element_request_pad_simple(compositor, "sink_%u");
            if (sinkpad) {
                GstElement* pipeline = it->second.pipeline;
                GstElement* source = gst_bin_get_by_name(GST_BIN(pipeline), "ksvideosrc0"); // This needs to be more generic
                if (source) {
                    GstPad* srcpad = gst_element_get_static_pad(source, "src");
                    if (srcpad) {
                        gst_pad_link(srcpad, sinkpad);
                        gst_object_unref(srcpad);
                    }
                    gst_object_unref(source);
                }
                gst_object_unref(sinkpad);
            }
            gst_object_unref(compositor);
        }
    }
}
