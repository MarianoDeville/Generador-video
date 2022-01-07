/********************************************************************************/
/*								INTERRUPCIONES									*/
/********************************************************************************/
void interrupt isr(void)
{
	if(TMR0IE && TMR0IF)				// Interrupción por TMR0.
	{
		TMR0L=BASE;						// Periodo de 64 Us.
		RD1=0;							// Bajo la luminancia.
		while(TMR0L<=BASE+3);
		if(lineas++!=LINEAS&&(lineas!=(LINEAS/2)))
		{
			RD0=0;						// Bajo el sincronismo.
			while(TMR0L<=BASE+19);		// Espero 4.8 Us
			RD0=1;						// 
			if(i++>0)					// Tamaño del punto
			{
				i=0;
				linea++;
				CLRWDT();
			}
			aux=dibujo[linea];			// Linea a imprimir en pantalla.
			TMR0IF=0;					// Bajo la bandera de la interrupción.
			while(TMR0L<=BASE+31);
			return;
		}
		else
		{
			TMR0L=65;
			RD0=0;						// Bajo el sincronismo.
			while(TMR0L<=72);
			RD0=1;
			while(TMR0L<=145);
			TMR0L=0;
			RD0=0;						// Bajo el sincronismo.
			while(TMR0L<=5);
			RD0=1;
			while(TMR0L<=79);
			TMR0L=0;
			RD0=0;						// Bajo el sincronismo.
			while(TMR0L<=5);
			RD0=1;
			while(TMR0L<=79);
			TMR0L=0;
			RD0=0;						// Bajo el sincronismo.
			while(TMR0L<=5);
			RD0=1;
			while(TMR0L<=79);
			TMR0L=0;
			RD0=0;						// Bajo el sincronismo.
			while(TMR0L<=5);
			RD0=1;
			while(TMR0L<=79);
			TMR0L=0;
			RD0=0;						// Bajo el sincronismo.
			while(TMR0L<=5);
			RD0=1;
			while(TMR0L<=79);
			TMR0L=0;
			RD0=0;						// Bajo el sincronismo.
			while(TMR0L<=72);
			RD0=1;
			while(TMR0L<=83);
			TMR0L=0;
			RD0=0;						// Bajo el sincronismo.
			while(TMR0L<=72);
			RD0=1;
			while(TMR0L<=83);
			TMR0L=0;
			RD0=0;						// Bajo el sincronismo.
			while(TMR0L<=72);
			RD0=1;
			while(TMR0L<=83);
			TMR0L=0;
			RD0=0;						// Bajo el sincronismo.
			while(TMR0L<=72);
			RD0=1;
			while(TMR0L<=83);
			TMR0L=0;
			RD0=0;						// Bajo el sincronismo.
			while(TMR0L<=72);
			RD0=1;
			while(TMR0L<=83);
			TMR0L=0;
			RD0=0;						// Bajo el sincronismo.
			while(TMR0L<=72);
			RD0=1;
			while(TMR0L<=83);
			TMR0L=65;
			RD0=0;						// Bajo el sincronismo.
			while(TMR0L<=72);
			RD0=1;
			while(TMR0L<=145);
			TMR0L=0;
			RD0=0;						// Bajo el sincronismo.
			while(TMR0L<=5);
			RD0=1;
			while(TMR0L<=79);
			TMR0L=0;
			RD0=0;						// Bajo el sincronismo.
			while(TMR0L<=5);
			RD0=1;
			while(TMR0L<=79);
			TMR0L=0;
			RD0=0;						// Bajo el sincronismo.
			while(TMR0L<=5);
			RD0=1;
			while(TMR0L<=79);
			TMR0L=0;
			RD0=0;						// Bajo el sincronismo.
			while(TMR0L<=5);
			RD0=1;
			while(TMR0L<=89);
			TMR0L=150;
			RD0=0;						// Bajo el sincronismo.
			while(TMR0L<=165);
			RD0=1;
			linea=0;
			lineas=0;
			paridad=1;
			TMR0IF=0;						// Bajo la bandera de la interrupción.
			while(TMR0L<=236);
		}
		return;
	}
	return;								// Salgo de las interrupciones.
}
