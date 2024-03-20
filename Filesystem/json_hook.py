import json
import fs_folder

def object_decoder(obj):
    if obj.__repr__ == 'fs_folder.Folder':
        return fs_folder.Folder(obj['tags'])
    return obj

class object_encoder(json.JSONEncoder):
    def default(self, o):
        # Check if name is filesystem
        _iterate_filesystem(o)
        return o.__dict__
    
def _iterate_filesystem(o):
    return str(o.root_path) + str(o.folders.dict)