;-----------------------------------------------------------------------------	
;----				  Aplauso			          ----
;-----------------------------------------------------------------------------

aplauso	btfsc	palmadas,2	;Si el movimiento actual del robot es distinto 
	goto	ssleep		;de paro con una palmada entra en modo sleep
	btfsc	palmadas,0
	goto	nocero
	btfsc	palmadas,1	;Comprobar que está en paro (palmadas = 0)
	goto	nocero
	btfsc	t2con,2		;Comprobar si el TMR 2 está encendido
	goto	cntpal
	clrf	tmr2
	bsf	t2con,2		;Activa el TMR2
	clrf	npalmadas	;Limpia la variable temporal palmadas
cntpal	incf	npalmadas,1	;Incrementa el valor de npalmadas
	return
nocero	movlw	d'5'		;Establece valor 5 para que entre en modo sleep
	movwf	palmadas
	return
ssleep	btfss	palmadas,0	;¿Está en modo sleep que son 5 palmadas?
	goto	nocero		;No. Se pone en modo sleep.
	clrf	palmadas	;Sí. Limpia palmadas y pasa a modo paro 
	return

;-----------------------------------------------------------------------------	
;----				  TMR2			                  ----
;-----------------------------------------------------------------------------

tmr2int	decfsz	timer,f	     ;¿Está timer a cero?
	return		     ;No
	banco0		    
	bcf	t2con,2	     ;Sí. Se deshabilita TMR2
	movlw	d'60'	     ;w toma el valor de 60 
	movwf	timer	     ;Inicializa la variable timer a 60
	movfw	npalmadas    ;w = npalmadas
	movwf	palmadas     ;palmadas = npalmadas
	bcf	pir1,tmr2if  ;Limpia la bandera del TMR2
	return
	
;----------------------------------------------------------------------------	
;----				  NPALMAS			         ----
;----------------------------------------------------------------------------
	
;--Modo Parado--
cero	PARO			;El robot se para
	btfss	siled,0		;Comprueba si se deberían encender las luces
	goto	ledsno0		;No se deben encender
	bsf     LED_DEL_DER	;Sí se deben encender
	bsf	LED_DEL_IZQ
	bsf	LED_TRAS_DER
	bsf	LED_TRAS_IZQ
	goto	bucle2
ledsno0	bcf     LED_DEL_DER	
	bcf	LED_DEL_IZQ
	bcf	LED_TRAS_DER
	bcf	LED_TRAS_IZQ
	goto	bucle2
	
	
;--Una Palmada--	
uno	AVANCE			;El robot avanza
	btfss	siled,0		;Comprueba si se deberían encender las luces
	goto	ledsno1		;No se deben encender
	bsf     LED_DEL_DER	;Sí, se deben encender las dos de delante
	bsf	LED_DEL_IZQ
	bcf	LED_TRAS_DER
	bcf	LED_TRAS_IZQ
	goto	bucle2	
ledsno1	bcf     LED_DEL_DER	
	bcf	LED_DEL_IZQ
	bcf	LED_TRAS_DER
	bcf	LED_TRAS_IZQ
	goto	bucle2
	
;--Dos Palmadas--
dos	RETROC			;El robot retrocede
	btfss	siled,0		;Comprueba si se debería encender las luces
	goto	ledsno2		;No se deben encender
	bsf     LED_TRAS_DER	;Sí, se deben encender las dos de atrás    
	bsf	LED_TRAS_IZQ
	bcf	LED_DEL_DER
	bcf	LED_DEL_IZQ
	goto	bucle2	
ledsno2	bcf     LED_TRAS_DER	
	bcf	LED_TRAS_IZQ
	bcf	LED_DEL_DER
	bcf	LED_DEL_IZQ
	goto	bucle2
	
;--Tres Palmadas--
tres	GIRO_D			;El robot gira a la derecha
	btfss	siled,0		;Comprueba si se debería encender las luces
	goto	ledsno3		;No se deben encender
	bsf     LED_DEL_DER	;Sí,se deben encender las dos de la derecha
	bsf	LED_TRAS_DER
	bcf	LED_TRAS_IZQ
	bcf	LED_DEL_IZQ
	goto	bucle2	
ledsno3	bcf     LED_DEL_DER	
	bcf	LED_TRAS_DER
	bcf	LED_TRAS_IZQ
	bcf	LED_DEL_IZQ
	goto	bucle2
	
;--Cuatro Palmadas--
cuatro	GIRO_I			;El robot gira a la izquierda 
	btfss	siled,0		;Comprueba si se debría encender las luces
	goto	ledsno4		;No se deben encender
	bsf     LED_DEL_IZQ	;Sí,se deben encender las dos de la izquierda
	bsf	LED_TRAS_IZQ
	bcf	LED_TRAS_DER
	bcf	LED_DEL_DER
	goto	bucle2
ledsno4	bcf     LED_DEL_DER	
	bcf	LED_TRAS_DER
	bcf	LED_TRAS_IZQ
	bcf	LED_DEL_IZQ
	goto	bucle2
	
;--- Cinco Palmadas y Modo Sleep ---
; Modo en el que entra al darle una palmada mientras está en cualquier otro estado
	
cinco	movlw   b'00010101'	;Deshabilita todas las interrupciones excepto 
	andwf	intcon,f	;la exterior. Borra flag int. exterior. Se 
				;conservan los flags de TMR0 y PORTB.
	
	banco0
	movf	porta,w		;Guarda el estado del Puerto A
	movwf	porta_temp
	
	
	bcf	izq_av		;Pone salidas puente H y Trigg. a 0
	bcf	izq_ret
	bcf	der_av
	bcf	der_ret
	bcf	TRIG
	
	bcf     LED_DEL_DER	
	bcf	LED_TRAS_DER
	bcf	LED_TRAS_IZQ
	bcf	LED_DEL_IZQ
	
	
	sleep			;El uC entra en modo Sleep y sale cuando se
				;produce una nueva interrupción externa.
				
				;Al estar deshabilitado el bit GIE, una vez
				;despierte el uC continuará con la instr. que
				;viene a continuación, en lugar de ir al vector
				;de interrupción (0004h).
	
	
	movlw	b'11110000'	;Se vuelven a habilitar las int. PEIE y T0IE
	iorwf	intcon,f	;(periféricos y TMR 0).
	
	banco0
	call	delay		;Tiempo de adquisición
	bsf     adcon0,go	;Se vuelve a iniciar la conversión A/D
	goto	bucle2		;Va bucle2 del programa principal
	 


