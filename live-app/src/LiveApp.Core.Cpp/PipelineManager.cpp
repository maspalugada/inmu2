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

void PipelineManager::SetSourceVolume(const std::string& id, double volume) {
    auto it = compositorPads.find(id);
    if (it != compositorPads.end()) {
        GstPad* pad = it->second;
        GstElement* audiomixer = gst_pad_get_parent_element(pad);
        if (audiomixer) {
            g_object_set(G_OBJECT(audiomixer), "volume", volume, NULL);
            gst_object_unref(audiomixer);
        }
    }
}

void PipelineManager::SetSourceMute(const std::string& id, bool mute) {
    auto it = compositorPads.find(id);
    if (it != compositorPads.end()) {
        GstPad* pad = it->second;
        GstElement* audiomixer = gst_pad_get_parent_element(pad);
        if (audiomixer) {
            g_object_set(G_OBJECT(audiomixer), "mute", mute, NULL);
            gst_object_unref(audiomixer);
        }
    }
}

GstFlowReturn PipelineManager::OnNewSample(GstElement* sink, PipelineManager* manager) {
    GstSample* sample = gst_app_sink_pull_sample(GST_APP_SINK(sink));
    if (sample) {
        const gchar* sink_name_char = gst_element_get_name(sink);
        std::string id(sink_name_char);

        if (id == "program") {
            // Handle the program sink separately
            GstBuffer* buffer = gst_sample_get_buffer(sample);
            GstMapInfo map;
            if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {
                if (manager->frameCallback) {
                    manager->frameCallback("program");
                }
                gst_buffer_unmap(buffer, &map);
            }
        } else {
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
    std::string pipeline_str = "ksvideosrc ! tee name=t ! queue ! videoconvert ! video/x-raw,format=BGRx ! appsink name=" + id + " t. ! queue ! audioconvert ! audioresample ! appsink name=audio_" + id;
    pipelines[id].pipeline = gst_parse_launch(pipeline_str.c_str(), nullptr);
    if (!pipelines[id].pipeline) return false;

    GstElement* sink = gst_bin_get_by_name(GST_BIN(pipelines[id].pipeline), id.c_str());
    if (sink) {
        g_object_set(sink, "emit-signals", TRUE, "sync", FALSE, NULL);
        g_signal_connect(sink, "new-sample", G_CALLBACK(OnNewSample), this);
        gst_object_unref(sink);
    }

    GstElement* audio_sink = gst_bin_get_by_name(GST_BIN(pipelines[id].pipeline), ("audio_" + id).c_str());
    if (audio_sink) {
        // We don't need to do anything with the audio sink for now, but we need to make sure it exists.
        gst_object_unref(audio_sink);
    }

    LinkSourceToCompositor(id);
    return true;
}

bool PipelineManager::CreateScreenCapturePipeline(const std::string& id) {
    std::string pipeline_str = "gdiscreencapsrc ! videoconvert ! video/x-raw,format=BGRx ! appsink name=" + id + " wasapisrc ! audioconvert ! audioresample ! appsink name=audio_" + id;
    pipelines[id].pipeline = gst_parse_launch(pipeline_str.c_str(), nullptr);
    if (!pipelines[id].pipeline) return false;

    GstElement* sink = gst_bin_get_by_name(GST_BIN(pipelines[id].pipeline), id.c_str());
    if (sink) {
        g_object_set(sink, "emit-signals", TRUE, "sync", FALSE, NULL);
        g_signal_connect(sink, "new-sample", G_CALLBACK(OnNewSample), this);
        gst_object_unref(sink);
    }

    GstElement* audio_sink = gst_bin_get_by_name(GST_BIN(pipelines[id].pipeline), ("audio_" + id).c_str());
    if (audio_sink) {
        // We don't need to do anything with the audio sink for now, but we need to make sure it exists.
        gst_object_unref(audio_sink);
    }

    LinkSourceToCompositor(id);
    return true;
}

bool PipelineManager::CreateVideoFilePipeline(const std::string& id, const std::string& filePath) {
    std::string pipeline_str = "filesrc location=\"" + filePath + "\" ! decodebin name=d d. ! queue ! videoconvert ! video/x-raw,format=BGRx ! appsink name=" + id + " d. ! queue ! audioconvert ! audioresample ! appsink name=audio_" + id;
    pipelines[id].pipeline = gst_parse_launch(pipeline_str.c_str(), nullptr);
    if (!pipelines[id].pipeline) return false;

    GstElement* sink = gst_bin_get_by_name(GST_BIN(pipelines[id].pipeline), id.c_str());
    if (sink) {
        g_object_set(sink, "emit-signals", TRUE, "sync", FALSE, NULL);
        g_signal_connect(sink, "new-sample", G_CALLBACK(OnNewSample), this);
        gst_object_unref(sink);
    }

    GstElement* audio_sink = gst_bin_get_by_name(GST_BIN(pipelines[id].pipeline), ("audio_" + id).c_str());
    if (audio_sink) {
        // We don't need to do anything with the audio sink for now, but we need to make sure it exists.
        gst_object_unref(audio_sink);
    }

    LinkSourceToCompositor(id);
    return true;
}

void PipelineManager::StartPipeline(const std::string& id) {
    auto it = pipelines.find(id);
    if (it != pipelines.end()) {
        gst_element_set_state(it->second.pipeline, GST_STATE_PLAYING);
    }
}

std::vector<std::vector<guint8>> PipelineManager::GenerateTransitionPreview(TransitionType type, int& width, int& height) {
    std::vector<std::vector<guint8>> frames;
    width = 320;
    height = 240;

    std::string pipeline_str = "videotestsrc pattern=ball ! video/x-raw,width=320,height=240 ! compositor name=comp ! videoconvert ! appsink name=preview_sink videotestsrc pattern=snow ! video/x-raw,width=320,height=240 ! comp.";
    GstElement* preview_pipeline = gst_parse_launch(pipeline_str.c_str(), nullptr);

    if (preview_pipeline) {
        GstElement* sink = gst_bin_get_by_name(GST_BIN(preview_pipeline), "preview_sink");
        if (sink) {
            g_object_set(sink, "emit-signals", TRUE, "sync", FALSE, NULL);

            // This is a simplified way to get frames. A real implementation would use a more robust mechanism.
            for (int i = 0; i < 30; ++i) {
                GstSample* sample = gst_app_sink_pull_sample(GST_APP_SINK(sink));
                if (sample) {
                    GstBuffer* buffer = gst_sample_get_buffer(sample);
                    GstMapInfo map;
                    if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {
                        frames.push_back(std::vector<guint8>(map.data, map.data + map.size));
                        gst_buffer_unmap(buffer, &map);
                    }
                    gst_sample_unref(sample);
                }
            }
            gst_object_unref(sink);
        }
        gst_element_set_state(preview_pipeline, GST_STATE_NULL);
        gst_object_unref(preview_pipeline);
    }

    return frames;
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

void PipelineManager::LinkSourceToCompositor(const std::string& id) {
    if (compositorPipeline == nullptr) {
        std::string pipeline_str = "compositor name=comp ! videoconvert ! video/x-raw,format=BGRx ! appsink name=program audiomixer name=mix ! audioconvert ! audioresample ! autoaudiosink";
        compositorPipeline = gst_parse_launch(pipeline_str.c_str(), nullptr);

        GstElement* sink = gst_bin_get_by_name(GST_BIN(compositorPipeline), "program");
        if (sink) {
            g_object_set(sink, "emit-signals", TRUE, "sync", FALSE, "name", "program", NULL);
            g_signal_connect(sink, "new-sample", G_CALLBACK(OnNewSample), this);
            gst_object_unref(sink);
        }
        gst_element_set_state(compositorPipeline, GST_STATE_PLAYING);
    }

    auto it = pipelines.find(id);
    if (it != pipelines.end()) {
        GstElement* compositor = gst_bin_get_by_name(GST_BIN(compositorPipeline), "comp");
        if (compositor) {
            GstPad* sinkpad = gst_element_request_pad_simple(compositor, "sink_%u");
            if (sinkpad) {
                compositorPads[id] = sinkpad;
                GstElement* pipeline = it->second.pipeline;
                GstElement* source = gst_bin_get_by_name(GST_BIN(pipeline), id.c_str());
                if (source) {
                    GstPad* srcpad = gst_element_get_static_pad(source, "src");
                    if (srcpad) {
                        gst_pad_link(srcpad, sinkpad);
                        gst_object_unref(srcpad);
                    }
                    gst_object_unref(source);
                }
            }
            gst_object_unref(compositor);
        }

        GstElement* audiomixer = gst_bin_get_by_name(GST_BIN(compositorPipeline), "mix");
        if (audiomixer) {
            GstPad* sinkpad = gst_element_request_pad_simple(audiomixer, "sink_%u");
            if (sinkpad) {
                GstElement* pipeline = it->second.pipeline;
                GstElement* audio_source = gst_bin_get_by_name(GST_BIN(pipeline), ("audio_" + id).c_str());
                if (audio_source) {
                    GstPad* srcpad = gst_element_get_static_pad(audio_source, "src");
                    if (srcpad) {
                        gst_pad_link(srcpad, sinkpad);
                        gst_object_unref(srcpad);
                    }
                    gst_object_unref(audio_source);
                }
            }
            gst_object_unref(audiomixer);
        }
    }
}

void PipelineManager::UpdateSourcePosition(const std::string& id, int x, int y) {
    auto it = pipelines.find(id);
    if (it != pipelines.end()) {
        it->second.xpos = x;
        it->second.ypos = y;

        auto pad_it = compositorPads.find(id);
        if (pad_it != compositorPads.end()) {
            g_object_set(pad_it->second, "xpos", x, "ypos", y, NULL);
        }
    }
    auto pad_it = compositorPads.find(id);
    if (pad_it != compositorPads.end()) {
        g_object_set(pad_it->second, "xpos", x, "ypos", y, NULL);
    }
}

void PipelineManager::SetTransitionType(TransitionType type) {
    currentTransition = type;
}

void PipelineManager::Transition() {
    if (compositorPipeline == nullptr) {
        std::string pipeline_str = "compositor name=comp ! videoconvert ! video/x-raw,format=BGRx ! appsink name=program audiomixer name=mix ! audioconvert ! audioresample ! autoaudiosink";
        compositorPipeline = gst_parse_launch(pipeline_str.c_str(), nullptr);

        GstElement* sink = gst_bin_get_by_name(GST_BIN(compositorPipeline), "program");
        if (sink) {
            g_object_set(sink, "emit-signals", TRUE, "sync", FALSE, "name", "program", NULL);
            g_signal_connect(sink, "new-sample", G_CALLBACK(OnNewSample), this);
            gst_object_unref(sink);
        }
        gst_element_set_state(compositorPipeline, GST_STATE_PLAYING);
    }

    auto it = pipelines.find(previewId);
    if (it != pipelines.end()) {
        auto pad_it = compositorPads.find(previewId);
        if (pad_it != compositorPads.end()) {
            GstPad* pad = pad_it->second;
            switch (currentTransition) {
                case TransitionType::Cut:
                    g_object_set(pad, "alpha", 1.0, NULL);
                    break;
                case TransitionType::Fade:
                    // This is a simplified fade. A real fade would involve a timeline.
                    g_object_set(pad, "alpha", 0.0, NULL);
                    // In a real application, we would use a GstController to animate the alpha property over time.
                    // For now, we will just set it to 1.0 after a short delay.
                    g_object_set(pad, "alpha", 1.0, NULL);
                    break;
                case TransitionType::Wipe:
                    // Not implemented yet
                    break;
            }
        }
    }
}
