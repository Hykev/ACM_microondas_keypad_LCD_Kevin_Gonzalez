#include <stdint.h>
#include <string.h>

#define PERIPH_BASE           (0x40000000)
#define AHB_BASE              (PERIPH_BASE + 0x20000)
#define IOPORT_BASE           (PERIPH_BASE + 0x10000000)
#define RCC_BASE              (AHB_BASE + 0x1000)
#define GPIOA_BASE            (IOPORT_BASE + 0x0000)
#define GPIOB_BASE            (IOPORT_BASE + 0x0400)
#define GPIOC_BASE            (IOPORT_BASE + 0x0800)

typedef struct {
    volatile uint32_t MODER;    // Offset 0x00
    volatile uint32_t OTYPER;   // Offset 0x04
    volatile uint32_t OSPEEDR;  // Offset 0x08
    volatile uint32_t PUPDR;    // Offset 0x0C
    volatile uint32_t IDR;      // Offset 0x10
    volatile uint32_t ODR;      // Offset 0x14
    volatile uint32_t BSRR;     // Offset 0x18
    volatile uint32_t LCKR;     // Offset 0x1C
    volatile uint32_t AFR[2];   // Offset 0x20-0x24
} GPIO_TypeDef;

typedef struct {
    volatile uint32_t CR;       // Offset 0x00
    volatile uint32_t ICSCR;    // Offset 0x04
    volatile uint32_t CFGR;     // Offset 0x08
    volatile uint32_t PLLCFGR;  // Offset 0x0C
    volatile uint32_t PLLSAI1CFGR; // Offset 0x10
    volatile uint32_t CIER;     // Offset 0x14
    volatile uint32_t CIFR;     // Offset 0x18
    volatile uint32_t CICR;     // Offset 0x1C
    volatile uint32_t IOPRSTR;  // Offset 0x20
    volatile uint32_t AHBRSTR;  // Offset 0x24
    volatile uint32_t APB2RSTR; // Offset 0x28
    volatile uint32_t APB1RSTR; // Offset 0x2C
    volatile uint32_t IOPENR;   // Offset 0x30
    volatile uint32_t AHBENR;   // Offset 0x34
    volatile uint32_t APB2ENR;  // Offset 0x38
    volatile uint32_t APB1ENR;  // Offset 0x3C
    volatile uint32_t IOPSMENR; // Offset 0x40
    volatile uint32_t AHBSMENR; // Offset 0x44
    volatile uint32_t APB2SMENR;// Offset 0x48
    volatile uint32_t APB1SMENR;// Offset 0x4C
    volatile uint32_t CCIPR;    // Offset 0x50
    volatile uint32_t CSR;      // Offset 0x54
} RCC_TypeDef;

#define GPIOA ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC ((GPIO_TypeDef *) GPIOC_BASE)
#define RCC   ((RCC_TypeDef *) RCC_BASE)

// Declaracion de direcciones de memoria

// -x-x-x-x- Funciones Generales -x-x-x-x-
void timer_loop(int32_t ticks){
    for(int32_t  t = 0; t < ticks; t++) {
        for(int32_t  t = 0; t < 100; t++) {
            __asm__ __volatile__ ("nop");
        }
    }
}

uint8_t display_digits[10] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111 // 9
};

// -x-x-x-x- Funciones Keypad -x-x-x-x-
int16_t keypad_read() {
    GPIOC->ODR |= (0x0F << 0);  // reset

    GPIOC->ODR &= ~(1 << 0);
    timer_loop(20);
    if (GPIOC-> IDR ^ (1 << 4)) {
        return 1; 
    } else if (GPIOC-> IDR ^ (1 << 5)) {
        return 4; 
    } else if (GPIOC-> IDR ^ (1 << 6)) {
        return 7; 
    } else if (GPIOC-> IDR ^ (1 << 7)) {
        return '*'; 
    }
    
    
    GPIOC->ODR |= (0x0F << 0);  // reset
    GPIOC->ODR &= ~(1 << 1);
    timer_loop(20);
    if (GPIOC-> IDR ^ (1 << 4)) {
        return 2; 
    } else if (GPIOC-> IDR ^ (1 << 5)) {
        return 5; 
    } else if (GPIOC-> IDR ^ (1 << 6)) {
        return 8; 
    } else if (GPIOC-> IDR ^ (1 << 7)) {
        return 0; 
    }

    GPIOC->ODR |= (0x0F << 0);  // reset
    GPIOC->ODR &= ~(1 << 2);
    timer_loop(20);
    if (GPIOC-> IDR ^ (1 << 4)) {
        return 3; 
    } else if (GPIOC-> IDR ^ (1 << 5)) {
        return 6; 
    } else if (GPIOC-> IDR ^ (1 << 6)) {
        return 9; 
    } else if (GPIOC-> IDR ^ (1 << 7)) {
        return '#'; 
    }

    GPIOC->ODR |= (0x0F << 0);  // reset
    GPIOC->ODR &= ~(1 << 3);
    timer_loop(20);
    if (GPIOC-> IDR ^ (1 << 4)) {
        return 'A';
    } else if (GPIOC-> IDR ^ (1 << 5)) {
        return 'B';
    } else if (GPIOC-> IDR ^ (1 << 6)) {
        return 'C'; 
    } else if (GPIOC-> IDR ^ (1 << 7)) {
        return 'D'; 
    }

}

