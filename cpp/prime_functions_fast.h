#include <gmp.h>
#include <gmpxx.h>
#include <ctime>

using namespace std;

/****************************************************************************************************************************/

inline void mpz_rshift(mpz_t rop, mpz_t op1, mp_bitcnt_t op2) {
	mpz_fdiv_q_2exp(rop, op1, op2);
}



/****************************************************************************************************************************/


void print_gmp(mpz_t val){
	// printf("val: ");
	mpz_out_str(stdout, 10, val);
	printf("\n");
}



/****************************************************************************************************************************/


bool trial_composite(mpz_t a, mpz_t d, mpz_t n, mpz_t s){
	mpz_t t1, t2, t3, nprime, one, two, iterator;
	mpz_inits(t1, t2, t3, nprime, one, two, iterator, NULL);

	mpz_set_ui(one, 1);
	mpz_set_ui(two, 2);
	mpz_sub_ui(nprime, n, 1);

	mpz_powm(t1, a, d, n);

	if (mpz_cmp(t1, one) == 0){
		mpz_clears(t1, t2, t3, nprime, one, two, iterator, NULL);
		return false;
	}

	mpz_set_ui(iterator, 0);
	while (true){
		mpz_powm(t2, two, iterator, n);
		mpz_mul(t2, t2, d);

		mpz_powm(t3, a, t2, n);
		if (mpz_cmp(t3, nprime) == 0){
			mpz_clears(t1, t2, t3, nprime,  one, two, iterator, NULL);
			return false;
		}

		mpz_add_ui(iterator, iterator, 1);
        if (mpz_cmp(iterator, s) == 0) { break; }
	}

	mpz_clears(t1, t2, t3, nprime, one, two, iterator, NULL);
	return true;
}



/****************************************************************************************************************************/



bool isPrime_mpz(mpz_t value){
	// printf("entered test\n");
	mpz_t l, a, d, dprime, s, zero;
	// printf("defined items\n");
	mpz_inits(l, a, d, dprime, s, zero, NULL);
	// printf("initialized to zero\n");

	// printf("\n");
	// printf("computing mod...");

	mpz_mod_ui(l, value, 10);
	
	// printf("validating primality...\t%.0f%% complete", 0.0);

	if (mpz_cmp_ui(l, 2) == 0 || mpz_cmp_ui(l, 4) == 0 || mpz_cmp_ui(l, 5) == 0 || mpz_cmp_ui(l, 6) == 0 || mpz_cmp_ui(l, 8) == 0 || mpz_cmp_ui(l, 0) == 0){
		printf("\rvalidating primality...\t%.0f%% complete", 100.0);
		printf("\n");
		return false;
	}

	mpz_set_ui(s, 0);
	mpz_sub_ui(d, value, 1);
	mpz_set_ui(zero, 0);

	while (true){
		mpz_mod_ui(dprime, d, 2);
		if (mpz_cmp(dprime, zero) != 0){
			break;
		}

		mpz_rshift(d, d, 1);
		mpz_add_ui(s, s, 1);
	}

	gmp_randstate_t rstate;
    gmp_randinit_mt(rstate);

	unsigned long seed;
    gmp_randseed_ui(rstate, seed);

	for (int i = 0; i < 8; i++){
		float percent = 100*i/8;
		printf("validating primality...\t%.0f%% complete", percent);

		mpz_urandomm(a, rstate, value);
		if (trial_composite(a, d, value, s)){
			gmp_randclear(rstate);
			mpz_clears(l, a, d, dprime, s, zero, NULL);
			printf("\n");
			return false;
		}
	}

	gmp_randclear(rstate);
	mpz_clears(l, a, d, dprime, s, zero, NULL);
	printf("\n");
	return true;
}
