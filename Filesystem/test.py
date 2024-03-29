import filesystem

def main():
    fs = filesystem.Filesystem()
    fs.list_recordings()

    while True:
        user_input = input("Search options:\n(1) Recording Info\n(2) Tag Search\n(3) Quit\n\n>> ")

        try:
            user_input = int(user_input)
        except ValueError:
            break

        if user_input >= len(fs.recordings):
            print("Invalid option...", end='\n\n')
            continue

        match user_input:
            case 3:
                break
            case 2:
                tag_search(fs)
            case 1:
                recording_info(fs)

        if user_input == 3:
            break

def tag_search(fs: filesystem.Filesystem):
    user_input = input("Enter tags: ")

    tags = user_input.split(" ")

    fs.search_tags(tags)

def recording_info(fs: filesystem.Filesystem):

    print("Recordings: ")
    fs.list_recordings()

    while True:
        user_input = input("Please select recording (or type quit): ")

        if user_input == "quit":
            break

        try:
            user_input = int(user_input)
        except ValueError:
            print("Invalid input detected...")
            break

        if user_input >= len(fs.recordings):
            print("Invalid recording index...")
            break

        fs.print_recording_info(user_input)




if __name__ == '__main__':
    main()