// g++ -o prime_finder prime_finder.cpp -lgmp

#include <iostream>
#include <math.h>
#include <cstdlib>
#include <cmath>
#include "prime_functions_fast.h"

int mode = 2;

FILE* primefile;

int main(void) {

	if (mode == 1) {
		unsigned int lower = 58*pow(10, 6);
		unsigned int upper = 59*pow(10, 6);

		unsigned int power = lower;

		unsigned int ntrials = upper-lower;
		unsigned int counter = 0;

		while (counter < ntrials){
			printf("\n\n\n`````````````````````````````````````````````````````\n");
			printf("finding new candidate...\n");
			while (power < upper){
				power += 1;
				if (isPrime(power)){
					break;
				}
			}

			float ndigits = log10(power);
			printf("\npower is %.0f digits (%d)\n", ndigits, power);

			bool val = LucasLehmer(power);

			if (val){
				printf("\ncandidate is prime\n");
				mpz_t result, base;
				mpz_inits(result, base, NULL);

				mpz_set_str(base, "2", 10);
				mpz_pow_ui(result, base, power);
				mpz_sub_ui(result, result, 1);

				primefile = fopen("prime.txt", "a");
				fputs("\n", primefile);
				fprintf(primefile, "%u", power);
				fputs(" -- ", primefile);
				mpz_out_str(primefile, 10, result);
				fputs("\n", primefile);
				fclose(primefile);

				mpz_clears(result, base, NULL);
			}
			else{
				printf("\ncandidate is not prime\n");
			}

			if (power > upper){
				break;
			}

			counter++;
		}
	}

	else if (mode == 2){

		gmp_randstate_t rstate;
		gmp_randinit_mt(rstate);

		unsigned long seed;
		gmp_randseed_ui(rstate, seed);

		mpz_t lower, upper, base, random;
		mpz_inits(lower, upper, base, random, NULL);

		mpz_set_ui(base, 10);
		mpz_pow_ui(lower, base, 5*pow(10, 3));
		mpz_pow_ui(upper, base, 5.1*pow(10, 3));


		mpz_urandomm(random, rstate, upper);

		mpz_add(lower, random, lower);
		mpz_add(upper, random, upper);


		cout << "\n\n\n`````````````````````````````````````````````````````" << endl;
		cout << "beginning search for twin primes..." << endl;
		cout << "\n`````````````````````````````````````````````````````" << endl;

		unsigned int counter = 0;
		unsigned int pcounter = 0;
		while (true){
			counter++;

			while(checkLastDigit(lower)){
				mpz_add_ui(lower, lower, 1);
			}

			// printf("\n\n\n`````````````````````````````````````````````````````\n");
			// printf("checking new candidate...\n");

			size_t length = mpz_sizeinbase(lower, 10);
			cout << "\rcandidate #" << counter << " -- " << pcounter << " primes found";
			cout.flush();
			// printf("\rcandidate #%d", counter);
			// printf("candidate is %lu digits\n", length);

			time_t t_to_start = time(NULL);
			if (isPrime_mpz_fast(lower, rstate)){
				pcounter++;
				// printf("\ncandidate was prime\n");
				primefile = fopen("twin_prime_candidates.txt", "a");
				fputs("\n", primefile);
				mpz_out_str(primefile, 10, lower);
				fputs("\n", primefile);
				fclose(primefile);
			}
			else {
				// printf("\ncandidate was not prime\n");
			}
			time_t t_to_end = time(NULL);

			// printf("(took %lds to validate)\n", t_to_end - t_to_start);

			mpz_add_ui(lower, lower, 1);
			int check = mpz_cmp(lower, upper);
			
			if (check > 0){ break; }
		}

		gmp_randclear(rstate);
		mpz_clears(lower, upper, base, random, NULL);
	}
	return 0;
}
