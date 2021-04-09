run: hy457_crypto.c
	gcc   hy457_crypto.c
clean: 
	rm -f *.o  a.out affine_encrypted feistel_encrypted ceasar_encrypted otp_encrypted plaifair_encrypted