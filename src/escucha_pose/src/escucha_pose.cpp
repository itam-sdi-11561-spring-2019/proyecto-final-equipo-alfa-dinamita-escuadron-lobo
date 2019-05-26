#include <ros/ros.h>
#include <geometry_msgs/Pose2D.h>
#include <math.h>
using namespace std;

#define pi 3.1415926536

float x=0, y=0, theta=0, bx=0, by=0, bt=0, dx=0, dy=0;

void callbackPose(const geometry_msgs::Pose2D &poseRobot)
{
	x = poseRobot.x;
	y = poseRobot.y;
	theta = poseRobot.theta;
	dx = bx-x;
	dy = by-y;
}

void callbackBall(const geometry_msgs::Pose2D &poseBall)
{
	bx = poseBall.x;
	by = poseBall.y;
	bt = poseBall.theta;
	dx = bx-x;
	dy = by-y;
	
}

int main(int argc, char **argv)
{
	ros::init(argc,argv,"escucha_pose_node");
	ros::NodeHandle n;
	
	ros::Subscriber sub_pose = n.subscribe("/y_r0", 1000, &callbackPose);
	ros::Subscriber sub_ball = n.subscribe("/ball", 1000, &callbackBall); // Navegacion sin obstaculos...
	ros::Publisher pub_dif = n.advertise<geometry_msgs::Pose2D> ("/pose_dif",1);
	
	ros::Rate rate(2);
	
	geometry_msgs::Pose2D dif;
	
	while(ros::ok())
	{
		float d, alpha, v, w;
		d = sqrt(dx*dx+dy*dy);
		alpha = atan2(dy,dx)-theta;
		v = d/10;
		w = alpha/5;
		dif.x = (v-115/2*w)/21/2/pi;
		dif.y = (v+115/2*w)/21/2/pi;
		dif.theta = 0.0;
		pub_dif.publish(dif);
		
		ros::spinOnce();
		rate.sleep();
	}
}
