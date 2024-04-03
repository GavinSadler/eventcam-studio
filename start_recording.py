import dearpygui.dearpygui as dpg
from Camera import Camera
from EventCamera import EventCamera
from FrameCamera import FrameCamera
import save_info as si

# # Setup Event Camera
# eventCam = EventCamera()
# eventCam.connect()
# eventCam.startStreaming()

# # Setup Frame Camera
# frameCam = FrameCamera()
# frameCam.connect()
# frameCam.startStreaming()

# # Viewport textures
# with dpg.texture_registry():
#     dpg.add_raw_texture(
#         eventCam.width,
#         eventCam.height,
#         eventCam.frameBuffer,
#         tag="eventCameraFrameBuffer",
#     )
#     dpg.add_raw_texture(
#         frameCam.width,
#         frameCam.height,
#         frameCam.frameBuffer,
#         tag="frameCameraFrameBuffer",
#     )

def setup_recorder():
    dpg.create_context()
    with dpg.window(label="Recorder", width=1000, height=610, no_close=True, no_collapse=True):
        # Event and Frame Camera Streams
        # with dpg.group(horizontal=True):
        #     dpg.add_image("eventCameraFrameBuffer", width=400, height=400)
        #     dpg.add_image("frameCameraFrameBuffer", width=400, height=400)
        with dpg.group(horizontal=False):
            dpg.add_spacer(height=100)
        with dpg.group(horizontal=True):
            dpg.add_spacer(width=400)
            dpg.add_button(label="Start recording", width=150, height=50, tag="start_stop_recording", callback=recording_callback)
            dpg.add_spacer(width=280)
            dpg.add_button(label="Bookmark", width=80, height=50, tag="bookmark", callback=bookmark_callback)
        with dpg.group(horizontal=False):
            dpg.add_spacer(height=100)
            dpg.add_text("Time recorded:  seconds")
            dpg.add_text("Frames captured: ")   

    dpg.create_viewport(x_pos=300, y_pos=30, width=1000, height=610)
    dpg.show_viewport()
    dpg.setup_dearpygui()
    dpg.start_dearpygui()
    dpg.destroy_context()  

recording = False
def recording_callback(sender, app_data):
    global recording
    if recording == False:
        print("Start recording button pressed")
        si.save_info()
        dpg.configure_item("start_stop_recording", label="Stop recording")
        recording = True
    else:
        print("Stop recording button pressed")
        dpg.configure_item("start_stop_recording", label="Start recording")
        recording = False

def bookmark_callback(sender, app_data):
    print("Bookmark button pressed")

# eventCam.stopStreaming()
# frameCam.stopStreaming()
if __name__ == "__main__":
    setup_recorder()