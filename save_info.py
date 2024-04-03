import dearpygui.dearpygui as dpg
import recording_playback as rp
from fssave import fssave

dpg.create_context()

def save_info():
    with dpg.window(label="Enter Recording Information", width=550, height=300, no_collapse=True, no_close=True, tag="save_info"):
        dpg.add_spacer(height=20)
        with dpg.group(horizontal=True):
            dpg.add_spacer(width=84)
            dpg.add_text("Title:")
            dpg.add_input_text(width=250, tag="title")
        with dpg.group(horizontal=True):
            dpg.add_spacer(width=63)
            dpg.add_text("Location:")
            dpg.add_input_text(width=250, tag="location")
        with dpg.group(horizontal=True):
            dpg.add_spacer(width=42)
            dpg.add_text("Description:")
            dpg.add_input_text(width=250, tag="description")
        with dpg.group(horizontal=True):
            dpg.add_spacer(width=84)
            dpg.add_text("Notes:")
            dpg.add_input_text(width=250, tag="notes")
        with dpg.group(horizontal=True):
            dpg.add_spacer(width=63)
            dpg.add_text("Recorder:")
            dpg.add_input_text(width=250, tag="recorder")
        with dpg.group(horizontal=True):
            dpg.add_spacer(width=91)
            dpg.add_text("Tags:")
            dpg.add_input_text(width=250, tag="tags")
            dpg.add_spacer(height=40)
        with dpg.group(horizontal=True):
            dpg.add_spacer(width=180)
            dpg.add_checkbox(label="Open after saving", tag="open_after", callback=open_after_callback)
        with dpg.group(horizontal=False):
            dpg.add_spacer(height=2)
        with dpg.group(horizontal=True):
            dpg.add_spacer(width=220)
            dpg.add_button(label="Save", width=70, height=40, tag="save_file", callback=save_file_callback)

def open_after_callback(sender, app_data):
    print("Open after saving checkbox clicked")
    rp.recording_playback()       

fssave = fssave()
def save_file_callback(sender, app_data):
    title = dpg.get_value("title")
    description = dpg.get_value("description")
    notes = dpg.get_value("notes")
    recorder = dpg.get_value("recorder")
    location = dpg.get_value("location")
    tags = dpg.get_value("tags")

    global fssave
    fssave.add_basic(title, description, notes, recorder)
    fssave.add_location(location)
    fssave.add_tags(tags)
    metadata = fssave.return_json()
    print(metadata)
    dpg.delete_item("save_info")
    
if __name__ == "__main__":
    save_info()
    dpg.create_viewport(x_pos=300, y_pos=30, width=550, height=340)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()