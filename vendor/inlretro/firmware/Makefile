#Single Makefile for all devices

all:
	make -f Make_avr clean hex
	make -f Make_stm_inl6 clean 
	make -f Make_stm_inl6 

avr:
	make -f Make_avr clean program
stm6:
	make -f Make_stm_inl6 program
stm6clean:
	make -f Make_stm_inl6 clean program
stm6p:
	make -f Make_stm_inl6p clean program
stmad:
	make -f Make_stm_adapter clean program
stmn:
	make -f Make_stm_nes clean program
