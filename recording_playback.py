import dearpygui.dearpygui as dpg
from Camera import Camera
from EventCamera import EventCamera
from FrameCamera import FrameCamera

# # === Setup Event Camera ===
# eventCam = EventCamera()
# eventCam.connect()
# eventCam.startStreaming()
# # === Setup Frame Camera ===
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

def recording_playback():
    dpg.create_context()
    with dpg.window(label="Recording Playback", width=1000, height=610, no_close=True, no_collapse=True):
        # Event and Frame Camera Streams
        # with dpg.group(horizontal=True):
        #     dpg.add_image("eventCameraFrameBuffer", width=400, height=400)
        #     dpg.add_image("frameCameraFrameBuffer", width=400, height=400)

        # Video slider
        with dpg.group(horizontal=False):
            dpg.add_slider_float(vertical=False, width=970, default_value=0, tag="slider")
            dpg.add_spacer(height=20)

        with dpg.group(horizontal=True):
            dpg.add_spacer(width=280)
            dpg.add_button(label="Back frame", width=110, height=40, callback=back_frame_callback, tag="back_frame")
            dpg.add_spacer(width=10)
            dpg.add_button(label="Start playback", width=110, height=40, callback=playback_callback, tag="playback")
            dpg.add_spacer(width=10)
            dpg.add_button(label="Forward frame", width=110, height=40, callback=forward_frame_callback, tag="forward_frame")
        with dpg.group(horizontal=False):
            dpg.add_spacer(height=60)
        with dpg.group(horizontal=True):
            dpg.add_spacer(width=20)
            dpg.add_button(label="Choose file", width=100, height=40, callback=lambda: dpg.show_item("file_dialog_id"), tag="choose_file")
            dpg.add_file_dialog(directory_selector=True, show=False, callback=open_file_callback, tag="file_dialog_id", width=700 ,height=400)
        
def back_frame_callback(sender, app_data):
    print("Back frame button clicked")
    
playback = False
def playback_callback(sender, app_data):
    global playback
    if playback == False:
        print("Start playback button pressed")
        dpg.configure_item("playback", label="Stop playback")
        playback = True
    else:
        print("Stop playback button pressed")
        dpg.configure_item("playback", label="Start playback")
        playback = False

def forward_frame_callback(sender, app_data):
    print("Forward frame button clicked")
    
def open_file_callback(sender, app_data):
    print("Open file button clicked")
    file_path = dpg.get_value("file_dialog_id")
    print("Selected File Path: ", file_path)
    print('OK was clicked.')
    print("Sender: ", sender)
    print("App Data: ", app_data)

# eventCam.stopStreaming()
# frameCam.stopStreaming()
if __name__ == "__main__":
    recording_playback()
    dpg.create_viewport(x_pos=300, y_pos=30, width=1000, height=610)
    dpg.show_viewport()
    dpg.setup_dearpygui()
    dpg.start_dearpygui()
    dpg.destroy_context()