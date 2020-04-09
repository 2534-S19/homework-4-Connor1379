#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "homework4.h"
enum flag {INIT, NUM2, NUM5, NUM3, NUM4, TERM};
int state = INIT;

int main(void)
{
    char rChar;
    char *response = "\n\n\r2534 is the best course in the curriculum!\r\n\n";

    // TODO: Declare the variables that main uses to interact with your state machine.


    // Stops the Watchdog timer.
    initBoard();
    // TODO: Declare a UART config struct as defined in uart.h.
    //       To begin, configure the UART for 9600 baud, 8-bit payload (LSB first), no parity, 1 stop bit.
  struct _eUSCI_eUSCI_UART_ConfigV1 UARTSTRUCT =
    {
              EUSCI_A_UART_CLOCKSOURCE_SMCLK,               // SMCLK Clock Source = 3MHz
              19,                                           // UCBR = 19
              8,                                            // UCBRF = 4800
              0xAA,                                         // UCBRS = 0xAA
              EUSCI_A_UART_NO_PARITY,                       // Even Parity
              EUSCI_A_UART_LSB_FIRST,                       // LSB First
              EUSCI_A_UART_ONE_STOP_BIT,                    // One stop bit
              EUSCI_A_UART_MODE,                            // UART mode
              EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION, // Oversampling
              EUSCI_A_UART_8_BIT_LEN
    };







    // TODO: Make sure Tx AND Rx pins of EUSCI_A0 work for UART and not as regular GPIO pins.
  GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
     GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);

  GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1,
     GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);


    // TODO: Initialize EUSCI_A0
  UART_initModule(EUSCI_A0_BASE,
                   &UARTSTRUCT);

    // TODO: Enable EUSCI_A0
 UART_enableModule(EUSCI_A0_BASE);
char string[200];
int i =0;



    while(1)
    {
        // TODO: Check the receive interrupt flag to see if a received character is available.
        //       Return 0xFF if no character is available.
       uint_fast8_t interuptStatus = UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);

       //UART_transmitData(EUSCI_A0_BASE, 'd');
char letter = 0xFF;

        // TODO: If an actual character was received, echo the character to the terminal AND use it to update the FSM.
        //       Check the transmit interrupt flag prior to transmitting the character.
 if (interuptStatus == EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG){

     letter = UART_receiveData(EUSCI_A0_BASE);

     sendByte(letter);
     if (charFSM(letter)){
         int i =0;
         while (response[i] != '\0'){
             sendByte(response[i]);
             i++;

         }
     }
 }





        // TODO: If the FSM indicates a successful string entry, transmit the response string.
        //       Check the transmit interrupt flag prior to transmitting each character and moving on to the next one.
        //       Make sure to reset the success variable after transmission.



}
}

void initBoard()
{
    WDT_A_hold(WDT_A_BASE);
}

// TODO: FSM for detecting character sequence.
bool charFSM(char rChar)
{
    bool finished = false;
    switch(state) {
          case INIT :
              if (rChar=='2'){
                  state = NUM2;
              }

             break;
          case NUM2 :
              if (rChar == '5'){
                  state = NUM5;
              }
              else{
                  state = INIT;
              }
              break;
          case NUM5 :
             if (rChar == '3'){
                 state = NUM3;
             }
             else {
                 state = INIT;
             }

             break;
          case NUM3 :
             if (rChar== '4'){

                 finished = true;
             }
             state = INIT;
             break;





       }

    return finished;
}

void sendByte(char sendChar){

    uint_fast8_t interuptStatus = UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG);
    while (!((interuptStatus & EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG) == EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)){
       interuptStatus = UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG);
    }

        UART_transmitData(EUSCI_A0_BASE, sendChar);





}
