/*````````````````````````````````````````````````````````````````````````````````````
we need gmp.h to make this work !!! :)

`````````````````````````````````````````````````````
Easy install: 

sudo apt install -y libgmp-dev
`````````````````````````````````````````````````````
Performance optimized:

sudo apt update
sudo apt install -y build-essential m4

wget https://gmplib.org/download/gmp/gmp-6.3.0.tar.xz  # Latest as of 2024
tar -xf gmp-6.3.0.tar.xz
cd gmp-6.3.0

CFLAGS="-march=armv8-a+crc -mtune=cortex-a53" \
CXXFLAGS="-march=armv8-a+crc -mtune=cortex-a53" \
./configure --enable-cxx --host=aarch64-linux-gnu --build=aarch64-linux-gnu

make -j$(nproc)
sudo make install
`````````````````````````````````````````````````````

To compile: g++ -o prime_finder prime_finder.cpp -lgmp
````````````````````````````````````````````````````````````````````````````````````*/


// import libraries
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <math.h>
#include <cstdlib>
#include <cmath>
#include <cstdlib>
#include "prime_functions/prime_functions_fast.h"

// initialize mode
int mode = 1;

// initialize file to write primes to
FILE* primefile;

// buffer to hold primes before writing them to a file
const size_t BUFFER_SIZE = 1000;
ostringstream prime_buffer;

char* str_lower = NULL;
const char* file = "rpi/twin_prime_candidates.txt";

// function to write primes to file
void write_primes_to_file(ostringstream& buffer, const char* filename){
	ofstream outfile(filename, ios::app);
	outfile << buffer.str();
	outfile.close();
	buffer.str("");
	buffer.clear();
}

