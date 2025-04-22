//=================> Hardware
//================= ENA-    --> GND Arduino
//================= ENA+    --> Pino 8 Arduino
//================= DIR-    --> GND Arduino
//================= DIR+    --> Pino 2 Arduino
//================= PUL-    --> GND Arduino
//================= PUL+    --> Pino 5 Arduino
//================= PUL-    --> GND Arduino
//================= GND     --> GND da Fonte Externa
//================= VCC     --> Fonte Externa 

/*
Graus validos:
                0.9                         36
                1.125                       45
                1.8                         72
                2.25                        90
                3.6                         180
                4.5                          
                5.625                       
                7.2                         
                9                           
                11.25                      
                14.4                        
                18
                22.5
               

*/

//=================> Declarações
const int stepPin = 5;
const int dirPin  = 2;
const int enPin   = 8;
const int ledPin  = 13;

const int stepsPerRevolution = 1600;
const float grausPorRevolucao = 360.0;
float grausAcumulados = 0.0;
const int delayPasso = 1000;

void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  digitalWrite(enPin, LOW);
  digitalWrite(ledPin, LOW);

  Serial.begin(9600);
  Serial.println("Sistema pronto. Envie 1 para girar ou 2 para ligar o LED.");
}

void girarGraus(float graus) {
  if (graus <= 0 || (grausAcumulados + graus) > grausPorRevolucao) {
    Serial.println("Movimento inválido, excede 360° ou é zero.");
    return;
  }

  int passos = (int)((graus / grausPorRevolucao) * stepsPerRevolution);

  Serial.print("Girando ");
  Serial.print(graus, 2);
  Serial.println(" graus.");

  digitalWrite(dirPin, HIGH);
  for (int x = 0; x < passos; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(delayPasso);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(delayPasso);
  }

  grausAcumulados += graus;
  Serial.print("Total girado: ");
  Serial.print(grausAcumulados, 2);
  Serial.println(" graus.");
}

void disparaFonte() {
  digitalWrite(ledPin, HIGH);
  Serial.println("LED ligado (Fonte disparada).");
}

void loop() {
  
  if (Serial.available()) {
    digitalWrite(ledPin, LOW);
    String comando = Serial.readStringUntil('\n');
    comando.trim();

    if (comando == "1") {
      while (Serial.available() == 0); // espera valor dos graus
      float graus = Serial.readStringUntil('\n').toFloat();
      girarGraus(graus);
    } 
    else if (comando == "2") {
      disparaFonte(); // liga o LED
    } 
    else {
      Serial.println("Comando inválido.");
    }
  }
}

