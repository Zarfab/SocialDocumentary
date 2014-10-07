// ****************************************************************************************
//
// Fubi User
// ---------------------------------------------------------
// Copyright (C) 2010-2013 Felix Kistler 
// 
// This software is distributed under the terms of the Eclipse Public License v1.0.
// A copy of the license may be obtained at: http://www.eclipse.org/org/documents/epl-v10.html
// 
// ****************************************************************************************
#pragma once

/** \file FubiHand.h 
 * \brief a header file containing the FubiUser class definition
*/ 

#include "FubiMath.h"
#include <opencv2\opencv.hpp>

#include <map>
#include <deque>
#include <queue>

class FubiUser
{
public:
	/**
	* \brief Tracking data with global and local positions/orientations and a timestamp
	*/
	struct TrackingData
	{
		TrackingData() : timeStamp(0) {}
		Fubi::SkeletonJointPosition jointPositions[Fubi::SkeletonJoint::NUM_JOINTS];
		Fubi::SkeletonJointPosition localJointPositions[Fubi::SkeletonJoint::NUM_JOINTS];
		Fubi::SkeletonJointOrientation jointOrientations[Fubi::SkeletonJoint::NUM_JOINTS];
		Fubi::SkeletonJointOrientation localJointOrientations[Fubi::SkeletonJoint::NUM_JOINTS];
		double timeStamp;
	};
	/**
	* \brief Acceleration data and a timestamp
	*/
	struct AccelerationData
	{
		AccelerationData() : timeStamp(0) {}
		Fubi::SkeletonJointAcceleration accelerations[Fubi::SkeletonJoint::NUM_JOINTS];
		double timeStamp;
	};

	/**
	* \brief Constructor
	*  Note: For internal use only!
	*/
	FubiUser();
	/**
	* \brief Destructor
	*  Note: For internal use only!
	*/
	~FubiUser();

	/**
	* \brief Operator used for comparing users according to their distance to the sensor
	* in the x-z plane --> get the closest users
	*/
	static bool closerToSensor(const FubiUser* u1, const FubiUser* u2)
	{
		const Fubi::SkeletonJointPosition& pos1 = u1->m_currentTrackingData->jointPositions[Fubi::SkeletonJoint::TORSO];
		const Fubi::SkeletonJointPosition& pos2 = u2->m_currentTrackingData->jointPositions[Fubi::SkeletonJoint::TORSO];

		if (u1->m_isTracked && pos1.m_confidence > 0.1f)
		{
			if (u2->m_isTracked  && pos2.m_confidence > 0.1f)
			{
				// Compare their distance (int the x,z-plane) to the sensor
				float dist1 = sqrtf(pos1.m_position.z*pos1.m_position.z + pos1.m_position.x*pos1.m_position.x);
				float dist2 = sqrtf(pos2.m_position.z*pos2.m_position.z + pos2.m_position.x*pos2.m_position.x);
				return dist1 < dist2;
			}
			else
			{
				// u1 is "closer" to the sensor (only valid user)
				return true;
			}
		}
		else if (u2->m_isTracked  && pos2.m_confidence > 0.1f)
		{
			return false; // u2 is "closer" to the sensor (only valid user)
		}

		// No valid user -> comparison has no meaning
		// but we compare the id to retain a strict weak ordering
		return u1->m_id < u2->m_id;
	}

	/**
	* \brief Operator used for comparing which user is more left of the sensor
	*/
	static bool moreLeft(const FubiUser* u1, const FubiUser* u2)
	{
		const Fubi::SkeletonJointPosition& pos1 = u1->m_currentTrackingData->jointPositions[Fubi::SkeletonJoint::TORSO];
		const Fubi::SkeletonJointPosition& pos2 = u2->m_currentTrackingData->jointPositions[Fubi::SkeletonJoint::TORSO];

		if (u1->m_isTracked && pos1.m_confidence > 0.1f)
		{
			if (u2->m_isTracked  && pos2.m_confidence > 0.1f)
			{
				// Compare their x value
				return pos1.m_position.x < pos2.m_position.x;
			}
			else
			{
				// u1 is "more left" to the sensor (only valid user)
				return true;
			}
		}
		else if (u2->m_isTracked  && pos2.m_confidence > 0.1f)
		{
			return false; // u2 is "more left" to the sensor (only valid user)
		}

		// No valid user -> comparison has no meaning
		// but we compare the id to retain a strict weak ordering
		return u1->m_id < u2->m_id;
	}

	/**
	* \brief Enable/disables the tracking of the shown number of fingers for each hand
	* Note: For internal use only, please use Fubi:enableFingerTracking:() instead!
	*/
	void enableFingerTracking(bool leftHand, bool rightHand, bool useConvexityDefectMethod = false);

	/**
	* \brief Gets the finger count optionally calculated by the median of the last 10 calculations
	* Note: For internal use only, please use Fubi::getFingerCount() instead!
	*/
	int getFingerCount(bool leftHand = false, bool getMedianOfLastFrames = true, bool useOldConvexityDefectMethod = false);
	bool leftHandCloser();
	int getFingerCountOnClosestHand(bool getMedianOfLastFrames = true, bool useOldConvexityDefectMethod = false);
	
	/**
	* \brief return true if the hand is at least minDistance closer to the sensor than the torso
	* Note: For internal use only, please use Fubi::userHandInFront() instead!
	*/
	bool handInFront(bool leftHand = false, int minDistance = 400);

