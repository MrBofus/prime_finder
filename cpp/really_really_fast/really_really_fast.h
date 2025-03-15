/****************************************************************************************************************************/
// import libraries
//
// libraries imported:
//    prime_functions.h  --  library importing gmp and gmpxx

#include <iostream>
#include <gmp.h>
#include <gmpxx.h>
#include <ctime>
#include <tuple>
#include <chrono>
#include <omp.h>

using namespace std::chrono;
using namespace std;

/****************************************************************************************************************************/

inline void mpz_rshift(mpz_t rop, mpz_t op1, mp_bitcnt_t op2) {
	mpz_fdiv_q_2exp(rop, op1, op2);
}

/****************************************************************************************************************************/

bool checkLastDigit(mpz_t value){

    mpz_t l;
    mpz_init(l);

    mpz_mod_ui(l, value, 10);

    if (mpz_cmp_ui(l, 2) == 0 || mpz_cmp_ui(l, 4) == 0 || 
        mpz_cmp_ui(l, 5) == 0 || mpz_cmp_ui(l, 6) == 0 || 
        mpz_cmp_ui(l, 8) == 0 || mpz_cmp_ui(l, 0) == 0){
			
		mpz_clear(l);
        return true;
    }

    mpz_clear(l);
    return false;

}


/****************************************************************************************************************************/


char* return_seed(mpz_t value, unsigned int pl){
	char* mpz_seed_str;

	mpz_t mpz_seed, mpz_seed_key, base;
	mpz_inits(mpz_seed, mpz_seed_key, base, NULL);
	
	mpz_set_ui(base, 10);
	mpz_pow_ui(mpz_seed_key, base, pl);
	
	mpz_mod(mpz_seed, value, mpz_seed_key);
	
	mpz_seed_str = mpz_get_str(NULL, 62, mpz_seed);
	
	mpz_clears(mpz_seed, mpz_seed_key, NULL);
	return mpz_seed_str;
}


/****************************************************************************************************************************/


void print_gmp(mpz_t val){
	// printf("val: ");
	mpz_out_str(stdout, 10, val);
	// printf("\n");
}


/****************************************************************************************************************************/


void fast_mpz_powm(mpz_t* res, mpz_t base, mpz_t pow, mpz_t mod){
	mpz_t last, one;
	mpz_inits(last, one, NULL);

	while ( mpz_cmp_ui(pow, 0) > 0 ){
		mpz_and(last, *res, one);

		if ( mpz_cmp_ui(last, 1) == 0){
			mpz_mul(*res, *res, base);
		}

		mpz_mul(base, base, base);
		mpz_rshift(pow, pow, 1);

		mpz_mod(*res, *res, mod);
	}

	mpz_clears(last, one);
}


/****************************************************************************************************************************/
// function: trial composite

