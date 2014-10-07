// ****************************************************************************************
//
// Fubi API
// ---------------------------------------------------------
// Copyright (C) 2010-2013 Felix Kistler 
// 
// This software is distributed under the terms of the Eclipse Public License v1.0.
// A copy of the license may be obtained at: http://www.eclipse.org/org/documents/epl-v10.html
// 
// ****************************************************************************************

#pragma once
#ifndef FUBI_H
#define FUBI_H

/** \file Fubi.h 
 * \brief The Fubi C++ API
*/ 

#if defined ( WIN32 ) || defined( _WINDOWS )
#	 ifdef FUBI_EXPORTS
#       define FUBI_API __declspec( dllexport )
#	 else
#       define FUBI_API __declspec( dllimport )
#    endif
#else
#	 define FUBI_API
#endif


#include <set>
#include <vector>

#include "FubiUser.h"
#include "FubiHand.h"

/**
* \mainpage Fubi - Full Body Interaction Framework
* 
* \section intro_sec Introduction
* 
* Full Body Interaction Framework (FUBI) is a framework for recognizing full body gestures and postures in real time from the data of a depth sensor compatible with OpenNI (1.x or 2.x) or the Kinect SDK.
* It further supports finger tracking using a Leap motion sensor or injecting formated tracking data of an own sensor
*/


/**
* \namespace Fubi
*
* \brief The Fubi namespace provides all methods to control the Full Body Interaction framework (FUBI).
*
*/
namespace Fubi
{
	/** \addtogroup FUBICPP FUBI C++ API
	* All the C++ API functions
	* 
	* @{
	*/

	/**
	* \brief Initializes Fubi with OpenN 1.x using the given xml file and sets the skeleton profile.
	*        If no xml file is given, Fubi will be intialized without OpenNI tracking enabled --> methods that need an openni context won't work.
	* 
	* @param openniXmlconfig name of the xml file for OpenNI 1.x initialization inlcuding all needed productions nodes 
	(should be placed in the working directory, i.e. "bin" folder)
	if config == 0x0, then OpenNI won't be initialized and Fubi stays in non-tracking mode
	* @param profile set the openNI skeleton profile
	* @param filterMinCutOffFrequency, filterVelocityCutOffFrequency, filterCutOffSlope options for filtering the tracking data if wanted
	* @param mirrorStreams whether the stream should be mirrored or not
	* @param registerStreams whether the depth stream should be registered to the color stream
	* @return true if succesfully initialized or already initialized before,
	false means bad xml file or other serious problem with OpenNI initialization
	*/
	FUBI_API bool init(const char* openniXmlconfig = 0x0, Fubi::SkeletonTrackingProfile::Profile profile = Fubi::SkeletonTrackingProfile::ALL,
		float filterMinCutOffFrequency = 1.0f, float filterVelocityCutOffFrequency = 1.0f, float filterCutOffSlope = 0.007f,
		bool mirrorStreams = true, bool registerStreams =true);


	/**
	* \brief Initializes Fubi with an options file for the sensor init
	* 
	* @param sensorOptions configuration for the sensor
	* @param filterOptions filter options for additionally filtering of the tracking data
	* @return true if succesfully initialized or already initialized before,
	false means problem with sensor init
	*/
	FUBI_API bool init(const SensorOptions& sensorOptions, const FilterOptions& filterOptions = FilterOptions());

	/**
	* \brief Initializes Fubi with specific options for the sensor init
	* 
	* @param depthWidth, depthHeight, depthFPS, rgbWidth, rgbHeight, rgbFPS,
	irWidth, irHeight, irFPS, sensorType, profile, filterMinCutOffFrequency, filterVelocityCutOffFrequency, filterCutOffSlope,
	mirrorStream, registerStreams configuration for the sensor as in the SensorOptions and FilterOptions struct
	* @return true if succesfully initialized or already initialized before,
	false means problem with sensor init
	*/
	FUBI_API bool init(int depthWidth, int depthHeight, int depthFPS = 30,
		int rgbWidth = 640, int rgbHeight = 480, int rgbFPS = 30,
		int irWidth = -1, int irHeight = -1, int irFPS = -1,
		Fubi::SensorType::Type sensorType = Fubi::SensorType::OPENNI2,
		Fubi::SkeletonTrackingProfile::Profile profile = Fubi::SkeletonTrackingProfile::ALL,
		float filterMinCutOffFrequency = 1.0f, float filterVelocityCutOffFrequency = 1.0f, float filterCutOffSlope = 0.007f,
		bool mirrorStream = true, bool registerStreams = true);

