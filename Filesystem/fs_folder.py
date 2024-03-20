class Folder:
    "Individual folder information"
    tags: dict = {}

    def __init__ (self, tags: dict):
        self.tags = tags
