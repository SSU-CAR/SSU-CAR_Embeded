# MOBIS HACKATHON_ Feedback service for driving safely
In the era of autonomous driving levels 2-3, safe driving by the human operator remains essential. Our service evaluates driver habits both externally and internally within the vehicle, providing feedback to enhance safety.
## Embeded part
### Vehicle internal(Desktop part)
Monitor the driver's forward attention by pupil, head pose and phone detection.  
Three evaluation factors - Closed eye detection, Head pose estimation, Phone detection

[Structure]
![image](https://github.com/SSU-CAR/SSU-CAR_Embeded/assets/107911398/3e5b8ff1-3ff0-49f1-a111-0ddbeedbca80)

### Vehicle external(Raspberry part)
Monitor the illegal lane change like changing in solid line and without direction light.  

DrivenDist - Report driving distance and current veloicity.  
Joystick_usb - Control RC car with joystick.  
LaeDetection, lane_ex - Lane detetction with HSV(HSL) filtering.  
[Structure]
![image](https://github.com/SSU-CAR/SSU-CAR_Embeded/assets/107911398/77e2e055-b323-4d55-bb0e-73a4f5f935af)

