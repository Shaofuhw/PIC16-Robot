;-------------------------------------------------------------------------
;			    Inicializaciones			    
;-------------------------------------------------------------------------
	
	
;-------------------------------------------------------------------------
;----		  Configuración del oscilador (8 MHz)	              ----
;-------------------------------------------------------------------------
ini_osc	
	banco1
	movlw	b'01110111'
	movwf	osccon
	return 
;-------------------------------------------------------------------------
;----		    Configuración de los puertos E/S		      ----
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
	movwf	ansel	    ;AN0 pin analógico y el resto pines digitales
	
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
;----		    Configuración de las interrupciones		      ----
;-------------------------------------------------------------------------	
ini_int banco0
	movlw  b'01110000' 
	movwf  intcon	    ;Habilita máscara PEIE, T0I y INTE	
	clrf	pir1	    ;Limpia banderas periféricos
	clrf	pir2	    ;Limpia banderas periféricos
	      
	;-- Interrupción externa de palmadas --
	banco1
	bsf  option_reg,INTEDG	;Interrupción pin INT en alto
	      
	;-- Interrupción periféricos(conversor A/D)--
	      
	bsf PIE1,ADIE	    ;Activa máscara que permite A/D interrupción
	bsf PIE1,TMR2IE	    ;Activa máscara que permite TMR2 interrupción
        bsf PIE1,TMR1IE	    ;Activa máscara que permite TMR1 interrupción
	      
	;-- Interrupción módulo CCP2 --
	      
	bsf PIE2,CCP2IE	    ;Activa máscara de CCP2
	
	clrf	pir2	    ;Limpia banderas periféricos
	bsf	intcon,gie  ;Habilita máscara global
	return
	      
;----------------------------------------------------------------------------
;----		    Configuración módulo conversión A/D			 ----
;----------------------------------------------------------------------------	
ini_ADC	banco0
	movlw  b'10000001' ;Canal AN0, Fosc/32 , DONE#, habilitada ADC.
	movwf  adcon0
	
	banco1
	movlw  b'00000000' ;Voltaje referencia interno (Vss,Vdd)
	movwf  adcon1	   ;Justificación izquierda
	                   ;8 bits más significativos en ADRESH
				
	call  delay	   ;Tiempo de adquisición (TACQ= 6us)
	banco0
	bsf   adcon0,go    ;Inicia la conversión A/D
	   
	return
	    
;--- Tiempo adquisición (Tacq= 6us), espera para nueva conversión A/D	      

delay	movlw	d'4'	    ;w toma el valor 4	
	movwf	dlay	    ;dlay = 4   
	
loop	decfsz  dlay,f	    ;dlay = dlay - 1 y salta si es cero
	goto	loop
	return      
	
;----------------------------------------------------------------------------
;----                    Configuración TMR2				 ----
;----------------------------------------------------------------------------		
ini_tmr2
	banco0
	movlw	b'01111011' ;PRE y POST scalers a 16. TMR2 desactivado
	movwf	t2con	    
	clrf	tmr2	    ;Inicializa el TMR2
	movlw	d'60'
	movwf	timer	    ;El número de veces que salta el TMR2 
			    ;Total= 3,2 segundos
	return
	
;-----------------------------------------------------------------------------
;---                   Configuración TMR1 y CCP2			  ----
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