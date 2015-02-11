# SocialDocumentary

Social Documentary is an artistic installation to navigate in a collection of crowd-sourced videos. Those videos are related to the events that happened at Gezi Park (Istanbul) in June 2013.
The visitors are invited to interact with the installation by choosing keywords through the manipulation of three cubes. Each cube represents a class of keywords (people, action, emotion). As the videos are cut into segments and tagged with the same keywords, the system will automatically choose the most relevant video according to the position of the cubes. A Kinect-based system allows us to track the two closest visitors’ faces and infer on their interest for the played video. The more the visitors are interested, the higher is the probability to display this video to the next visitors, as recommendation system on video broadcast platforms. In this way, the installation evolves with its public. Some segments will emerge from the others as the "most popular".

## Equipment

The installation requires:
- [MS kinect sensor] (currently, only the first version is supported)
- [reactable] hardware (building one is not too complicated, search on the web for tutorials)

It runs on one or two computers under Windows OS (one for fiducials tracking and reactable display, one for face tracking and video display).

## Installation
### Third party softwares
Face tracking software
- download and install [Kinect SDK 1.8]
- download, install [OpenCV 2.4.8] in "c:\opencv" and add "C:\opencv\build\x86\vc11\bin" to Windows Path

Fiducial tracking
- download and install [reacTIVision 1.4]
- edit reacTIVision.xml with the right IPs
```xml
    <tuio host="127.0.0.1" port="3333" /> <!-- reactable display address -->>
    <tuio host="127.0.0.1" port="3334" /> <!-- player address -->>
```

Player and Reactable display
- download and install [Processing 2.x]
- install Processing libraries [oscP5], [TUIO], [keystone]

### Video calibration
- open and run "ReactableDisplay.pde" on the reactable computer
- press 'c' key
- drag and place the corners of the image with the mouse
- press 's' key to save the video calibration
- press 'c' to go back to the default screen

### camera calibration
This operation requires a second screen connected to the reactable computer
- while "ReactableDisplay.pde" is running, press 'c' key to switch to calibration mode
- run "reacTIVision.exe" and place its windows on the second screen
- press 'c' to switch to camera calibration mode
- adjust each point of the calibration pattern
- press 'c' to save calibration grid

## Launch installation
Be sure the IP addresses are correctly set up in reacTIVision.xml and SocialDoc_Player.pde

On the reactable computer
- run "reacTIVision.exe"
- open and run "ReactableDisplay.pde"

On the player computer
- run "KinterestSocialDoc.exe"
- open and run "SocialDoc_Player.pde"


[MS kinect sensor]:http://www.microsoft.com/en-us/kinectforwindows/default.aspx
[reactable]:http://www.reactable.com/
[Kinect SDK 1.8]:http://www.microsoft.com/en-us/download/details.aspx?id=40278
[OpenCV 2.4.8]:http://opencv.org/opencv-2-4-8.html
[reacTIVision 1.4]:http://sourceforge.net/projects/reactivision/files/reacTIVision/reacTIVision-1.4/
[Processing 2.x]:https://processing.org/download/
[oscP5]:http://www.sojamo.de/libraries/oscP5/
[TUIO]:http://www.tuio.org/?processing
[keystone]:http://keystonep5.sourceforge.net/
