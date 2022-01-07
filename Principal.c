/********************************************************************************/
/*					GENERADOR DE IMÁGENES ESTÁTICAS CON SALIDA DE VIDEO							*/
/*				Revisión:				1.00									*/
/*				PIC:					PIC18F4620								*/
/*				MODEM:					SIM340CZ								*/
/*				RTC:					DS1307									*/
/*				Compilador:				MPLAB IDE 8.36 - HI-TECH PIC18 9.50 PL3	*/
/*				Checksum:				0X0f41	(PROTEGIDO)						*/
/*										0X1769	(ABIERTO)						*/
/*				Fecha de creación:		14/10/2009								*/
/*				Fecha de modificación:	10/05/2010								*/
/*				Autor:					Mariano Ariel Deville					*/
/********************************************************************************/
#define		PIC_CLK			40000000	// 40Mhz.								*/
#define		ENTRADA			1			//										*/
#define		SALIDA			0			//										*/
#define 	MODO_IMP		RD2=0;RC5=1	// Sal./ent. RS232 a la impresora.		*/
#define		MODO_MODEM		RD2=1;RC5=0	// Sal./ent. RS232 al MODEM.			*/
#define		MODO_PC			RD2=1;RC5=1	// Sal./ent. RS232 exclusiva a PC.		*/
#define		BASE			103			//										*/
#define		LINEAS			506			//										*/
/*------------------------------------------------------------------------------*/
/*				Defino los nombres de los pines de E/S							*/
#define		DATO_PS2		RD7			// Datos desde un teclado PS2.			*/
#define		CLOCK_PS2		RB0			// Clock desde un teclado PS2.			*/
#define		DCD				RB2			// Entrada DCD desde el MODEM.			*/
#define		POWER_MODEM		RB4			// Manejo el POWER del MODEM.			*/
#define		LED				RB1			// LED DEBUG.							*/
#define		NET_STATE		RB5			// Estado de conección del MODEM.		*/
/********************************************************************************/
/*						VARIABLES GLOBALES										*/
unsigned int lineas,linea;
unsigned long aux;
unsigned char i,otro=0b00011100;
bit paridad;
/********************************************************************************/
/*						PROTOTIPO DE FUNCIONES									*/