void move_microwave_time(uint8_t *microwave_time_ptr[4], int8_t key) {
    *microwave_time_ptr[3] = key;
    *microwave_time_ptr[2] = *microwave_time_ptr[3];
    *microwave_time_ptr[1] = *microwave_time_ptr[2];
    *microwave_time_ptr[0] = *microwave_time_ptr[1];
}



// -x-x-x-x- Funciones LCD -x-x-x-x-

void escribir_caracter_lcd(char caracter) {
    // Extraer nibbles del caracter en ASCII
    uint8_t alto, bajo = 0;
    alto = (caracter >> 4) & 0x0F;
    bajo = caracter & 0x0F;


    GPIOB->BSRR = (0x1F << 0+16); // Reset D0, D1, D2, D3, E
    GPIOB->BSRR = (alto << 0); // Enviar alto
    GPIOB->BSRR = (1 << 4); // Set E
    timer_loop(20);
    GPIOB->BSRR = (0x1F << 0+16); // Reset D0, D1, D2, D3, E
    timer_loop(20);
    GPIOB->BSRR = (bajo << 0); // Enviar bajo
    GPIOB->BSRR = (1 << 4); // Set E
    timer_loop(20);

}

void escribir_texto_lcd(char* mensaje) {
    GPIOB->BSRR = (1 << 5); // RS Modo datos
    int16_t len = strlen(mensaje);
    for (int16_t i = 0; i < len; i++) {
        escribir_caracter_lcd(mensaje[i]);
    }
}

void salto_linea_lcd() {
    // Comando salto linea alto (0xC0 = 0b1100_0000)
    GPIOB->BSRR = (0x3F << 0+16); // Reset D0, D1, D2, D3, E y RS
    GPIOB->BSRR = (0xC << 0); // enviar alto
    GPIOB->BSRR = (1 << 4); // Set E
    timer_loop(20);
    GPIOB->BSRR = (0x1F << 0+16); // Reset D0, D1, D2, D3, E
    timer_loop(20);
    GPIOB->BSRR = (0x0 << 0); // Enviar bajo
    GPIOB->BSRR = (1 << 4); // Set E
    timer_loop(20);
}

void limpiar_lcd() {
    // Comando limpiar LCD (0x01 = 0b0000_0001)
    GPIOB->BSRR = (0x3F << 0+16); // Reset D0, D1, D2, D3, E y RS
    GPIOB->BSRR = (0x0 << 0); // enviar alto
    GPIOB->BSRR = (1 << 4); // Set E
    timer_loop(20);
    GPIOB->BSRR = (0x1F << 0+16); // Reset D0, D1, D2, D3, E
    timer_loop(20);
    GPIOB->BSRR = (0x1 << 0); // Enviar bajo
    GPIOB->BSRR = (1 << 4); // Set E
    timer_loop(20);
}

// -x-x-x-x- Main -x-x-x-x-

