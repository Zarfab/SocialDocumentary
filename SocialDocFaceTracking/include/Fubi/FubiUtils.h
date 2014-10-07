// ****************************************************************************************
//
// Fubi Utility Functions
// ---------------------------------------------------------
// Copyright (C) 2010-2013 Felix Kistler 
// 
// This software is distributed under the terms of the Eclipse Public License v1.0.
// A copy of the license may be obtained at: http://www.eclipse.org/org/documents/epl-v10.html
// 
// ****************************************************************************************
#pragma once

/** \file FubiUtils.h 
 * \brief Utility functions and data structures
*/ 

#include <time.h>
#include <string>
#include <sstream>


namespace Fubi
{
	/**
	* \brief Options for image rendering
	*/
	struct ImageType
	{
		/**
		* \brief The possible image types
		*/
		enum Type
		{
			Color,
			Depth,
			IR,
			Blank
		};
	};

	/**
	* \brief The number of channels in the image
	*/
	struct ImageNumChannels
	{
		enum Channel
		{
			C1 = 1,
			C3 = 3,
			C4 = 4
		};
	};
	/**
	* \brief The depth of each channel
	*/
	struct ImageDepth
	{

		enum Depth
		{
			D8 = 8,
			D16 = 16
		};
	};
	/**
	* \brief How the depth image should be modified for depth differences
	*		 being easier to distinguish by the human eye
	*/
	struct DepthImageModification
	{

		enum Modification
		{
			Raw,
			UseHistogram,
			StretchValueRange,
			ConvertToRGB
		};
	};
	/**
	* \brief options for (de-)activating different parts of the rendered tracking information
	*/
	struct RenderOptions
	{
		/**
		* \brief The possible formats for the tracking info rendering
		*/
		enum Options
		{
			None							= 0,
			Shapes							= 1,
			Skeletons						= 2,
			UserCaptions					= 4,
			LocalOrientCaptions				= 8,
			GlobalOrientCaptions			= 16,
			LocalPosCaptions				= 32,
			GlobalPosCaptions				= 64,
			Background						= 128,
			SwapRAndB						= 256,
			FingerShapes					= 512,
			DetailedFaceShapes				= 1024,
			BodyMeasurements				= 2048,
			UseFilteredValues				= 4096
		};

		/**
		* \brief IDs for the Joints to define which of them should be rendered
		*/
		enum Joint
		{
			ALL_JOINTS		= 0xFFFFFFFF,

			HEAD			= 0x00000001,
			NECK			= 0x00000002,
			TORSO			= 0x00000004,
			WAIST			= 0x00000008,

			LEFT_SHOULDER	= 0x00000010,
			LEFT_ELBOW		= 0x00000020,
			LEFT_WRIST		= 0x00000040,
			LEFT_HAND		= 0x00000080,

			RIGHT_SHOULDER	= 0x00000100,
			RIGHT_ELBOW		= 0x00000200,
			RIGHT_WRIST		= 0x00000400,
			RIGHT_HAND		= 0x00000800,

			LEFT_HIP		= 0x00001000,
			LEFT_KNEE		= 0x00002000,
			LEFT_ANKLE		= 0x00004000,
			LEFT_FOOT		= 0x00008000,

			RIGHT_HIP		= 0x00010000,
			RIGHT_KNEE		= 0x00020000,
			RIGHT_ANKLE		= 0x00040000,
			RIGHT_FOOT		= 0x00080000,

			FACE_NOSE		= 0x00100000,
			FACE_LEFT_EAR	= 0x00200000,
			FACE_RIGHT_EAR	= 0x00400000,
			FACE_FOREHEAD	= 0x00800000,
			FACE_CHIN		= 0x01000000,

			PALM			= 0x02000000,
			FINGER_ONE		= 0x04000000,
			FINGER_TWO		= 0x08000000,
			FINGER_THREE	= 0x10000000,
			FINGER_FOUR		= 0x20000000,
			FINGER_FIVE		= 0x40000000
		};
	};

	/**
	* \brief IDs for all supported body tracking skeleton joints
	*/
	struct SkeletonJoint
	{
		enum Joint
		{
			HEAD			= 0,
			NECK			= 1,
			TORSO			= 2,
			WAIST			= 3,