/********************************************************************************/
#include	"htc.h"				// Necesario para el compilador.				*/
#include	"imagen.h"			//												*/
#include	"Interrup.c"		// Manejo de interrupciones.					*/
/********************************************************************************/
__CONFIG(1,IESOEN & FCMEN & HSPLL);					//							*/
__CONFIG(2,BOREN & WDTEN & PWRTEN);					//							*/
__CONFIG(3,MCLRDIS & LPT1DIS & PBDIGITAL);			//							*/
__CONFIG(4,XINSTEN & DEBUGDIS & LVPDIS & STVREN);	//							*/
__CONFIG(5,UNPROTECT);								//							*/
__CONFIG(6,WRTEN);									//							*/
/********************************************************************************/
void main(void)
{
/********************************************************************************/
/*				Configuración de los puertos									*/
/*------------------------------------------------------------------------------*/
	ADCON1=0b00001111;		// Sin entradas analógicas.							*/
	PORTA=0;
	PORTB=0x00010000;
	PORTC=0;
	PORTD=0;
	PORTE=0;
/*------------------------------------------------------------------------------*/
	TRISA0=SALIDA;	   		// Salida para el LCD RS.							*/
	TRISA1=SALIDA;   		// Salida para el LCD E.							*/
	TRISA2=SALIDA; 	  		// Salida para el LCD AD4.							*/
	TRISA3=SALIDA;   		// Salida para el LCD AD5.							*/
	TRISA4=SALIDA;   		// Salida para el LCD AD6.							*/
	TRISA5=SALIDA;   		// Salida para el LCD AD7.							*/
/*------------------------------------------------------------------------------*/
	TRISB0=ENTRADA;			// PS2 - CLOCK.										*/
	TRISB1=SALIDA;			// Salida al LED de ESTADO.							*/
	TRISB2=ENTRADA;			// DCD del MODEM.									*/
	TRISB3=ENTRADA;			// ICSP.											*/
	TRISB4=SALIDA;			// Manejo POWER del MODEM							*/
	TRISB5=ENTRADA;			// Estado del MODEM (NET STATE).					*/
	TRISB6=ENTRADA;			// ICSP.											*/
	TRISB7=ENTRADA;			// ICSP.											*/
/*------------------------------------------------------------------------------*/
	TRISC0=SALIDA;			// Entrada auxiliar C 1.							*/
	TRISC1=ENTRADA;			// Entrada auxiliar C 2.							*/
	TRISC2=ENTRADA;			// Entrada auxiliar C 3.							*/
	TRISC3=ENTRADA;			// I2C - SCL serial clock.							*/
	TRISC4=ENTRADA;			// I2C - SDA serial data.							*/
	TRISC5=SALIDA;			// Habilitación RS232 para el MODEM.				*/
	TRISC6=SALIDA;			// RS232 - Salida TX.								*/
	TRISC7=ENTRADA;			// RS232 - Entrada RX.								*/
/*------------------------------------------------------------------------------*/
	TRISD0=SALIDA;			// Entrada auxiliar D 1.							*/
	TRISD1=SALIDA;			// Entrada auxiliar D 2.							*/
	TRISD2=SALIDA;			// Habilitación RS232 para la impresora.			*/
	TRISD3=SALIDA;			//													*/
	TRISD4=SALIDA;			//													*/
	TRISD5=SALIDA;			//													*/
	TRISD6=SALIDA;			//													*/
	TRISD7=ENTRADA;			// PS2 - DATOS.										*/
/*------------------------------------------------------------------------------*/
	TRISE0=SALIDA;			//													*/
	TRISE1=SALIDA;			//													*/
	TRISE2=SALIDA;			//													*/
/********************************************************************************/
/*			TIMER 0 - 								*/
/*------------------------------------------------------------------------------*/
	T0CS=0;					// Oscilador interno.								*/
	T0SE=0;					// Flanco ascendente.								*/
	PSA=0;					// Uso el preescaler.								*/
	T0PS0=1;
	T0PS1=0;
	T0PS2=0;
	TMR0IF=0;				// Bajo la bandera de la interrupción.				*/
	T08BIT=1;
	TMR0L=0;
	TMR0ON=1;
/********************************************************************************/
/*			TIMER 1 - 	*/
/*------------------------------------------------------------------------------*/
	T1CKPS0=0; 				// Preescaler TMR1 a 1:8.							*/
	T1CKPS1=0; 				//													*/
	T1SYNC=0;				// No sincronizo con clock externo.					*/
	T1OSCEN=0;				// Oscilador deshabilitado.							*/
	TMR1CS=0;  				// Reloj interno Fosc/4.							*/
	TMR1IF=0;				// Bajo la bandera de la interrupción.				*/
	TMR1ON=0;				// Apago el TMR1.									*/
	T1RD16=1;
	TMR1H=0;				// Configuro el tiempo que tarda en generar			*/
	TMR1L=0;				// la interrupcion.									*/
	TMR1=65530;				// la interrupcion.									*/
/********************************************************************************/
/*			TIMER 2 - NO UTILIZADO												*/
/*------------------------------------------------------------------------------*/
	TMR2ON=0;				// Timer 2 apagado.									*/
	T2CKPS0=0;				// Configuro el Preescaler.							*/
	T2CKPS1=0;				// 													*/
	TMR2IF=0;				// Bajo la bandera de la interrupción.				*/
/********************************************************************************/
/*			Configuración de las interrupciones									*/
/*------------------------------------------------------------------------------*/
	IPEN=0;					// Deshabilito las prioridades para las int.		*/
	GIE=0;					// Utilizo interrupciones.							*/
	PEIE=1;					// Interrupcion externa habilitada.					*/
	INT0IE=0;				// Interrupcion RB0/INT deshabilitada.				*/
	INT1IE=0;				// Interrupcion RB1/INT deshabilitada.				*/
	INT2IE=0;				// Interrupcion RB2/INT deshabilitada.				*/
	TMR0IE=1;				// Interrupcion desborde TMR0 deshabilitada.		*/
	TMR1IE=0;				// Interrupcion desborde TMR1 deshabilitada.		*/
	TMR2IE=0;				// Interrupcion desborde TMR2 deshabilitada.		*/
	CCP1IE=0;				// CCP1 Interrupt disable.							*/
	CCP2IE=0;				// CCP2 Interrupt disable.							*/
	CMIE=0;					// Comparator Interrupt disable.					*/
	EEIE=0;					// EEPROM Write Operation Interrupt disable.		*/
	SSPIE=0;				// Interrupcion por comunicacion I2C.				*/
	PSPIE=0;				// Slave Port Read/Write Interrupt disable.			*/
	BCLIE=0;				// Bus Collision Interrupt disable.					*/
	ADIE=0;					// Interrupcion del conversor AD deshabilitada.		*/
	RBIE=0;					// Interrupcion por RB deshabilitada.				*/
	RCIE=0;					// Interrupcion recepcion USART habilitada.			*/
 	INTEDG0=0;				// Interrupcion en el flanco descendente de RB0.	*/
	RBPU=1;					// RB pull-ups estan deshabilitadas.				*/
/********************************************************************************/
	Cargar_Imagen();
	LED=1;					// Led de POWER encendido.
	GIE=1;
	while(1)
	{
		aux=aux>>1;				// Voy al punto siguiente.
		RD1=aux&paridad;		// Solo imprimo las lineas impares.
	}
}
