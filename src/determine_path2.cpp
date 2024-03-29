#include <ros/ros.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Int32MultiArray.h>
#include <sensor_msgs/LaserScan.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <iostream>
#include <fstream>

#define PI 3.1415926
using namespace std;

class Planner{
	public:
		Planner();
	private:
		ros::NodeHandle nh_;
		ros::Publisher pub_;
		ros::Publisher testpub;
                ros::Publisher valleypub;
                ros::Publisher boundrypub;                          
		ros::Subscriber sub_;
		void scanVal(const sensor_msgs::LaserScan::ConstPtr& laser);
};
	
Planner::Planner(){
	sub_ = nh_.subscribe<sensor_msgs::LaserScan>("/space1/scan", 1, &Planner::scanVal, this);
	pub_ = nh_.advertise<std_msgs::Float32MultiArray>("det_out", 1);
	testpub = nh_.advertise<std_msgs::Int8>("test", 1);
        valleypub=nh_.advertise<std_msgs::Float64MultiArray>("valleyarray", 1);
        boundrypub=nh_.advertise<std_msgs::Int32MultiArray>("valleybou", 1);

}


void Planner::scanVal(const sensor_msgs::LaserScan::ConstPtr& laser){

        std_msgs::Float32MultiArray det;
	det.data.clear();
	//test messgas define
        std_msgs::Int8 r;
        std_msgs::Float64MultiArray valleyarray;
        std_msgs::Int32MultiArray valleybou;

	int n = laser -> ranges.size();
        float unit=laser -> angle_increment*(180/PI);

        int count=0;  
        float value;
        float threshold =0.5f;//the obstacle distance that car still can avoid
        double car_size=0.3;
        vector<int> det_safe;
        vector<int> valley;
        vector< vector<int> > valley_set;
        vector<double> subdirection;
        vector<double> delta_vector;
        double dire;
        double direction=90.0;
        double delta;
         
        double theta =acos(1-pow(car_size,2.0)/(pow(threshold,2.0)*2))*180/PI;
        int limit=round(2*theta/unit);


        //take care of "nan" data and normalize range 
	bool stuck=0;
	//int abc=0;
	const char *path="/home/odroid/lin_ws/src/zumo/src/range.csv";
	ofstream file(path);

        for (int i=0; i<n; i++){
            value= laser -> ranges[i];
	    //file<<abc<<" ";
	    file<<value<<"\n";
	    //abc=abc+1;

	       if (i == (n+1)/2){
		  det.data.push_back(value);
	       }
		   
            if (!isnan(value)){

	      // if (i == (n+1)/2){
		  //det.data.push_back(value);
	       //}

               if (value >= threshold){
                  det_safe.push_back(1);//safe==1
               }
               else{
                  det_safe.push_back(0);//unsafe==0
               }

	       if (value <=0.25){
		  stuck=1;
	       }

             }
             else{   
                 det_safe.push_back(1);
             }
                 
        }  
       
        //count beams in safe valleys
        valleybou.data.clear();
        for (int j=0; j< n; j++){
             
             if ((det_safe[j] == 0) || (j== n-1)   ){

                  if (det_safe[j] ==1){
                      count++;     
                  }         
                  if (!(count ==0) && count >= limit ){
                                         
                      valley.push_back(j);
                      valleybou.data.push_back(j);

                      valley.push_back(count);
		      valleybou.data.push_back(j-count);
                      valley_set.push_back(valley);
                      
                      valley.clear(); 

                  }
                  count=0; 
                  continue;

              }
              else{
                  count++;
              }
                  
        }

        valleyarray.data.clear();
        for(int i=0; i<valley_set.size();i++){
                dire=(valley_set[i][0]-valley_set[i][1]/2)*unit;
                delta=abs(dire-direction);
                subdirection.push_back(dire);
                valleyarray.data.push_back(dire);
                delta_vector.push_back(delta);              
        }

        //choose the subfirection wihch closest to direction
        int min_index=min_element(delta_vector.begin(),delta_vector.end())- delta_vector.begin();

        if (valley_set.size() == 0 ||stuck==1  ){
             det.data.push_back(-1.0);//no safe valley to go, car rotate to find another direction 
               
        }       

        else{
               det.data.push_back(subdirection[min_index]);
      
      }       
       

	r.data = limit; 
        
        boundrypub.publish(valleybou);
	testpub.publish(r);
        valleypub.publish(valleyarray);
	pub_.publish(det);

}

int main(int argc, char **argv){
	ros::init(argc, argv, "Planner");
	ROS_INFO_STREAM("Hokuyo Analyzer node active!");
	Planner planner;

	while(ros::ok()){
		ros::spin();
	}
}
