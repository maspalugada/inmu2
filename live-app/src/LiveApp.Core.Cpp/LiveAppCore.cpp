#include "LiveAppCore.h"
#include <gst/gst.h>

namespace LiveAppCore {
    System::String^ CoreFunctions::GetGStreamerVersion() {
        guint major, minor, micro, nano;
        gst_version(&major, &minor, &micro, &nano);
        return System::String::Format("GStreamer version: {0}.{1}.{2}.{3}", major, minor, micro, nano);
    }
}