bool trial_composite_fast(mpz_t t1, mpz_t t2, mpz_t t3,
                          mpz_t nprime, mpz_t two,
                          mpz_t a, mpz_t d, mpz_t n, mpz_t s, bool verbose){

    mpz_t iterator;
    mpz_init(iterator);
    mpz_set_ui(iterator, 0);

	if (mpz_cmp_ui(t1, 1) == 0){
		return false;
	}

	if (verbose){
		cout << "\t\t-----> starting while loop..." << endl;
		cout.flush();
	}

	auto start = high_resolution_clock::now();
    while (mpz_cmp(iterator, s) < 0){

		// auto powm1start = high_resolution_clock::now();
        mpz_powm(t2, two, iterator, n);
		// auto powm1stop = high_resolution_clock::now();
		// auto mulstart = high_resolution_clock::now();
        mpz_mul(t2, t2, d);
		// auto mulstop = high_resolution_clock::now();
		// auto powm2start = high_resolution_clock::now();
        mpz_powm(t3, a, t2, n);
		// auto powm2stop = high_resolution_clock::now();

		// auto duration1 = duration_cast<microseconds>(powm1stop - powm1start);
		// auto duration2 = duration_cast<microseconds>(mulstop - mulstart);
		// auto duration3 = duration_cast<microseconds>(powm2stop - powm2start);

		// cout << "op 1: " << duration1.count() << "us" << endl;
		// cout << "op 2: " << duration2.count() << "us" << endl;
		// cout << "op 3: " << duration3.count() << "us" << endl;

        if (mpz_cmp(t3, nprime) == 0){

			if (verbose){
				auto stop = high_resolution_clock::now();
				auto duration = duration_cast<microseconds>(stop - start);

				cout << "\t\t\t-----> loop ";
				print_gmp(iterator);
				cout << " took " << duration.count() << "us" << endl;

				cout.flush();
			}
	
            mpz_clear(iterator);
            return false;
        } else {
			if (verbose){
				auto stop = high_resolution_clock::now();
				auto duration = duration_cast<microseconds>(stop - start);

				cout << "\t\t\t-----> loop ";
				print_gmp(iterator);
				cout << " took " << duration.count() << "us" << endl;

				cout.flush();
				start = high_resolution_clock::now();
			}
		}

		mpz_add_ui(iterator, iterator, 1);
    }

    mpz_clear(iterator);
    return true;
}


/****************************************************************************************************************************/


bool isPrime_mpz_fast(mpz_t value, gmp_randstate_t rstate, bool verbose){

	mpz_t a, d, dprime, s;
	mpz_inits(a, d, dprime, s, NULL);

    mpz_t t1, t2, t3, nprime, two;
    mpz_inits(t1, t2, t3, nprime, two, NULL);

    mpz_set_ui(two, 2);
    mpz_sub_ui(nprime, value, 1);

	if (verbose){
		cout << "\n\t-----> checking last digit..." << endl;
		cout.flush();
	}

    auto start = high_resolution_clock::now();
	if (checkLastDigit(value)){
		if (verbose){
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			cout << "\t-----> last digit checked, took " << duration.count() << "us" << endl;
			cout.flush();
		}

        mpz_clears(a, d, dprime, s, NULL);
        mpz_clears(t1, t2, t3, nprime, two, NULL);
		return false;
	} else {
		if (verbose){
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			cout << "\t-----> last digit checked, took " << duration.count() << "us" << endl;
			cout.flush();
		}}

    mpz_set_ui(s, 0);
    mpz_sub_ui(d, value, 1);

	if (verbose){
		cout << "\n\t-----> determining \'s\' and \'d\'..." << endl;
		cout.flush();
	}

	start = high_resolution_clock::now();
    mpz_mod_ui(dprime, d, 2);
	while (mpz_cmp_ui(dprime, 0) == 0){

		mpz_rshift(d, d, 1);
		mpz_add_ui(s, s, 1);

        mpz_mod_ui(dprime, d, 2);
	}

	if (verbose){
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);
		cout << "\t-----> finished, took " << duration.count() << "us" << endl;
		cout.flush();
	}

	for (int i = 0; i < 8; i++){
        mpz_urandomm(a, rstate, value);
        if (mpz_cmp_ui(a, 0) == 0) { mpz_set_ui(a, 1); }
        mpz_powm(t1, a, d, value);

		if (verbose){
			cout << "\n\t-----> starting trial-composite fast test..." << endl;
			cout.flush();
		}
		start = high_resolution_clock::now();
        if (trial_composite_fast(t1, t2, t3,
								nprime, two,
								a, d, value, s, verbose)){

            if (verbose){
				auto stop = high_resolution_clock::now();
				auto duration = duration_cast<microseconds>(stop - start);
				cout << "\t-----> finished, took " << duration.count() << "us" << endl;
				cout.flush();
			}

            mpz_clears(a, d, dprime, s, NULL);
            mpz_clears(t1, t2, t3, nprime, two, NULL);
            return false;
		}
		else{
			if (verbose){
				auto stop = high_resolution_clock::now();
				auto duration = duration_cast<microseconds>(stop - start);
				cout << "\t-----> finished, took " << duration.count() << "us" << endl;
				cout.flush();
			}
		}
	}

	mpz_clears(a, d, dprime, s, NULL);
   	mpz_clears(t1, t2, t3, nprime, two, NULL);

	return true;
}



