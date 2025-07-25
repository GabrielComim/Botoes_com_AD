# Projeto: Leitura de botões com ADC no ESP32.
 Autor: Gabriel Comim

## Descrição: Este projeto demonstra como realizar a leitura de múltiplos botões físicos utilizando apenas uma entrada analógica (ADC) no microcontrolador **ESP32**.  

A técnica utilizada é conhecida como **"Leitura de Botões via Divisor de Tensão com ADC"**, uma solução eficiente para economizar pinos GPIO.

## Aplicações
- Projetos embarcados com limitação de GPIOs
- Sistemas com múltiplos botões físicos
- Projetos de automação usando **ESP32**
- Sistemas IoT com leitura analógica de botões

 ## Tecnoogias utilizadas
 Placa de desenvolvimento: ESP32 
 framework: ESP_IDF
 
 # HARDWARE:
 CONSISTE EM 4 BOTÕES LIGADOS EM UMA ENTRADA AD PARA CAPTAR QUAL FOI ACIONADO CONFORME A TENSÃO QUE É GERADA NO AD.
 A VANTAGEM DE UTILIZAR UM AD PARA DETECTAR QUAL BOTÃO FOI ACIONADO É A QUANTIDADE DE GPIOS, POIS DESSA FORMA CONSEGUE-SE UTILIZAR VÁRIOS BOTÕES E APENAS UM GPIO.
 LEMBRE-SE DE GERAR TENSÕES DIFERENTES AO CLICAR CADA BOTÃO, ISTO É, DEVE-SE MONTAR DIVISORES DE TENSÕES COM RESISTORES PARA CADA BOTÃO, DE VALORES DIFERENTES.
 OS RESISTORES UTILIZADOS CONFORME DISPONIBILIDADE:
 
 *R1 = 2K2 - CALCULADO: 1725
 
 *R2 = 680 - CALCULADO: 621
 
 *R3 = 330 - CALCULADO: 320
 
 *R4 = ADOTADO:1000
 

*NESTE EXEMPLO FORAM UTILIZADOS 4 BOTÕES PARA NAVEGAR EM UM DISPLAY OLED DE 128X64. 
 FOI MANTIDO UMA MARGEM DE SEGURANÇA PARA A DETECÇÃO DO CLIQUE, NO FIM DO CÓDIGO COM OS VALORES EM MILIVOLTS.
 
 BOTÃO 1: 800 À 1000
 
 BOTÃO 2: 1600 À 1700
 
 BOTÃO 3: 2400 À 2600
 
 BOTÃO 4: >3100
 
 
 
 # FIRMWARE: 
 CONSISTE EM UM MENU QUE É NAVEGÁVEL ATRAVÉS DOS BOTÕES, NA SEGUINTE CONFIGURAÇÃO:
  			BOTÃO 1 - SETA PARA CIMA
  			BOTÃO 2 - ENTER
  			BOTÃO 3 - SETA PARA BAIXO 
  			BOTÃO 4 - VOLTAR
  			É POSSÍVEL ENTRAR E SAIR DOS MENUS ATRAVÉS DOS BOTÕES.

## Palavras-chave
ESP32, ADC, Leitura de Botões, Divisor de Tensão, Projeto ESP-IDF, Gabriel Comim
