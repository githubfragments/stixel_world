/*
 *  Copyright 2013 Néstor Morales Hernández <nestor@isaatc.ull.es>
 * 
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef STIXELSAPPLICATION_H
#define STIXELSAPPLICATION_H

#include <string>

#include <boost/program_options.hpp>

#include "utils.h"
#include "polarcalibration.h"
#include "motionevaluation.h"

#include "applications/stixel_world_lib/stixel_world_lib.hpp"
#include "video_input/VideoInputFactory.hpp"
#include "video_input/preprocessing/CpuPreprocessor.hpp"
#include "stereo_matching/stixels/AbstractStixelWorldEstimator.hpp"
#include "stereo_matching/stixels/motion/DummyStixelMotionEstimator.hpp"
#include "stixelstracker.h"
#include "oflowtracker.h"

#include<pcl_ros/point_cloud.h>

#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_datatypes.h>
#include "std_msgs/String.h"
#include <image_transport/image_transport.h>
#include <sensor_msgs/CameraInfo.h>
#include <camera_calibration_parsers/parse_yml.h>
#include <rosgraph_msgs/Clock.h>

#include <boost/thread/thread.hpp>

using namespace std;
using namespace stixel_world;

namespace stixel_world_ros {

class StixelsApplicationROS
{
public:
    StixelsApplicationROS(const string & optionsFile);
    
    void runStixelsApplication();
private:
    boost::program_options::variables_map parseOptionsFile(const string& optionsFile);
    bool iterate();
    void update();
    void visualize();
    void visualize2();
    void visualize3();
    void publishStixels();
    void publishStixelsInObjects();
    bool rectifyPolar();
    void transformStixels();
    
    void publishROS();
    void publishFakePointCloud();
    void publishPointCloud(const pcl::PointCloud<pcl::PointXYZRGB>::Ptr & pointCloud);
    void publishStereoPointCloud();
    
    boost::shared_ptr<doppia::AbstractVideoInput> mp_video_input;
    boost::shared_ptr<doppia::AbstractStixelWorldEstimator> mp_stixel_world_estimator;
    boost::shared_ptr<StixelsTracker> mp_stixel_motion_estimator;
    boost::shared_ptr<oFlowTracker> mp_stixel_oflow_motion_estimator;
    boost::shared_ptr<MotionEvaluation> mp_stixel_motion_evaluator;
    
    vector< boost::shared_ptr<StixelsTracker> > mp_stixels_tests;
    
    doppia::AbstractVideoInput::input_image_t m_prevLeftRectified, m_prevRightRectified;
    doppia::AbstractVideoInput::input_image_t m_polarLt0, m_polarRt0, m_polarLt1, m_polarRt1;
    
    deque <doppia::AbstractVideoInput::input_image_t> m_frameBufferLeft, m_frameBufferRight;
    
    boost::shared_ptr<stixels_t> mp_prevStixels;
    
    stixels_t m_transfStixelsLt0, m_transfStixelsLt1;
    
    boost::program_options::variables_map m_options;
    
    boost::shared_ptr<PolarCalibration> mp_polarCalibration;
    
    vector<cv::Point2d> basePointsTransfLt0, topPointsTransfLt0, basePointsTransfLt1, topPointsTransfLt1;
    
    uint32_t m_initialFrame;
    
    uint32_t m_waitTime;
    
    cv::Mat m_currLeft, m_currRight;
    
    bool m_doPolarCalib;
    
    double m_accTime;
    
    bool m_firstIteration;
    
    uint32_t m_frameBufferLength;
    
    int m_increment;
    
    // ROS Publishers/suscribers
    ros::Publisher m_clockPub;
    ros::Publisher m_pointCloudPub;
    ros::Publisher m_fakePointCloudPub;
    ros::Publisher m_stereoPointCloudPub;
    
    image_transport::Publisher m_leftImgPub;
    image_transport::Publisher m_rightImgPub;
    
    ros::Publisher m_leftInfoPub;
    ros::Publisher m_righttInfoPub;
    sensor_msgs::CameraInfo m_leftCameraInfo, m_rightCameraInfo;
    
// protected:
//     void waitForKey(&m_waitTime arg1);
};

    
}

#endif // STIXELSAPPLICATION_H
