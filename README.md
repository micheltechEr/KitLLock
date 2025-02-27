# Sistema de Autenticação com Display OLED, LEDs e Buzzer - Raspberry Pi Pico

## Descrição do Projeto
Este projeto foi desenvolvido para implementar um sistema de autenticação utilizando o **Kit BitDogLab*. Ele combina um **display OLED SSD1306**, dois **LEDs** (verde e vermelho), um **buzzer** e comunicação via **terminal serial** para criar uma aplicação interativa de controle de acesso.  
Para acessar um vídeo demonstrativo (caso exista), adicione o link aqui: [clique aqui](https://drive.google.com/file/d/1V8qliTdz6ApxlUpUbqbUllNp8asoTTNB/view?usp=sharing).

O sistema utiliza:
- O **terminal serial** para entrada de senha e comandos pelo usuário.
- **LEDs** para feedback visual: verde indica acesso permitido, vermelho indica falha ou bloqueio.
- Um **buzzer** para feedback auditivo com bips distintos para sucesso ou erro.
- Um **display OLED** para exibir mensagens como "Digite a senha" ou "Acesso Permitido".

---

## Componentes Utilizados
- **LED Verde (Pino 11):** Indica acesso permitido ou comandos válidos.
- **LED Vermelho (Pino 13):** Sinaliza falha na autenticação ou bloqueio do sistema.
- **Buzzer (Pino 10):** Emite bips para feedback auditivo.
- **Display OLED SSD1306:**
  - SDA (Pino 14): Comunicação I2C para dados.
  - SCL (Pino 15): Comunicação I2C para clock.
- **Raspberry Pi Pico:** Placa principal para executar o código.
- **Resistores:** 330Ω em série com os LEDs para limitar a corrente.
- **Terminal Serial:** Interface via USB para entrada de senha e comandos.

---

## Funcionalidades do Projeto
1. **Autenticação por Senha:**
   - O usuário digita uma senha via terminal serial, que é comparada com "KITLAB".
   - Máximo de 3 tentativas antes de bloquear o sistema por 30 segundos.

2. **Feedback Visual e Auditivo:**
   - LED verde acende e buzzer emite 1 bip ao autenticar com sucesso.
   - LED vermelho acende e buzzer emite 2 bips ao falhar; 3 bips ao bloquear.

3. **Exibição no Display OLED:**
   - Mostra mensagens como "Digite a senha", "Acesso Permitido" ou "Acesso Bloqueado".

4. **Comandos Pós-Autenticação:**
   - Após autenticação, aceita comandos como "status", "mensagem" e "sair" via serial.

---

## Guia de Funcionamento na Sua Máquina

Para executar este projeto localmente, siga as instruções abaixo:

### 1. **Clone o repositório:**
   - Abra o **VS Code** e clone este repositório para sua máquina.

### 2. **Importe o projeto:**
   - Certifique-se de ter as extensões do **Raspberry Pi Pico** instaladas no VS Code.
   - Importe o projeto para compilá-lo e executá-lo na placa RP2040.

### 3. **Conecte a placa:**
   - Conecte o Raspberry Pi Pico ao computador via USB e coloque-o no modo **BOOTSEL**.

### 4. **Compile o código:**
   - Compile o código diretamente no VS Code.

### 5. **Execute na placa:**
   - Após a compilação, com a placa no modo **BOOTSEL**, clique em **Executar** ou **Run** para carregar o programa.

---

## Funcionamento do Sistema

1. **Estado Inicial:**
   - LEDs apagados, display mostra "Digite a senha".
   - Aguarda entrada de senha via terminal serial.

2. **Autenticação:**
   - Senha correta: LED verde acende, buzzer toca 1 bip, display mostra "Acesso Permitido".
   - Senha incorreta: LED vermelho acende, buzzer toca 2 bips, display mostra "Senha Incorreta".
   - Após 3 falhas: display mostra "Acesso Bloqueado", buzzer toca 3 bips, bloqueio por 30 segundos.

3. **Comandos Pós-Autenticação:**
   - "status": Display mostra "Sistema OK!".
   - "mensagem": Display mostra "Embarcatech 2025".
   - "sair": Sai do modo autenticado, retornando ao estado inicial.

---

## Código Fonte

O código fonte está organizado da seguinte forma:
- **Função `setup_leds`:** Configura os pinos dos LEDs como saídas.
- **Função `setup_buzzer`:** Configura o pino do buzzer como saída.
- **Função `read_password`:** Lê a senha digitada e verifica contra "KITLAB".
- **Função `read_command`:** Lê e processa comandos pós-autenticação.
- **Função `display_message`:** Exibe mensagens no display OLED.
- **Loop Principal (`main`):** Gerencia autenticação, feedback e comandos.

---

## Observações Finais

Este projeto foi desenvolvido com foco em boas práticas de programação e documentação, sendo ideal para estudantes que desejam explorar **I2C**, **GPIO** e **comunicação serial** no Raspberry Pi Pico. O circuito pode ser simulado no Wokwi antes da montagem física.

Caso tenha dúvidas ou sugestões, sinta-se à vontade para abrir uma **issue** ou entrar em contato.

---

### Créditos
- **Autor:** Ângelo Miguel Ribeiro Cerqueira Lima
- **Data:** 26/02/2025