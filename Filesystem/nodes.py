# All nodes have tags
class node:
    name: str
    tags: dict[str, str]

    def __init__ (self, name: str, tags: dict):
        self.name = name
        self.tags = tags

# Files are initiated with type "file"
class file(node):
    type: str = "file"

# Folders are initiated with type "folder". Folders also contain subdirectories of nodes.
class folder(node):
    type: str = "folder"
    

    def __init__ (self, name: str, tags: dict):
        self.name = name
        self.tags = tags