
;--- Retardo1 = 15 us ----
retardo	movlw	d'10'
	movwf	contador
cuenta	decfsz	contador,f
	goto	cuenta	    
	return			    
	
;--- Retardo2 = 0.66 s ----
retardo2
	movlw	d'200'		
	movwf	contador	
cuenta2	decfsz	contador,f
	goto	cuenta3	    
	return
	
cuenta3	movlw	d'200'
	movwf	contador2
cuenta4	decfsz	contador2,f
	goto	cuenta5   
	goto	cuenta2

cuenta5	movlw	d'10'		
	movwf	contador3
cuenta6	decfsz	contador3,f
	goto	cuenta6   
	goto	cuenta4
    
;--- Retardo3 = 90 ms -----
retardo3
	movlw	d'200'		
	movwf	contador	
cuen2	decfsz	contador,f
	goto	cuen3	    
	return
	
cuen3	movlw	d'100'
	movwf	contador2
cuen4	decfsz	contador2,f
	goto	cuen5   
	goto	cuen2

cuen5	movlw	d'2'		
	movwf	contador3
cuen6	decfsz	contador3,f
	goto	cuen6   
	goto	cuen4


