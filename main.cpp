#include<gst/gst.h>

int main(int argc, char *argv[])
{
  GstElement *pipeline, *source, *videoconvert, *videoscale, *capsfilter, *queue, *encoder, *muxer, *sink;
  GstCaps *caps;
  GstBus *bus;
  GstMessage *message;

  // Initialize GStreamer
  gst_init(&argc, &argv);

  // Create the elements
  source = gst_element_factory_make("v4l2src", "source");
  videoconvert = gst_element_factory_make("videoconvert", "videoconvert");
  videoscale = gst_element_factory_make("videoscale", "videoscale");
  capsfilter = gst_element_factory_make("capsfilter", "capsfilter");
  queue = gst_element_factory_make("queue", "queue");
  encoder = gst_element_factory_make("x264enc", "encoder");
  muxer = gst_element_factory_make("flvmux", "muxer");
  sink = gst_element_factory_make("rtmpsink", "sink");

  // Set properties
  g_object_set(encoder, "bitrate", 5000, NULL);
  g_object_set(muxer, "streamable", TRUE, NULL);
  g_object_set(sink, "location", "rtmp://localhost/stream", NULL);

  caps = gst_caps_new_simple("video/x-raw",
      "width", G_TYPE_INT, 3840,
      "height", G_TYPE_INT, 2160,
      NULL);

  g_object_set(capsfilter, "caps", caps, NULL);
  gst_caps_unref(caps);

  pipeline = gst_pipeline_new("rtmpcam");

  gst_bin_add_many(GST_BIN(pipeline), source, videoconvert, videoscale, capsfilter, queue, encoder, muxer, sink, NULL);

  if (!gst_element_link_many(source, videoconvert, videoscale, capsfilter, queue, encoder, muxer, sink, NULL))
  {
    g_error("Failed to link elements");

    return -1;
  }

  gst_element_set_state(pipeline, GST_STATE_PLAYING);

  bus = gst_element_get_bus(pipeline);
  message = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

  if (message != NULL)
  {
    gst_message_unref(message);
  }

  gst_object_unref(bus);
  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(pipeline);

  return 0;
}
