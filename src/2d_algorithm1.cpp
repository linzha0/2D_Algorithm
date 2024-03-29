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
		double userinput;
		int interrupt;
	private:
		ros::NodeHandle nh_;
		ros::Publisher pub_;
		ros::Publisher testpub;
        	ros::Publisher subdirections_pub;
        	ros::Publisher boundrypub;                          
		ros::Subscriber sub_laser;
		ros::Subscriber sub_xbox;
		void scanVal(const sensor_msgs::LaserScan::ConstPtr& laser);
		void receiveDirect(const std_msgs::Float32  xbox);
};
	
Planner::Planner(){
	sub_laser = nh_.subscribe<sensor_msgs::LaserScan>("scan", 1, &Planner::scanVal, this);
	sub_xbox = nh_.subscribe<std_msgs::Float32>("direction", 1, &Planner::receiveDirect, this);
	pub_ = nh_.advertise<std_msgs::Float32MultiArray>("det_out", 1);
	testpub = nh_.advertise<std_msgs::Int8>("debug_test", 1);
   	subdirections_pub=nh_.advertise<std_msgs::Float64MultiArray>("debug_subdirections", 1);
        boundrypub=nh_.advertise<std_msgs::Int32MultiArray>("debug_valleybou", 1);

}

void Planner::receiveDirect(std_msgs::Float32 xbox){

	double temp=xbox.data;
	if (temp != -1.0){

	  userinput=temp;
	  interrupt=0;
	}
	else{
	  interrupt=-1;//interrupt the rover to stop 
	}
}

void Planner::scanVal(const sensor_msgs::LaserScan::ConstPtr& laser){

        std_msgs::Float32MultiArray det;
	    det.data.clear();
//test messgas define
        std_msgs::Int8 r;
        std_msgs::Float64MultiArray sub_directions;
        std_msgs::Int32MultiArray valleybou;

	    int n = laser -> ranges.size();
        float unit=laser -> angle_increment*(180/PI);

        int count=0;  
        float value;
        float threshold =0.5f;//the obstacle distance that car still can avoid
        double car_size=0.3;
        vector<int> det_safe;
         
        double theta =acos(1-pow(car_size,2.0)/(pow(threshold,2.0)*2))*180/PI;
        int limit=round(2.5*theta/unit);//number of vectors in a minimum safe valley

//take care of "nan" data and normalize range 
	bool stuck=0;
	//int abc=0;
	//const char *path="/home/odroid/lin_ws/src/zumo/src/range.csv";
	//ofstream file(path);

        for (int i=0; i<n; i++){
            value= laser -> ranges[i];
	    //file<<abc<<" ";
	   // file<<value<<"\n";
	    //abc=abc+1;

	       /*if (i == (n+1)/2){
		  det.data.push_back(value);
	       }*/
		   
            if (!isnan(value)){

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
		vector<double> valley;
		vector< vector<double> > valley_set;
		vector<double> safe_boundary;
		vector< vector<double> > safe_area;
		vector<double> subdirection;

        valleybou.data.clear();
        for (int j=0; j< n; j++){
             
             if ((det_safe[j] == 0) || (j== n-1)   ){

                  if (det_safe[j] ==1){
                      count++;     
                  }         
                  if (!(count ==0) && count >= limit ){
                      
					  double right_bound = (j - count)*unit;
					  double left_bound = j*unit;
                      valley.push_back(right_bound);
					  valley.push_back(left_bound);
					  valley_set.push_back(valley);
					  valley.clear();

					  safe_boundary.push_back(right_bound + limit*unit / 2);
					  safe_boundary.push_back(left_bound - limit*unit / 2);
					  safe_area.push_back(safe_boundary);
					  safe_boundary.clear();

					  subdirection.push_back(right_bound + limit*unit / 2);
					  subdirection.push_back(left_bound - limit*unit / 2);

					  sub_directions.data.push_back(right_bound + limit*unit / 2);
					  sub_directions.data.push_back(left_bound + limit*unit / 2);

					  valleybou.data.push_back(j - count);
					  valleybou.data.push_back(j);                     

                  }
                  count=0; 
                  continue;

              }
              else{
                  count++;
              }
                  
        }

		det.data.push_back(interrupt);
		bool inside = 0;
		for (int i = 0; i < safe_area.size(); i++){
			if (userinput >= safe_area[i][0] && userinput <= safe_area[i][1]){

			    inside = 1;
				break;
			}
		}

		if (inside){
			det.data.push_back(userinput);
		}
		else
		{
			double delta;
			vector<double> delta_vector;
			for (int i = 0; i < subdirection.size(); i++){
				delta = abs(subdirection[i] - userinput);
				delta_vector.push_back(delta);
			}

			//choose the subfirection wihch closest to direction
			int min_index = min_element(delta_vector.begin(), delta_vector.end()) - delta_vector.begin();

			if (valley_set.size() == 0 || stuck == 1){
				det.data.push_back(-2.0);//no safe valley to go, car rotate to find another direction 

			}

			else{
				det.data.push_back(subdirection[min_index]);

			}
		}

       /* valleyarray.data.clear();
        for(int i=0; i<valley_set.size();i++){
                dire=(valley_set[i][0]-valley_set[i][1]/2)*unit;
                delta=abs(dire-userinput);
                subdirection.push_back(dire);
                valleyarray.data.push_back(dire);
                delta_vector.push_back(delta);              
        }

        if (valley_set.size() == 0 ||stuck==1  ){
             det.data.push_back(-2.0);//no safe valley to go, car rotate to find another direction 
               
        }       

        else{
             det.data.push_back(subdirection[min_index]);
      
      }       */
       

	r.data = limit; 
        
        boundrypub.publish(valleybou);
	testpub.publish(r);
        subdirections_pub.publish(sub_directions);
	pub_.publish(det);

}

int main(int argc, char **argv){
	ros::init(argc, argv, "algorithm_node");
	ROS_INFO_STREAM("Algorithm node active!");
	Planner planner;

	ros::spin();
	return 0;
}
