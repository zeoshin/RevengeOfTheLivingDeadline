#include <iostream>
#include <fstream>
#include <string.h>
#include <queue>
#include "sorted_priority_queue.h"
#include "poorman_priority_queue.h"
#include "binary_heap.h"
#include "pairing_heap.h"
#include <getopt.h>
#include <sstream>
#include <vector>

using namespace std;

//sample functor
struct zb{
  string name;
  int speed;
  int health;
  int distance;
  int round_num;
  int round_num_dead;
};

struct rd{
  int round_num;
  int ai_num;
  int spy_num;
};

/*struct dead_zb{
  int dround;
  string name;
};*/

struct les{
  bool operator()(const zb* x, 
  			  const zb* y) const{
    int ETA1 = x->distance / x->speed;
	int ETA2 = y->distance / y->speed;
	if(ETA1 < ETA2) return 0;
	else if(ETA2 < ETA1) return 1;
	else {
	  return (x->name > y->name);
	}
  }
};

struct least{
  bool operator()(const zb* x, 
  			  const zb* y) const{
    int ETA1 = x->distance / x->speed;
	int ETA2 = y->distance / y->speed;
	if(ETA1 > ETA2) return 0;
	else if(ETA2 > ETA1) return 1;
	else {
	  return (x->name > y->name);
	}
  }
};

