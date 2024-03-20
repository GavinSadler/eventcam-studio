import filesystem
import time

def main():
    fs = filesystem.Filesystem()
    
    fs.create_folder("test_folder", {"time": str(time.localtime), "contains_data": "False"})
    fs.create_folder("test_folder_two", {"time": str(time.localtime), "contains_data": "False"})

    fs.list_tags("test_folder")
    fs.list_tags("test_folder_two")

    fs.update_tags("test_folder", {"random_value": "just to test"})

    fs.list_tags("test_folder")

    fs.remove_folder("test_folder")

if __name__ == "__main__":
    main()
