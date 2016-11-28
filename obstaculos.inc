;-------------------------------------------------------------------------
;			    Sensor de ultrasonido			    
;-------------------------------------------------------------------------

;------------------------------------------------------------------------------	
;----            	      Interrupción CCP2		         	    ---
;------------------------------------------------------------------------------	
ccpint	banco0
	btfss	ccp2con,0   ;¿Está el modo captura en subida?
	goto	bajada
	bcf	ccp2con,0   ;Pone el CCP2 para que salte con bajada

	clrf	tmr1l	    ;Limpia los registros de tmr1
	clrf	tmr1h
	bsf	t1con,0	    ;Activa el tmr1
	bcf	pir2,0	    ;Limpia la bandera de ccp2
	return
bajada	banco0
	btfss   t1con,0	    ;¿Está el tmr1 activado?
	goto	noobstac    ;No. Significa que no hay obstáculo
	bcf	t1con,0	    ;Sí. Entonces desactiva el tmr1 
	bsf	ccp2con,0   ;Pone el CCP2 para que salte con subida
	
; Ahora se compara el valor de anchura del Echo (que está en ccpr2h y ccpr2l)
; con una distancia de 35 cm.
	
	movlw	b'00001111' 
	subwf	ccpr2h,w    ;(ccpr2h)-(w)-> w
	btfss	status,c    ; Comprueba si obstáculo está a más de 35cm
	goto	obstac	    ;(ccpr2h)< w por tanto hay obstaculo a d<=35cm
	bcf     pir2,0      ;(ccpr2h)< w por tanto el obstáculo está a d >35cm
			    ; y no modifica su trayectoria
			    
	bcf	pir2,0	    ;Limpia la bandera ccp2
	bcf	t1con,0	    ;Desactiva el tmr1
	clrf	tmr1h	    ;Limpia los registros de tmr1 
	clrf	tmr1l
	return
	
obstac	PARO		    ;Hay obstáculo y se detiene durante 0.66 seg
	call	retardo2    
	RETROC		    ;Retrocede durante 1.32 seg
	call	retardo2   
	call	retardo2
	PARO		    ;Se detiene durante 0.66 seg
	call	retardo2
	GIRO_I		    ;Gira durante 0.66 seg
	call	retardo2
	PARO		    ;Se detiene
	bcf	pir2,0	    ;Limpia la bandera ccp2
	bcf	t1con,0	    ;Desactiva el tmr1
	clrf	tmr1h	    ;Limpia los registros de tmr1
	clrf	tmr1l
	return
	
noobstac bcf	pir2,0		;Limpia la bandera ccp2
	 bsf	ccp2con,ccp2m0	;Pone el CCP2 para que salte con subida
	 return
	 
;------------------------------------------------------------------------------
;---			    Interrupción TMR1				   ----
;------------------------------------------------------------------------------	 
;Significa que no hay obstáculo a una distancia de 3 o 4 metros	

tmr1int	 banco0
	 bcf	t1con,0   ;Desactiva el tmr1
	 bcf	pir1,0	  ;Borra la bandera tmr1
	 nop
	 return
	
