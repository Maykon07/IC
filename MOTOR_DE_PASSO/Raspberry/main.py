import network
import socket
import time
from machine import Pin
import re
import json
import gc
import machine

# ===============================================
# --- CONFIGURAÇÕES DE REDE ---
# ===============================================
SEU_SSID = "MS"
SUA_SENHA = "07092002"
# ===============================================

# ===============================================
# --- CONEXÕES DE HARDWARE E MOTOR ---
# ===============================================
step_pin = Pin(15, Pin.OUT)
dir_pin = Pin(14, Pin.OUT)
en_pin = Pin(13, Pin.OUT)
relay_pin = Pin(12, Pin.OUT)

PASSOS_POR_REVOLUCAO = 1600
GRAUS_POR_REVOLUCAO = 360.0
DELAY_PASSO_US = 1000
PASSO_BASE_PADRAO = 1.8

# ===============================================
# --- Variáveis Globais ---
# ===============================================
graus_acumulados = 0.0
passo_base_ativo = PASSO_BASE_PADRAO
graus_configurados = 1.8
PICO_IP_ADDRESS = "Nao conectado"

# Inicialização do hardware
en_pin.value(1)
relay_pin.value(1)

# ===============================================
# --- FUNÇÕES DE CONTROLE DO MOTOR ---
# ===============================================
# (Todas as funções: girar_graus, dispara_fonte, reiniciar_contador,
# get_status, salvar_config, _gerar_status_voltas, get_config_data


def girar_graus(graus):
    global graus_acumulados, passo_base_ativo, PASSO_BASE_PADRAO
    if graus <= 0 or (graus_acumulados + graus) > GRAUS_POR_REVOLUCAO + 0.01:
        msg = f"Erro: Movimento invalido. Total acumulado {graus_acumulados:.2f}. Tentativa de girar {graus:.2f}."
        print(msg)
        return msg, False
    if abs(graus_acumulados - 0.0) < 0.01:
        passo_base_ativo = graus
        print(f"Novo passo base ativo definido: {passo_base_ativo} graus.")
    en_pin.value(0)
    time.sleep_ms(1)
    passos = int((graus / GRAUS_POR_REVOLUCAO) * PASSOS_POR_REVOLUCAO)
    print(f"Girando {graus:.2f} graus ({passos} passos)...")
    dir_pin.value(1)
    for _ in range(passos):
        step_pin.value(1)
        time.sleep_us(DELAY_PASSO_US)
        step_pin.value(0)
        time.sleep_us(DELAY_PASSO_US)
    graus_acumulados += graus
    reset_msg = ""
    if abs(graus_acumulados - 360.0) < 0.01:
        graus_acumulados = 0.0
        passo_base_ativo = PASSO_BASE_PADRAO
        reset_msg = " Total de 360 atingido, contador e passo base zerados!"
        print("Total de 360° atingido, contador e passo base zerados!")
    en_pin.value(1)
    msg = f"Sucesso: Girou {graus:.2f} graus. Total acumulado: {graus_acumulados:.2f}.{reset_msg}"
    print(msg)
    return msg, True

def dispara_fonte():
    print("Fonte disparada (relé ativado).")
    relay_pin.value(0)
    time.sleep(5)
    relay_pin.value(1)
    print("Fonte desligada (relé desativado).")
    return "Sucesso: Fonte disparada por 5 segundos.", True

def reiniciar_contador():
    global graus_acumulados, passo_base_ativo, PASSO_BASE_PADRAO, graus_configurados
    graus_acumulados = 0.0
    passo_base_ativo = PASSO_BASE_PADRAO
    graus_configurados = PASSO_BASE_PADRAO
    print("Contador de graus zerado! Passo base e graus configurados resetados.")
    return "Sucesso: Contador de graus zerado.", True

def get_status():
    return f"Status: {graus_acumulados:.2f} graus acumulados.", True

def salvar_config(config_data):
    global graus_configurados
    try:
        if 'graus' in config_data:
            graus_configurados = float(config_data['graus'])
            print(f"Novos graus configurados: {graus_configurados}")
            return f"Sucesso: Graus por passo definidos como {graus_configurados}", True
        else:
            return "Erro: 'graus' nao encontrado no JSON.", False
    except Exception as e:
        print(f"Erro ao salvar config: {e}")
        return "Erro: Formato de dados invalido.", False

def _gerar_status_voltas(passo_base=None):
    global graus_acumulados, passo_base_ativo
    base = passo_base
    if base is None:
        base = passo_base_ativo 
    if base <= 0:
        return "Passo base invalido"
    try:
        total_voltas_possiveis = int(round(360.0 / base))
        vezes_giradas = int(round(graus_acumulados / base))
        vezes_faltantes = total_voltas_possiveis - vezes_giradas
        str_vezes_giradas = "vez" if vezes_giradas == 1 else "vezes"
        str_vezes_faltantes = "vez" if vezes_faltantes == 1 else "vezes"
        base_str = f"{base:.2f}"
        if float(base_str) != base: base_str = str(base)
        return f"Para {base_str} graus, girou {vezes_giradas} {str_vezes_giradas} e falta {vezes_faltantes} {str_vezes_faltantes} para completar 360 graus"
    except Exception as e:
        print(f"Erro ao calcular status de voltas: {e}")
        return "Erro no calculo de voltas"

