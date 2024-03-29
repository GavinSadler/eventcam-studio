class CameraStruct:
    model: str

    resolutionX: int
    resolutionY: int

    file_format: str
    file_prefix: str
    file_name: str

    def __repr__(self):
        return str(self.__dict__)

class FrameCamera(CameraStruct):
    frame_count: str
    
    def __repr__(self):
        return str(self.__dict__)

class EventCamera(CameraStruct):
    length: str

    def __repr__(self):
        return str(self.__dict__)

class RecordingData:
    name: str
    description: str
    notes: str
    name_of_recorder: str
    location: str

    timestamp: str
    frame_cam: FrameCamera = FrameCamera()

    event_cam: EventCamera = EventCamera()

    tags: list[str]
    points_of_interest: dict[int, str]

    def __repr__(self):
      return str(self.__dict__)

    def toJSON(self) -> str:
      return str(repr(self.__dict__) + repr(self.frame_cam) + repr(self.event_cam))
