#include <Servo.h>

class MiServo : public Servo {
  private: 
    unsigned long last_time = 0;
    int angle_no_linear = 5;
    int velocity_slow = 150;

    bool motion_in_progress = false;
    bool direction = true;
    int slow_start_angle = 0;
    int current_angle = 0;

  public:
    void NonlinearMotion(int angle) {
        current_angle = read();
        if (current_angle != angle) {
          direction = true ? current_angle < angle : false;
          if (direction){
            slow_start_angle = max(current_angle, angle - angle_no_linear);
          }
          else {
            slow_start_angle = min(current_angle, angle + angle_no_linear);
          }
          motion_in_progress = true;
        }
        else {
          return;
        }

        write(slow_start_angle);

        if (direction) {
          for (int i = slow_start_angle; i <= angle; i++) {
            write(i);
            delay(velocity_slow); 
          }
        }
        else {
          for (int i = slow_start_angle; i >= angle; i--) {
            write(i);
            delay(velocity_slow); 
          }
        }
      }
    
    void update() {
      if (millis() - last_time > 1000) {
        last_time = millis();
        Serial.println("Hola");
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

void setup() {
  Serial.begin(9600);
  miServo1.attach(10);
  miServo2.attach(11);
  miServo1.Origin();
  miServo2.Origin();
  delay(3000);
  miServo1.Midle();
  miServo2.Midle();
  delay(3000);
  miServo1.NonlinearMotion(50);
  miServo2.NonlinearMotion(130);
}

void loop() {
  // Tu código aquí
}
