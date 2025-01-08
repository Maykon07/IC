 
//=================> Hardware
//================= ENA-    --> GND Arduino
//================= ENA+    --> Pino 8 Arduino
//================= DIR-    --> GND Arduino
//================= DIR+    --> PIno 2 Ardunio
//================= PUL-    --> GND Arduino
//================= PUL+    --> PIno 5 Ardunio
//================= PUL-    --> GND Arduino
//================= GND     --> GND da Fonte Externa
//================= VCC     --> Fonte Externa 



//=================> Declarações

const int stepPin = 5; 
const int dirPin  = 2; 
const int enPin   = 8;

void setup() {
  pinMode (stepPin,OUTPUT); 
  pinMode  (dirPin,OUTPUT);
  pinMode   (enPin,OUTPUT);
  digitalWrite (enPin,LOW);
}

void loop() {
  digitalWrite(dirPin,HIGH); // Permite que o moto se mova em uma direção específica
  for(int x = 0; x < 1600; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(500); 
  }

  delay(1000); // espera 1 segundo

  digitalWrite(dirPin,LOW); //Muda o sentido da rotação
  for(int x = 0; x < 1600; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(500);
  }
  delay(1000); 
}
