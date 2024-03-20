import json
import fs_folder
import filesystem

def object_decoder(obj):
    if type(obj) == fs_folder.Folder:
        return fs_folder.Folder(obj['tags'])
    return obj
    
def iterate_filesystem(o):
    result = "{" + _create_tag("root_dir", str(o.root_dir))

    for key in o.folders.keys():
        result += _create_tag(key, o.folders[key].tags)

    result = result[:-1] + "}"

    return result

# Creates a tag for json ending in a comma (,).
def _create_tag(key: str, value: any) -> str:
    if type(value) != str:
        return "\"" + key + "\": " + str(value).replace("\'", "\"") + ","
    else:
        return "\"" + key + "\": \"" + value + "\","