#include "MsTimer2.h"
//#include <Wire.h>
//#include <MechaQMC5883.h>

#define MAX_ROTATE_VAL 255
#define DEFAULT_FB_POWER 0.15
#define DEFAULT_LR_POWER 0.25
#define STOP_DISTANCE 480

unsigned int high_len_3 = 0;
unsigned int low_len_3 = 0;
unsigned int len_mm_3 = 0;
unsigned int high_len_1 = 0;
unsigned int low_len_1 = 0;
unsigned int len_mm_1 = 0;

//MechaQMC5883 qmc;

// pinmode define
int MOTOR_C_P = 10;
int MOTOR_C_N = 11;
int MOTOR_D_P = 12;
int MOTOR_D_N = 13;
int MOTOR_A_P = 6;
int MOTOR_A_N = 7;
int MOTOR_B_P = 5;
int MOTOR_B_N = 4;

int forward_yaw;
int left_yaw;
int back_yaw;
int right_yaw;
int yaw_flag = 0;

int state_yaw = 1;

// transform power
int get_power(float power);

// motor function
void motor_A_forward(float power);
void motor_A_backward(float power);
void motor_B_forward(float power);
void motor_B_backward(float power);
void motor_C_forward(float power);
void motor_C_backward(float power);
void motor_D_forward(float power);
void motor_D_backward(float power);

// robot move function
void move_forward(float power);
void move_backward(float power);
void move_right(float power);
void move_left(float power);
void turn_left(float power);
void turn_right(float power);
void stop_car();
bool bool_turn_left();

void move_straight();


// change blocked
void change_blocked();
float cal_distance_s3();
float cal_distance_s1();

// return yaw value
int get_yaw();
void init_yaw();

void setup() {
  // Motor pinMode
  pinMode(MOTOR_A_P, OUTPUT);
  pinMode(MOTOR_A_N, OUTPUT);
  pinMode(MOTOR_B_P, OUTPUT);
  pinMode(MOTOR_B_N, OUTPUT);
  pinMode(MOTOR_C_P, OUTPUT);
  pinMode(MOTOR_C_N, OUTPUT);
  pinMode(MOTOR_D_P, OUTPUT);
  pinMode(MOTOR_D_N, OUTPUT);

//  Wire.begin();
//  qmc.init();

  Serial3.begin(9600);
  Serial2.begin(9600);
  // Serial.begin(9600);
  Serial.begin(115200);
//  MsTimer2::set(100, change_blocked);
//  MsTimer2::start();
}

void loop() {
  delay(1);
  bool active = false;
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd == 'D') {
      char key = Serial.read();
      while (key == 87) {
        move_forward(DEFAULT_FB_POWER);
        delay(10);
        active = true;
        char stop_cmd = Serial.read();
        if (stop_cmd == 'U') {
          stop_car();
          break;
        }
      }
      while (key == 83) {
        move_backward(DEFAULT_FB_POWER);
        delay(10);
        active = true;
        char stop_cmd = Serial.read();
        if (stop_cmd == 'U') {
          stop_car();
          break;
        }
      }
      while (key == 65) {
        turn_left(DEFAULT_LR_POWER);
        delay(10);
        active = true;
        char stop_cmd = Serial.read();
        if (stop_cmd == 'U') {
          stop_car();
          break;
        }
      }
      while (key == 68) {
        turn_right(DEFAULT_LR_POWER);
        delay(10);
        active = true;
        char stop_cmd = Serial.read();
        if (stop_cmd == 'U') {
          stop_car();
          break;
        }
      }
    }
  }
  if (!active)
    stop_car();
}

//void init_yaw() {
//  int count = 0;
//  while (count < 8000) {
//    forward_yaw = get_yaw();
//    left_yaw = forward_yaw + 90;
//    back_yaw = left_yaw + 90;
//    right_yaw = back_yaw + 90;
//    yaw_flag = 1;
//    count++;
//  }
//}

//int get_yaw() {
//  int x, y, z;
//  int yaw;
//  qmc.read(&x, &y, &z);
//  yaw = qmc.azimuth(&y,&x);
//  return yaw;
//}
//
//void move_straight() {
//  if (!yaw_flag) {
//    init_yaw();
//  }
//  change_blocked();
//  move_forward(DEFAULT_FB_POWER);
//  delay(30);
//  change_blocked();
//  int loop_yaw = get_yaw();
//  if (loop_yaw > forward_yaw) {
//    motor_A_forward(0.13);
//    motor_D_forward(0.13);
//    motor_B_forward(0.1);
//    motor_C_forward(0.1);
//    delay(20);
//  }
//  change_blocked();
//  if (loop_yaw < forward_yaw) {
//    motor_A_forward(0.1);
//    motor_D_forward(0.1);
//    motor_B_forward(0.14);
//    motor_C_forward(0.14);
//    delay(20);
//  }
//}

bool bool_turn_left() {
  // if (cal_distance_s1() > STOP_DISTANCE)
  return true;
}

float cal_distance_s1() {
  Serial1.flush();
  Serial1.write(0X55);
  delay(500);
  if (Serial1.available() >= 2) {
      high_len_1 = Serial1.read();
      low_len_1 = Serial1.read();
      len_mm_1 = high_len_1 * 256 + low_len_1;
  }
  return len_mm_1;
}

float cal_distance_s3() {
  Serial3.flush();
  Serial3.write(0X55);
  delay(500);
  if (Serial3.available() >= 2) {
      high_len_3 = Serial3.read();
      low_len_3 = Serial3.read();
      len_mm_3 = high_len_3 * 256 + low_len_3;
      // 有效的测距的结果在1mm 到 10m 之间
  }
  return len_mm_3;
}


void change_blocked() {
  MsTimer2::stop();
  if (cal_distance_s3() < STOP_DISTANCE)
    stop_car();
  MsTimer2::start();
}


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
  return int(MAX_ROTATE_VAL * power);
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
