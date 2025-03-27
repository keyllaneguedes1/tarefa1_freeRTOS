# Controle de LED com Botão usando FreeRTOS no Raspberry Pi Pico

Este repositório contém um código em C para o **Raspberry Pi Pico** que utiliza o **FreeRTOS** para gerenciar três tarefas simultaneamente:

1. **Leitura do Botão** - Lê periodicamente o estado do botão.
2. **Processamento do Botão** - Verifica se o botão foi pressionado e aciona a próxima tarefa.
3. **Controle do LED** - Acende o LED enquanto o botão estiver pressionado.

## Requisitos
- **Placa:** Raspberry Pi Pico
- **SDK:** Raspberry Pi Pico SDK
- **Sistema Operacional:** FreeRTOS
- **Ferramentas:** CMake, GNU Arm Embedded Toolchain

## Circuito
Componentes utilizados da BitDogLab conforme abaixo:

- **Botão:** GPIO 5 (com pull-up interno)
- **LED Verde:** GPIO 11

## Como Funciona
O código usa **semáforos** para sincronizar as tarefas:

1. A **Tarefa 1** (“Leitura do Botão”) lê o estado do botão a cada **100ms** e, caso detecte uma pressão, aciona a **Tarefa 2**.
2. A **Tarefa 2** (“Processamento do Botão”) verifica se o botão foi pressionado e aciona a **Tarefa 3**.
3. A **Tarefa 3** (“Controle do LED”) acende o LED enquanto o botão estiver pressionado.


3. Envie o arquivo `.uf2` para o Raspberry Pi Pico:
   - Conecte o Pico ao PC segurando o **botão BOOTSEL**.
   - Copie o arquivo gerado para a unidade montada.

## Autor
Desenvolvido por [Seu Nome].

## Licença
Este projeto está sob a licença MIT.

