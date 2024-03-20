import os
import json
import fs_folder
import json_hook

class Filesystem:
    '''Creates and manages a simple filesystem'''
    root_dir: str = None
    folders: dict[str, dict] = {}

    def __init__ (self, root_dir: str = None):
        '''Initializes filesystem. If root_dir is not a real directory then the 
        folder is created in the working directory.'''
        if os.path.exists("fs.json"):
            self._load_previous_filesystem()
        else:
            self._create_new_filesystem(root_dir)

        print("Root: " + str(self.root_dir), end='\n')
        print("Folders: " + str(self.folders))

    def create_folder(self, name: str, tags: dict) -> bool:
        '''Creates a folder in the filesystem if not already present. Returns 
        'false' if folder name is already being used.'''

        if name in self.folders.keys():
            return False
        
        os.mkdir(self.root_dir + "/" + name)

        new_folder = fs_folder.Folder(tags)

        self.folders[name] = new_folder
        self._save_fs()


    def remove_folder(self, name):
        '''Removes the folder name specified.'''
        if not name in self.folders.keys():
            return
        
        os.rmdir(self.root_dir + "/" + name)

        self.folders.pop(name)
        self._save_fs

    def update_tags(self, name, tags: dict):
        '''Updates the current tags for a folder.'''
        if not name in self.folders.keys():
            return

        self.folders[name].tags.update(tags)

    def list_tags(self, name: str):
        if not name in self.folders.keys():
            return
        
        print("Tags for " + name + ":\n" + str(self.folders[name].tags))


    def _load_previous_filesystem(self):
        '''Unmarshalls filesystem saved to disk and initializes class'''
        with open("fs.json", "r") as file:
            json_object = json.loads(file.read(), object_hook=json_hook.object_decoder)
            self.root_dir = json_object['root_dir']
        #    json_object = json.loads(file.read())

        #    for variable_name in json_object.keys():
        #        if variable_name == "root_dir":
        #            self.root_dir = json_object[variable_name]
        #        else:
        #            folder_list: dict = json_object[variable_name]
        #            for folder_name in folder_list:
        #                self.folders[folder_name].tags = folder_list[folder_name]
        #                print(self.folders[folder_name], end="\n\n")
            

    def _create_new_filesystem(self, root_dir: str):
        '''Uses specified directory or current directory.'''
        if root_dir == None or root_dir == "":
            self.root_dir = os.getcwd()
        else:
            self.root_dir = os.mkdir(root_dir)

    def _save_fs(self):
        '''Saves the filesystem to json.'''
        with open(self.root_dir + "/fs.json", "w") as file:
            encoded_fs = json.dumps(self, cls=json_hook.object_encoder)
            file.write(encoded_fs)

            #root_dir = {"root_dir", self.root_dir}
            #for key in self.folders.keys():


            #saved_data: str = "{\"" + "root_dir" + "\":\"" + str(self.root_dir) + "\", \"folders\": {"
            #for key in self.folders.keys():
            #    saved_data = saved_data + "\"" + key + "\": " + str(self.folders[key].tags) + ","

            #saved_data = saved_data[:-1] + "}}"

            #file.write(saved_data.replace("'", "\""))