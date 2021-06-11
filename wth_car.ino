
// pinmode define
#define MOTOR_C_P 10
#define MOTOR_C_N 11
#define MOTOR_D_P 12
#define MOTOR_D_N 13
#define MOTOR_A_P 6
#define MOTOR_A_N 7
#define MOTOR_B_P 5
#define MOTOR_B_N 4

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

void setup() {

  pinMode(MOTOR_A_P, OUTPUT);
  pinMode(MOTOR_A_N, OUTPUT);
  pinMode(MOTOR_B_P, OUTPUT);
  pinMode(MOTOR_B_N, OUTPUT);
  pinMode(MOTOR_C_P, OUTPUT);
  pinMode(MOTOR_C_N, OUTPUT);
  pinMode(MOTOR_D_P, OUTPUT);
  pinMode(MOTOR_D_N, OUTPUT);
  
  Serial.begin(115200);
//  Serial.println("online");
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
#define TIMEOUT 2000;
int _state = STOP;
int _timeleft = 0;

int tick = 0;
void loop() {
//  tick++;
//  if (tick % 100 == 0) {
//    Serial.print("_state: ");
//    Serial.print(_state);
//    Serial.print(", _timeleft: ");
//    Serial.println(_timeleft);
//  }
  
  while (Serial.available() > 0) {
    byte cmd = Serial.read(), key;
    switch (cmd) {
      case 'D':
        key = Serial.read();
        int next_state;
        switch (key) {
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
}
