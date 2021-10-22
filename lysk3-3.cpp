#include <mutex>
#include <iostream>
#include <thread>
#include <vector>
using namespace std;

const int workers = 3;
const int mutex_count = 3;
mutex m1;
mutex m2;
mutex m3;
///////Veliau pakeisiu. Kol kas naudojami masyvai.

///Pagal bankininko algoritma bandoma. 

void getNeed();
bool safeCheck();
void worker(int i);
void shared_cout_thread_even(int i);
void shared_cout_thread_odd(int i);
void Avoidance();
int max_res[workers][mutex_count] = {{1, 1, 1}, // daugiausia galimu resursu
                       {1, 1, 1},
                       {1,1, 1}} ;


int need[workers][mutex_count] = {{1, 1, 1}, //kiek reikia procesams resursu
                       {1, 1, 1},
                       {1 ,1, 1}} ;

int avail_res[mutex_count] = {1, 1, 1}; // kiek yra resursu
            
 int alloc_res[workers][mutex_count] = {{0, 0, 0}, //kiek skiriama resursu
                       {0, 0, 0},
                       {0 ,0, 0}} ; 

//int finished[workers];

int main(){
    thread tid[workers];

  for (int i = 0; i < workers; ++i) {
    tid[i] = thread(worker, i);
  }
  for (int i = 0; i < workers; ++i) {
    tid[i].join();
  }
}

void init(){
    for (int i = 0; i< workers; i++){
        for(int j = 0; j < workers; j++){
            max_res[i][j] = 1;
            need[i][j] = 1;
            avail_res[j] = 1;
            alloc_res[i][j] = 1;
            //finished[j] = 0;

        }
    }
}
///funkcija nustatyti, ar yra deadlock
bool safeCheck(){
    bool safe = true;
    for(int i = 0; i < workers; i++){
        for (int j = 0; j < workers; j++){
            if(need[i][j] > avail_res[i]){
                safe = false;
            }
        }
    }
    return safe;

}

//funkcija nustatyti, kiek resusrsu reikes gijai
void getNeed(){
    for(int i = 0; i < workers; i++){
        for (int j = 0; j < workers; j++){
            need[i][j] = avail_res[i] - alloc_res[i][j];
        }
    }
}

/*void Check_Resources(){
   
        //m1.unlock();
        avail_res[0] = 0;
        for(int i = 0; i < workers; i ++){
            alloc_res[i][0] = 1;
        }
        getNeed();


        cout<<"fff"<<endl;
        //m2.unlock();
        avail_res[1] = 0;
        for(int i = 0; i < workers; i ++){
            alloc_res[i][1] = 1;
        }
        getNeed();

   
        cout<<"hgkkg"<<endl;
        //m3.unlock();
        for(int i = 0; i < workers; i ++){
            alloc_res[i][2] = 1;
        }
        avail_res[2] = 0;
    
    getNeed();
}*/
//funkcija pasalinti deadlock'us
void Avoidance(){
    for (int i = 0; i < workers; i++) {
    for (int j = 0; j < workers; j++) {
        getNeed();
     
  
       bool safe = safeCheck();
       if(!safe){
           cout<<"s"<<endl;
           break;
       }
        if (safe && need[i][j] == 0){
          for (int k = 0; k < mutex_count; k++){
              m1.unlock();
              m2.unlock();
              m3.unlock();
           /* if(k == 0 ){ m1.unlock();
            }
            if(k == 1 ){ m2.unlock();
            }
            if(k == 2 ){ m3.unlock();
           }*/
            avail_res[k] += alloc_res[i][k];
           
            
        }
      
      }
    }
  }
}

void worker(int i){
    while(true){
        init();
        getNeed();
        Avoidance();
       // Check_Resources();
        
    if (i % 2 == 0){
        shared_cout_thread_even(i);
        
    }else{
        shared_cout_thread_odd(i);
    }  
      }

}

void shared_cout_thread_even(int i){
  lock_guard<mutex> g1(m1);
  /*avail_res[0] = 0;
  alloc_res[i][0] = 1;
  need[i][0] = 0;*/
  //avoidance.Resources(i,0);
  lock_guard<mutex> g2(m2);
  /*avail_res[1] = 0;
  alloc_res[i][1] = 1;
  need[i][1] = 0;*/

  //avoidance.Resources(i,1);
  lock_guard<mutex> g3(m3);
  /*avail_res[2] = 0;
  alloc_res[i][2] = 0;
  need[i][2] = 0;*/
  cout <<" " <<  i << " " << endl;
}

void shared_cout_thread_odd(int i) {
  lock_guard<mutex> g3(m3);
/*avail_res[2] = 0;
  alloc_res[i][2] = 0;
  need[i][2] = 0;*/
//avoidance.Resources(i,2);
  lock_guard<mutex> g2(m2);
  /*avail_res[1] = 0;
  alloc_res[i][1] = 1;
  need[i][1] = 0;*/
//avoidance.Resources(i,1);
  lock_guard<mutex> g1(m1);
   /*avail_res[0] = 0;
  alloc_res[i][0] = 1;
  need[i][0] = 0;*/
 // avoidance.Resources(i,0);
        cout << " " << i << " ";
  
  
  
}