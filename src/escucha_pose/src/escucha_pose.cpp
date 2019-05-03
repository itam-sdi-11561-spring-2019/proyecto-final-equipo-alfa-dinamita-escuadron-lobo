#include <ros/ros.h>
#include <geometry_msgs/Pose2D.h>
#include <std_msgs/Float64.h>
#include <cmath>
#include <math.h>
using namespace std;

float x, y, theta, bx, by, bt;

void callbackPose(const geometry_msgs::Pose2D &pose)
{
	x = pose.x;
	y = pose.y;
	theta = pose.theta;
}

void callbackBall(const geometry_msgs::Pose2D &pose)
{
	bx = pose.x;
	by = pose.y;
	bt = pose.theta;
}

int main(int argc, char **argv)
{
	ros::init(argc,argv,"escucha_pose_node");
	ros::NodeHandle n;
	
	ros::Subscriber sub_pose = n.subscribe("/y_r0", 1000, &callbackPose);
	ros::Subscriber sub_ball = n.subscribe("/ball", 1000, &callbackBall);
	ros::Publisher pub_dif = n.advertise<geometry_msgs::Pose2D> ("/pose_dif",1);
	
	geometry_msgs::Pose2D dif;
	
	float dx = bx-x;
	float dy = by-y;
	
	while(ros::ok())
	{
		dif.x = sqrt(pow(dx,2)+pow(dy,2));
		dif.y = 20;
		dif.theta = atan2(dy,dx);
		pub_dif.publish(dif);
		
		ros::spinOnce();
	}
}