int main() {

    // inicializacion de los perifericos

    // Otras variables
    uint8_t fsm_keypad = 1; // Para leer la columna del keypad
    int16_t key = -1; // tecla presionada
    int16_t last_key = -2; // ultima tecla presionada (para debounce) -2 para que no sea igual a -1
    uint8_t microwave_time[4] = {0, 0, 0, 0}; // tiempo a mostrar en el display, MM:SS
    uint16_t counter = 0; // contador para el tiempo del microondas

    // Variables para mensajes en LCD
    char mensaje_tiempo[5] = "00:00"; // mensaje a mostrar en el display
    char mensaje1[] = "Poporopos";
    char mensaje2[] = "Pizza";
    char mensaje3[] = "Carne";
    char mensaje4[] = "Verduras";
    uint8_t display_fsm = 0; // fsm para el display
    uint8_t microwave_running = 0; // estado del microondas

    while(1) {

        key = keypad_read(&fsm_keypad); // boton presionado
        
        if (key == last_key) {
            key = -1;
        } else {
            last_key = key;
        } // debounce

        fsm_keypad++;
        if (fsm_keypad > 4) fsm_keypad = 1; // reset fsm

        if (key != -1 && (key >= 0 && key <= 9)) {
        move_microwave_time(microwave_time, key); // actualizar tiempo del microondas
        }

        switch (key)
        {
        case 'A':
            // Mostrar en la LCD "Poporopos 2:30"
            limpiar_lcd();
            escribir_texto_lcd(mensaje1);
            salto_linea_lcd();
            strcpy(mensaje_tiempo, "02:30");
            escribir_texto_lcd(mensaje_tiempo);
            microwave_time[0] = 0;
            microwave_time[1] = 2;
            microwave_time[2] = 3;
            microwave_time[3] = 0;
            break;
        case 'B':
            // Mostrar en la LCD "Pizza 1:30"
            limpiar_lcd();
            escribir_texto_lcd(mensaje2);
            salto_linea_lcd();
            strcpy(mensaje_tiempo, "01:30");
            escribir_texto_lcd(mensaje_tiempo);
            microwave_time[0] = 0;
            microwave_time[1] = 1;
            microwave_time[2] = 3;
            microwave_time[3] = 0;
            break;
        case 'C':
            // Mostrar en la LCD "Carne 10:00"
            limpiar_lcd();
            escribir_texto_lcd(mensaje3);
            salto_linea_lcd();
            strcpy(mensaje_tiempo, "10:00");
            escribir_texto_lcd(mensaje_tiempo);
            microwave_time[0] = 1;
            microwave_time[1] = 0;
            microwave_time[2] = 0;
            microwave_time[3] = 0;
            break;
        case 'D':
            // Mostrar en la LCD "Verduras 3:00"
            limpiar_lcd();
            escribir_texto_lcd(mensaje4);
            salto_linea_lcd();
            strcpy(mensaje_tiempo, "03:00");
            escribir_texto_lcd(mensaje_tiempo);
            microwave_time[0] = 0;
            microwave_time[1] = 3;
            microwave_time[2] = 0;
            microwave_time[3] = 0;
            break;
        case '#':
            // Limpiar LCD y cancelar
            limpiar_lcd();
            microwave_time[0] = 0;
            microwave_time[1] = 0;
            microwave_time[2] = 0;
            microwave_time[3] = 0;
            break;
        case '*':
            // Iniciar microondas
            microwave_running = 1;
            break;
        default:
            break;
        }

        switch (display_fsm)
        {
        case 0:
            GPIOA->BSRR = (0x7FF << 0+16); // Reset display pines de 0 a 11
            GPIOA->BSRR = (display_digits[microwave_time[0]] << 0); // Mostrar digito 1
            GPIOA->BSRR = (0x1 << 8); // Encender display 1
            timer_loop(20);
            display_fsm++;
            break;
        case 1:
            GPIOA->BSRR = (0x7FF << 0+16); // Reset display pines de 0 a 11
            GPIOA->BSRR = (display_digits[microwave_time[1]] << 0); // Mostrar digito 2
            GPIOA->BSRR = (0x1 << 9); // Encender display 1
            timer_loop(20);
            display_fsm++;
            break;
        case 2:
            GPIOA->BSRR = (0x7FF << 0+16); // Reset display pines de 0 a 11
            GPIOA->BSRR = (display_digits[microwave_time[2]] << 0); // Mostrar digito 3
            GPIOA->BSRR = (0x1 << 10); // Encender display 1
            timer_loop(20);
            display_fsm++;
            break;
        case 3:
            GPIOA->BSRR = (0x7FF << 0+16); // Reset display pines de 0 a 11
            GPIOA->BSRR = (display_digits[microwave_time[3]] << 0); // Mostrar digito 4
            GPIOA->BSRR = (0x1 << 11); // Encender display 1
            timer_loop(20);
            display_fsm = 0;
            break;

        default:
            GPIOA->BSRR = (0x7FF << 0+16); // Reset display pines de 0 a 11
            break;
        }

        if(microwave_running == 1) {
            counter++;
            if (counter >= 100) {
                counter = 0;
                if (microwave_time[3] > 0) {
                    microwave_time[3]--;
                } else {
                    if (microwave_time[2] > 0) {
                        microwave_time[2]--;
                        microwave_time[3] = 9;
                    } else {
                        if (microwave_time[1] > 0) {
                            microwave_time[1]--;
                            microwave_time[2] = 5;
                            microwave_time[3] = 9;
                        } else {
                            if (microwave_time[0] > 0) {
                                microwave_time[0]--;
                                microwave_time[1] = 9;
                                microwave_time[2] = 5;
                                microwave_time[3] = 9;
                            } else {
                                // Tiempo terminado
                                microwave_running = 0;
                            }
                        }
                    }
                }
            }
        }

    }
}