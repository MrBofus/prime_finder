/****************************************************************************************************************************/
// import libraries
//
// libraries imported:
//    prime_functions.h  --  library importing gmp and gmpxx

#include "prime_functions.h"

/****************************************************************************************************************************/
// function: trial composite

bool trial_composite_fast(mpz_t t1, mpz_t t2, mpz_t t3,
                          mpz_t nprime, mpz_t two,
                          mpz_t a, mpz_t d, mpz_t n, mpz_t s){
	
    // cout << "here" << endl;
    mpz_t iterator;
    mpz_init(iterator);
    mpz_set_ui(iterator, 0);

	if (mpz_cmp_ui(t1, 1) == 0){
		return false;
	}

    while (mpz_cmp(iterator, s) < 0){
        mpz_powm(t2, two, iterator, n);
        mpz_mul(t2, t2, d);

        mpz_powm(t3, a, t2, n);
        if (mpz_cmp(t3, nprime) == 0){
            return false;
        }

		mpz_add_ui(iterator, iterator, 1);
    }
    return true;
}



/****************************************************************************************************************************/
// function: is prime

void mpz_powm_fast(mpz_t result, mpz_t base, mpz_t exponent, mpz_t mod){
    cout << "here" << endl;
    mpz_t iterator;
    mpz_init(iterator);
    mpz_set_ui(iterator, 2);
    
    mpz_mul(result, base, base);
    cout << "here" << endl;

    while(mpz_cmp(iterator, exponent) < 0){

        mpz_mul(result, result, base);
        mpz_mod(result, result, mod);

        mpz_add_ui(iterator, iterator, 1);
    }
}

/****************************************************************************************************************************/


bool checkLastDigit(mpz_t value){

    mpz_t l;
    mpz_init(l);

    mpz_mod_ui(l, value, 10);

    if (mpz_cmp_ui(l, 2) == 0 || mpz_cmp_ui(l, 4) == 0 || 
        mpz_cmp_ui(l, 5) == 0 || mpz_cmp_ui(l, 6) == 0 || 
        mpz_cmp_ui(l, 8) == 0 || mpz_cmp_ui(l, 0) == 0){
        return true;
    }

    mpz_clear(l);
    return false;

}


/****************************************************************************************************************************/

bool isPrime_mpz_fast(mpz_t value, gmp_randstate_t rstate){

	mpz_t a, d, dprime, s;
	mpz_inits(a, d, dprime, s, NULL);

    mpz_t t1, t2, t3, nprime, two;
    mpz_inits(t1, t2, t3, nprime, two, NULL);

    mpz_set_ui(two, 2);
    mpz_sub_ui(nprime, value, 1);
	
    // printf("validating primality...\t%.0f%% complete", 0.0);

	if (checkLastDigit(value)){
		// cout << "\rvalidating primality...\t100% complete";
        mpz_clears(a, d, dprime, s, NULL);
        mpz_clears(t1, t2, t3, nprime, two, NULL);
		return false;
	}

    mpz_set_ui(s, 0);
    mpz_sub_ui(d, value, 1);

    mpz_mod_ui(dprime, d, 2);
	while (mpz_cmp_ui(dprime, 0) == 0){

		mpz_rshift(d, d, 1);
		mpz_add_ui(s, s, 1);

        mpz_mod_ui(dprime, d, 2);
	}

	for (int i = 0; i < 12; i++){
		float percent = 100*i/12;
		// cout << "\rvalidating primality...\t" << percent << "% complete";

        mpz_urandomm(a, rstate, value);
        mpz_powm(t1, a, d, value);

        if (trial_composite_fast(t1, t2, t3,
                                nprime, two,
                                a, d, value, s)){
            
            // cout << "\rvalidating primality...\t100% complete";
            // cout << endl;
		
			mpz_clears(a, d, dprime, s, NULL);
            mpz_clears(t1, t2, t3, nprime, two, NULL);
			return false;
		}
	}

    // cout << "\rvalidating primality...\t100% complete";
    // cout << endl;

	mpz_clears(a, d, dprime, s, NULL);
    mpz_clears(t1, t2, t3, nprime, two, NULL);

	return true;
}
