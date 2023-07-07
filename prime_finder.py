##````````````````````````````````````````````````````````````````````````````````````````````````````````````````##
#   written by ME :)
#

##````````````````````````````````````````````````````````````````````````````````````````````````````````````````##
#   import libraries
#       lib.prime_core  -   contains parallelized prime-finding algorithm
#       random          -   generates random numbers to check for primality
#       time            -   used for metrics to see how long iterations take
#       os              -   creates 'resources' folder if one does not currently exists

import lib.prime_core as pc
import random
import time
import os


##````````````````````````````````````````````````````````````````````````````````````````````````````````````````##
#   initialize parameters
#
#   

# check if 'resources' folder exists, and if it
# does not, make one
if not os.path.exists('resources/'):
    os.mkdir('resources/')

# specify the number of digits of prime number to search for
number_of_digits =  3

# specify the number of threads to use to concurrently search for primes
number_of_threads = 40

# specify number of candidates to check per thread
number_of_searches_per_batch_per_thread = 5

# specify number of checks each thread completes
number_of_batches = 10



mode = 'mersenne'



##````````````````````````````````````````````````````````````````````````````````````````````````````````````````##
#   initial calculations
#
#   before checking for primes, the probabilies are calculated
#   and the random values are generated

# odds of random value being prime
odds_of_prime = 1/number_of_digits

# probability of a thread finding a prime per batch
probability = (1-(1-odds_of_prime)**(number_of_searches_per_batch_per_thread*number_of_threads)) * 100

# probability of a thread fiding a prime after all batches are complete
probability_ = (1-(1-odds_of_prime)**(number_of_searches_per_batch_per_thread*number_of_threads*number_of_batches)) * 100 

# show probabilities on screen
print('(there is a ' + str(round(probability, 2)) + '% chance of finding a prime per batch,', end='')
print(' ' + str(round(probability_, 2)) + '% chance after all batches)\n\n')

# initialize random value to check for primality
lower_bound = random.randint(10**(number_of_digits-1), 10**number_of_digits)
upper_bound = lower_bound + number_of_searches_per_batch_per_thread*number_of_threads

# check if text file exists, and if not, create it
if not os.path.exists('resources/primes.txt'):
    with open('resources/primes.txt', 'w') as f:
        f.write('')
else:
    if mode == 'mersenne':
        # read saved list of primes
        with open('resources/primes.txt') as f:
            lines = f.readlines()

        # split strings into list of primes
        mpowers = []
        for i in range(0, len(lines), 2): 
            mpowers.append(int(lines[i].split(' ')[0]))


##````````````````````````````````````````````````````````````````````````````````````````````````````````````````##
#   main loop
#
#   batches are run to check for primality
#   any primes that are found are appended to
#   'resources/primes.txt'

# initialize batch at zero
batch = 0

# iterate through the number of batches
while batch < number_of_batches:
    # print batch number and number of searches per thread
    print('\t  == starting batch ' + str(batch+1) + ' ==')
    print('   checking ' + str(number_of_searches_per_batch_per_thread) 
            + ' candidates per thread\n\n')

    # begin timer
    t_to_start = time.monotonic()

    # search for random primes using all threads
    if mode == 'random':
        primes = pc.findPrimes(int(lower_bound), int(upper_bound), int(number_of_threads))

    # search for mersenne primes using all threads
    elif mode == 'mersenne':
        primes = pc.findMersennePrimes(mpowers, int(number_of_threads))

    # end timer
    t_to_end = time.monotonic()
    
    # any primes found in search is appended to prime list in 'resources' folder
    for p in primes:
        with open('resources/primes.txt', 'a') as f:
            f.write(str(p) + ' is prime\n\n')
    
    # align text to screen
    if len(primes) == 1: plural = ''
    else: plural = 's'
    
    # print wrap-up information at end of batch
    print('\t(' + str(len(primes)) + 
            ' prime' + plural + ' found this batch)') # \n\n\n\n')
    print('\t     (' + str(round(t_to_end - t_to_start, 1)) + 's per batch)\n\n\n\n')

    # move window to check over
    lower_bound += number_of_searches_per_batch_per_thread*number_of_threads
    upper_bound += number_of_searches_per_batch_per_thread*number_of_threads

    # incriment batch
    batch += 1



##````````````````````````````````````````````````````````````````````````````````````````````````````````````````##
#   post-processing
#
#   check any primes found for potential
#   twin or triplet primes

# read saved list of primes
with open('resources/primes.txt') as f:
    lines = f.readlines()

# split strings into list of primes
primelist = []
for i in range(0, len(lines), 2): 
    primelist.append(int(lines[i].split(' ')[0]))

# iterate through each prime stored in the list of primes
pcounter = 0
for i in range(1, len(primelist)):
    # if there is a twin prime, append it to the twin primes text file
    if primelist[i] - primelist[i-1] == 2:
        
        print('twin prime found')
        with open('resources/twin_primes.txt', 'a') as f:
            f.write('```````````````````\ntwin primes:\n\n')
            f.write(str(primelist[i-1]) + '\n\n')
            f.write(str(primelist[i]) + '\n\n')
            f.write('```````````````````\n\n\n\n')
    
    
    
    if pcounter > 0:
        # if there is a triplet prime, append it to the twin primes text file
        if ((primelist[i] - primelist[i-1] == 4 and 
                primelist[i] - primelist[i-2] == 6) or 
                (primelist[i] - primelist[i-1] == 2 and 
                    primelist[i] - primelist[i-2] == 6)):
            
            print('triplet prime found')
            with open('resources/twin_primes.txt', 'a') as f:
                f.write('```````````````````\ntriplet primes:\n\n')
                f.write(str(primelist[i-2]) + '\n\n')
                f.write(str(primelist[i-1]) + '\n\n')
                f.write(str(primelist[i]) + '\n\n')
                f.write('```````````````````\n\n\n\n')
    
    pcounter += 1

# keep window open when code finishes running
_ = input()