	/**
	* \brief Allows you to switch between different sensor types during runtime
	*		  Note that this will also reinitialize most parts of Fubi
	* 
	* @param options options for initializing the new sensor
	* @return true if the sensor has been succesfully initialized
	*/
	FUBI_API bool switchSensor(const SensorOptions& options);

	/**
	* \brief Allows you to switch between different sensor types during runtime
	*		  Note that this will also reinitialize most parts of Fubi
	* 
	* @param sensorType the sensor type to switch to
	* @param sensorType, depthWidth, depthHeight, depthFPS, rgbWidth, rgbHeight, rgbFPS,
	irWidth, irHeight, irFPS, profile, mirrorStream, registerStreams
	configuration for the sensor as in the SensorOptions struct
	* @return true if the sensor has been succesfully initialized
	*/
	FUBI_API bool switchSensor(Fubi::SensorType::Type sensorType, int depthWidth, int depthHeight, int depthFPS = 30,
		int rgbWidth = 640, int rgbHeight = 480, int rgbFPS = 30,
		int irWidth = -1, int irHeight = -1, int irFPS = -1,
		Fubi::SkeletonTrackingProfile::Profile profile = Fubi::SkeletonTrackingProfile::ALL,
		bool mirrorStream = true, bool registerStreams =true);


	/**
	* \brief Get the currently available sensor types (defined in FubiConfig.h before compilation)
	* 
	* @return an int composed of the currently available sensor types (see SensorType enum for the meaning)
	*/
	FUBI_API int getAvailableSensorTypes();

	/**
	* \brief Get the type of the currently active sensor
	* 
	* @return the current sensor type
	*/
	FUBI_API Fubi::SensorType::Type getCurrentSensorType();


	/**
	* \brief Shuts down OpenNI and the tracker, releasing all allocated memory
	* 
	*/
	FUBI_API void release();

	/**
	* \brief Returns true if OpenNI has been already initialized
	* 
	*/
	FUBI_API bool isInitialized();


	/**
	* \brief Updates the sensor to get the next frame of depth, rgb, and tracking data.
	*        Also searches for users in the scene and loads the default tracking calibration for new users or request a calibration
	* 
	*/
	FUBI_API void updateSensor();

	/**
	* \brief retrieve an image from one of the OpenNI production nodes with specific format and optionally enhanced by different
	*        tracking information 
	*		  Some render options require an OpenCV installation!
	*
	* @param outputImage [out] pointer to a unsigned char array
	*        Will be filled with wanted image
	*		  Array has to be of correct size, e.g. depth image (640x480 std resolution) with tracking info
	*		  requires 4 channels (RGBA) --> size = 640*480*4 = 1228800
	* @param type can be color, depth, or ir image
	* @param numChannels number channels in the image 1, 3 or 4
	* @param depth the pixel depth of the image, 8 bit (standard) or 16 bit (mainly usefull for depth images
	* @param renderOptions options for rendering additional informations into the image (e.g. tracking skeleton) or swapping the r and b channel
	* @param jointsToRender defines for which of the joints the trackinginfo (see renderOptions) should be rendererd
	* @param depthModifications options for transforming the depth image to a more visible format
	* @param userId If set to something else than 0 an image will be cut cropped around (the joint of interest of) this user, if 0 the whole image is put out.
	* @param jointOfInterest the joint of the user the image is cropped around and a threshold on the depth values is applied.
	If set to num_joints fubi tries to crop out the whole user.
	* @param moveCroppedToUpperLeft moves the cropped image to the upper left corner
	*/
	FUBI_API bool getImage(unsigned char* outputImage, ImageType::Type type, ImageNumChannels::Channel numChannels, ImageDepth::Depth depth, 
		int renderOptions = (RenderOptions::Shapes | RenderOptions::Skeletons | RenderOptions::UserCaptions),
		int jointsToRender = RenderOptions::ALL_JOINTS,
		DepthImageModification::Modification depthModifications = DepthImageModification::UseHistogram,
		unsigned int userId = 0, Fubi::SkeletonJoint::Joint jointOfInterest = Fubi::SkeletonJoint::NUM_JOINTS, bool moveCroppedToUpperLeft = false);