			LEFT_SHOULDER	= 4,
			LEFT_ELBOW		= 5,
			LEFT_WRIST		= 6,
			LEFT_HAND		= 7,

			RIGHT_SHOULDER	=8,
			RIGHT_ELBOW		=9,
			RIGHT_WRIST		=10,
			RIGHT_HAND		=11,

			LEFT_HIP		=12,
			LEFT_KNEE		=13,
			LEFT_ANKLE		=14,
			LEFT_FOOT		=15,

			RIGHT_HIP		=16,
			RIGHT_KNEE		=17,
			RIGHT_ANKLE		=18,
			RIGHT_FOOT		=19,

			FACE_NOSE		=20,
			FACE_LEFT_EAR	=21,
			FACE_RIGHT_EAR	=22,
			FACE_FOREHEAD	=23,
			FACE_CHIN		=24,

			NUM_JOINTS
		};
	};

	/**
	* \brief IDs for all supported finger tracking skeleton joints
	*/
	struct SkeletonHandJoint
	{
		enum Joint
		{
			PALM			= 0,
			FINGER_ONE		= 1,
			FINGER_TWO		= 2,
			FINGER_THREE	= 3,
			FINGER_FOUR		= 4,
			FINGER_FIVE		= 5,

			NUM_JOINTS
		};
	};

	/**
	* \brief IDs for all supported body measurements
	*/
	struct BodyMeasurement
	{
		enum Measurement
		{
			BODY_HEIGHT			= 0,
			TORSO_HEIGHT		= 1,
			SHOULDER_WIDTH		= 2,
			HIP_WIDTH			= 3,
			ARM_LENGTH			= 4,
			UPPER_ARM_LENGTH	= 5,
			LOWER_ARM_LENGTH	= 6,
			LEG_LENGTH			= 7,
			UPPER_LEG_LENGTH	= 8,
			LOWER_LEG_LENGTH	= 9,
			NUM_MEASUREMENTS
		};
	};

	/**
	* \brief Profiles for tracking specific parts of the body (not working for all sensors)
	*/
	struct SkeletonTrackingProfile
	{
		enum Profile
		{
			/** No joints at all (not really usefull)**/
			NONE		= 1,

			/** All joints (standard) **/
			ALL			= 2,

			/** All the joints in the upper body (torso and upwards) **/
			UPPER_BODY	= 3,

			/** All the joints in the lower body (torso and downwards) **/
			LOWER_BODY	= 4,

			/** The head and the hands (minimal profile) **/
			HEAD_HANDS	= 5
		};
	};

	/**
	* \brief All supported body tracking sensor types, need to be enabled in the FubiConfig.h
	*        Further installation instructions in the readme/doc
	*/
	struct SensorType
	{
		enum Type
		{
			/** No sensor in use **/
			NONE = 0,
			/** Sensor based on OpenNI 2.x**/
			OPENNI2 = 0x01,
			/** Sensor based on OpenNI 1.x**/
			OPENNI1 = 0x02,
			/** Sensor based on the Kinect for Windows SDK 1.x**/
			KINECTSDK = 0x04
		};
	};

	/**
	* \brief All supported finger tracking sensor types, need to be enabled in the FubiConfig.h
	*        Further installation instructions in the readme/doc
	*/
	struct FingerSensorType
	{
		enum Type
		{
			/** No sensor in use **/
			NONE = 0,
			/** Finger tracking with LEAP **/
			LEAP = 0x01
		};
	};

	/**
	* \brief A recognizer can target a specific type of sensor that provides the correct data
	*/
	struct RecognizerTarget
	{
		enum Target
		{
			/** No sensor supported, usually means something went wrong **/
			NO_SENSOR = -1,
			/** A default body tracking sensor such as the Kinect with tracking software **/
			BODY_SENSOR = 0,
			/** A finger tracking sensor such as the leap motion **/
			FINGER_SENSOR = 1,
			/** All types of sensors can be used, e.g. finger count recognizer**/
			ALL_SENSORS = 3
		};
	};

