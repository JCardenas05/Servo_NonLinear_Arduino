#include <Servo.h>

class MiServo : public Servo {
  public:
    // Función adicional para hacer algo específico
  
    void NonlinearMotion(int angle, int velocity_slow) {
        int angle_no_linear = 10;
        int current_angle = read();
        int slow_start_angle = max(angle_no_linear - angle, angle - angle_no_linear);
        
        write(slow_start_angle);

        if (current_angle < slow_start_angle) {
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
  miServo1.NonlinearMotion(180, 200);
  miServo2.NonlinearMotion(0, 200);
}

void loop() {
  // Tu código aquí
}
