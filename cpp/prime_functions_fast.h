/****************************************************************************************************************************/
// import libraries
//
// libraries imported:
//    prime_functions.h  --  library importing gmp and gmpxx

#include "prime_functions.h"

/****************************************************************************************************************************/
// function: trial composite

bool trial_composite_fast(mpz_t t1, mpz_t t2, mpz_t t3,
                          mpz_t nprime, mpz_t one, mpz_t two,
                          mpz_t iterator,
                          mpz_t a, mpz_t d, mpz_t n, mpz_t s){
	

	if (mpz_cmp(t1, one) == 0){
		return false;
	}

    while (true){
        mpz_powm(t2, two, iterator, n);
        mpz_mul(t2, t2, d);

        mpz_powm(t3, a, t2, n);
        if (mpz_cmp(t3, nprime) == 0){
            return false;
        }

		mpz_add_ui(iterator, iterator, 1);
        if (mpz_cmp(iterator, s) == 0) { break; }
    }
    return true;
}



/****************************************************************************************************************************/
// function: is prime

void mpz_powm_fast(mpz_t result, mpz_t base, mpz_t exponent, mpz_t mod, mpz_t iterator){
    while(true){
        mpz_mul(result, base, base);
        mpz_mod(result, result, mod);

        if(mpz_cmp(iterator, exponent) == 0){
            break;
        }

        mpz_add_ui(iterator, iterator, 1);
    }
}




/****************************************************************************************************************************/

bool isPrime_mpz_fast(mpz_t value){
	// printf("entered test\n");
	mpz_t l, a, d, dprime, s, zero;
	// printf("defined items\n");
	mpz_inits(l, a, d, dprime, s, zero, NULL);

    mpz_t t1, t2, t3, nprime, one, two, iterator, jiterator;
    mpz_inits(t1, t2, t3, nprime, one, two, iterator, jiterator, NULL);

    mpz_set_ui(one, 1);
    mpz_set_ui(two, 2);
    mpz_sub_ui(nprime, value, 1);

    // printf("initialized to zero\n");

    // printf("\n");
    // printf("computing mod...");

    mpz_mod_ui(l, value, 10);
	
    // printf("validating primality...\t%.0f%% complete", 0.0);

	if (mpz_cmp_ui(l, 2) == 0 || mpz_cmp_ui(l, 4) == 0 || mpz_cmp_ui(l, 5) == 0 || mpz_cmp_ui(l, 6) == 0 || mpz_cmp_ui(l, 8) == 0 || mpz_cmp_ui(l, 0) == 0){
		printf("\rvalidating primality...\t%.0f%% complete", 100.0);
		printf("\n");
        mpz_clears(l, a, d, dprime, s, zero, NULL);
        mpz_clears(t1, t2, t3, nprime, one, two, iterator, NULL);
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

	for (int i = 0; i < 12; i++){
		float percent = 100*i/8;
		printf("validating primality...\t%.0f%% complete", percent);

        mpz_urandomm(a, rstate, value);

        mpz_set_ui(jiterator, 0);
        mpz_powm_fast(t1, a, d, value, jiterator);
        mpz_set_ui(iterator, 0);

        if (trial_composite_fast(t1, t2, t3,
                                nprime, one, two,
                                iterator,
                                a, d, value, s)){
			gmp_randclear(rstate);
			mpz_clears(l, a, d, dprime, s, zero, NULL);
            mpz_clears(t1, t2, t3, nprime, one, two, iterator, NULL);
			printf("\n");
			return false;
		}
	}

	gmp_randclear(rstate);
	mpz_clears(l, a, d, dprime, s, zero, NULL);
    mpz_clears(t1, t2, t3, nprime, one, two, iterator, NULL);
	printf("\n");
	return true;
}
