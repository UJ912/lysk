
#include <mutex>
#include <vector>
#include <iostream>
#include "timeout.h" // nes kol kas reikia ir čia ir main, bet duplikuojasi jeigu main naudoju, tai čia naudoju lol

using namespace std;

vector<int> avail_res; // vector for checking if a lock is available -- value is either 0 or 1
vector<int> flag; // vector for flag checking if a process has completed it's job -- value is either 0 or 1



class Removal{
  vector<mutex *> mutexes;

public:
  Removal(int mutex_count){
    for (int i = 0; i < mutex_count; ++i){
      this->mutexes.push_back(new mutex());
    }

  }

/*Ka sita funkcija padaro :
-Jei yra deadlock, t.y. procesas neuzbaigtas ir nera norimo resurso,  tada:
paraso kad sistema yra aklavieteje, atrakina visus muteksus(atlaisvina resursus
- pagal tai padaro, kad visi resursai yra available
- po to paraso, kad visi like procesai yra uzbaigti.
-po to kiti procesai vyksta
*/
  void Recovery(){
      //Jei yra aklaviete, visi procesai 'nuzudomi' lol. Cia galbut nera pats geriausias metodas bet vis delto pabandziau su kitu atveju ir siaip turetu veikti.
      cout << "Sistema yra aklavieteje. Stabdomi visi procesai . . ."<<endl;
          for(int i = 0 ; i < mutexes.size(); i++){
          this->mutexes[i]->unlock();}
          avail_res.resize(mutexes.size(),1);
          flag.resize(mutexes.size(),1);
          cout << "Aklaviete pasalinta. Procesai sustabdyti. "<<endl;
  }


  void lock(int i){
    while(true){
        
      if(flag[i] == 0){
        if(avail_res[i] == 1){
          this->mutexes[i]->lock();
          avail_res[i] = 0;
          flag[i] = 1;
        }
        else if(avail_res[i] == 0){ 
           Recovery();
           
          continue;
        }
      }
      if(flag[i] == 1){
        break;
      }
    }

  }

  void unlock(int i){
    this->mutexes[i]->unlock();
    avail_res[i] = 1;
    flag[i] = 0;
  }

  void cleanup(){
    for (int i = 0; i < mutexes.size(); ++i){
      delete this->mutexes[i];
    }
  }
};
