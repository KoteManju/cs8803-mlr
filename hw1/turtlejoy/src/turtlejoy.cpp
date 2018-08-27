#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>

class TurtleJoy {
    public:
        TurtleJoy();
    private:
        void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);

        ros::NodeHandle nh;
        
        ros::Publisher vel_pub;
        ros::Subscriber joy_sub;

        int linear_axis_pos = 5;
        int linear_axis_neg = 2;
        int angular_axis = 0;

        double l_scale = 1.0;
        double a_scale = 1.0;
};

TurtleJoy::TurtleJoy() {
    vel_pub = nh.advertise<geometry_msgs::Twist>("/vrep/twistCommand", 1);
    joy_sub = nh.subscribe<sensor_msgs::Joy>("joy", 10, &TurtleJoy::joyCallback, this);
}

void TurtleJoy::joyCallback(const sensor_msgs::Joy::ConstPtr& joy) {
    geometry_msgs::Twist twist;
    twist.angular.z = a_scale * joy->axes[angular_axis];
    twist.linear.x = l_scale * (joy->axes[linear_axis_neg] - joy->axes[linear_axis_pos]) / 2.0;
    vel_pub.publish(twist);
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "turtlejoy");
    TurtleJoy turtle_joy;
    
    ros::spin();
}
