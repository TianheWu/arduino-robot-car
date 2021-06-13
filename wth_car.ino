#define MAX_NUM_POINT 50
#define MAX_NUM_VECTOR 10
#define MAX_NUM_QUEUE 200
#define N 5
#define M 5

int traverse_end = 0;
int cur_dir = 1;
int cur_i = 4, cur_j = 4;

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
char _map[N][M] = {{'X', 'X', 'X', 'X', 'E'}, {'X', 'X', 'X', 'X', 'X'},
                   {'X', 'X', 'X', 'X', 'X'}, {'X', 'X', 'X', 'X', 'X'}, {'X', 'X', 'X', 'X', 'S'}};

bool visited[MAX_NUM_POINT];
vector<char> ans;
vector<Edge> edges;
vector<int> G[MAX_NUM_POINT];

void init_start_end() {
  s_pos.x = 4;
  s_pos.y = 4;
  e_pos.x = 0;
  e_pos.y = 0;
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

// pinmode define
#define MOTOR_C_P 10
#define MOTOR_C_N 11
#define MOTOR_D_P 12
#define MOTOR_D_N 13
#define MOTOR_A_P 6
#define MOTOR_A_N 7
#define MOTOR_B_P 5
#define MOTOR_B_N 4

#define LED_PIN 15
#define IS_OBSTACLE_PIN 2

void stop_car() {
  analogWrite(MOTOR_C_P, 0);
  analogWrite(MOTOR_C_N, 0);
  analogWrite(MOTOR_B_N, 0);
  analogWrite(MOTOR_B_P, 0);
  analogWrite(MOTOR_A_P, 0);
  analogWrite(MOTOR_A_N, 0);
  analogWrite(MOTOR_D_P, 0);
  analogWrite(MOTOR_D_N, 0);
}


void turn_right(float power) {
  motor_A_forward(power);
  motor_B_backward(power);
  motor_C_backward(power);
  motor_D_forward(power);
}


void turn_left(float power) {
  motor_A_backward(power);
  motor_B_forward(power);
  motor_C_forward(power);
  motor_D_backward(power);
}


void move_forward(float power) {
  motor_A_forward(power);
  motor_B_forward(power);
  motor_C_forward(power);
  motor_D_forward(power);
}


void move_backward(float power) {
  motor_A_backward(power);
  motor_B_backward(power);
  motor_C_backward(power);
  motor_D_backward(power);
}


void move_right(float power) {
  motor_A_forward(power);
  motor_B_backward(power);
  motor_C_forward(power);
  motor_D_backward(power);
}


void move_left(float power) {
  motor_A_backward(power);
  motor_B_forward(power);
  motor_C_backward(power);
  motor_D_forward(power);
}


int get_power(float power) {
  return int(255 * power);
}


void motor_A_forward(float power) {
  analogWrite(MOTOR_A_P, get_power(power));
  analogWrite(MOTOR_A_N, 0);
}


void motor_B_forward(float power) {
  analogWrite(MOTOR_B_N, 0);
  analogWrite(MOTOR_B_P, get_power(power));
}


void motor_C_forward(float power) {
  analogWrite(MOTOR_C_P, 0);
  analogWrite(MOTOR_C_N, get_power(power));
}


void motor_D_forward(float power) {
  analogWrite(MOTOR_D_N, 0);
  analogWrite(MOTOR_D_P, get_power(power));
}


void motor_A_backward(float power) {
  analogWrite(MOTOR_A_P, 0);
  analogWrite(MOTOR_A_N, get_power(power));
}


void motor_B_backward(float power) {
  analogWrite(MOTOR_B_N, get_power(power));
  analogWrite(MOTOR_B_P, 0);
}


void motor_C_backward(float power) {
  analogWrite(MOTOR_C_P, get_power(power));
  analogWrite(MOTOR_C_N, 0);
}


void motor_D_backward(float power) {
  analogWrite(MOTOR_D_N, get_power(power));
  analogWrite(MOTOR_D_P, 0);
}

#define STOP_DISTANCE 400 
unsigned int high_len_3 = 0;
unsigned int low_len_3 = 0;
unsigned int len_mm_3 = 0;

float cal_distance_s3() {
  Serial3.flush();
  Serial3.write(0X55);
  delay(500);
  if (Serial3.available() >= 2) {
      high_len_3 = Serial3.read();
      low_len_3 = Serial3.read();
      len_mm_3 = high_len_3 * 256 + low_len_3;
  }
  return len_mm_3;
}


int is_obstacle = HIGH;
enum {
  FRONT = 1,
  LEFT,
  BACK,
  RIGHT
};

void fill_map(int i, int j) {
  is_obstacle = digitalRead(IS_OBSTACLE_PIN);
  if (is_obstacle == LOW) {
    digitalWrite(LED_PIN, HIGH);
  } else {
      int one_op = 0;
      digitalWrite(LED_PIN, LOW);
      switch(cur_dir) {
        case FRONT: 
           if (i - 1 >= 0) {
             _map[i - 1][j] = '.';
             if (cur_i > 0)
               cur_i--;
           }
           break;
        case LEFT:
           if (j - 1 >= 0) {
             _map[i][j - 1] = '.';
             if (cur_j > 0)
               cur_j--;
           }
           break;
        case BACK: 
           if (i + 1 < N) {
             _map[i + 1][j] = '.';
             if (cur_i < N - 1)
               cur_i++;
           }
           break;
        case RIGHT: 
           if (j + 1 < M) {
             _map[i][j + 1] = '.';
             if (cur_j < M - 1)
               cur_j++;
           }
           break;
      }
  }
}

void send_path() {
  
}

void setup() {
  pinMode(MOTOR_A_P, OUTPUT);
  pinMode(MOTOR_A_N, OUTPUT);
  pinMode(MOTOR_B_P, OUTPUT);
  pinMode(MOTOR_B_N, OUTPUT);
  pinMode(MOTOR_C_P, OUTPUT);
  pinMode(MOTOR_C_N, OUTPUT);
  pinMode(MOTOR_D_P, OUTPUT);
  pinMode(MOTOR_D_N, OUTPUT);

  Serial3.begin(9600);
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(IS_OBSTACLE_PIN, INPUT);
}

enum {
  STOP,
  FORWARD,
  BACKWARD,
  TRUN_LEFT,
  TRUN_RIGHT
};

#define DEFAULT_FB_POWER 0.18
#define DEFAULT_LR_POWER 0.2
#define TIMEOUT 500;
int _state = STOP;
int _timeleft = 0;

int tick = 0;
void loop() {
  fill_map(cur_i, cur_j);
  while (Serial.available() > 0) {
    byte cmd = Serial.read(), key;
    switch (cmd) {
      case 'D':
        key = Serial.read();
        int next_state;
//        if (cal_distance_s3() < 400 && key == 87) {
//          stop_car();
//          continue;
//        }
        switch (key) {
          case 70: cur_dir = FRONT; continue;
          case 76: cur_dir = LEFT; continue;
          case 82: cur_dir = RIGHT; continue;
          case 66: cur_dir = BACK; continue;
          case 69: traverse_end = 1; break;
          case 87: next_state = FORWARD; break;
          case 83: next_state = BACKWARD; break;
          case 65: next_state = TRUN_LEFT; break;
          case 68: next_state = TRUN_RIGHT; break;
          default: continue;
        }
        if (_state == next_state) {
          _timeleft = TIMEOUT;
        } else {
          stop_car();
          _state = next_state;
          switch (_state) {
            case FORWARD: move_forward(DEFAULT_FB_POWER); break;
            case BACKWARD: move_backward(DEFAULT_FB_POWER); break;
            case TRUN_LEFT: turn_left(DEFAULT_LR_POWER); break;
            case TRUN_RIGHT: turn_right(DEFAULT_LR_POWER); break;
          }
          _timeleft = TIMEOUT;
        }
        break;
      case 'U':
        key = Serial.read();
        int cancel_state;
        switch (key) {
          case 87: cancel_state = FORWARD; break;
          case 83: cancel_state = BACKWARD; break;
          case 65: cancel_state = TRUN_LEFT; break;
          case 68: cancel_state = TRUN_RIGHT; break;
          default: continue;
        }
        if (_state == cancel_state) {
          _state = STOP;
          stop_car();
        }
        break;
    }
  }
  if (_state != STOP) {
    if (!--_timeleft) {
      _state = STOP;
      stop_car();
    }
  }
  delay(1);
  if (traverse_end) {
    stop_car();
    init_start_end();
    init_map();
    rev_bfs();
    bfs();
    send_path();
  }
}
