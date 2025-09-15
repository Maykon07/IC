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
                0.225
                0.45
                0.9                         
                1.125                       
                1.8                         
                2.25                        
                3.6                         
                4.5                          
                5.625                       
                7.2                         
                9                           
                11.25                      
                14.4                        
                18
                22.5
                36
                45
                72
                70
                180
*/

//=================> Declarações
const int stepPin = 5;
const int dirPin  = 2;
const int enPin   = 8;
const int ledPin  = 3; // Define o pino que controla o relé (a fonte)

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
  digitalWrite(ledPin, HIGH);

  Serial.begin(9600);
  Serial.println("Sistema pronto. Envie 1 para girar, 2 Diparar fonte, 3 reinicar contador de graus");
}

void girarGraus(float graus) {
  if (graus <= 0 || (grausAcumulados + graus) > grausPorRevolucao) {
    Serial.println("Movimento inválido, excede 360° ou é zero.");
    return;
  }

  // Ativa driver antes de girar
  digitalWrite(enPin, LOW); // habilita TB6600

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

  if (abs(grausAcumulados - 360.0) < 0.01) {
    grausAcumulados = 0.0;
    Serial.println("Total de Graus igual a 360, contador de graus zerado!!");
}

  // Desativa driver para reduzir aquecimento enquanto o motor está parado
  digitalWrite(enPin, HIGH); // desabilita TB6600 (reduz corrente nas bobinas)

}

void disparaFonte() {
  digitalWrite(ledPin, LOW); // Ativa o relé (nível lógico baixo)
  Serial.println("Fonte disparada (relé ligado).");
  delay(5000); // Mantém ligado por 5 segundos
  digitalWrite(ledPin, HIGH); // Desativa o relé
  Serial.println("Fonte desligada (relé desligado).");
}

void reinicarContador(){
  grausAcumulados  = 0.0;
  Serial.println("Contador de graus zerado!!");
  Serial.print("Total girado: ");
  Serial.print(grausAcumulados, 2);
  Serial.println(" graus.");
}

void loop() {
  
  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();

    if (comando == "1") {
      Serial.println("Insira quantos graus quer mover!! ");
      while (Serial.available() == 0); // espera valor dos graus
      float graus = Serial.readStringUntil('\n').toFloat();
      girarGraus(graus);
    } 
    else if (comando == "2") {
      disparaFonte();
    }else if (comando == "3") {
      reinicarContador();
    } 
    else {
      Serial.println("Comando inválido.");
    }
  }
}