	/**
	* \brief Calculates filtered and local transformations and updates the combination recognizers
	* Note: For internal use only, please use Fubi::updateSensor() instead!
	*/
	void update();

	/**
	* \brief Update the tracking info from the given sensor
	* Note: For internal use only, please use Fubi::updateSensor() instead!
	*/ 
	void updateTrackingData(class FubiISensor* sensor);

	/**
	* \brief Reset the user to an initial state 
	* Note: For internal use only, please use Fubi::resetTracking() instead!
	*/
	void reset();

	/**
	* \brief Manually update the tracking info
	* Note: For internal use only, please use Fubi::updateTrackingData() instead!
	*/
	void addNewTrackingData(Fubi::SkeletonJointPosition* positions,
		double timeStamp = -1, Fubi::SkeletonJointOrientation* orientations = 0);

	/**
	* \brief and acceleartion data
	* Note: For internal use only, please use Fubi::updateTrackingData() instead!
	*/
	void addNewAccelerationData(Fubi::SkeletonJointAcceleration* acceleration, double timeStamp = 0);

	/**
	* \brief Get the debug image of the last finger count detection
	* Note: For internal use only!
	*/
	const Fubi::FingerCountImageData* getFingerCountImageData(bool left = false)
	{
		return left ? &m_leftFingerCountImage : &m_rightFingerCountImage;
	}

	/**
	* \brief Whether the user is currently seen in the depth image
	*/
	bool m_inScene;

	/**
	* \brief Id of this user
	*/
	unsigned int m_id;

	/**
	* \brief Whether the user is currently tracked
	*/
	bool m_isTracked;

	/**
	* \brief Whether the face of the user is currently tracked
	*/
	bool m_isFaceTracked;

	/**
	* \brief Current and last, filtered and unfiltered tracking data including joint positions and orientations (both local and global ones)
	*/
	TrackingData *m_currentTrackingData, *m_lastTrackingData, *m_lastFilteredTrackingData, *m_filteredTrackingData;

	/**
	* \brief Acceleration data
	*/
	AccelerationData *m_accelerationData;

	/**
	* \brief The user's body measurements
	*/
	Fubi::BodyMeasurementDistance m_bodyMeasurements[Fubi::BodyMeasurement::NUM_MEASUREMENTS];


	// Added for KITV
	void pushName(std::string newName);
	std::string getName();

	std::vector<float> getHeadCenter();
	std::vector<float> getHeadRotation();
	std::vector<float> getAnimationUnits();
	std::vector<float> getShapeUnits();
	cv::Rect m_faceRectCv;
	
	/**
	* \brief Set the new screen watched and update the following variables:
	*	- m_screenWatched
	*	- m_screenWatchedChanged
	*	- m_interestTime
	*	- m_interest
	*/
	void updateScreenWatched(int screen);
	
	/**
	* \brief The screen the user is currently watching 
	*/
	int m_screenWatched;
	std::deque<int> m_screenQueue;

	/**
	* \brief The screen the user is currently watching has just changed
	*/
	bool m_screenWatchedChanged;

	/**
	* \brief The time since the user is watching a screen
	*/
	double m_interestTime;
	double m_interestBegTime;
	double m_interestLastChangeTime;

	enum Interest
	{
		NONE,		// < 1.5s
		ORIENTING,	// [1.5, 6[s
		ENGAGED,	// [6, 15]s
		STARING		// > 15s
	};
	/**
	* \brief The state of interest of the user
	*/
	Interest m_interest;

	/**
	* \brief True if the interest has changed
	*/
	bool m_interestChanged;


private:
	/**
	* \brief Adds a finger count detection to the deque for later median calculation
	*/
	void addFingerCount(int count, bool leftHand = false);

	/**
	* \brief Apply a filter on the tracking data
	*/
	void calculateFilteredTransformations();


	/**
	* \brief Update the finger count calculation
	*/
	void updateFingerCount();

	/**
	* \brief Calculate the median of the given finger count array
	*/
	int calculateMedianFingerCount(const std::deque<int>& fingerCount);

	/**
	* \brief Update the body measurements out of the currently tracked positions
	*/
	void updateBodyMeasurements();

	/**
	* \brief When the last detection of the finger count of each hand happened, -1 if disabled
	*/
	double m_lastRightFingerDetection, m_lastLeftFingerDetection;	
	/**
	* \brief What method to use for the finger count detection
	*/
	bool m_useConvexityDefectMethod;	
	/**
	* \brief Number of last finger count detections to use for median calculation
	*/
	unsigned int m_maxFingerCountForMedian;
	/**
	* \brief Lhe last finger count detections
	*/
	std::deque<int> m_rightFingerCount, m_leftFingerCount;
	/**
	* \brief Time between the finger count detection of one hand
	*/
	double m_fingerTrackIntervall;
	/**
	* \brief Degbug images created during the finger count detection
	*/
	Fubi::FingerCountImageData m_leftFingerCountImage, m_rightFingerCountImage;

	/**
	* \brief Timestamp of the last body measurement update
	*/
	double m_lastBodyMeasurementUpdate;

	/**
	* \brief Additional filtering history for the velocities
	*/
	Fubi::Vec3f m_lastFilteredVelocity[Fubi::SkeletonJoint::NUM_JOINTS];

	const unsigned int m_nbNamesInDeque;
	std::deque<std::string> m_nameDeque;
	std::string m_name;
	std::vector<float> m_headRotationDegrees;
	std::vector<float> m_AUs;
	std::vector<float> m_SUs;
};
/*! @}*/