	/**
	* \brief save an image from one of the OpenNI production nodes with specific format and optionally enhanced by different
	*        tracking information
	*
	* @param fileName filename where the image should be saved to
	*        can be relative to the working directory (bin folder) or absolute
	*		  the file extension determins the file format (should be jpg)
	* @param jpegQuality qualitiy (= 88) of the jpeg compression if a jpg file is requested, ranges from 0 to 100 (best quality)
	* @param type can be color, depth, or ir image
	* @param numChannels number channels in the image 1, 3 or 4
	* @param depth the pixel depth of the image, 8 bit (standard) or 16 bit (mainly usefull for depth images
	* @param renderOptions options for rendering additional informations into the image (e.g. tracking skeleton) or swapping the r and b channel
	* @param jointsToRender defines for which of the joints the trackinginfo (see renderOptions) should be rendererd
	* @param depthModifications options for transforming the depht image to a more visible format
	* @param userId If set to something else than 0 an image will be cut cropped around (the joint of interest of) this user, if 0 the whole image is put out.
	* @param jointOfInterest the joint of the user the image is cropped around and a threshold on the depth values is applied.
	If set to num_joints fubi tries to crop out the whole user.
	*/
	FUBI_API bool saveImage(const char* fileName, int jpegQuality, ImageType::Type type, ImageNumChannels::Channel numChannels, ImageDepth::Depth depth,
		int renderOptions = (RenderOptions::Shapes | RenderOptions::Skeletons | RenderOptions::UserCaptions),
		int jointsToRender = RenderOptions::ALL_JOINTS,
		DepthImageModification::Modification depthModifications = DepthImageModification::UseHistogram,
		unsigned int userId = 0, Fubi::SkeletonJoint::Joint jointOfInterest = Fubi::SkeletonJoint::NUM_JOINTS);

	/**
	* \brief Returns the color for a user in the background image
	* 
	* @param id OpennNI user id of the user of interest
	* @param r, g, b [out] returns the red, green, and blue components of the color in which the users shape is displayed in the tracking image
	*/
	FUBI_API void getColorForUserID(unsigned int id, float& r, float& g, float& b);


	/**
	* \brief Returns user id from the user index
	* 
	* @param index index of the user in the user array
	* @return id of that user or 0 if not found
	*/
	FUBI_API unsigned int getUserID(unsigned int index);

	/**
	* \brief Creates a user defined finger count recognizer
	* 
	* @param handJoint the hand joint of interest
	* @param minFingers the minimum number of fingers the user should show up
	* @param maxFingers the maximum number of fingers the user should show up
	* @param atIndex (= -1) if an index is given, the corresponding recognizer will be replaced instead of creating a new one
	* @param name (= 0) sets a name for the recognizer (should be unique!)
	* @param minConfidence (=-1) if given this is the mimimum confidence required from tracking for the recognition to be succesful
	* @param useMedianCalculation (=false) if true, the median for the finger count will be calculated over several frames instead of always taking the current detection
	* @param useFilteredData (=false) if true, the recognizer will use the filtered tracking data instead of the raw one
	*
	* @return index of the recognizer needed to call it later
	*/
	FUBI_API unsigned int addFingerCountRecognizer(Fubi::SkeletonJoint::Joint handJoint,
		unsigned int minFingers, unsigned int maxFingers,
		int atIndex = -1,
		const char* name = 0,
		float minConfidence =-1,
		bool useMedianCalculation = false,
		bool useFilteredData = false);

	/**
	* \brief Returns all current users with their tracking information
	* 
	* @param pUserContainer (=0) pointer where a pointer to the current users will be stored at
	*        The maximal size is Fubi::MaxUsers, but the current size can be requested by leaving the Pointer at 0
	* @return the current number of users (= valid users in the container)
	*/
	FUBI_API unsigned short getCurrentUsers(FubiUser*** pUserContainer = 0);

