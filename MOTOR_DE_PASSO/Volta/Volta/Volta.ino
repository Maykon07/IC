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
const int ledPin  = 13; // LED embutido do Arduino Uno

const int stepsPerRevolution = 1600; // Ajuste conforme o microstepping configurado
const float grausPorRevolucao = 360.0; // Rotação total do motor

float grausAcumulados = 0.0; // Para rastrear quantos graus já girou
const int delayPasso = 1000;  // Controla a velocidade do motor

float input = 90.0;

void setup() {
  pinMode(stepPin, OUTPUT); 
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  digitalWrite(enPin, LOW); // Ativa o driver do motor
  digitalWrite(ledPin, LOW); // Garante que o LED inicia apagado

  Serial.begin(9600); // Comunicação serial para ler comandos do teclado

  // Aguarda um comando para continuar
  Serial.println("Pressione ENTER para continuar...");
  while (Serial.available() == 0) {} // Espera entrada do usuário

  Serial.read(); // Limpa o buffer serial
  digitalWrite(ledPin, LOW); // Desliga o LED para indicar que está pronto para novo comando
}


// Função para girar um número específico de graus 
void girarGraus(float graus) {
  if (graus <= 0 || (grausAcumulados + graus) > grausPorRevolucao) {
    Serial.println("Movimento inválido, excede 360° ou é zero.");
    return;
  }

  // Calcula a quantidade de passos exata para os graus informados
  int passos = (int)((graus / grausPorRevolucao) * stepsPerRevolution);

  Serial.print("Girando ");
  Serial.print(graus, 2); // Exibe até 2 casas decimais
  Serial.println(" graus.");

  digitalWrite(dirPin, HIGH); // Define a direção de rotação (horário)

  for (int x = 0; x < passos; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(delayPasso);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(delayPasso);
  }

  grausAcumulados += graus; // Atualiza o total de graus girados
  digitalWrite(ledPin, HIGH); // Liga o LED para indicar que finalizou

  Serial.print("Total girado: ");
  Serial.print(grausAcumulados, 2);
  Serial.println(" graus.");

  // Aguarda um comando para continuar
  Serial.println("Pressione ENTER para continuar...");
  while (Serial.available() == 0) {} // Espera entrada do usuário

  Serial.read(); // Limpa o buffer serial
  digitalWrite(ledPin, LOW); // Desliga o LED para indicar que está pronto para novo comando
}

void loop() {
  if (grausAcumulados < grausPorRevolucao) {
    girarGraus(input); 
  } else {
    Serial.println("Rotação completa de 360° atingida. Reiniciando...");
    grausAcumulados = 0.0; // Reseta a contagem e permite começar de novo
  }
}