	/**
	* \brief Result of a gesture recognition
	*/
	struct RecognitionResult
	{
		enum Result
		{
			TRACKING_ERROR = -1,
			NOT_RECOGNIZED = 0,
			RECOGNIZED = 1,
			WAITING_FOR_LAST_STATE_TO_FINISH = 2	// Only for combinations with waitUntilLastStateRecognizersStop flag
		};
	};

	/**
	* \brief Additional information about what went wrong and how to correct it
	*/
	struct RecognitionCorrectionHint
	{
		/**
		* \brief the type of what needs to be changed
		*/
		enum ChangeType
		{
			SPEED,
			LENGTH,
			DIRECTION,
			FINGERS
		};
		/**
		* \brief the direction (if any) of what needs to be changed
		*/
		enum ChangeDirection
		{
			DIFFERENT,
			MORE,
			LESS
		};

		SkeletonJoint::Joint m_joint;
		float m_dirX, m_dirY, m_dirZ;
		bool m_isAngle;
		ChangeType m_changeType;
		ChangeDirection m_changeDirection;
		int m_failedState;

		RecognitionCorrectionHint(SkeletonJoint::Joint joint = SkeletonJoint::NUM_JOINTS, float dirX = 0, float dirY = 0, float dirZ = 0,
			bool isAngle = false, ChangeType changeType = SPEED, ChangeDirection changeDirection = DIFFERENT, int failedState = -1)
			: m_joint(joint), m_dirX(dirX), m_dirY(dirY), m_dirZ(dirZ), m_isAngle(isAngle),
			m_changeType(changeType), m_changeDirection(changeDirection), m_failedState(failedState)
		{}
	};
	/**
	* \brief Options for configuring a stream of a sensor
	*/
	struct StreamOptions
	{
		StreamOptions(int width = 640, int height = 480, int fps = 30)
			: m_width(width), m_height(height), m_fps(fps)
		{}
		void invalidate()
		{ m_width = -1; m_height = -1; m_fps = -1; }
		bool isValid() const
		{ return m_width > 0 && m_height > 0 && m_fps > 0; }
		int m_width;
		int m_height;
		int m_fps;
	};

	/**
	* \brief Parameters for configuring the filtering of joint tracking data
	*/
	struct FilterOptions
	{
		FilterOptions(float minCutOffFrequency = 1.0f, float velocityCutOffFrequency = 1.0f, float cutOffSlope = 0.007f)
			:  m_minCutOffFrequency(minCutOffFrequency), 
			m_velocityCutOffFrequency(velocityCutOffFrequency), m_cutOffSlope(cutOffSlope)
		{}
		float m_minCutOffFrequency;
		float m_velocityCutOffFrequency;
		float m_cutOffSlope;
	};

	/**
	* \brief Options for a sensor, including which streams to use, tracking options and global options for all streams
	*/
	struct SensorOptions
	{
		SensorOptions(const StreamOptions& depthOptions = StreamOptions(),
			const StreamOptions& rgbOptions = StreamOptions(), const StreamOptions& irOptions = StreamOptions(-1, -1, -1),
			SensorType::Type sensorType = SensorType::OPENNI2,
			const SkeletonTrackingProfile::Profile trackingProfile = SkeletonTrackingProfile::ALL,
			bool mirrorStreams = true, bool registerStreams = true)
			: m_depthOptions(depthOptions), m_irOptions(irOptions), m_rgbOptions(rgbOptions),
			m_trackingProfile(trackingProfile), m_mirrorStreams(mirrorStreams), m_registerStreams(registerStreams),
			m_type(sensorType)
		{}
		StreamOptions m_depthOptions;
		StreamOptions m_irOptions;
		StreamOptions m_rgbOptions;

		SkeletonTrackingProfile::Profile m_trackingProfile;
		bool m_mirrorStreams;
		bool m_registerStreams;

		SensorType::Type m_type;
	};

	/**
	* \brief Processed part of the depth stream with tracking data
	*        by Fubi's build in finger tracking
	*/
	struct FingerCountImageData
	{
		FingerCountImageData() : image(0x0), timeStamp(-1) {}
		void* image;
		double timeStamp;
		int fingerCount;
		int posX, posY;
	};

