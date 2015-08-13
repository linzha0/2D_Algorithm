#include <ros/ros.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/Int32MultiArray.h>
#include <sensor_msgs/LaserScan.h>
#include <vector>
#include <algorithm>
#include <math.h>

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
                ros::Publisher countypub;                          
		ros::Subscriber sub_;
		void scanVal(const sensor_msgs::LaserScan::ConstPtr& laser);
};
	
Planner::Planner(){
	sub_ = nh_.subscribe<sensor_msgs::LaserScan>("/space1/scan", 1, &Planner::scanVal, this);
	pub_ = nh_.advertise<std_msgs::Int8>("det_out", 1);
	testpub = nh_.advertise<std_msgs::Float64>("test", 1);
        valleypub=nh_.advertise<std_msgs::Float64MultiArray>("valleyarray", 1);
        boundrypub=nh_.advertise<std_msgs::Int32MultiArray>("valleybou", 1);
        countypub=nh_.advertise<std_msgs::Int32MultiArray>("count_", 1);

}


void Planner::scanVal(const sensor_msgs::LaserScan::ConstPtr& laser){

        std_msgs::Int8 det;
        std_msgs::Float64 r;
        std_msgs::Float64MultiArray valleyarray;
        std_msgs::Int32MultiArray valleybou;
        std_msgs::Int32MultiArray count_;

	int n = laser -> ranges.size();
        float unit=laser -> angle_increment*(180/3.1415926);

        int count=0;  
        float value;
        float threshold =0.2f;
        double car_size=0.1;
        vector<int> det_safe;
        vector<int> valley;
        vector< vector<int> > valley_set;
        vector<double> subdirection;
        vector<double> delta_vector;
        double dire;
        double direction=90.0;
        double delta;
         
        double theta =acos(1-pow(car_size,2.0)/(pow(threshold,2.0)*2));
        int limit=round(1.5*theta/unit);

	ROS_INFO_STREAM("Planner::scanVal!");

        //take care of "nan" data and normalize range 
        for (int i=0; i<n; i++){
             value= laser -> ranges[i];
            if (!isnan(value)){

               if (value >= threshold){
                  det_safe.push_back(1);
               }
               else{
                  det_safe.push_back(0);
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
                      count_.data.push_back(count);
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
        double rotate=abs(direction-subdirection[min_index]);

        if (valley_set.size() == 0){
             det.data=1;//no safe valley to go, car backward 
               
        }       

        else{
       
            if(subdirection[min_index] <90){//turn right

               if (rotate >= 60.0){
                  det.data=8; //60 degree
               }
               else if (rotate >=30.0 && rotate <60.0){
                  det.data=9; //30 degree
               }
               else if (rotate >=10.0 && rotate <30.0){
                  det.data=10; //10 degree
               }
               else if (rotate >=5.0 && rotate <10.0){
                  det.data=11; //5 degree
               }
               else if(rotate >=2.0 && rotate <5.0) {
                  det.data=12; //1 degree
               }
               else{
                  det.data=2;//go forward
               }

            }
           else{//turn left

              if (rotate >= 60.0){
                  det.data=3; // 60 degree
              }
              else if (rotate >=30.0 && rotate <60.0){
                  det.data=4; // 30 degree
              }
              else if (rotate >=10.0 && rotate <30.0){
                 det.data=5; //10 degree
              }
              else if (rotate >=5.0 && rotate <10.0){
                 det.data=6; // 5 degree
              }
              else if (rotate >=2.0 && rotate <5.0)  {
                 det.data=7; // 1 degree
              }
              else{
                  det.data=2;//go forward
              }
          }
      }       
       

	r.data = rotate; 
        
        countypub.publish(count_);
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
