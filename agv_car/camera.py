import requests
import cv2 as cv
from cv2 import aruco
import numpy as np
import math
import time
import socket
import serial

# arduino = serial.Serial('com6', 9600)

calib_data_path = r"C:\Users\91779\Desktop\AStra\IMAGE PROcesss\calib_data\MultiMatrix.npz"

calib_data = np.load(calib_data_path)
print(calib_data.files)
cam_mat = calib_data["camMatrix"]
dist_coef = calib_data["distCoef"]
r_vectors = calib_data["rVector"]
t_vectors = calib_data["tVector"]

dictionary = cv.aruco.getPredefinedDictionary(cv.aruco.DICT_4X4_100)
parameters = cv.aruco.DetectorParameters()
detector = cv.aruco.ArucoDetector(dictionary, parameters)

MARKER_SIZE = 5

marker_dict = dictionary

param_markers = parameters

# Use the IP camera as the video source
cap = cv.VideoCapture(0)


esp8266_ip = '192.168.30.42'
def send_number(number):
    response = requests.get(f'http://{esp8266_ip}/number?value={number}')
    print(response.text)







while True:
    ret, frame = cap.read()
    if not ret:
        break
    gray_frame = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    marker_corners, marker_IDs, reject = aruco.detectMarkers(
        gray_frame, marker_dict, parameters=param_markers
    )

    if marker_corners:
        rVec, tVec, _ = aruco.estimatePoseSingleMarkers(
            marker_corners, MARKER_SIZE, cam_mat, dist_coef
        )
        total_markers = range(0, marker_IDs.size)

        def ranges(angle):
            return (angle + 180) % 360 - 180

        for i in range(len(marker_IDs)):
            R, _ = cv.Rodrigues(rVec[i])
            roll, pitch, yaw = [math.degrees(x) for x in cv.RQDecomp3x3(R)[0]]

            print(f"Roll: {ranges(roll):.2f} degrees", f"Pitch: {ranges(pitch):.2f} degrees", f"Yaw: {ranges(yaw):.2f} degrees")

        for ids, corners, i in zip(marker_IDs, marker_corners, total_markers):
            cv.polylines(
                frame, [corners.astype(np.int32)], True, (0, 255, 255), 4, cv.LINE_AA
            )
            corners = corners.reshape(4, 2)
            corners = corners.astype(int)
            top_right = corners[0].ravel()
            top_left = corners[1].ravel()
            bottom_right = corners[2].ravel()
            bottom_left = corners[3].ravel()

            distance = np.sqrt(
                tVec[i][0][2] ** 2 + tVec[i][0][0] ** 2 + tVec[i][0][1] ** 2
            )

            send_number(ids[0])
            print(f"id: {ids[0]} Dist: {round(distance*2.54, 2)}")
            print(f"x:{round(tVec[i][0][0], 1)} y: {round(tVec[i][0][1], 1)} ")

    cv.imshow("frame", frame)
    key = cv.waitKey(1)
    if key == ord("q"):
        break
cap.release()
cv.destroyAllWindows()
