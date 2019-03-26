//Kevin Shannon
//usage: ./search <search-string pattern> <file-name pattern>

#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <map>
#include <sstream>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

struct alpha_only: ctype<char> {
  alpha_only(): ctype<char>(get_table()) {}
  static ctype_base::mask const* get_table() {
    static vector<ctype_base::mask>
    rc(ctype<char>::table_size,ctype_base::space);
    fill(&rc['a'], &rc['z'], ctype_base::lower);
    return &rc[0];
  }
};

typedef struct {
  bool* readyMaps;
  sem_t full;
  sem_t empty;
  sem_t mutex;
} sbuf_t;

sbuf_t shared;

void *Map(void *threadid);
void *Reduce(void* threadid);

int m, r, linesToAssign;
string file, line, split, word;
vector<string> lines;
pthread_mutex_t lock_it, map_mutex, reduce_mutex;
pthread_cond_t enough;

int reduceWorkers = 0;
int threadsWaiting = 0;
int mapsAssigned = 0;


int main(int argc, char *argv[]) {
  if(argc != 5)
    return 0;
  file = argv[1];
  m = atoi(argv[2]);
  split = argv[3];
  r = atoi(argv[4]);
  int linesAssigned = 0;
  int* skew;
  int n = 0;
  ifstream infile(argv[1]);
  while(getline(infile, line)) {
    n++;
    transform(line.begin(), line.end(), line.begin(), ::tolower);
    lines.push_back(line);
  }

  if(split == "skewed") {
    skew = new int[m];
    for(int i = 0; i < m; i++)
      skew[i] = 1;
    linesAssigned += m;
    while(linesAssigned < n) {
      for(int i = 0; i < m; i++) {
        if(n - linesAssigned >= i + 1) {
          skew[i] += i + 1;
          linesAssigned += i + 1;
        } else {
          skew[i] += n - linesAssigned;
          linesAssigned = n;
        }
      }
    }
  }

  linesAssigned = 0;
  for(int i = 0; i < m; i++) {
    if(split == "equal")
      linesToAssign = (n-linesAssigned)/(m-i);
    else if(split == "skewed")
      linesToAssign = skew[i];
    ofstream outfile("split-" + to_string(i + 1) + ".txt");
    for(int j = linesAssigned; j < linesAssigned + linesToAssign; j++)
      outfile << lines[j] << endl;
    linesAssigned += linesToAssign;
    outfile.close();
  }

  pthread_t m_threads[m];
  pthread_t r_threads[r];
  sem_init( &shared.full, 0, 0 );
  sem_init(&shared.empty, 0, m);
  sem_init(&shared.mutex, 0, 1);
  shared.readyMaps = new bool[m];

  for(long t = 0; t < m; t++) {
    shared.readyMaps[t] = 0;
    pthread_create(&m_threads[t], NULL, Map, (void *)t);
  }
  for(long t = 0; t < r; t++) {
    pthread_create(&r_threads[t], NULL, Reduce, (void *)t);
  }
  pthread_exit(NULL);
  return 0;
}

void *Map(void* threadid) {
  long tid;
  map<string, int> kv;
  tid = (long)threadid;
  string word;
  ifstream infile("split-" + to_string(tid + 1) + ".txt");
  infile.imbue(locale(locale(), new alpha_only));
  while(infile >> word) {
    if(kv.find(word) == kv.end()) {
      kv.insert(pair<string, int>(word, 1));
    } else {
      map<string, int>::iterator it = kv.find(word);
      it->second++;
    }
  }
  infile.close();
  ofstream outfile("map-out-" + to_string(tid + 1) + ".txt");
  for(auto& t : kv)
    outfile << t.first << " " << t.second << endl;
  outfile.close();

  sem_wait(&shared.empty);
  sem_wait(&shared.mutex);
  cout << "finishing map thread " << tid + 1 << endl;
  shared.readyMaps[tid] = 1;
  threadsWaiting++;
  if(threadsWaiting >= round((float)(m - mapsAssigned)/(r-reduceWorkers))) {
    sem_post(&shared.full);
    threadsWaiting -= round((float)(m - mapsAssigned)/(r-reduceWorkers));
  }
  sem_post(&shared.mutex);
  pthread_exit(NULL);
}

void *Reduce(void* threadid) {
  long tid;
  string word;
  int freq;
  map<string, int> kv;
  tid = (long)threadid;
  sem_wait(&shared.full);
  sem_wait(&shared.mutex);
  int num_inputs = round((float)(m - mapsAssigned)/(r-reduceWorkers));
  reduceWorkers++;
  for(int i = 0; i < m; i++) {
    if(shared.readyMaps[i] && num_inputs) {
      num_inputs--;
      cout << i + 1 << " -> " << tid + 1 << endl;
      ifstream infile("map-out-" + to_string(i + 1) + ".txt");
      while(infile >> word >> freq) {
        if(kv.find(word) == kv.end()) {
          kv.insert(pair<string, int>(word, freq));
        } else {
          map<string, int>::iterator it = kv.find(word);
          it->second += freq;
        }
      }
      infile.close();
      mapsAssigned++;
      shared.readyMaps[i] = 0;
    }
  }
  sem_post(&shared.mutex);
  sem_post(&shared.empty);
  ofstream outfile("reduce-out-" + to_string(tid + 1) + ".txt");
  for(auto& t : kv)
    outfile << t.first << " " << t.second << endl;
  outfile.close();
  pthread_exit(NULL);
}