	/**
	* \brief Maximum depth value that can occure in the depth image
	*/
	static const int MaxDepth = 10000;
	/**
	* \brief And maximum value in the IR image
	*/
	static const unsigned short MaxIR = 1024;
	/**
	* \brief Maximum number of tracked users
	*/
	static const int MaxUsers = 15;
	/**
	* \brief And hands
	*/
	static const int MaxHands = 2*MaxUsers;


	/**
	* \brief Logging functions
	*  Note: For internal use only!
	*/
	class Logging
	{
	public:
		static void logInfo(const char* msg, ...);
		static void logDbg(const char* file, int line, const char* msg, ...);
		static void logWrn(const char* file, int line, const char* msg, ...);
		static void logErr(const char* file, int line, const char* msg, ...);
	};
#ifdef _MSC_VER
#define Fubi_logInfo(msg, ...) Fubi::Logging::logInfo((msg), __VA_ARGS__)
#define Fubi_logDbg(msg, ...) Fubi::Logging::logDbg(__FILE__, __LINE__, (msg), __VA_ARGS__)
#define Fubi_logWrn(msg, ...) Fubi::Logging::logWrn(__FILE__, __LINE__, (msg), __VA_ARGS__)
#define Fubi_logErr(msg, ...) Fubi::Logging::logErr(__FILE__, __LINE__, (msg), __VA_ARGS__)
#else
#define Fubi_logInfo(msg, ...) Fubi::Logging::logInfo((msg), ##__VA_ARGS__)
#define Fubi_logDbg(msg, ...) Fubi::Logging::logDbg(__FILE__, __LINE__, (msg), ##__VA_ARGS__)
#define Fubi_logWrn(msg, ...) Fubi::Logging::logWrn(__FILE__, __LINE__, (msg), ##__VA_ARGS__)
#define Fubi_logErr(msg, ...) Fubi::Logging::logErr(__FILE__, __LINE__, (msg), ##__VA_ARGS__)
#endif

	/**
	* \brief Convert to a uniform string only containing lower case letters and no white spaces
	*
	* @param str the string to convert
	* @return the converted string without any instances of ' ', '_', '-', or '|' and all upper case letters converted to lower case
	*/
	static std::string removeWhiteSpacesAndToLower(const std::string& str)
	{
		std::string ret;
		for (unsigned int i = 0; i < str.length(); i++)
		{
			const char& c = str[i];
			if (c != ' ' && c != '_' && c != '-' && c != '|')
			{
				if (c >= 'A' && c <= 'Z')
					ret += (c + ('a'-'A'));
				else
					ret += c;
			}
		}
		return ret;
	}

	/**
	* \brief Converts a number to string
	*
	* @param num the number to convert, should be a numeric type
	* @param precision (=2) number of decimal places to be displayed
	* @return the converted string without any instances of ' ', '_', '-', or '|' and all upper case letters converted to lower case
	*/
	template<class T> std::string numToString(T num, unsigned short precision = 2)
	{
		std::stringstream ss;
		ss.precision(precision);
		ss << std::fixed << num;
		return ss.str();
	}

	/**
	* \brief Ensures that a value is between certain bounds
	* 
	* @param  value the value to be clamped
	* @param  min the minimum allowed for the value
	* @param  max the maximum allowed for the value
	* @return returns the clamped value (value if between min and max, else min or max)
	*/
	template<class T> static inline T clamp( T value, T min, T max )
	{
		if( value < min ) value = min;
		else if( value > max ) value = max;
		return value;
	}

	/**
	* \brief Ensures that a value is between certain bounds
	* 
	* @param  value the value to be clamped
	* @param  min the minimum allowed for the value
	* @param  max the maximum allowed for the value
	* @param valueToSet this will be returned if a value exceeds the given borders
	* @return returns the clamped value (value if between min and max, else min or max)
	*/
	template<class T> static inline T clamp( T value, T min, T max, T valueToSet)
	{
		if( value < min ) value = valueToSet;
		else if( value > max ) value = valueToSet;
		return value;
	}