// main loop
int main(void) {

	// present user with menu 
	cout << "choose a mode: \n\t(1) - for lucas lehmer";
	cout << "\n\t(2) - for random miller-rabin";
	cout << "\n\t(3) - specific value primality test";
	cout << "\n\t(4) - for random miller-rabin + minimal UI";
	cout << "\n\nremember to have fun out there !! :)\n";
	cout << "mode: ";
	cin >> mode;

	// ``````````````````````````````````````````````````````````````````````````````````````` //
	// Lucas-Lehmer search mode:
	if (mode == 1) {
		
		// initialize lower and upper bounds for powers to check
		unsigned int lower = 65123456;
		unsigned int upper = 65223456;

		// initialize power variable
		unsigned int power = lower;

		// determine ntrials and initialize counter to zero
		unsigned int ntrials = upper-lower;
		unsigned int counter = 0;

		// LL search loop
		while (counter < ntrials){
			// clear the screen
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

			// show full art logo + some info
			std::cout << full_logo << '\n';
			printf("              **Lucas-Lehmer Variant**\n");
			printf("\n\n\n`````````````````````````````````````````````````````\n");
			printf("finding new candidate...\n");

			// find a new power candidate (power must be prime)
			while (power < upper){
				// if power is not prime, increment by one
				power += 1;
				if (isPrime(power)){
					// if power is prime, break
					break;
				}
			}

			// determine number of digits of power + display it
			float ndigits = log10(power);
			printf("\npower is %.0f digits (%d)\n", ndigits, power);

			// check if mersenne number is prime given power
			bool val = LucasLehmer(power);

			// if the mersenne number prime, enter here
			if (val){
				printf("\ncandidate is prime\n");

				// initialize mpz number to 0
				mpz_t result, base;
				mpz_inits(result, base, NULL);

				// compute prime mersenne number given power
				mpz_set_str(base, "2", 10);
				mpz_pow_ui(result, base, power);
				mpz_sub_ui(result, result, 1);

				// write prime mersenne number to file
				primefile = fopen("text_files/test.txt", "a");
				fputs("\n", primefile);
				fprintf(primefile, "%u", power);
				fputs(" -- ", primefile);
				mpz_out_str(primefile, 10, result);
				fputs("\n", primefile);
				fclose(primefile);

				// clear the mpz numbers

				mpz_clears(result, base, NULL);
			}
			else{
				printf("\ncandidate is not prime\n");
			}

			// if all numbers in range have been checked, break
			if (power > upper){
				break;
			}

			// iterate counter
			counter++;
		}
	}


	// ``````````````````````````````````````````````````````````````````````````````````````` //
	// miller-rabin random search mode:
	else if (mode == 2) {

		// initialize time counter and number of digits
		float delta_t = 0;
		unsigned int val = 0;
		char* mpz_seed_str;

		// define and initialize random number generator
		gmp_randstate_t rstate;
		gmp_randinit_mt(rstate);

		// define and initialize random seed for rng
		unsigned long seed = time(0);
		gmp_randseed_ui(rstate, seed);

		// define and initialize the mpz numbers to zero
		mpz_t lower, upper, base, random;
		mpz_inits(lower, upper, base, random, NULL);

		// intialize 'base' to 10
		mpz_set_ui(base, 10);

		// display info + ask user how many digits for prime number
		cout << "\n\n\n\n" << endl;
		cout << "beginning random number prime search !! :)" << endl;
		cout << "number of digits: ";
		cin >> val;

		// define lower and upper bound values given number of digits
		mpz_pow_ui(lower, base, val-1);
		mpz_pow_ui(upper, base, val);

		// generate random number in range
		mpz_urandomm(random, rstate, upper);

		// add random number to values being checked
		mpz_add(lower, random, lower);
		mpz_add(upper, random, upper);
		
		//`````````````````````````````````````````````````````//

		// clear screen and display info
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
		cout << "\n\n\n`````````````````````````````````````````````````````" << endl;
		cout << "beginning search for " << val << " digit prime..." << endl;
		cout << "\n`````````````````````````````````````````````````````" << endl;
		cout << "\n\n\n\n" << endl;

		// initialize counter and prime number counter
		unsigned int counter = 0;
		unsigned int pcounter = 0;

		// begin miller-rabin random search
		while (true){

			// iterate counter
			counter++;

			// check the last digit of number to see if it's worth checking
			while(checkLastDigit(lower)){
				mpz_add_ui(lower, lower, 1);
			}

			// determine number of digits in candidate and chance of being prime
			size_t length = mpz_sizeinbase(lower, 10);
			float chance = 100.00/length;
			
			// show logo and info to user
			cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
			std::cout << full_logo << '\n';
			printf("            **Random Miller-Rabin Search**\n");
			cout << "\n````````````````````````````````";
			cout << "``````````````````````````" << endl;
			mpz_seed_str = return_seed(lower, 1000);
			cout << "seed: " << mpz_seed_str << endl;
			cout << "\n````````````````````````````````";
			cout << "``````````````````````````" << endl;
			// print_gmp(lower);
			cout << "\tanalyzing candidate #" << counter << endl;
			cout << "\t | --- candidate has " << length << " digits" << endl;
			cout << "\t | --- prev. candidate took " << delta_t << "s to validate" << endl;
			cout << "\t | --- total of " << pcounter << " found" << endl;
			cout << "\t | --- (" << chance << "% chance of being prime)" << endl;
			cout.flush();

			// start timer and check if value is prime given value and 
			// rng state
			clock_t t_to_start = clock();
			if (isPrime_mpz_fast(lower, rstate)){

				// if it is prime, increment the prime counter
				pcounter++;

				// tell user it was prime
				cout << "\tcandidate was prime (total of " << pcounter << " found)" << endl;

				// write prime to text file
				primefile = fopen("text_files/test.txt", "a");
				fputs("\n", primefile);
				mpz_out_str(primefile, 10, lower);
				fputs("\n", primefile);

				fclose(primefile);

			}
			else {
				cout << "\tcandidate was not prime (total of " << pcounter << " found)" << endl;
			}

			// stop clock
			clock_t t_to_end = clock();

			// determine time it took to validate candidate
			delta_t = t_to_end - t_to_start;
			delta_t = delta_t/1000000;
			cout << "\t(took " << delta_t << "s to validate)" << endl;

			// increment candidate by one
			mpz_add_ui(lower, lower, 1);

			// if all values have been checked, break
			int check = mpz_cmp(lower, upper);
			if (check > 0){ break; }
		}

		// clear rng and mpz numbers
		gmp_randclear(rstate);
		mpz_clears(lower, upper, base, random, NULL);
	}
	

	// ``````````````````````````````````````````````````````````````````````````````````````` //
	// individual value mode:
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


	// ``````````````````````````````````````````````````````````````````````````````````````` //
	// mode 4
	else if (mode == 4) {
		
		// initialize time counter and number of digits
		float delta_t = 0;
		unsigned int val = 0;
		char* mpz_seed_str;

		// define and initialize random number generator
		gmp_randstate_t rstate;
		gmp_randinit_mt(rstate);

		// define and initialize random seed for rng
		srand((unsigned int)time(NULL));
		unsigned long seed = rand()%100000000000;
		gmp_randseed_ui(rstate, seed);

		// define and initialize the mpz numbers to zero
		mpz_t lower, upper, base, random;
		mpz_inits(lower, upper, base, random, NULL);

		// intialize 'base' to 10
		mpz_set_ui(base, 10);

		// display info + ask user how many digits for prime number
		cout << "\n\n\n\n" << endl;
		cout << "beginning random number prime search !! :)" << endl;
		cout << "seed: " << seed << endl;
		cout << "number of digits: ";
		cin >> val;

		// define lower and upper bound values given number of digits
		mpz_pow_ui(lower, base, val-1);
		mpz_pow_ui(upper, base, val);

		// generate random number in range
		mpz_urandomm(random, rstate, upper);

		// add random number to values being checked
		mpz_add(lower, random, lower);
		mpz_add(upper, random, upper);
		
		//`````````````````````````````````````````````````````//

		// clear screen and display info
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
		mpz_seed_str = return_seed(lower, 32);
		cout << "seed: " << mpz_seed_str << endl;
		cout << "\n\n\n`````````````````````````````````````````````````````" << endl;
		cout << "beginning search for " << val << " digit prime..." << endl;
		cout << "\n`````````````````````````````````````````````````````" << endl;
		cout << "\n\n\n\n" << endl;

		// initialize counter and prime number counter
		unsigned int counter = 0;
		unsigned int pcounter = 0;
		unsigned int pcounter_check = 0;

		// begin miller-rabin random search
		while (true){

			// iterate counter
			counter++;

			// check the last digit of number to see if it's worth checking
			while(checkLastDigit(lower)){
				mpz_add_ui(lower, lower, 1);
			}

			// determine number of digits in candidate and chance of being prime
			size_t length = mpz_sizeinbase(lower, 10);
			float chance = 100.00/length;
			
			cout << "\r  analyzing candidate #" << counter;
			cout << " --- total of " << pcounter << " found";
			// cout.flush();

			// check if value is prime given value and rng state
			if (isPrime_mpz_fast(lower, rstate)){

				// if it is prime, increment the prime counter
				pcounter++;
				pcounter_check++;

				// write prime to text file
				str_lower = mpz_get_str(NULL, 10, lower);
				prime_buffer << "\n" << str_lower << "\n";
				if (pcounter_check >= BUFFER_SIZE){
					write_primes_to_file(prime_buffer, file);
					pcounter_check = 0;
				}
			}
			else{}

			// increment candidate by one
			mpz_add_ui(lower, lower, 1);

			// if all values have been checked, break
			int check = mpz_cmp(lower, upper);
			if (check > 0){ break; }
		}

		// clear rng and mpz numbers
		gmp_randclear(rstate);
		mpz_clears(lower, upper, base, random, NULL);
	}

	return 0;
}
