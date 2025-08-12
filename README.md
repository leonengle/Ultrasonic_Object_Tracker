# UltrasonicTracker
## Purpose:
To demonstrate that complex tasks such as object localization can be performed using lightweight embedded architecture, in this case the PIC 18F4331. 

## Mechanics: 
4 ultrasonic sensors are oriented at different angles with respect to each other, enabling rangefinding at four discrete points. 2 MG90S servos oriented on different axis enable movement of the tracker through 180 degrees theta and phi. To facilitate integration on a short time frame, 6 separate PIC 18F4331 microcontrollers are used, although in a future version of the system less microcontrollers would do. 

## Manual Mode: 
2 bluetooth transcievers enable wireless communication between the tracker and a joystick, When the manual control switch is set HI, the joystick controls theta and phi movement of the object tracker, as well as the state of a 5mW laser built into the nose of the tracker. 

## Automatic mode: 
When the manual control switch is set LO, the master program enters the automatic mode loop. Initially, the tracker scans for an object by panning through all combinations of theta and phi angles. When one of the ultrasonic sensors detects an object, the tracker switches to locking mode, where it moves in the direction of the object until all 4 ultrasonic sensors are triggered and a lock is achieved. When a lock is achieved, the laser is triggered and an SPI command is sent to an SSD1306 OLED driver to display the theta and phi location of the object. If the object moves, the tracker moves with it to keep all 4 sensors focused on the object. Since the 4 sensors are each angled differently, some spatial understanding of the world around the tracker is possible. 

### Tracking Mode Demo:
<video src="videos/videxample.mov" width="250" controls aria-label="Example video: videxample.mov">
  Your browser does not support the video element. 
  <a href="videos/videxample.mov">Download the video</a>.
</video>
