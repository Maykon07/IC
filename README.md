# Controle de Motor de Passo Nema 23 com Arduino

Este repositório contém o código e a documentação para o controle de um motor de passo Nema 23 utilizando um Arduino UNO, um driver TB6600 e uma fonte chaveada. O projeto inclui explicações detalhadas sobre os componentes, suas conexões e o funcionamento do sistema.

---

## Componentes do Projeto

### 1. **Motor de Passo Nema 23**
- **Modelo:** 23HD86001Y-21B (Bulk Man 3D)
- **Descrição:**
  - Motor de passo de alta potência, ideal para aplicações que exigem torque elevado.
  - Passo de 1,8° por pulso (200 passos por revolução no modo completo).
  - Corrente nominal de 2,1A por fase.
<p align="center">
  <img src="https://github.com/user-attachments/assets/22316a49-4116-415a-93a0-ab2d05f2d3a9" alt="Motor_nema_23" width="300">
</p>
### 2. **Driver TB6600**
- **Descrição:**
  - Driver de motor de passo de alta corrente, suporta até 4A.
  - Oferece controle de microstepping (1/2, 1/4, 1/8, etc.) para maior precisão.
  - Proteções contra sobrecorrente e sobretemperatura.
<p align="center">
  <img src="https://github.com/user-attachments/assets/2a5b8c2a-2bc2-45d9-b5e5-c677be16b0ed" alt="Driver TB6600" width="300">
</p>




### 3. **Fonte Chaveada**
- **Especificações:**
  - Tensão de saída: 24V (ou compatível com o motor e driver).
  - Corrente suficiente para o motor e driver (sugere-se pelo menos 3A para segurança).
  - Conectada ao driver para alimentação do motor.

<p align="center">
  <img src="https://github.com/user-attachments/assets/621630e5-fdda-4b9e-8d8c-193cfe867658" alt="Fonte_Chaveada" width="300">
</p>

### 4. **Arduino UNO**
- **Descrição:**
  - Microcontrolador responsável por gerar os sinais de controle (pulsos e direção).
  - Envia comandos para o driver TB6600 através de pinos digitais.

---

## Ligações do Circuito

As conexões foram realizadas conforme descrito abaixo. Foi feito um **jumper** conectando os pinos negativos do driver (ENA-, DIR-, PUL-) ao **GND do Arduino**, para poder ocupar somente uma entrada GND do arduino e não 3, que seria se ligase cada uma.

### Ligações do Driver TB6600 com o Arduino
- **ENA-** → GND do Arduino
- **ENA+** → Pino 8 do Arduino
- **DIR-** → GND do Arduino
- **DIR+** → Pino 2 do Arduino
- **PUL-** → GND do Arduino
- **PUL+** → Pino 5 do Arduino

### Ligações do Driver com a Fonte Chaveada
- **GND** → GND da Fonte
- **VCC** → +24V da Fonte

### Ligações do Motor com o Driver ( siga as cores que identificão o A+, A-, B+, e B- que vem no motor de passo)
- **A+ e A-** → Conectados à bobina A do motor
- **B+ e B-** → Conectados à bobina B do motor

---

## Funcionamento do Sistema
1. O Arduino gera sinais de controle para o driver TB6600:
   - Pulsos no pino PUL+ determinam os passos do motor.
   - O sinal no pino DIR+ define o sentido de rotação (horário ou anti-horário).
   - O pino ENA+ habilita ou desabilita o driver.
2. O driver converte esses sinais em corrente para as bobinas do motor, movimentando-o conforme o padrão definido.
3. A fonte chaveada alimenta o driver e o motor, garantindo potência suficiente para o sistema.

---

## Como Usar
1. Monte o circuito conforme o esquema de ligações acima.
2. Carregue o código fornecido no Arduino UNO.
3. Ajuste as configurações no driver (Pulse/rev e Current) de acordo com a documentação:
   - **Pulse/rev:** Configurado para 1600 pulsos por revolução (1/8 microstepping).
   - **Current (A):** Configurado para 2,1A ou 2,9A.
4. Alimente o circuito e teste o motor de passo.

---

## Observações
- Certifique-se de que todos os fios estejam bem conectados para evitar falhas.
- Utilize uma fonte compatível com o consumo do motor para evitar sobrecarga.

