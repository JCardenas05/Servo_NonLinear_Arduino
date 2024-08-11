#include <Servo.h>

class MiServo : public Servo {
  private: 
    unsigned long last_time = 0;
    int angle_no_linear = 10;
    int velocity_slow = 300;

    bool motion_in_progress = false;
    bool direction = true;
    int slow_start_angle = 0;
    int current_angle = 0;
    int end_angle = 0;
    int id_servo = 0;

  public:
    void init (int id, int pin) {
      id_servo = id;
      attach(pin);
    }

    int getId() {
      return id_servo;
    }

    void NonlinearMotion(int angle) {
        current_angle = read();
        end_angle = angle;
        if (current_angle != end_angle) {
          direction = current_angle < end_angle;
          if (direction){
            slow_start_angle = max(current_angle, end_angle - angle_no_linear);
          }
          else {
            slow_start_angle = min(current_angle, end_angle + angle_no_linear);
          }
          motion_in_progress = true;
          write(slow_start_angle);
          current_angle = slow_start_angle;
        }
        else {
          motion_in_progress = false;
          return;
        }
      }
    
    void update() {
      unsigned long current_time = millis();
      if (motion_in_progress & current_time - last_time > velocity_slow) {
        last_time = millis();
        if (direction) {
          if (current_angle < end_angle) {
            current_angle++;
          }
          else {
            motion_in_progress = false;
          }
        }
        else {
          if (current_angle > end_angle) {
            current_angle--;
          }
          else {
            motion_in_progress = false;
          }
        }
        write(current_angle);
      }
    }

    void Origin(){
      write(0);
    }
  	void Midle(){
      write(90);
    }
};

MiServo miServo1;
MiServo miServo2;
MiServo miServo3;

MiServo servos[3] = {miServo1, miServo2, miServo3};
int connections[3] = {9, 10, 11};

void setup() {
  Serial.begin(9600);
  for (int i = 0; i <= 3; i++) {
    servos[i].init(i, connections[i]);
    servos[i].Origin();
  }
  delay(3000);
}

void loop() {
  // update servos
  for (int i = 0; i <= 3; i++) {
    servos[i].update();
  }
  serialEvent();
}

void serialEvent() {
  while (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    int servoIndex = input.substring(0, 1).toInt(); 
    String servoMode = input.substring(1, 3); // Obtener el modo del servo (NL: Movimiento no lineal, LI: Movimiento lineal)
    int servoValue = input.substring(3).toInt();  

    Serial.println("Servo: "+String(servoIndex)+" Mode: "+servoMode+" Value: "+String(servoValue)+"\n");

    if (servoMode == "NL") {
      servos[servoIndex].NonlinearMotion(servoValue);
    } else if (servoMode == "LI") {
      servos[servoIndex].write(servoValue);
    }
  }
}


