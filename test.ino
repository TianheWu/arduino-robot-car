#define MAX_ROTATE_VAL 255
#define DEFAULT_FB_POWER 0.12
#define DEFAULT_LR_POWER 0.2

int MOTOR_C_P = 10;
int MOTOR_C_N = 11;
int MOTOR_D_P = 12;
int MOTOR_D_N = 13;
int MOTOR_A_P = 6;
int MOTOR_A_N = 7;
int MOTOR_B_P = 5;
int MOTOR_B_N = 4;

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


void setup() {
  // put your setup code here, to run once:
  pinMode(MOTOR_A_P, OUTPUT);
  pinMode(MOTOR_A_N, OUTPUT);
  pinMode(MOTOR_B_P, OUTPUT);
  pinMode(MOTOR_B_N, OUTPUT);
  pinMode(MOTOR_C_P, OUTPUT);
  pinMode(MOTOR_C_N, OUTPUT);
  pinMode(MOTOR_D_P, OUTPUT);
  pinMode(MOTOR_D_N, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  move_forward(DEFAULT_FB_POWER);
  delay(1200);
//  move_left(DEFAULT_LR_POWER);
//  delay(2000);
//  move_backward(DEFAULT_FB_POWER);
//  delay(2000);
  turn_left(DEFAULT_LR_POWER);
  delay(800);
//  turn_right(DEFAULT_LR_POWER);
//  delay(2000);
//  move_right(DEFAULT_LR_POWER);
//  delay(2000);
//  move_forward(DEFAULT_FB_POWER);
//  delay(2000);
//  turn_left(DEFAULT_LR_POWER);
//  motor_C_forward(DEFAULT_FB_POWER);
//  delay(1000);
//  motor_D_forward(DEFAULT_FB_POWER);
//  delay(1000);
//  motor_C_forward(DEFAULT_FB_POWER);
//  delay(1000);
//  motor_D_forward(DEFAULT_FB_POWER);
//  delay(1000);
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
