;-------------------------------------------------------------------------
;			    Inicializaciones			    
;-------------------------------------------------------------------------
	
	
;-------------------------------------------------------------------------
;----		  Configuraci�n del oscilador (8 MHz)	              ----
;-------------------------------------------------------------------------
ini_osc	
	banco1
	movlw	b'01110111'
	movwf	osccon
	return 
;-------------------------------------------------------------------------
;----		    Configuraci�n de los puertos E/S		      ----
;-------------------------------------------------------------------------    
ini_es	
	;---- Puerto A ---
	banco0
	clrf	porta
	banco1
	movlw	b'00000001'
	movwf	trisa	    ;Trisa0 entrada y el resto salidas
	banco3
	movlw	b'00000001'
	movwf	ansel	    ;AN0 pin anal�gico y el resto pines digitales
	
	;----- Puerto B ---
	banco3 
	clrf	anselh	    ;RB[5...0] como E/S digital
	movlw	b'11000001'
	movwf	trisb	    ;RB7, RB6 y RB0 entradas, y el resto salidas
	banco0
	clrf	portb	    
	
	;----- Puerto C ---
	banco0
	clrf	portc	    ;Se inicializa a 0 el puerto C
	banco1
	movlw	b'00000010' ;RC1 entrada y el resto salidas
	movwf	trisc
	    
	return
	   
;-------------------------------------------------------------------------
;----		    Configuraci�n de las interrupciones		      ----
;-------------------------------------------------------------------------	
ini_int banco0
	movlw  b'01110000' 
	movwf  intcon	    ;Habilita m�scara PEIE, T0I y INTE	
	clrf	pir1	    ;Limpia banderas perif�ricos
	clrf	pir2	    ;Limpia banderas perif�ricos
	      
	;-- Interrupci�n externa de palmadas --
	banco1
	bsf  option_reg,INTEDG	;Interrupci�n pin INT en alto
	      
	;-- Interrupci�n perif�ricos(conversor A/D)--
	      
	bsf PIE1,ADIE	    ;Activa m�scara que permite A/D interrupci�n
	bsf PIE1,TMR2IE	    ;Activa m�scara que permite TMR2 interrupci�n
        bsf PIE1,TMR1IE	    ;Activa m�scara que permite TMR1 interrupci�n
	      
	;-- Interrupci�n m�dulo CCP2 --
	      
	bsf PIE2,CCP2IE	    ;Activa m�scara de CCP2
	
	clrf	pir2	    ;Limpia banderas perif�ricos
	bsf	intcon,gie  ;Habilita m�scara global
	return
	      
;----------------------------------------------------------------------------
;----		    Configuraci�n m�dulo conversi�n A/D			 ----
;----------------------------------------------------------------------------	
ini_ADC	banco0
	movlw  b'10000001' ;Canal AN0, Fosc/32 , DONE#, habilitada ADC.
	movwf  adcon0
	
	banco1
	movlw  b'00000000' ;Voltaje referencia interno (Vss,Vdd)
	movwf  adcon1	   ;Justificaci�n izquierda
	                   ;8 bits m�s significativos en ADRESH
				
	call  delay	   ;Tiempo de adquisici�n (TACQ= 6us)
	banco0
	bsf   adcon0,go    ;Inicia la conversi�n A/D
	   
	return
	    
;--- Tiempo adquisici�n (Tacq= 6us), espera para nueva conversi�n A/D	      

delay	movlw	d'4'	    ;w toma el valor 4	
	movwf	dlay	    ;dlay = 4   
	
loop	decfsz  dlay,f	    ;dlay = dlay - 1 y salta si es cero
	goto	loop
	return      
	
;----------------------------------------------------------------------------
;----                    Configuraci�n TMR2				 ----
;----------------------------------------------------------------------------		
ini_tmr2
	banco0
	movlw	b'01111011' ;PRE y POST scalers a 16. TMR2 desactivado
	movwf	t2con	    
	clrf	tmr2	    ;Inicializa el TMR2
	movlw	d'60'
	movwf	timer	    ;El n�mero de veces que salta el TMR2 
			    ;Total= 3,2 segundos
	return
	
;-----------------------------------------------------------------------------
;---                   Configuraci�n TMR1 y CCP2			  ----
;-----------------------------------------------------------------------------	
ini_ccp2
	banco0
	movlw	b'00000000' ;Reloj interno,Prescaler a 1, TMR1 deshabilitado
	movwf	t1con	    
	clrf	tmr1l	    ;Inicializa el TIMER1
	clrf	tmr1h	    	
	
	movlw	b'00000101' ;Modo captura cada pulso de subida
	movwf	ccp2con
	clrf	ccpr2	    ;Inicializa los registros del CCP2 (ccpr2L y ccpr2H)
	
	return