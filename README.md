# MOBIS HACKATHON_ Feedback service for driving safely
In the era of autonomous driving levels 2-3, safe driving by the human operator remains essential. Our service evaluates driver habits both externally and internally within the vehicle, providing feedback to enhance safety.
## Embedded part
### Vehicle internal(Desktop part)
#### Framework, language:
<img src="https://img.shields.io/badge/ros-#22314E?style=for-the-badge&logo=ros&logoColor=white">

Monitor the driver's forward attention by pupil, head pose and phone detection.  
Three evaluation factors - Closed eye detection, Head pose estimation, Phone detection  

1. cam2node: Generate ros2 image topic from /dev/video0. This node transfers camera image to the other nodes.
2. closedEyeDetection: By using HSV filter, this node make a decide driver close his eyes whether or not. From dlib's facial key points, it focuses on eye points and detect your pupil.  
4. head_pose_estimation: Using dlib's facial key points, chin(1), mouth(2), nose(1) and eyes(2). Assume each 3D points from intrinsic parameters and calculate its rotation vector from zero point.
5. phone_detection: It's essential yolov3 pretrained model. This detects mobile phone for using yolov3.
6. inner_scenario: This node makes a decide what is drvier's state. It evaluates every 3 seconds and when it detects driver sleeping, it reports to server for using http.  


[Structure]
![image](https://github.com/SSU-CAR/SSU-CAR_Embeded/assets/107911398/3e5b8ff1-3ff0-49f1-a111-0ddbeedbca80)

### Vehicle external(Raspberry part)
#### Framework, language: python, C++, C  
Monitor the illegal lane change like changing in solid line and without direction light.  

DrivenDist - Report driving distance and current veloicity.  
Joystick_usb - Control RC car with joystick.  
LaeDetection, lane_ex - Lane detetction with HSV(HSL) filtering.  

1. Joystick_usb: Left bottom button and right bottom buttons are for LED. So that it is maybe useless for other people. Joystick and moter codes are combined. It can be adjust by correcting sensitivity constants in Joystick code.
2. LaneDetection, lane_ex: ROI - HSV filtered - sliding window. It detects lane within sliding window's area and when it detects, it reports to server for using http. But it didn't test http with server.
3. DrivenDist: Every 6 seconds, it reports vehicle's current velocity and accumulate driven distance. But it didn't test http with server too. 2, 3 codes are essential to verify its http code.

[Structure]
![image](https://github.com/SSU-CAR/SSU-CAR_Embeded/assets/107911398/f5f10145-ed10-4ec9-9722-8f5671cca517)



