# Sources:
# https://docs.opencv.org/3.3.0/dc/dbb/tutorial_py_calibration.html
import numpy as np
import cv2 as cv
import glob

class Calibration:
    __objpoints: list = [] # 3d point in real world space
    __imgpoints: list = [] # 2d points in image plane.
    __objp: np.float32 = None
    __criteria: tuple[any, int, float] = (None, None, None)
    __gray: cv.Mat = None

    def __init__ (self):
        '''Initialize data to be filled by calibration calculation.'''
        # termination criteria
        self.__criteria = (cv.TERM_CRITERIA_EPS + cv.TERM_CRITERIA_MAX_ITER, 30, 0.001)

        # prepare object points, like (0,0,0), (1,0,0), (2,0,0) ....,(6,5,0)
        self.__objp = np.zeros((6*7,3), np.float32)
        self.__objp[:,:2] = np.mgrid[0:7,0:6].T.reshape(-1,2)

    def load_images(self, images_path: str, image_extension: str) -> list[glob.glob]:
        '''Finds image paths to be loaded for calibration.'''
        image_expression: str = images_path + "/" + '*.' + image_extension

        images: list = glob.glob(image_expression)

        return images

    def calculate_fisheye(self, images: list[glob.glob], debug_on: bool):
        '''Calculates the fisheye of the camera based on the image paths sent to it. If debug is on then images are printed as they are processed.'''

        for fname in images:
            img = cv.imread(fname)
            self.__gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)

            # Find the chess board corners
            ret, corners = cv.findChessboardCorners(self.__gray, (7,6), None)

            # If found, add object points, image points (after refining them)
            if ret == True:
                self.__objpoints.append(self.__objp)
                corners2 = cv.cornerSubPix(self.__gray,corners, (11,11), (-1,-1), self.__criteria)
                self.__imgpoints.append(corners)

                if debug_on:
                    # Draw and display the corners
                    cv.drawChessboardCorners(img, (7,6), corners2, ret)
                    cv.imshow('img', img)
                    cv.waitKey(1)
    
    def undistort_image(self, img) -> any:

        ret, mtx, dist, rvecs, tvecs = cv.calibrateCamera(self.__objpoints, self.__imgpoints, self.__gray.shape[::-1], None, None)

        h, w = img.shape[:2]

        newcameramtx, roi = cv.getOptimalNewCameraMatrix(mtx, dist, (w, h), 1, (w, h))

        dst = cv.undistort(img, mtx, dist, None, newcameramtx)
        x, y, w, h = roi
        dst = dst[y:y+h, x:x+w]
        
        return dst

    def __del__ (self):
        cv.destroyAllWindows()