int main(int argc, char* argv[])
{
  eecs281priority_queue<zb*, les> *zombie_pq;
  eecs281priority_queue<zb*, les> *zombie_pq2;
  eecs281priority_queue<zb*, least> *zombie_pq3;
  string line;
  string arrow_type;
  string container_type;
  int N = 0, verbose_flag = 0, c_flag  = 0, a_flag = 0, debug_flag = 0;

  extern char *optarg;
  extern int optind; 
  int arg;
  const char* const short_options = "hc:a:v:d";
  const struct option long_options[] = {
	{"help",		0,	0,	'h'},
	{"container",	1,	0,	'c'},
	{"arrow",		1,	0,	'a'},
	{"verbose",		1,	0,	'v'},
	{"debug",		0,	0,	'd'},
	{0,				0,	0,	 0 }
  };

  do{
	arg = getopt_long(argc, argv, short_options, long_options, NULL);
	switch (arg){
	case 'c':{
	  c_flag = 1;
	  if(!strcmp(optarg, "BINARY")) container_type = "BINARY";
	  else if(!strcmp(optarg, "POOR_MAN")) container_type = "POOR_MAN";
	  else if(!strcmp(optarg, "SORTED")) container_type = "SORTED";
	  else if(!strcmp(optarg, "PAIRING")) container_type = "PAIRING";
	  else {fprintf(stderr, "Wrong container type\n"); exit(1);}
	  //printf("%s\n", container_type.c_str());
	  break;
	}
	case 'a':{
	  a_flag = 1;
	  if(!strcmp(optarg, "LIGHT")) arrow_type = "LIGHT";
	  else if(!strcmp(optarg, "NORMAL")) arrow_type = "NORMAL";
	  else exit(1);
	  //printf("%s\n", arrow_type.c_str());
	  break;
	}
	case 'v':{
	  N = atoi(optarg);
	  verbose_flag = 1;
	  //printf("%d\n", N);
	  break;
	}
	case 'h':
	  exit(0);
	  break;
	case 'd':
	  debug_flag = 1;
	  break;
	case '?':
	  exit(1);
	  break;
	case -1:
	  break;
	default:
	  abort();
	}
  }while(arg != -1);

  if(!(c_flag && a_flag)) exit(1);

  string filename;
  if (optind < argc)
  {
    filename.append(argv[optind++]);
    while (optind < argc){
	  filename.append(" ");
      filename.append(argv[optind++]);
	}
	//printf("%s\n", filename.c_str());
  }
  else exit(1);

  //choose priority_queue_type
  if(container_type == "BINARY") 
	zombie_pq = new binary_heap<zb*, les>;
	zombie_pq2 = new binary_heap<zb*, les>;
	zombie_pq3 = new binary_heap<zb*, least>;
  if(container_type == "SORTED") 
	zombie_pq = new sorted_priority_queue<zb*, les>;
	zombie_pq2 = new sorted_priority_queue<zb*, les>;
	zombie_pq3 = new sorted_priority_queue<zb*, least>;
  if(container_type == "POOR_MAN")
	zombie_pq = new poorman_priority_queue<zb*, les>;
	zombie_pq2 = new poorman_priority_queue<zb*, les>;
	zombie_pq3 = new poorman_priority_queue<zb*, least>;
  if(container_type == "PAIRING"){
	zombie_pq = new pairing_heap<zb*, les>;
	zombie_pq2 = new pairing_heap<zb*, les>;
	zombie_pq3 = new pairing_heap<zb*, least>;
  }

  ifstream infile;
  infile.open(filename);
  int line_num = 0;
  int quiver_cap = 0;
  int random_seed = 0;
  int max_rand_dis = 0;
  int max_rand_speed = 0;
  int max_rand_health = 0;
  int player_health = 0;
  string temp;
  istringstream iss;
  queue<zb*> spy_zombie;
  queue<rd*> round;
  vector<zb*> zombie_info;
  vector<zb*> dead;
  while(getline(infile, line) && !line.empty()) {
	if(line_num == 0){
	  temp.assign(line, 17, line.length() - 17);
      quiver_cap = atoi(temp.c_str());
	}
	if(line_num == 1){
	  temp.assign(line, 13, line.length() - 13);
      random_seed = atoi(temp.c_str());
      srand(random_seed);
	}
	if(line_num == 2){
	  temp.assign(line, 19, line.length() - 19);
      max_rand_dis = atoi(temp.c_str());
	}
	if(line_num == 3){
	  temp.assign(line, 16, line.length() - 16);
      max_rand_speed = atoi(temp.c_str());
	}
	if(line_num == 4){
	  temp.assign(line, 17, line.length() - 17);
      max_rand_health = atoi(temp.c_str());
	}
	if(line_num == 5){
	  temp.assign(line, 15, line.length() - 15);
      player_health = atoi(temp.c_str());
	}
    if(!strcmp(line.c_str(), "---")){
	  while(getline(infile, line)){
		rd* rdt = new rd;
	    iss.str(line);
		iss >> temp >> rdt->round_num;
		iss.clear();
		getline(infile, line);
	    iss.str(line);
		iss >> temp >> rdt->ai_num;
		iss.clear();
        int spy_Num = 0;
        while(getline(infile, line) && strcmp(line.c_str(), "---")){
	      iss.str(line);
          zb* zbt = new zb;
		  iss >> zbt->distance >> zbt->speed >> zbt->health >> zbt->name;
		  zbt->round_num = rdt->round_num;
		  iss.clear();
		  zbt->round_num_dead = 0;
		  spy_zombie.push(zbt);
		  spy_Num++;
		}
		rdt->spy_num = spy_Num;
		round.push(rdt);
	  }
	}
    line_num++;
  }
  infile.close();

  //kill zombies
  string killer;
  string last_zombie;
  int curr_round = 1;
  bool player_dead = 0;
  int zb_counter = 0;
  int quiver_num = quiver_cap;
  while(!(zombie_pq->empty() && spy_zombie.empty() 
	&& round.empty()) && !player_dead){
	if(debug_flag) printf("Round: %d\n", curr_round);
	//refill quiver
	quiver_num = quiver_cap;
	//update zombie
	int first = 0;
	for(unsigned i = 0; i < zombie_info.size(); i++){
	  if(zombie_info[i]->round_num_dead == 0){
		int random_offset = (rand() % zombie_info[i]->speed) / 2;
	    zombie_info[i]->distance = 
		(zombie_info[i]->distance - 
		zombie_info[i]->speed - random_offset >= 0) ? 
	    (zombie_info[i]->distance - 
		zombie_info[i]->speed - random_offset) : 0;
	    if(debug_flag && zombie_info[i]->health){
		  printf("Moved: %s %d\n", 
		  zombie_info[i]->name.c_str(), zombie_info[i]->distance);
	    }
	    if(zombie_info[i]->distance == 0){
		  if(arrow_type == "LIGHT") player_health = 0;
		  else player_health -= zombie_info[i]->health;
	    }
	    if(player_health <= 0){
		  first++;
		  if(first == 1){
		    killer = zombie_info[i]->name;
		    player_dead = 1;
		  }
	    }
	  }
	}
	//create zombies
	if(!round.empty() && round.front()->round_num == curr_round){
	  //create ai zombies
	  for(int i = 0; i < round.front()->ai_num; i++){
	    zb* zbt = new zb;
 		zbt->distance = rand() % max_rand_dis + 1;
		zbt->speed = rand() % max_rand_speed + 1;
		zbt->health = rand() % max_rand_health + 1;
		zbt->name = "AI";
		zbt->name += to_string(zb_counter++);
		zbt->round_num = curr_round;
		zbt->round_num_dead = 0;
		zombie_pq->push(zbt);
		zombie_info.push_back(zbt);
		if(debug_flag){
		  printf("Created: %s %d\n", zbt->name.c_str(), zbt->distance);
		}
	  }
	  //create specified zombies
	  while(!spy_zombie.empty() && 
		spy_zombie.front()->round_num == curr_round){
		zombie_pq->push(spy_zombie.front());
		zombie_info.push_back(spy_zombie.front());
		if(debug_flag){
		  printf("Created: %s %d\n", 
		  spy_zombie.front()->name.c_str(), 
		  spy_zombie.front()->distance);
		}
		spy_zombie.pop();
	  }
	  round.pop();
	}
	if(player_dead) continue;
	//shooting zombies
	if(!zombie_pq->empty()) zombie_pq->make_heap();
	while(quiver_num > 0 && !zombie_pq->empty()){
	  if(!zombie_pq->empty() && 
		(zombie_pq->top()->health <= quiver_num || arrow_type == "LIGHT")){
		quiver_num -= (arrow_type == "LIGHT") ? 1 : zombie_pq->top()->health;
		zombie_pq->top()->health -= zombie_pq->top()->health;
		last_zombie = zombie_pq->top()->name;
		zombie_pq->top()->round_num_dead = curr_round;
		zombie_pq2->push(zombie_pq->top());
		zombie_pq3->push(zombie_pq->top());
		dead.push_back(zombie_pq->top());
		zombie_pq->pop();
		if(container_type == "PAIRING") zombie_pq->make_heap();
	  }
	  else if(!zombie_pq->empty() && 
		zombie_pq->top()->health > quiver_num){
		zombie_pq->top()->health -= quiver_num;
		quiver_num = 0;
	  }
	  else{}
	}
	curr_round++;
  }
  if(player_dead)
	printf("DEFEAT IN ROUND %d! %s ate your brains!\n", 
	curr_round, killer.c_str());
  else 	printf("VICTORY IN ROUND %d! %s was the last zombie. You survived with %d health left.\n", curr_round - 1, 
  last_zombie.c_str(), player_health);

  if(verbose_flag){
	printf("Zombies still active: %d\n", zombie_pq->size());
	int range = (N > dead.size()) ? dead.size() : N;
	printf("First zombies killed:\n");
	for(unsigned i = 0; i < range && i < dead.size(); i++){
	  printf("%s %d\n", dead[i]->name.c_str(), i + 1);
	}
	printf("Last zombies killed:\n");
	for(unsigned i = 0; i < range && i < dead.size(); i++){
	  printf("%s %d\n", dead[dead.size() - 1 - i]->name.c_str(), range - i);
	}
  }
  if(player_dead){
    if(verbose_flag){
	  for(unsigned i = 0; i < zombie_info.size(); i++){
	    if(zombie_info[i]->round_num_dead == 0){
		  zombie_info[i]->round_num_dead = curr_round;
		  zombie_info[i]->round_num--;
	    }
	    else zombie_info[i]->round_num--;
	    zombie_info[i]->distance = (zombie_info[i]->round_num_dead 
	    - zombie_info[i]->round_num)*(-zombie_info[i]->speed);
	  }
	  while(!zombie_pq->empty()){
	    zombie_pq2->push(zombie_pq->top());
	    zombie_pq3->push(zombie_pq->top());
	    zombie_pq->pop();
	  }
	  zombie_pq2->make_heap();
	  printf("Most active zombies:\n");
	  for(unsigned i = 0; i < N && !zombie_pq2->empty(); i++){
	    printf("%s %d\n", zombie_pq2->top()->name.c_str(), 
		zombie_pq2->top()->round_num_dead - zombie_pq2->top()->round_num);
	    zombie_pq2->pop();
		if(container_type == "PAIRING") zombie_pq2->make_heap();
	  }
	  zombie_pq3->make_heap();
	  printf("Least active zombies:\n");
	  for(unsigned i = 0; i < N && !zombie_pq3->empty(); i++){
	    printf("%s %d\n", zombie_pq3->top()->name.c_str(), 
		zombie_pq3->top()->round_num_dead - zombie_pq3->top()->round_num);
	    zombie_pq3->pop();
		if(container_type == "PAIRING") zombie_pq3->make_heap();
	  }
	}
  }
  else{
    if(verbose_flag){
	  for(unsigned i = 0; i < zombie_info.size(); i++){
	    if(zombie_info[i]->round_num_dead == 0){
		  zombie_info[i]->round_num_dead = curr_round;
	    }
	    zombie_info[i]->distance = (zombie_info[i]->round_num_dead 
	    - zombie_info[i]->round_num)*(-zombie_info[i]->speed);
	  }
	  while(!zombie_pq->empty()){
	    zombie_pq2->push(zombie_pq->top());
	    zombie_pq3->push(zombie_pq->top());
	    zombie_pq->pop();
	  }
	  zombie_pq2->make_heap();
	  printf("Most active zombies:\n");
	  for(unsigned i = 0; i < N && !zombie_pq2->empty(); i++){
	    printf("%s %d\n", zombie_pq2->top()->name.c_str(), 
		zombie_pq2->top()->round_num_dead - zombie_pq2->top()->round_num + 1);
	    zombie_pq2->pop();
		if(container_type == "PAIRING") zombie_pq2->make_heap();
	  }
	  zombie_pq3->make_heap();
	  printf("Least active zombies:\n");
	  for(unsigned i = 0; i < N && !zombie_pq3->empty(); i++){
	    printf("%s %d\n", zombie_pq3->top()->name.c_str(), 
		zombie_pq3->top()->round_num_dead - zombie_pq3->top()->round_num + 1);
	    zombie_pq3->pop();
		if(container_type == "PAIRING") zombie_pq3->make_heap();
	  }
	}
  }
  return 0;
}