	/**
	* \brief Returns one user with his tracking information
	* 
	* @param id OpenNI id of the user
	* @return a pointer to the user data
	*/
	FUBI_API FubiUser* getUser(unsigned int id);


	/**
	* \brief Returns the current depth resolution or -1, -1 if failed
	* 
	* @param width, height [out] the resolution
	*/
	FUBI_API void getDepthResolution(int& width, int& height);

	/**
	* \brief Returns the current rgb resolution or -1, -1 if failed
	* 
	* @param width, height [out] the resolution
	*/
	FUBI_API void getRgbResolution(int& width, int& height);

	/**
	* \brief Returns the current ir resolution or -1, -1 if failed
	* 
	* @param width, height [out] the resolution
	*/
	FUBI_API void getIRResolution(int& width, int& height);

	/**
	* \brief Returns the number of shown fingers detected at the hand of one user (REQUIRES OPENCV!)
	* 
	* @param userID OpenNI id of the user
	* @param leftHand looks at the left instead of the right hand
	* @param getMedianOfLastFrames uses the precalculated median of finger counts of the last frames (still calculates new one if there is no precalculation)
	* @param useOldConvexityDefectMethod if true using old method that calculates the convexity defects
	* @return the number of shown fingers detected, 0 if there are none or there is an error
	*/
	FUBI_API int getFingerCount(unsigned int userID, bool leftHand = false, bool getMedianOfLastFrames = true, bool useOldConvexityDefectMethod = false);
	
	/**
	* \brief Returns true if the left hand is closer to the sensor
	*
	* @param userID OpenNI id of the user
	*/
	FUBI_API bool leftHandCloser(unsigned int userID);

	/**
	* \brief Returns the number of shown fingers detected at the hand of one user (REQUIRES OPENCV!)
	* 
	* @param userID OpenNI id of the user
	* @param getMedianOfLastFrames uses the precalculated median of finger counts of the last frames (still calculates new one if there is no precalculation)
	* @param useOldConvexityDefectMethod if true using old method that calculates the convexity defects
	* @return the number of shown fingers detected, 0 if there are none or there is an error
	*/
	FUBI_API int getFingerCountOnClosestHand(unsigned int userID, bool getMedianOfLastFrames = true, bool useOldConvexityDefectMethod = false);

	/**
	* \brief Returns true if the hand is at least minDistance closer to the sensor than the torso
	*
	* @param userID OpenNI id of the user
	* @param leftHand true for leftHand
	* @param minDistance distance in mm
	* @return true if the hand is at least minDistance closer to the sensor than the torso
	*/
	FUBI_API bool userHandInFront(unsigned int userID, bool leftHand = false, int minDistance = 400);

	/**
	* \brief push a name in the deque of the user
	*
	* @param userID OpenNI id of the user
	* @param name string containing the name
	*/
	FUBI_API void pushUserName(unsigned int userID, std::string name);
	
	/**
	* \brief Returns the name of a user
	*
	* @param userID OpenNI id of the user
	*/
	FUBI_API std::string getUserName(unsigned int userID);

	/**
	* \brief Returns a vector of 3 values representing the distances between the sensor and the head of a user in meters
	*
	* @param userID OpenNI id of the user
	*/
	FUBI_API std::vector<float> getUserHeadCenter(unsigned int userID);

	/**
	* \brief Returns a vector of 3 values representing the rotations of the head of a user in degrees
	*
	* @param userID OpenNI id of the user
	*/
	FUBI_API std::vector<float> getUserHeadRotation(unsigned int userID);

	/**
	* \brief Returns a vector of 6 values representing the animation units of a user in degrees
	*
	* @param userID OpenNI id of the user
	*/
	FUBI_API std::vector<float> getUserAnimationUnits(unsigned int userID);

	/**
	* \brief Returns a vector of 11 values representing the shape units of a user in degrees
	*
	* @param userID OpenNI id of the user
	*/
	FUBI_API std::vector<float> getUserShapeUnits(unsigned int userID);

	/**
	* \brief Update the screen currently watched by a certain user
	*
	* @param userID OpenNI id of the user
	* @param screen screen's number in the installation
	*/
	FUBI_API void updateUserScreenWatched(unsigned int userID, int screen);

