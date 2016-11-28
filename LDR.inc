
;==============================================================================	
;				Subrutina LDR   			      
;==============================================================================
	
;   Con luz tensión de AN0 = 3.76 V   
;   Sin luz tensión de AN0 = 0.9 V 	
	
;------------------------------------------------------------------------------
;			    Comparación con un nivel
;				Nivel = 1280 mV	
;------------------------------------------------------------------------------
LDR	    
	movlw   b'00111111'  ; w está entre 1260 mV y 1280 mV
	subwf   ADRESH,w     ;(ADRESH)-(w) ->(w)
	btfss   status,c   
	goto    LED_ON	   ;Como(ADRESH < w) no hay luz, las luces se encienden
	bcf	siled,0	   ;Como(ADRESH >=w) hay luz, las luces del coche 
			   ;se deberan apagar y la variable siled toma valor 0
	return
     
LED_ON	bsf	siled,0	    ;Las luces de coche se deberán encender, por tanto
			    ;la variable siled toma el valor 1

	return