	/**
	* \brief Checks whether two values have the same sign
	* 
	* @param  a first value
	* @param  b second value
	* @return true if they have the same sign
	*/
	template<class T> static inline bool sameSign( T a, T b )
	{
		return (a <= 0 && b <= 0) || (a >= 0 && b >= 0);
	}



	/**
	* \brief Number of seconds since the program start
	*  Note: For internal use only, use Fubi::getCurrentTime() instead!
	*/
	double currentTime();

	/**
	* \brief Get a string representing the joint name for a specific joint id
	*/
	static const char* getJointName(SkeletonJoint::Joint id)
	{
		switch(id)
		{
		case SkeletonJoint::HEAD:
			return "head";
		case SkeletonJoint::NECK:
			return "neck";
		case SkeletonJoint::TORSO:
			return "torso";
		case SkeletonJoint::WAIST:
			return "waist";

		case SkeletonJoint::LEFT_SHOULDER:
			return "leftShoulder";
		case SkeletonJoint::LEFT_ELBOW:
			return "leftElbow";
		case SkeletonJoint::LEFT_WRIST:
			return "leftWrist";
		case SkeletonJoint::LEFT_HAND:
			return "leftHand";

		case SkeletonJoint::RIGHT_SHOULDER:
			return "rightShoulder";
		case SkeletonJoint::RIGHT_ELBOW:
			return "rightElbow";
		case SkeletonJoint::RIGHT_WRIST:
			return "rightWrist";
		case SkeletonJoint::RIGHT_HAND:
			return "rightHand";

		case SkeletonJoint::LEFT_HIP:
			return "leftHip";
		case SkeletonJoint::LEFT_KNEE:
			return "leftKnee";
		case SkeletonJoint::LEFT_ANKLE:
			return "leftAnkle";
		case SkeletonJoint::LEFT_FOOT:
			return "leftFoot";

		case SkeletonJoint::RIGHT_HIP:
			return "rightHip";
		case SkeletonJoint::RIGHT_KNEE:
			return "rightKnee";
		case SkeletonJoint::RIGHT_ANKLE:
			return "rightAnkle";
		case SkeletonJoint::RIGHT_FOOT:
			return "rightFoot";

		case SkeletonJoint::FACE_CHIN:
			return "faceChin";
		case SkeletonJoint::FACE_FOREHEAD:
			return "faceForeHead";
		case SkeletonJoint::FACE_LEFT_EAR:
			return "faceLeftEar";
		case SkeletonJoint::FACE_NOSE:
			return "faceNose";
		case SkeletonJoint::FACE_RIGHT_EAR:
			return "faceRightEar";
		default:
			return "";
		}

		return "";
	}

	/**
	* \brief Get the joint id out of the given joint name as a string
	*/
	static SkeletonJoint::Joint getJointID(const char* name)
	{
		if (name)
		{
			std::string lowerName = removeWhiteSpacesAndToLower(name);
			if (lowerName == "head")
				return SkeletonJoint::HEAD;
			if (lowerName == "neck")
				return SkeletonJoint::NECK;
			if (lowerName == "torso")			
				return SkeletonJoint::TORSO;
			if (lowerName == "waist")	
				return SkeletonJoint::WAIST;

			if (lowerName == "leftshoulder")
				return SkeletonJoint::LEFT_SHOULDER;
			if (lowerName == "leftelbow")
				return SkeletonJoint::LEFT_ELBOW;
			if (lowerName == "leftwrist")
				return SkeletonJoint::LEFT_WRIST;
			if (lowerName == "lefthand")
				return SkeletonJoint::LEFT_HAND;


			if (lowerName == "rightshoulder")			
				return SkeletonJoint::RIGHT_SHOULDER;
			if (lowerName == "rightelbow")
				return SkeletonJoint::RIGHT_ELBOW;
			if (lowerName == "rightwrist")
				return SkeletonJoint::RIGHT_WRIST;
			if (lowerName == "righthand")
				return SkeletonJoint::RIGHT_HAND;

			if (lowerName == "lefthip")
				return SkeletonJoint::LEFT_HIP;
			if (lowerName == "leftknee")
				return SkeletonJoint::LEFT_KNEE;
			if (lowerName == "leftankle")
				return SkeletonJoint::LEFT_ANKLE;
			if (lowerName == "leftfoot")
				return SkeletonJoint::LEFT_FOOT;

			if (lowerName == "righthip")
				return SkeletonJoint::RIGHT_HIP;
			if (lowerName == "rightknee")
				return SkeletonJoint::RIGHT_KNEE;
			if (lowerName == "rightankle")
				return SkeletonJoint::RIGHT_ANKLE;
			if (lowerName == "rightfoot")
				return SkeletonJoint::RIGHT_FOOT;

			if (lowerName == "facechin")
				return SkeletonJoint::FACE_CHIN;
			if (lowerName == "faceforehead")
				return SkeletonJoint::FACE_FOREHEAD;
			if (lowerName == "faceleftear")
				return SkeletonJoint::FACE_LEFT_EAR;
			if (lowerName == "facenose")
				return SkeletonJoint::FACE_NOSE;
			if (lowerName == "facerightear")
				return SkeletonJoint::FACE_RIGHT_EAR;
		}

		return SkeletonJoint::NUM_JOINTS;
	}

