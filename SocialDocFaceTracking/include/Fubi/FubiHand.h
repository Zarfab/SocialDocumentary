// ****************************************************************************************
//
// Fubi Hand
// ---------------------------------------------------------
// Copyright (C) 2010-2013 Felix Kistler 
// 
// This software is distributed under the terms of the Eclipse Public License v1.0.
// A copy of the license may be obtained at: http://www.eclipse.org/org/documents/epl-v10.html
// 
// ****************************************************************************************
#pragma once

/** \file FubiHand.h 
 * \brief a header file containing the FubiHand class definition
*/ 

#include "FubiUtils.h"
#include "FubiMath.h"

#include <map>
#include <deque>
#include <queue>

class FubiHand
{
public:
	/**
	 * \brief Tracking data with global and local positions/orientations, the current finger count and a timestamp
	 */
	struct FingerTrackingData
	{
		FingerTrackingData() : timeStamp(0) {}
		Fubi::SkeletonJointPosition jointPositions[Fubi::SkeletonHandJoint::NUM_JOINTS];
		Fubi::SkeletonJointOrientation jointOrientations[Fubi::SkeletonHandJoint::NUM_JOINTS];
		Fubi::SkeletonJointPosition localJointPositions[Fubi::SkeletonHandJoint::NUM_JOINTS];
		Fubi::SkeletonJointOrientation localJointOrientations[Fubi::SkeletonHandJoint::NUM_JOINTS];
		int fingerCount;
		double timeStamp;
	};

	/**
	 * \brief Constructor for FubiHand
	 * Note: For internal use only!
	 */
	FubiHand();
	/**
	 * \brief Destructor for FubiHand
	 * Note: For internal use only!
	 */
	~FubiHand();

	/**
	 * \brief Calculates filtered and local transformations and updates the combination recognizers
	 * Note: For internal use only, please use Fubi::updateSensor() instead!
	 */
	void update();

	/**
	 * \brief Update the tracking info from the given sensor
	 * Note: For internal use only, please use Fubi::updateSensor() instead!
	 */
	void updateFingerTrackingData(class FubiIFingerSensor* sensor);

	/**
	 * \brief Reset the user to an initial state
	 * Note: For internal use only, please use Fubi::resetTracking() instead!
	 */
	void reset();

	/**
	 * \brief Id of this user
	 */
	unsigned int m_id;

	/**
	 * \brief Whether the hand is currently tracked
	 */
	bool m_isTracked;
	
	/**
	 * \brief Current and last, filtered and unfiltered tracking data including joint positions and orientations (both local and global ones)
	 */
	FingerTrackingData *m_currentFingerTrackingData, *m_lastFingerTrackingData, *m_lastFilteredFingerTrackingData, *m_filteredFingerTrackingData;

private:
	/**
	 * \brief Apply a filter on the tracking data
	 */
	void calculateFilteredTransformations();

	/**
	 * \brief Additional filtering history for the velocities
	 */
	Fubi::Vec3f m_lastFilteredVelocity[Fubi::SkeletonHandJoint::NUM_JOINTS];
};

/*! @}*/