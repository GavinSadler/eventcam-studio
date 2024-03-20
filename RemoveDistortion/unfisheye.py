# Sources: 
# https://www.geeksforgeeks.org/extract-images-from-video-in-python/
# https://www.geeksforgeeks.org/python-writing-to-video-with-opencv/

# TODO: Program currently uses JPGs which I dont think we wanna use due to its lossy nature.

import cv2
import os

import calibration

class Unfisheye:
    out_file_name: str
    in_file_name: str
    calibration_image_directory: str

    _in_file_fps: int
    _in_file_length: int
    _completion_bar_on: bool
    _input_stream: cv2.VideoCapture
    _output_stream: cv2.VideoWriter
    _calibrate: calibration

    def __init__(self, out_file_name: str, in_file_name: str, calibration_image_directory: str, completion_bar_on: bool = False):

        self._completion_bar_on = completion_bar_on

        # Init files
        self.out_file_name = out_file_name
        self.in_file_name = in_file_name
        self.calibration_image_directory = calibration_image_directory

        # Remove previous instance
        if os.path.exists(self.out_file_name):
            os.remove(self.out_file_name)

        # Start calibration
        self._calibrate = calibration.Calibration()
        images = self._calibrate.load_images(self.calibration_image_directory, "jpg")
        self._calibrate.calculate_fisheye(images, debug_on=False)

        # Read video
        self._input_stream = cv2.VideoCapture(self.in_file_name)
        self._in_file_fps = self._input_stream.get(cv2.CAP_PROP_FPS)
        self._in_file_length = int(self._input_stream.get(cv2.CAP_PROP_FRAME_COUNT))

    def _init_video_stream(self, frame) -> cv2.VideoWriter:

        # Grab frame dimensions
        frame_width = int(frame.shape[0])
        frame_height = int(frame.shape[1])
        out_video_proportions: tuple[int, int] = (frame_height, frame_width)

        # Initialize video stream
        stream: cv2.VideoWriter = cv2.VideoWriter(self.out_file_name, cv2.VideoWriter_fourcc(*"MJPG"), self._in_file_fps, out_video_proportions)

        return stream

    def remove_distortion(self):

        frame_count = 0
        first_iteration: bool = True

        while True:

            is_valid_frame, frame = self._input_stream.read() 

            if is_valid_frame: 
                undistorted_frame = self._calibrate.undistort_image(frame)

                if first_iteration:
                    self._output_stream = self._init_video_stream(undistorted_frame)
                    first_iteration = False

                self._output_stream.write(undistorted_frame)

            else: 
                break

            frame_count += 1

            if self._completion_bar_on:
                print("Percent completion: " + str(round((frame_count / self._in_file_length) * 100)), end='%\r')

    def __del__ (self):
        del self._calibrate
        self._input_stream.release()
        self._output_stream.release()
        cv2.destroyAllWindows() 