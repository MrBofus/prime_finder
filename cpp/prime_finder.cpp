// g++ -o prime_finder prime_finder.cpp -lgmp

#include <iostream>
#include <iomanip>
#include <math.h>
#include <cstdlib>
#include <cmath>
#include "prime_functions/prime_functions_fast.h"

int mode = 1;

FILE* primefile;


int main(void) {

	cout << "choose a mode: \n\t(1) - for lucas lehmer";
	cout << "\n\t(2) - for random miller-rabin";
	cout << "\n\t(3) - specific value primality test\n\n";
	cout << "remember to have fun out there !! :)\n";
	cout << "mode: ";
	cin >> mode;

	if (mode == 1) {
		// unsigned int lower = 65123456;
		// unsigned int upper = 65223456;
		unsigned int lower = 65123456;
		unsigned int upper = 65223456;

		unsigned int power = lower;

		unsigned int ntrials = upper-lower;
		unsigned int counter = 0;

		while (counter < ntrials){
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

			std::cout << full_logo << '\n';

			printf("              **Lucas-Lehmer Variant**\n");
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

				primefile = fopen("text_files/test.txt", "a");
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

	else if (mode == 2) {
		float delta_t = 0;
		unsigned int val = 0;

		gmp_randstate_t rstate;
		gmp_randinit_mt(rstate);

		unsigned long seed;
		gmp_randseed_ui(rstate, seed);

		mpz_t lower, upper, base, random;
		mpz_inits(lower, upper, base, random, NULL);

		mpz_set_ui(base, 10);

		cout << "\n\n\n\n" << endl;
		cout << "beginning random number prime search !! :)" << endl;
		cout << "number of digits: ";
		cin >> val;

		// mpz_pow_ui(lower, base, 49999);
		// mpz_pow_ui(upper, base, 50000);
		mpz_pow_ui(lower, base, val-1);
		mpz_pow_ui(upper, base, val);

		mpz_urandomm(random, rstate, upper);

		mpz_add(lower, random, lower);
		mpz_add(upper, random, upper);

		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
		cout << "\n\n\n`````````````````````````````````````````````````````" << endl;
		cout << "beginning search for " << val << " digit prime..." << endl;
		cout << "\n`````````````````````````````````````````````````````" << endl;
		cout << "\n\n\n\n" << endl;

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
			float chance = 100.00/length;
			// cout << "\rcandidate #" << counter << " -- " << pcounter << " primes found -- (expect 1 prime in every " << length << " candidates)"; // -- takes " << delta_t << "s to validate";

			cout << "\n\n\n\n" << endl;
			std::cout << truncated_logo << '\n';
			printf("            **Random Miller-Rabin Search**\n");
			cout << "\n````````````````````````````````";
			cout << "``````````````````````````" << endl;
			// print_gmp(lower);
			cout << "\tanalyzing candidate #" << counter << endl;
			cout << "\t | --- candidate has " << length << " digits" << endl;
			cout << "\t | --- (" << chance << "% chance of being prime)" << endl;
			cout.flush();

			clock_t t_to_start = clock();
			if (isPrime_mpz_fast(lower, rstate)){
				pcounter++;

				cout << "\tcandidate was prime (total of " << pcounter << " found)" << endl;
				// printf("\ncandidate was prime\n");

				primefile = fopen("text_files/test.txt", "a");
				fputs("\n", primefile);
				mpz_out_str(primefile, 10, lower);
				fputs("\n", primefile);

				fclose(primefile);

			}
			else {
				cout << "\tcandidate was not prime (total of " << pcounter << " found)" << endl;
			}

			clock_t t_to_end = clock();

			delta_t = t_to_end - t_to_start;
			delta_t = delta_t/1000000;
			cout << "\t(took " << delta_t << "s to validate)" << endl;

			mpz_add_ui(lower, lower, 1);
			int check = mpz_cmp(lower, upper);

			if (check > 0){ break; }
		}

		gmp_randclear(rstate);
		mpz_clears(lower, upper, base, random, NULL);
	}
	
	else if (mode == 3){

		// initialize time counter
		float delta_t = 0;

		// define and initialize random number generator
		gmp_randstate_t rstate;
		gmp_randinit_mt(rstate);

		// define and initialize random seed for rng
		unsigned long seed;
		gmp_randseed_ui(rstate, seed);

		// define and initialize the mpz numbers to zero
		mpz_t value, random;
		mpz_inits(value, random, NULL);

		// print pretty logo and request a number to validate
		// primality
		cout << "\n\n\n\n" << endl;
		cout << full_logo << "\n\n\n";
		cout << "is your value prime?" << endl;
		cout << "enter a value: ";

		// define a string and read it from the line
		std::string val;
		cin >> val;

		// determine length of string + push it into 
		// a char* array for mpz
		const int strlength = val.length(); 
		char* char_array = new char[strlength + 1];
		strcpy(char_array, val.c_str()); 

		// set the mpz number to the char array, and tell
		// it base 10
		mpz_set_str(value, char_array, 10);

		// inform user progress was made
		cout << "\n\n\n`````````````````````````````````````````````````````" << endl;
		cout << "determining primality..." << endl;
		cout << "\n`````````````````````````````````````````````````````" << endl;

		// determine information about the number and show it before validating
		size_t length = mpz_sizeinbase(value, 10);
		float chance = 100.00/length;
		cout << "\t | --- candidate has " << length << " digits" << endl;
		cout << "\t | --- (" << chance << "% chance of being prime)" << endl;

		// flush cout to make it print, sometimes it waits for later to print it
		cout.flush();

		// start the clock
		clock_t t_to_start = clock();

		// determine if the value is prime given the rng
		if (isPrime_mpz_fast(value, rstate)){
			// if it's prime let the user know
			cout << "\tvalue is prime" << endl;
		}
		else {
			// if it's not prime let the user know
			cout << "\tvalue is not prime" << endl;
		}

		// stop the clock
		clock_t t_to_end = clock();

		// determine the time it took to validate primality + let user know
		delta_t = t_to_end - t_to_start;
		delta_t = delta_t/1000000;
		cout << "\t(took " << delta_t << "s to validate)\n\n" << endl;

		// clear the rng and the mpz numbers
		gmp_randclear(rstate);
		mpz_clears(value, random, NULL);
	}

	return 0;
}
