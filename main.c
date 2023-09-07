/**
 * @author Rafael Martins de Sousa - RA: 220023201
 * @brief Arquivo referente à "Tarefa 01 - Leitura temperatura do nucleo Raspberry Pi Pico W"
 */

// Inclui a biblioteca padrão do SDK
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"

// o ADC tem resolução de 12 bits.
const float conversion_factor = 3.3f / (1 << 12);

float convert_volts_to_celsius(float volts)
{
    // Durante meus testes, funcionou melhor alterando este
    // valor de 0.706 para 0.606, empiricamente.
    // Indicando assim um valor de 21~22 ºC
    // Com o valor padrão, o retorno médio se deu 77~78ºC

    // Apesar disso, deixando valor de referencia
    // do raspberry-pi-pico-c-sdk.pdf - pagina 69
    return 27 - (volts - 0.706) / 0.001721;
}

bool timer_callback(struct repeating_timer *t)
{
    float voltage = 0;
    printf("Nova amostra de temperatura\n");
    voltage = adc_read() * conversion_factor;
    printf("Voltage: %fV => %fºC\n", voltage, convert_volts_to_celsius(voltage));
    printf("---------------------------\n");
    return true;
}

int main()
{
    struct repeating_timer timer;

    // Inicializa pinagem geral
    stdio_init_all();
    /*
     * No Modelo Raspberry Pi Pico W o LED Onboard está ligado
     * ao Módulo wifi, dessa forma é necessário inicializa-lo
     * para então executar o código
     */
    cyw43_arch_init();

    // Configura o ADC
    adc_init();
    // Input 4 é o sensor onboard!
    adc_select_input(4);

    // Indicativo de gravação.
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

    // Inicializa um timer periodico de 2 segundos.
    add_repeating_timer_ms(2000, &timer_callback, NULL, &timer);

    // Loop infinito para o funcionamento do pisca pisca
    while (true)
    {
        sleep_ms(10);
    }
    return 0;
}