	/**
	* \brief Enables/Disables finger tracking for the hands of one user
	*        If enabled the finger count will be tracked over time and the 
	*		  median of these value will be returned in case of a query
	*		  (REQUIRES OPENCV!)
	* 
	* @param userID OpenNI id of the user
	* @param leftHand enable/disable finger tracking for the left hand
	* @param rightHand enable/disable finger tracking for the right hand
	* @param useConvexityDefectMethod use the old method for extracting the users hand out of the depth image (not recommended)
	*/
	FUBI_API void enableFingerTracking(unsigned int userID, bool leftHand, bool rightHand, bool useConvexityDefectMethod = false);


	/**
	* \brief  Whether the user is currently seen in the depth image
	*
	* @param userID OpenNI id of the user
	*/
	FUBI_API bool isUserInScene(unsigned int userID);

	/**
	* \brief Whether the user is currently tracked
	*
	* @param userID OpenNI id of the user
	*/
	FUBI_API bool isUserTracked(unsigned int userID);


	/**
	* \brief Get the most current tracking info of the user
	* (including all joint positions and orientations (local and global) and a timestamp)
	*
	* @param userId id of the user
	* @param filteredData if true the returned data will be data smoothed by the filter configured in the sensor
	* @return the user tracking info struct
	*/
	FUBI_API FubiUser::TrackingData* getCurrentTrackingData(unsigned int userId, bool filteredData = false);

	/**
	* \brief Get the last tracking info of the user (one frame before the current one)
	* (including all joint positions and orientations (local and global) and a timestamp)
	*
	* @param userId id of the user
	* @param filteredData if true the returned data will be data smoothed by the filter configured in the sensor
	* @return the user tracking info struct
	*/
	FUBI_API FubiUser::TrackingData* getLastTrackingData(unsigned int userId, bool filteredData = false);


	/**
	* \brief  Get the skeleton joint position out of the tracking info
	*
	* @param trackingInfo the trackinginfo struct to extract the info from
	* @param joint the considered joint id 
	* @param x, y, z [out] where the position of the joint will be copied to
	* @param confidence [out] where the confidence for this position will be copied to
	* @param timeStamp [out] where the timestamp of this tracking info will be copied to (seconds since program start)
	* @param localPosition if set to true, the function will return local position (vector from parent joint)
	*/
	FUBI_API void getSkeletonJointPosition(FubiUser::TrackingData* trackingInfo, SkeletonJoint::Joint joint, 
		float& x, float& y, float& z, float& confidence, double& timeStamp, bool localPosition = false);

	/**
	* \brief  Get the skeleton joint orientation out of the tracking info
	*
	* @param trackingInfo the trackinginfo struct to extract the info from
	* @param joint the considered joint id 
	* @param mat [out] rotation 3x3 matrix (9 floats)
	* @param confidence [out] the confidence for this position
	* @param timeStamp [out] (seconds since program start)
	* @param localOrientation if set to true, the function will local orientations (cleared of parent orientation) instead of globals
	*/
	FUBI_API void getSkeletonJointOrientation(FubiUser::TrackingData* trackingInfo, SkeletonJoint::Joint joint,
		float* mat, float confidence, double& timeStamp, bool localOrientation = true);

	/**
	* \brief  Creates an empty vector of UserTrackinginfo structs
	*
	*/
	FUBI_API std::vector<FubiUser::TrackingData>* createTrackingDataVector();

	/**
	* \brief  Releases the formerly created vector
	*
	* @param vec the vector that will be released
	*/
	FUBI_API void releaseTrackingDataVector(std::vector<FubiUser::TrackingData>* vec);

	/**
	* \brief  Returns the size of the vector
	*
	* @param vec the vector that we get the size of
	*/
	FUBI_API unsigned int getTrackingDataVectorSize(std::vector<FubiUser::TrackingData>* vec);
	/**
	* \brief  Returns one element of the tracking info vector
	*
	* @param vec the vector that we get the element of
	* @param index element index with the tracking data vector
	*/
	FUBI_API FubiUser::TrackingData* getTrackingData(std::vector<FubiUser::TrackingData>* vec, unsigned int index);

