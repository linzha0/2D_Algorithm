#include <ros/ros.h>
#include <std_msgs/Int8.h>
#include <sensor_msgs/LaserScan.h>

class Control{
	public:
		Control()
		{
		 detSub = nh.subscribe("det_out", 1, &Control::ReceiveDet, this); 
		 pub = nh.advertise<std_msgs::Int8>("move_zumo", 1);
		}

		void ReceiveDet(std_msgs::Int8 det);

	private:
		ros::NodeHandle nh;
		ros::Subscriber detSub;
		ros::Publisher pub;

};
	
//Control::Control(void){
//}

void Control::ReceiveDet(std_msgs::Int8 det){
	std_msgs::Int8 out;
	out.data = det.data;
        pub.publish(out);
}

int main(int argc, char **argv){
	ros::init(argc, argv, "Zumo");
	ROS_INFO_STREAM("Zumo Controller Active!");

	//ros::NodeHandle nh;
	Control control;
	//ros::Subscriber detSub = nh.subscribe("det_out", 1, &Control::ReceiveDet, &control);
	//ros::Publisher pub = nh.advertise<std_msgs::Int8>("move_zumo", 1);
	
	ros::Rate rate(1);
	while(ros::ok()){
		ros::spinOnce();
		//pub.publish(control.out);
		rate.sleep();
	}
}