	/**
	* \brief Get the body measurement id out of the given measure name
	*/
	static BodyMeasurement::Measurement getBodyMeasureID(const char* name)
	{
		if (name)
		{
			std::string lowerName = removeWhiteSpacesAndToLower(name);
			if (lowerName =="bodyheight")
				return BodyMeasurement::BODY_HEIGHT;
			if (lowerName =="torsoheight")
				return BodyMeasurement::TORSO_HEIGHT;
			if (lowerName =="shoulderwidth")
				return BodyMeasurement::SHOULDER_WIDTH;
			if (lowerName =="hipwidth")
				return BodyMeasurement::HIP_WIDTH;
			if (lowerName =="armlength")
				return BodyMeasurement::ARM_LENGTH;
			if (lowerName =="upperarmlength")
				return BodyMeasurement::UPPER_ARM_LENGTH;
			if (lowerName =="lowerarmlength")
				return BodyMeasurement::LOWER_ARM_LENGTH;
			if (lowerName =="leglength")
				return BodyMeasurement::LEG_LENGTH;
			if (lowerName =="upperleglength")
				return BodyMeasurement::UPPER_LEG_LENGTH;
			if (lowerName =="lowerleglength")
				return BodyMeasurement::LOWER_LEG_LENGTH;
		}

		return BodyMeasurement::NUM_MEASUREMENTS;
	}

	/**
	* \brief Get the name string representation of a body measurement id
	*/
	static const char* getBodyMeasureName(BodyMeasurement::Measurement id)
	{
		switch(id)
		{
		case BodyMeasurement::BODY_HEIGHT:
			return "bodyHeight";
		case BodyMeasurement::TORSO_HEIGHT:
			return "torsoHeight";
		case BodyMeasurement::SHOULDER_WIDTH:
			return "shoulderWidth";
		case BodyMeasurement::HIP_WIDTH:
			return "hipWidth";
		case BodyMeasurement::ARM_LENGTH:
			return "armLength";
		case BodyMeasurement::UPPER_ARM_LENGTH:
			return "upperArmLength";
		case BodyMeasurement::LOWER_ARM_LENGTH:
			return "lowerArmLength";
		case BodyMeasurement::LEG_LENGTH:
			return "legLength";
		case BodyMeasurement::UPPER_LEG_LENGTH:
			return "upperLegLength";
		case BodyMeasurement::LOWER_LEG_LENGTH:
			return "lowerLegLength";
		default:
			return "";
		}
		return "";
	}

	/**
	* \brief Get a string representing the hand joint name for a specific hand joint id
	*/
	static const char* getHandJointName(SkeletonHandJoint::Joint id)
	{
		switch(id)
		{
		case SkeletonHandJoint::PALM:
			return "palm";
		case SkeletonHandJoint::FINGER_ONE:
			return "fingerOne";
		case SkeletonHandJoint::FINGER_TWO:
			return "fingerTwo";
		case SkeletonHandJoint::FINGER_THREE:
			return "fingerThree";
		case SkeletonHandJoint::FINGER_FOUR:
			return "fingerFour";
		case SkeletonHandJoint::FINGER_FIVE:
			return "fingerFive";
		default:
			return "";
		}

		return "";
	}

