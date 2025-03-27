#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "semphr.h"

const uint led_green = 11;

const uint button_pin = 5;

SemaphoreHandle_t semaforoBotao; // semáforo para acionar tarefa 2
SemaphoreHandle_t semaforoLed; // semáforo para acionar tarefa 3

void tarefaLeituraBotao(void *parametro) {
    static int estadoBotaoAnterior = 1;

    for (;;) {
        int estadoBotaoAtual = gpio_get(button_pin);
        
        printf("estado botao: %d\n", estadoBotaoAtual);

        if (estadoBotaoAtual == 0 && estadoBotaoAnterior == 1) { 
            vTaskDelay(pdMS_TO_TICKS(10)); 
            if (gpio_get(button_pin) == 0) { 
                xSemaphoreGive(semaforoBotao);
            }
        }

        estadoBotaoAnterior = estadoBotaoAtual;
        vTaskDelay(pdMS_TO_TICKS(100)); 
    }
}

void tarefaProcessamentoBotao(void *parametro) {
    for (;;) {
        if (xSemaphoreTake(semaforoBotao, portMAX_DELAY)) {
            xSemaphoreGive(semaforoLed);
        }
    }
}

void tarefaControleLed(void *parametro) {
    for (;;) {
        if (xSemaphoreTake(semaforoLed, portMAX_DELAY)) {
            gpio_put(led_green, 1);
            while (gpio_get(button_pin) == 0) { // Mantém o LED aceso enquanto o botão estiver pressionado
                vTaskDelay(pdMS_TO_TICKS(100)); 
            }
            gpio_put(led_green, 0);
        }
    }
}



void setupLedsAndButton() {
    gpio_init(led_green);
    gpio_set_dir(led_green, GPIO_OUT);
    gpio_put(led_green, 0);
    
    gpio_init(button_pin);
    gpio_set_dir(button_pin, GPIO_IN);
    gpio_pull_up(button_pin);
}

int main(void) {
    stdio_init_all();
    setupLedsAndButton();

    semaforoBotao = xSemaphoreCreateBinary();
    semaforoLed = xSemaphoreCreateBinary();

    if (semaforoBotao == NULL || semaforoLed == NULL) {
        printf("Erro ao criar semáforos!\n");
        while (1);
    }

    xTaskCreate(tarefaLeituraBotao, "Leitura Botão", 1024, NULL, 1, NULL);
    xTaskCreate(tarefaProcessamentoBotao, "Processamento Botão", 1024, NULL, 2, NULL);
    xTaskCreate(tarefaControleLed, "Controle LED", 1024, NULL, 3, NULL);

    vTaskStartScheduler();

    for (;;);
}