	/**
	* \brief Returns the OpenNI id of the user standing closest to the sensor (x-z plane)
	*/
	FUBI_API unsigned int getClosestUserID();

	/**
	* \brief Returns the ids of all users order by their distance to the sensor (x-z plane)
	* Closest user is at the front, user with largest distance or untracked users at the back
	* @param maxNumUsers if greater than -1, the given number of closest users is additionally ordered from left to right position
	* @return a deque including the user ids
	*/
	FUBI_API std::deque<unsigned int> getClosestUserIDs(int maxNumUsers = -1);

	/**
	* \brief Returns the ids of all users order by their distance to the sensor (x-z plane)
	* Closest user is at the front, user with largest distance or untracked users at the back
	* @param userIds an array big enough to receive the indicated number of user ids (Fubi::MaxUsers at max)
	* @param maxNumUsers if greater than -1, the given number of closest users is additionally ordered from left to right position
	* @return the actual number of user ids written into the array
	*/
	FUBI_API unsigned int getClosestUserIDs(unsigned int* userIds, int maxNumUsers = -1);

	/**
	* \brief Returns the user standing closest to the sensor (x-z plane)
	*/
	FUBI_API FubiUser* getClosestUser();

	/**
	* \brief Returns all users order by their distance to the sensor (x-z plane)
	* Closest user is at the front, user with largest distance or untracked users at the back
	* If maxNumUsers is given, the given number of closest users is additionally ordered from left to right position
	*/
	FUBI_API std::deque<FubiUser*> getClosestUsers(int maxNumUsers = -1);

	/**
	* \brief Stops and removes all user defined recognizers
	*/
	FUBI_API void clearUserDefinedRecognizers();

	/**
	* \brief Set the current tracking info of one user
	* (including all joint positions. Optionally the orientations and a timestamp)
	*
	* @param userId id of the user
	* @param positions an array of the joint positions
	* @param timeStamp the timestamp of the tracking value (if -1 an own timestamp will be created)
	* @param orientations an array of the joint positions (if 0, the orientations will be approximated from the given positions)
	* @param accelerations an array of joint accelerations (optional)
	*/
	FUBI_API void updateTrackingData(unsigned int userId, Fubi::SkeletonJointPosition* positions,
		double timeStamp = -1, Fubi::SkeletonJointOrientation* orientations = 0, Fubi::SkeletonJointAcceleration* accelerations = 0);
	/**
	* \brief same function as before, but skeleton and acceleartion as a plain float array 
	*
	* @param userId id of the user
	* @param skeleton i.e. NUM_JOINTS * (position+orientation) with position, orientation all as 4 floats (x,y,z,conf) in milimeters or degrees
	* @param timeStamp in seconds or -1 for self calculation
	* @param accelerations i.e. NUM_JOINTS * (acceleartion) with accelerations ass 4 floats (x,y,z,conf) in milimeters / second²
	*/
	FUBI_API void updateTrackingData(unsigned int userId, float* skeleton, double timeStamp = -1, float* accelerations = 0);

	/**
	* \brief Calculate from real world coordinates (milimeters) to screen coordinates (pixels in the depth/rgb/ir image).
	* Uses the data of a present sensor, or alternatively calculates the projection according to given sensor values, or to standard values
	*
	* @param realWorldVec vector with real world coordinates (in milimeters)
	* @param xRes x resolution of the screen (depth/rgb/ir image)
	* @param yRes y resolution of the screen (depth/rgb/ir image)
	* @param hFOV the sensors horizontal field of view 
	* @param vFOV the sensors vertical field of view 
	* @return vector with screen coordinates (pixels in the depth/rgb/ir image)
	*/
	FUBI_API Fubi::Vec3f realWorldToProjective(const Fubi::Vec3f& realWorldVec, int xRes = 640, int yRes = 480,
		double hFOV = 1.0144686707507438, double vFOV = 0.78980943449644714);