	/**
	* \brief Get the hand joint id out of the given hand joint name as a string
	*/
	static SkeletonHandJoint::Joint getHandJointID(const char* name)
	{
		if (name)
		{
			std::string lowerName = removeWhiteSpacesAndToLower(name);
			if (lowerName == "palm")
				return SkeletonHandJoint::PALM;
			if (lowerName == "fingerone")
				return SkeletonHandJoint::FINGER_ONE;
			if (lowerName == "fingertwo")			
				return SkeletonHandJoint::FINGER_TWO;
			if (lowerName == "fingerthree")	
				return SkeletonHandJoint::FINGER_THREE;
			if (lowerName == "fingerfour")
				return SkeletonHandJoint::FINGER_FOUR;
			if (lowerName == "fingerfive")
				return SkeletonHandJoint::FINGER_FIVE;
		}

		return SkeletonHandJoint::NUM_JOINTS;
	}

	/**
	 * \brief Creates a string message out of a RecognitionCorrectionHint
	*/
	static std::string createCorrectionHintMsg(const RecognitionCorrectionHint& hint)
    {
        std::stringstream msg;
		msg << std::fixed;

        if (hint.m_failedState > -1)
        {
            msg << "State " << hint.m_failedState << " - ";
        }

        if (hint.m_changeType == RecognitionCorrectionHint::FINGERS)
        {
			msg.precision(0);
            if (hint.m_dirX > 0)
                msg << "Please show " << hint.m_dirX << " more fingers!\n";
            else
                msg << "Please show " << hint.m_dirX << " less fingers!\n";
        }
        else if (hint.m_changeType == RecognitionCorrectionHint::DIRECTION)
        {
			msg.precision(2);
            std::string action = "move";
            if (hint.m_isAngle)
                action = "turn";
            std::string direction = "";
            if (hint.m_dirX > 0.1f)
                direction += "right ";
            else if (hint.m_dirX < -0.1f)
                direction += "left ";
            if (hint.m_dirY > 0.1f)
                direction += "up ";
            else if (hint.m_dirY < -0.1f)
                direction += "down ";
            if (hint.m_dirZ > 0.1f)
                direction += "backward ";
            else if (hint.m_dirZ < -0.1f)
                direction += "forward ";
            msg << "Please " << action << " " << getJointName(hint.m_joint) << " "
                << direction << ":" << hint.m_dirX << "/" << hint.m_dirY << "/" << hint.m_dirZ << "\n";
        }
        else
        {
			msg.precision(2);
            for (int dirI = 0; dirI < 3; ++dirI)
            {
                float value = hint.m_dirX;
                std::string direction = "";
                if (dirI == 1)
                {
                    value = hint.m_dirY;
                    if (value < 0)
                        direction = "down";
                    else
                        direction = "up";
                }
                else if (dirI == 2)
                {
                    value = hint.m_dirZ;
                    if (value < 0)
                        direction = "forward";
                    else
                        direction = "backward";
                }
                if (std::fabsf(value) > 0.01f)
                {
                    std::string mod = "";
					if (hint.m_changeType == RecognitionCorrectionHint::LENGTH)
                    {
                        if (hint.m_changeDirection == RecognitionCorrectionHint::MORE)
                            mod = "more ";
                        else if (hint.m_changeDirection == RecognitionCorrectionHint::LESS)
                            mod = "less ";
                    }
                    else if (hint.m_changeType == RecognitionCorrectionHint::SPEED)
                    {
                        if (hint.m_changeDirection == RecognitionCorrectionHint::MORE)
                            mod = "faster ";
                        else if (hint.m_changeDirection == RecognitionCorrectionHint::LESS)
                            mod = "slower ";
                    }
					std::string action = "move";
                    if (hint.m_isAngle)
                        action = "turn";
                    msg << "Please " << action << " " << getJointName(hint.m_joint) << " "
			            << mod << direction << ": " << value << "\n";
                }
            }
        }
        return msg.str();
    }
};
