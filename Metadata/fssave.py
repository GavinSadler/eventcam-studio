import recording_data
import json

class fssave:
    metadata: recording_data.RecordingData = recording_data.RecordingData()

    def __init__(self):

        # Init to None
        self.add_basic()
        self.add_location()
        self.add_timestamp()
        self.add_frame_camera()
        self.add_event_camera()
        self.add_tags()
        self.points_of_interest()

        return

    def add_basic(self, name: str = None, description: str = None, notes: str = None, name_of_recorder: str = None):
        self.metadata.name = name
        self.metadata.description = description
        self.metadata.notes = notes
        self.metadata.name_of_recorder = name_of_recorder

    def add_location(self, name: str = None):
        self.metadata.name = name

    def add_timestamp(self, timestamp: str = None):
        self.metadata.timestamp = timestamp

    def add_frame_camera(self, model: str = None, resolution_x: int = None, resolution_y: int = None, 
        file_format: str = None, file_prefix: str = None, file_name: str = None, frame_count: str = None):

        self.metadata.frame_cam.model = model
        self.metadata.frame_cam.resolutionX = resolution_x
        self.metadata.frame_cam.resolutionY = resolution_y

        self.metadata.frame_cam.file_format = file_format
        self.metadata.frame_cam.file_prefix = file_prefix
        self.metadata.frame_cam.file_name = file_name
        self.metadata.frame_cam.frame_count = frame_count

    def add_event_camera(self, model: str = None, resolution_x: int = None, resolution_y: int = None, 
        file_format: str = None, file_prefix: str = None, file_name: str = None, length: str = None):
        
        self.metadata.event_cam.model = model
        self.metadata.event_cam.resolutionX = resolution_x
        self.metadata.event_cam.resolutionY = resolution_y

        self.metadata.event_cam.file_format = file_format
        self.metadata.event_cam.file_prefix = file_prefix
        self.metadata.event_cam.file_name = file_name
        self.metadata.event_cam.length = length

    def add_tags(self, tags: list[str] = None):
        self.metadata.tags = tags

    def update_tags(self, tags: list[str] = None):
        self.metadata.tags = self.metadata.tags + tags

    def points_of_interest(self, points_of_interest: dict[int, str] = None):
        self.metadata.points_of_interest = points_of_interest

    def update_points_of_interest(self, points_of_interest: dict[int, str] = None):
        self.metadata.points_of_interest = self.metadata.points_of_interest | points_of_interest

    def return_json(self) -> str:
        return self.metadata.toJSON()

