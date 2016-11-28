;-------------------------------------------------------------------------
;				Macros			    
;-------------------------------------------------------------------------
	
	
;-------------------------------------------------------------------------	
;----	  Direccionamiento directo a los bancos de memoria de datos   ----
;-------------------------------------------------------------------------
	
;---- Acceso directo a banco 0 ----		
banco0	macro
	bcf	status,rp1
	bcf	status,rp0
	endm
;---- Acceso directo a banco 1 -----
banco1	macro
	bcf	status,rp1
	bsf	status,rp0
	endm	    
;---- Acceso directo a banco 2 -----
banco2	macro
	bsf	status,rp1
	bcf	status,rp0
	endm
;---- Acceso directo a banco 3 -----
banco3	macro
	bsf	status,rp1
	bsf	status,rp0
	endm	

	
;-------------------------------------------------------------------------	
;----			Movimiento del coche			      ----
;-------------------------------------------------------------------------
	
RETROC	macro
	banco0
	bsf	der_ret		;Rueda derecha hacia atrás
	bsf	izq_ret		;Rueda izquierda hacia atrás
	endm
	
AVANCE	macro
	banco0
	bsf	der_av		;Rueda derecha hacia delante	    
	bsf	izq_av	        ;Rueda izquierda hacia delante
	endm
	
GIRO_D	macro
	banco0
	bsf	izq_av		;Rueda izquierda hacia delante
	bsf	der_ret		;Rueda derecha hacia atrás
	endm
	
GIRO_I	macro
	banco0
	bsf	der_av		;Rueda derecha hacia delante
	bsf	izq_ret	        ;Rueda izquierda hacia atrás
	endm
	
PARO	macro
	banco0
	bcf	der_ret		
	bcf	der_av
	bcf	izq_av
	bcf	izq_ret
	endm	
	