/****************************************************************************************************************************/


bool __split_parrallel__(mpz_t a, mpz_t d, mpz_t dprime, mpz_t s, 
						 mpz_t t1, mpz_t t2, mpz_t t3, mpz_t nprime, mpz_t two,
						 mpz_t value, gmp_randstate_t rstate, bool verbose){
	

	int truth = 0;

	# pragma omp parallel for reduction(+ : truth)
	for (int i = 0; i < 8; i++){

        mpz_urandomm(a, rstate, value);
		if (mpz_cmp_ui(a, 0) == 0) { mpz_set_ui(a, 1); }
		mpz_powm(t1, a, d, value);

		if (verbose){
			cout << "\n\t-----> starting trial-composite fast test..." << endl;
			cout.flush();
		}

		auto start = high_resolution_clock::now();
		if (trial_composite_fast(t1, t2, t3,
								nprime, two,
								a, d, value, s, verbose)){

			if (verbose){
				auto stop = high_resolution_clock::now();
				auto duration = duration_cast<microseconds>(stop - start);
				cout << "\t-----> finished, took " << duration.count() << "us" << endl;
				cout.flush();
			}

			mpz_clears(a, d, dprime, s, NULL);
			mpz_clears(t1, t2, t3, nprime, two, NULL);
			truth++;
		}
		else{
			if (verbose){
				auto stop = high_resolution_clock::now();
				auto duration = duration_cast<microseconds>(stop - start);
				cout << "\t-----> finished, took " << duration.count() << "us" << endl;
				cout.flush();
			}
		}
	}
	
	if (truth == 0){ return true; } else { return false;}
}


/****************************************************************************************************************************/


bool isP_mpz_fast_split_parallel(mpz_t value, gmp_randstate_t rstate, bool verbose){

	mpz_t a, d, dprime, s;
	mpz_inits(a, d, dprime, s, NULL);

    mpz_t t1, t2, t3, nprime, two;
    mpz_inits(t1, t2, t3, nprime, two, NULL);

    mpz_set_ui(two, 2);
    mpz_sub_ui(nprime, value, 1);

	if (verbose){
		cout << "\n\t-----> checking last digit..." << endl;
		cout.flush();
	}

    auto start = high_resolution_clock::now();
	if (checkLastDigit(value)){
		if (verbose){
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			cout << "\t-----> last digit checked, took " << duration.count() << "us" << endl;
			cout.flush();
		}

        mpz_clears(a, d, dprime, s, NULL);
        mpz_clears(t1, t2, t3, nprime, two, NULL);
		return false;
	} else {
		if (verbose){
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			cout << "\t-----> last digit checked, took " << duration.count() << "us" << endl;
			cout.flush();
		}}

    mpz_set_ui(s, 0);
    mpz_sub_ui(d, value, 1);

	if (verbose){
		cout << "\n\t-----> determining \'s\' and \'d\'..." << endl;
		cout.flush();
	}

	start = high_resolution_clock::now();
    mpz_mod_ui(dprime, d, 2);
	while (mpz_cmp_ui(dprime, 0) == 0){

		mpz_rshift(d, d, 1);
		mpz_add_ui(s, s, 1);

        mpz_mod_ui(dprime, d, 2);
	}

	if (verbose){
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);
		cout << "\t-----> finished, took " << duration.count() << "us" << endl;
		cout.flush();
	}

	__split_parrallel__(a, d, dprime, s, t1, t2, t3, nprime, two,
						value,  rstate, verbose);

	mpz_clears(a, d, dprime, s, NULL);
   	mpz_clears(t1, t2, t3, nprime, two, NULL);

	return true;
}