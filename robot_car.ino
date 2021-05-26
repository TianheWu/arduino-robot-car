#define MAX_NUM_POINT 50
#define MAX_NUM_VECTOR 10
#define MAX_NUM_QUEUE 200
#define N 7
#define M 7


int FRONT_BLOCKED = 0;
int BACK_BLOCKED = 0;
int RIGHT_BLOCKED = 0;
int LEFT_BLOCKED = 0;
int FORWARD = 1;
int FIRST_TRAVERSAL = 0;


// define queue
template <typename T>
class queue {
private:
  int m_size;
  int q_front;
  int rear;
  T* arr;
  
public:
  queue();
  void push(T item);
  void pop();
  T front();
  bool empty();
};

template <typename T>
queue<T>::queue() {
  m_size = MAX_NUM_QUEUE;
  arr = new T[m_size];
  q_front = 0;
  rear = 0;
}

template <typename T>
void queue<T>::push(T item) {
  arr[rear] = item;
  rear = (rear + 1) % m_size;
}

template <typename T>
void queue<T>::pop() {
  q_front = (q_front + 1) % m_size;
}

template <typename T>
T queue<T>::front() {
  return arr[q_front];
}

template <typename T>
bool queue<T>::empty() {
  return q_front == rear;
}


// define vector
template <typename T>
class vector {
private:
  int my_first;
  int my_last;
  T arr[MAX_NUM_VECTOR];
  
public:
  vector();
  void push_back(T item);
  int size();
  T& operator[](int i) {
    return arr[i];
  }
};

template <typename T>
vector<T>::vector() {
  my_first = 0;
  my_last = -1;
}

template <typename T>
void vector<T>::push_back(T item) {
  my_last++;
  arr[my_last] = item;
}

template <typename T>
int vector<T>::size() {
  return my_last - my_first + 1;
}


struct Point {
  int x, y;
};

struct Edge {
  int u, v;
  char c;
  Edge() {}
  Edge(int _u, int _v, char _c) : u(_u), v(_v), c(_c) {}
};

Point s_pos, e_pos;
int s_idx, e_idx;
int d[MAX_NUM_POINT];
char _map[N][M];

bool visited[MAX_NUM_POINT];
vector<char> ans;
vector<Edge> edges;
vector<int> G[MAX_NUM_POINT];


void setup() {
  // put your setup code here, to run once:
  
}

void init_start_end() {
  s_pos.x = 0;
  s_pos.y = 0;
  e_pos.x = 9;
  e_pos.y = 9;
  s_idx = s_pos.x * M + s_pos.y;
  e_idx = e_pos.x * M + e_pos.y;
}

void init_map() {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      int cur_idx = i * M + j;
      if (_map[i][j] == 'X')
        continue;
      if (i - 1 >= 0 && (_map[i - 1][j] == '.' || _map[i - 1][j] == 'E' || _map[i - 1][j] == 'S'))
        add_edge(cur_idx, (i - 1) * M + j, 'U');
      if (i + 1 < N && (_map[i + 1][j] == '.' || _map[i + 1][j] == 'E' || _map[i + 1][j] == 'S'))
        add_edge(cur_idx, (i + 1) * M + j, 'D');
      if (j - 1 >= 0 && (_map[i][j - 1] == '.' || _map[i][j - 1] == 'E' || _map[i][j - 1] == 'S'))
        add_edge(cur_idx, i * M + j - 1, 'L');
      if (j + 1 < M && (_map[i][j + 1] == '.' || _map[i][j + 1] == 'E' || _map[i][j + 1] == 'S'))
        add_edge(cur_idx, i * M + j + 1, 'R');
    }
  }
}

void add_edge(int u, int v, char c) {
  edges.push_back(Edge(u, v, c));
  int index = edges.size() - 1;
  G[u].push_back(index);
}

void rev_bfs() {
  memset(visited, false, sizeof(visited));
  queue<int> que;
  que.push(e_idx);
  visited[e_idx] = true;
  d[e_idx] = 0;
  while (!que.empty()) {
    int u = que.front();
    que.pop();
    for (int i = 0; i < G[u].size(); i++) {
      int e = G[u][i];
      int v = edges[e].v;
      if (!visited[v]) {
        visited[v] = true;
        d[v] = d[u] + 1;
        que.push(v);
      }
    }
  }
}

void bfs() {
  queue<int> que;
  que.push(s_idx);
  while (!que.empty()) {
    int ele = que.front();
    que.pop();
    int node = -1;
    char min_c = 'z';
    for (int i = 0; i < G[ele].size(); i++) {
      int u = G[ele][i];
      if (d[ele] - d[edges[u].v] == 1 && edges[u].c < min_c) {
        node = edges[u].v;
        min_c = min(min_c, edges[u].c);
      }
    }
    if (node != -1) {
      que.push(node);
      ans.push_back(min_c);
    }
  }
}


void fill_map() {

}


void stop_robot_car() {

}


void go_straight() {
 
}


void go_backwards() {
 
}


void turn_left() {

}


void turn_right() {

}


void turn_left_backwards() {

}


void turn_right_backwards() {

}


bool check_end() {
  go_straight();
  return ((!FRONT_BLOCKED) && (!BACK_BLOCKED) && (!LEFT_BLOCKED) && (!RIGHT_BLOCKED));
  go_backwards();
}


void loop() {
  if (!FIRST_TRAVERSAL) {
    if ((!FRONT_BLOCKED) && (!BACK_BLOCKED) && (!LEFT_BLOCKED) && (!RIGHT_BLOCKED)) {
      if (check_end()) {
        stop_robot_car();
        FIRST_TRAVERSAL = 1;
      }
    }
    if (FORWARD) {
      if ((!FRONT_BLOCKED) && LEFT_BLOCKED && RIGHT_BLOCKED) {
         go_straight();
      }
      else if (FRONT_BLOCKED) {
        if (RIGHT_BLOCKED && (!LEFT_BLOCKED)) {
          turn_left();
        }
        else if (LEFT_BLOCKED && (!RIGHT_BLOCKED)) {
          turn_right();
        }
        else if (LEFT_BLOCKED && RIGHT_BLOCKED) {
          FORWARD = 0;
        }
      }
    }
    else {
      if ((!BACK_BLOCKED) && LEFT_BLOCKED && RIGHT_BLOCKED) {
        go_backwards();
      }
      else if (BACK_BLOCKED) {
        if (RIGHT_BLOCKED && (!LEFT_BLOCKED)) {
          turn_left_backwards();
        }
        else if (LEFT_BLOCKED && (!RIGHT_BLOCKED)) {
          turn_right_backwards();
        }
        else if (RIGHT_BLOCKED && LEFT_BLOCKED) {
          FORWARD = 1;
        }
      }
    }
  }
  else {
    init_start_end();
    init_map();
    rev_bfs();
    bfs();
//    if (d[s_idx] == 0)
//      cout << -1;
//    else {
//      cout << d[s_idx] << "\n";
//      for (int i = 0; i < ans.size(); i++)
//        cout << ans[i];
//    }
  }
  

}
