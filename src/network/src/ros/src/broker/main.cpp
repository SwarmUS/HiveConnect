#include "CommunicationBroker.h"
#include <ros/ros.h>

int main(int argc, char** argv) {
    ros::init(argc, argv, "communication_broker");

    CommunicationBroker communicationBroker;
    ros::Rate r(10); // 10 hz
    while (ros::ok()) {
        ros::spinOnce();
        r.sleep();
    }

    return 0;
};