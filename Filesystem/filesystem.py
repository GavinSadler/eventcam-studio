import json
import nodes
import os
import re

class Filesystem:
    root_dir: str = None
    recordings: list[str] = []

    def __init__(self, root_dir=None):
        self._find_filesystem(root_dir)
    
    def _find_filesystem(self, root_dir: str):
        '''Uses specified directory or current directory.'''
        if root_dir == None:
            self.root_dir = os.getcwd()
        else:
            self.root_dir = os.mkdir(root_dir)

        for root, _, files in os.walk(self.root_dir, topdown=False):
            for name in files:
                if name.endswith(".json"):
                    self.recordings.append(root)

    def list_recordings(self):
        print("Available recordings:", end='\n')
        for index, recording_path in enumerate(self.recordings):
            print(f"({index}) {recording_path}", end='\n')

        print("\n")

    def print_recording_info(self, recording_number: int):
        configuration = json.loads(self._read_config(recording_number))
        print(
            f"Name: {configuration['name']}\nDescription: {configuration['description']}\nNotes: {configuration['notes']}\nRecorder: {configuration['recorder']}",
            end='\n\n'
        )

    def load_tags(self, recording_number: int) -> list[str]:
        configuration = self._read_config(recording_number)

        values = re.findall(":(.+),", configuration)

        return str(values)
    
    def search_tags(self, tags: list[str]):
        for index, _ in enumerate(self.recordings):
            tags_found = True
            local_tags = self.load_tags(index)
            for tag_name in tags:
                if not (tag_name in local_tags):
                    tags_found = False
                    #print(f"Tag: '{tag_name}' found in recording ({index})", end='\n')
            if tags_found:
                print(f"Tags found in recording ({index})", end='\n')
            

        print("\n")

    def _read_config(self, recording_number: int) -> str:
        with open(self.recordings[recording_number] + "/md.json", "r") as file:
            configuration = file.read()

        return configuration
