#include <iostream>
#include <mutex>
#include <vector>
using namespace std;

class Banker {
  vector<mutex *> mutexes;
  vector<bool> available;

 public:
  Banker(int mutex_count) {
    for (int i = 0; i < mutex_count; ++i) {
      this->mutexes.push_back(new mutex());
    }
    this->available.resize(mutex_count, 1);
  }


  void lock(vector<bool> request) {
      vector<bool> finished(this -> available.size()/mutexes.size());
      for (int i =0; i < finished.size(); ++i){
            finished[i] = 0;
        }
        int add = 0 ;
        bool safe = true;
   while (true) {
       
        vector<bool> result(this->available.size());
        vector<bool>work(this->available.size());
        
///////////////////////////////////
        for (int i = 0; i < this->available.size(); i++) {
          if (request[i]) {
            this->mutexes[i]->lock();
          }
        }
        this->available = result;
//////////////////////////////////
       //Visas bankininko algoritmas aptikti susidariusia aklaviete.

        for(int i = 0; i < available.size();++i){
            work[i] = available[i];
        }
        for(int i = 0; i <request.size(); ++i){
            if((finished[i] == 0) & (request[i] <= work[i])){
                work[i] = request[i];
                finished[i] = 1; // Pazymima, kad procesas uzbaigtas.
            }else if(finished[i] == 1){
                continue;
                
            }else{
                safe = false; // Aptinkama aklaviete, kada nera pakankamai resursu patenkinti prasyma.
                cout<< "deadlock detected . . "<<endl;
               break;
           }
              
            }
        
        if(safe == false){ //Atimami visi resursai.
            for(int i = 0; i < request.size();++i){
                request[i] = false;
            }
        }
       for(int i = 0; i <finished.size(); ++i){ //Kada visi procesai baigti, baigiamas darbas.
            add += finished[i];
            cout<<add<<endl;
            cout<<finished.size()<<endl;

        }
        if(add == finished.size()){
            break;
        }
  }

  }
  void unlock(vector<bool> request) {
    for (int i = 0; i < request.size(); i++) {
      if (request[i]) {
        this->mutexes[i]->unlock();
        this->available[i] = true;
      }
    }
  }

  void cleanup() {
    for (int i = 0; i < mutexes.size(); ++i) {
      delete this->mutexes[i];
    }
  }
};