def get_config_data():
    global graus_configurados, PICO_IP_ADDRESS
    try:
        if graus_configurados <= 0:
            passos_totais = 0
        else:
            passos_totais = int(round(360.0 / graus_configurados))
    except Exception:
        passos_totais = 0
    config_data = {
        "ip": PICO_IP_ADDRESS,
        "graus": graus_configurados,
        "passos_totais": passos_totais
    }
    return json.dumps(config_data), True

# ===============================================
# --- CONEXÃO WI-FI ---
# ===============================================
def connect_to_wifi(ssid, password):
    global PICO_IP_ADDRESS
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    wlan.connect(ssid, password)
    print(f"Conectando a rede '{ssid}'...")
    timeout = 10
    while timeout > 0:
        if wlan.status() < 0 or wlan.status() >= 3:
            break
        timeout -= 1
        print(".")
        time.sleep(1)
    if wlan.status() != 3:
        PICO_IP_ADDRESS = "Nao conectado"
        print("Falha ao conectar!")
        return None
    else:
        PICO_IP_ADDRESS = wlan.ifconfig()[0]
        print(f"Conectado com sucesso! IP: {PICO_IP_ADDRESS}")
        return PICO_IP_ADDRESS

# ===============================================
# --- SERVIDOR WEB ---
# ===============================================
def start_server(ip_address):
    addr = socket.getaddrinfo('0.0.0.0', 80)[0][-1]
    s = socket.socket()
    s.bind(addr)
    
    # Aumenta a fila de "espera" de 1 para 5 ---
    s.listen(5) 
    
    print(f"Servidor escutando em http://{ip_address}")
    while True:
        try:
            cl, addr = s.accept()
            print('Cliente conectado de:', addr)
            request_bytes = cl.recv(1024)
            request_str = request_bytes.decode('utf-8')
            
            headers_end = request_str.find('\r\n\r\n')
            if headers_end == -1:
                cl.close()
                continue
                
            header_lines = request_str[:headers_end].split('\r\n')
            first_line = header_lines[0]
            body = request_str[headers_end+4:]
            
            method, url, _ = first_line.split(' ')
            print(f"Requisicao recebida: {method} {url}")
            
            status_code = 200
            response_msg = ""
            success = True
            passo_base_req = None

            # --- Definição das Rotas---
            if method == 'GET' and url == '/':
                response_msg = "Servidor do Goniometro esta online."
            elif method == 'GET' and url == '/girar':
                passo_base_req = graus_configurados
                response_msg, success = girar_graus(graus_configurados)
            elif method == 'GET' and url.startswith('/girar/'):
                try:
                    graus_str = url.split('/')[-1]
                    graus = float(graus_str)
                    passo_base_req = graus 
                    response_msg, success = girar_graus(graus)
                except (ValueError, IndexError):
                    response_msg = "Erro: Formato invalido. Use /girar/NUMERO"; success = False
            elif method == 'GET' and url == '/disparar':
                response_msg, success = dispara_fonte()
            elif method == 'GET' and url == '/reiniciar':
                response_msg, success = reiniciar_contador()
            elif method == 'GET' and url == '/status':
                response_msg, success = get_status()
            elif method == 'GET' and url == '/config':
                response_msg, success = get_config_data()
            elif method == 'POST' and url == '/salvar_config':
                try:
                    data = json.loads(body)
                    response_msg, success = salvar_config(data)
                except Exception as e:
                    print(f"Erro JSON: {e}"); response_msg = "Erro: JSON mal formatado."; success = False
            else:
                response_msg = "Erro: Rota nao encontrada."; success = False; status_code = 404

            # --- Resposta Otimizada (Evita Vazamento de Memória) ---
            if success and status_code != 404: status_code = 200
            elif status_code != 404: status_code = 400
            
            cl.send('HTTP/1.1 ' + str(status_code) + ' OK\r\n')
            cl.send('Content-Type: application/json\r\n')
            cl.send('Connection: close\r\n\r\n')
            
            if url == '/config' and success:
                # Envia o JSON da rota /config (que já é uma string)
                cl.sendall(response_msg)
            else:
                # Monta o JSON de status manualmente (sem f-string)
                status_voltas_str = _gerar_status_voltas(passo_base_req)
                
                cl.sendall('{')
                cl.sendall('"sucesso": ' + ("true" if success else "false") + ',')
                cl.sendall('"mensagem": "' + response_msg + '",')
                cl.sendall('"graus_acumulados": ' + str(graus_acumulados) + ',')
                cl.sendall('"vezes_giradas": "' + status_voltas_str + '"')
                cl.sendall('}')
            
            cl.close()
            print("Resposta enviada, conexao fechada.")
            
            # Chama o coletor de lixo APÓS fechar a conexão
            gc.collect() 
            

        except OSError as e:
            if 'cl' in locals(): # Garante que o cliente seja fechado em caso de erro
                cl.close()
            print(f'Erro no servidor: {e}')
            # Se o erro for de memória, reinicia
            if e.args[0] == 12: # ENOMEM (Out of Memory)
                print("!!! ERRO FATAL: Memória esgotada. Reiniciando... !!!")
                time.sleep(1)
                machine.reset() 

# ===============================================
# --- EXECUÇÃO PRINCIPAL ---
# ===============================================
ip = connect_to_wifi(SEU_SSID, SUA_SENHA)
if ip:
    start_server(ip)
else:
    print("Nao foi possivel conectar ao Wi-Fi. O servidor nao sera iniciado.")