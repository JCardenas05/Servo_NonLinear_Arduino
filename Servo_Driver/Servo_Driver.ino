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

  public:
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

void setup() {
  miServo1.attach(10);
  miServo2.attach(11);
  miServo3.attach(9);
  miServo1.Origin();
  miServo2.Origin();
  miServo3.Origin();
  delay(3000);
  miServo1.Midle();
  miServo2.Midle();
  delay(3000);
  miServo1.NonlinearMotion(25);
  miServo2.NonlinearMotion(95);
  miServo3.NonlinearMotion(180);
}

void loop() {
  // Tu código aquí
  miServo1.update();
  miServo2.update();
  miServo3.update();
}
