import unfisheye as uf

def main():

    VIDEO_NAME: str = r"stop"
    IN_PATH: str = r"Video Files/" + VIDEO_NAME + r".mp4"
    OUT_PATH: str = VIDEO_NAME + r"_unfisheyed.avi"

    unfisheye = uf.Unfisheye(out_file_name=OUT_PATH, in_file_name=IN_PATH, 
        calibration_image_directory=r"/home/zeph/Desktop/Programming_Projects/Python/RemoveDistortion/Calibration", completion_bar_on=True)
    
    unfisheye.remove_distortion()

    del unfisheye

if __name__ == "__main__":
    main()