	/**
	* \brief Calculate from real world coordinates (milimeters) to screen coordinates (pixels in the depth/rgb/ir image).
	* Uses the data of a present sensor, or alternatively calculates the projection according to given sensor values, or to standard values
	*
	* @param realWorldX, realWorldY, realWorldZ vector with real world coordinates (in milimeters)
	* @param screenX, screenY, screenZ [out] vector with screen coordinates (pixels in the depth/rgb/ir image)
	* @param xRes x resolution of the screen (depth/rgb/ir image)
	* @param yRes y resolution of the screen (depth/rgb/ir image)
	* @param hFOV the sensors horizontal field of view 
	* @param vFOV the sensors vertical field of view 
	*/
	FUBI_API void realWorldToProjective(float realWorldX, float realWorldY, float realWorldZ, float& screenX, float& screenY, float& screenZ,
		int xRes = 640, int yRes = 480,	double hFOV = 1.0144686707507438, double vFOV = 0.78980943449644714);

	/**
	* \brief resests the tracking of all users
	*/
	FUBI_API void resetTracking();

	/**
	* \brief get time since program start in seconds
	*/
	FUBI_API double getCurrentTime();

	/**
	* \brief set the filtering options for smoothing the skeleton according to the 1€ filter (still possible to get the unfiltered data)
	*
	* @param minCutOffFrequency (=1.0f) the minimum cutoff frequency for low pass filtering (=cut off frequency for a still joint)
	* @param velocityCutOffFrequency (=1.0f) the cutoff frequency for low pass filtering the velocity
	* @param cutOffSlope (=0.007f) how fast a higher velocity will higher the cut off frequency (->apply less smoothing with higher velocities)
	*/
	FUBI_API void setFilterOptions(float minCutOffFrequency = 1.0f, float velocityCutOffFrequency = 1.0f, float cutOffSlope = 0.007f);

	/**
	* \brief get the filtering options for smoothing the skeleton according to the 1€ filter (still possible to get the unfiltered data)
	*
	* @param minCutOffFrequency [out] the minimum cutoff frequency for low pass filtering (=cut off frequency for a still joint)
	* @param velocityCutOffFrequency [out] the cutoff frequency for low pass filtering the velocity
	* @param cutOffSlope [out] how fast a higher velocity will higher the cut off frequency (->apply less smoothing with higher velocities)
	*/
	FUBI_API void getFilterOptions(float& minCutOffFrequency, float& velocityCutOffFrequency, float& cutOffSlope);

	/**
	* \brief initalizes a finger sensor such as the leap motion for tracking fingers
	* 
	* @param type the sensor type (see FingerSensorType definition)
	* @param offsetPosX, offsetPosY, offsetPosZ position of the finger sensor in relation to a second sensor (e.g. the Kinect) to align the coordinate systems
	* @return true if successful initialized
	*/
	FUBI_API bool initFingerSensor(Fubi::FingerSensorType::Type type, float offsetPosX = 0, float offsetPosY = -600.0f, float offsetPosZ = 200.0f);

	/**
	* \brief Get the currently available finger sensor types (defined in FubiConfig.h before compilation)
	* 
	* @return an int composed of the currently available sensor types (see FingerSensorType enum for the meaning)
	*/
	FUBI_API int getAvailableFingerSensorTypes();

	/**
	* \brief Get the type of the currently active sensor
	* 
	* @return the current sensor type
	*/
	FUBI_API Fubi::FingerSensorType::Type getCurrentFingerSensorType();

	/**
	* \brief Returns the number of currently tracked hands
	* 
	* @return the current number of hands
	*/
	FUBI_API unsigned short getNumHands();

	/**
	* \brief Returns the hand id from the user index
	* 
	* @param index index of the hand in the hand array
	* @return hand id of that user or 0 if not found
	*/
	FUBI_API unsigned int getHandID(unsigned int index);

	/**
	* \brief Get the offset position of the current finger sensor to the main sensor
	* 
	* @param xOffset, yOffset, zOffset [out] a vector from the main sensor to the finger sensor, (0,0,0) if no sensor present
	*/
	FUBI_API void getFingerSensorOffsetPosition(float& xOffset, float& yOffset, float& zOffset);

	/**
	* \brief Set the offset position of the current finger sensor to the main sensor
	* 
	* @param xOffset, yOffset, zOffset the vector from the main sensor to the finger sensor
	*/
	FUBI_API void setFingerSensorOffsetPosition(float xOffset, float yOffset, float zOffset);

	/*! @}*/
}

#endif