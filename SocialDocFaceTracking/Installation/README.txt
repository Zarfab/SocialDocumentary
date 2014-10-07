Installation instructions for KinterestTV

1. Go to Dependences\KinectSDK
	1.1 Run KinectSDK-v1.8-Setup.exe
	1.2 Once installed, run KinectDeveloperToolkit-v1.8.0-Setup.exe
 
2. Go to Dependances\opencv
	2.1. Run opencv-2.4.8.exe
	2.2. Select C:\ as the installation directory
	2.3. Run PathEditor and add "C:\opencv\build\x86\vc12\bin" to the path
	2.4. Restart the computer
	
3. Connect the Kinect sensor to a USB port on the computer and plug the power supply
	
4. Go to Bin\
	4.1 Run DemoKISD.bat
	4.2 If the program crashes with a message "msvcXX.dll not found", you can copy it from Dependances\msvc_dlls\yourPlatform to the Bin directory
	
	
Settings:
- You can change the OSC sender address and listener ports in DemoKISD.bat.
 For example, if you want to send data to a computer which local IP is 192.168.1.52 on port 3333, 
 you can add "-oscclient2 192.168.1.52:3333" in argument (osclistener just need the port to listen)
 
- if you need to load another screen configuration XML file, you can change it with the argument "-screens yourFile.xml"

- in ScreenConfig.xml, you can change the screen width and height and the position of the Kinect camera relatively to the center of the screen. All values are in meters. Angles are not used yet.
For example, if the screen is 1.10m x 0.80m and the camera is placed just below the screen, aligned with its center, the XML file should be:

<RoomConfig>
   <Screen>
		<id>1</id>
		<width>1.1</width>
		<height>0.8</height>
		<resolutionX>1980</resolutionX>
		<resolutionY>1080</resolutionY>
		<CamPosition>
			<centerx>0</centerx>
			<centery>-0.4</centery>
			<centerz>0</centerz>
			<angleX>0</angleX>
			<angleY>0</angleY>
			<angleZ>0</angleZ>
		</CamPosition>
   </Screen>
</RoomConfig>
