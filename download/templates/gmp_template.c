///////////////////////////////////////////
// File Name: gmp_template.c
// Author: Vainpointer
// Created Time: Mon Sep 28 23:12:15 2015
///////////////////////////////////////////

//gcc gmp_template.c -I/usr/local/include -L/usr/local/lib -lgml
#include <stdio.h>
#include <gmp.h>

int main()
{
	mpz_t a,b,c;
	char *s1 = "123111111111111122222222222444444444111111112211";
	char *s2 = "213213214531231232132122112332122222211111112221";
	mpz_init(c);
	mpz_init_set_str(a, s1, 10);
	mpz_init_set_str(b, s2, 10);
	mpz_mul(c, a, b);
	mpz_out_str(stdout, 10, c);
	printf("\n");
	return 0;
}
