#include <thread>
#include <iostream>
#include "main.h"
// #include "definitions.h"
#include "genetics.h"
#include "mth.h"
#include "termgui.h"

using namespace mth;


// GLOBALS
////////////////////////////////////////////////////////////////////////////////
const int    NUM_OF_CANDLES = 100;
const int    NUM_OF_NEURONS = 200;
const int    SIZE_OF_POP    = 20;
const double MUTATE_CHANCE  = 0.6;
const double BATTLE_CHANCE  = 0.05;
////////////////////////////////////////////////////////////////////////////////


int main(){

  Genetics ga(SIZE_OF_POP, 4, BATTLE_CHANCE);

  // Set Up Menu -------------------------------
  TermGui gui(
    Menus({

      Menu("Main Menu",
        Choices({
          Choice({"Run Algorithm",         "1"}),
          Choice({"Load Saved Generation", "3"}),
          Choice({"Exit",                  "4"})
        })
      ),

      Menu("Paused",
        Choices({
          Choice({"Continue..",          "11"}),
          Choice({"Save Generation",     "12"}),
          Choice({"Return to Main Menu", "13"})
        })
      )

    })
  );
  // --------------------------------------------

  string result, prev_result;

  CLEAR;
  result = "Main Menu";
  while(result != "-1"){

    prev_result = result;
    result = gui.run(result);

    if     (result ==  "1"){ run_algorithm(ga, result);   }
    else if(result ==  "2"){ CLEAR; result = "Settings";  }
    else if(result ==  "3"){
      result = prev_result;
      ga.load();
      print("Finished Loading!");
      PAUSE; CLEAR;
    }
    else if(result ==  "4"){ result = "-1";               }
    else if(result == "11"){ run_algorithm(ga, result);   }
    else if(result == "12"){ 
      result = prev_result;
      ga.save();
    }
    else if(result == "13"){ CLEAR; result = "Main Menu"; }

  };

  print("\nSee you later!\n");
  PAUSE; CLEAR;
  return 0;
};


void check_for_pause(int &flag){
  std::cin.ignore();
  flag = 1;
};


void run_algorithm(Genetics &ga, string &result){
  int flag = 0;
  std::thread t(check_for_pause, std::ref(flag));

  while(flag != 1){
    ga.run();
  };

  t.join();

  result = "Paused";
};

