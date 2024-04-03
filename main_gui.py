import dearpygui.dearpygui as dpg
import start_recording as sr
import save_info as si
import recording_playback as rp

def setup_gui():
    sr.setup_recorder()
    
if __name__ == "__main__":
    setup_gui()