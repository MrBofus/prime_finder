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
import math


##````````````````````````````````````````````````````````````````````````````````````````````````````````````````##
#

def random_search(number_of_batches, number_of_digits, number_of_searches_per_batch_per_thread, number_of_threads):
    # initialize random value to check for primality
    lower_bound = random.randint(10**(number_of_digits-1), 10**number_of_digits)
    upper_bound = lower_bound + number_of_searches_per_batch_per_thread*number_of_threads

    # initialize batch at zero
    batch = 0
    while batch < number_of_batches:
        # print batch number and number of searches per thread
        print('\t  == starting batch ' + str(batch+1) + ' ==')
        print('   checking ' + str(number_of_searches_per_batch_per_thread) 
                + ' candidates per thread\n\n')

        # begin timer
        t_to_start = time.monotonic()

        # search for random primes using all threads
        primes = pc.findPrimes(int(lower_bound), int(upper_bound), int(number_of_threads))

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


def mersenne_search(number_of_batches, number_of_digits, number_of_searches_per_batch_per_thread, number_of_threads):

    with open('resources/primes.txt', 'w') as f:
        f.write('')

    random_search(number_of_batches, number_of_digits, number_of_searches_per_batch_per_thread, number_of_threads)

    # read saved list of primes
    with open('resources/primes.txt') as f:
        lines = f.readlines()

    if len(lines) == 0:
        print('no prime list!')
        exit()

    # split strings into list of primes
    mpowers = []
    for i in range(0, len(lines), 2): 
        mpowers.append(int(lines[i].split(' ')[0]))

    number_of_batches = 1
    number_of_searches_per_batch_per_thread = 1
    number_of_threads = len(mpowers)

    # print batch number and number of searches per thread
    print('\t  == starting mersenne test ==')
    print('\t   checking ' + str(number_of_threads) 
            + ' mersenne candidates\n\n')
    
    # begin timer
    t_to_start = time.monotonic()
    
    primes = pc.findMersennePrimes(mpowers, int(number_of_threads))

    # end timer
    t_to_end = time.monotonic()
    
    # any primes found in search is appended to prime list in 'resources' folder
    ndigits_old = 0
    new_max = 0
    for p in primes:
        ndigits = math.log10(p)+1
        if ndigits > ndigits_old:
            new_max = ndigits

        ndigits_old = ndigits

        with open('resources/mersenne_primes.txt', 'a') as f:
            f.write(str(p) + ' is prime\n\n')
    
    # align text to screen
    if len(primes) == 1: plural = ''
    else: plural = 's'
    
    # print wrap-up information at end of batch
    print('\t(' + str(len(primes)) + 
            ' prime' + plural + ' found this batch)') # \n\n\n\n')
    print('\t     (' + str(round(t_to_end - t_to_start, 1)) + 's total)\n\n\n\n')
    print('  largest prime found was ' + str(int(new_max)) + ' digits\n\n\n\n')


def bounded_mersenne_search(lower, upper):
    trials = 100
    tcount = 0
    while tcount < trials:
        print('\n\n\n`````````````````````````````````````````````````````')
        print('finding new candidate...')
        while True:
            mpower = random.randint(lower, upper)

            _ = pc.is_Prime(0, mpower, 8)

            if _:
                break

        print('\npower is ' + str(int(math.log10( mpower ) + 1)) + ' digits (' + str(mpower) + ')')

        candidate = 2**(mpower) - 1

        print('candidate is ' + str(int(math.log10( candidate ) + 1)) + ' digits')

        print('probability of being prime: ' + str(round(100*math.log10(mpower)/mpower, 5)) + '%\n\n')

        t_to_start = time.monotonic()
        p = pc.LucasLehmer(1, mpower, True)

        t_to_end = time.monotonic()

        print('\n\n\n')
        print('time: ' + str(round(t_to_end - t_to_start, 2)) + 's\n\n')

        if p == -1:
            print('candidate is not prime')
        else:
            print('candidate is prime')
            with open('resources/mersenne_primes.txt', 'a') as f:
                f.write(str(p) + ' is prime\n\n')

        tcount += 1





##````````````````````````````````````````````````````````````````````````````````````````````````````````````````##
#   initialize parameters
#
#   

# check if 'resources' folder exists, and if it
# does not, make one
if not os.path.exists('resources/'):
    os.mkdir('resources/')

# specify the number of digits of prime number to search for
number_of_digits =  5

# specify the number of threads to use to concurrently search for primes
number_of_threads = 40

# specify number of candidates to check per thread
number_of_searches_per_batch_per_thread = 5

# specify number of checks each thread completes
number_of_batches = 10

lower, upper = 5*10**4, 6*10**4

mode = 'bounded mersenne'



##````````````````````````````````````````````````````````````````````````````````````````````````````````````````##
#   initial calculations
#
#   before checking for primes, the probabilies are calculated
#   and the random values are generated

if mode == 'random':
    # odds of random value being prime
    odds_of_prime = 1/number_of_digits

    # probability of a thread finding a prime per batch
    probability = (1-(1-odds_of_prime)**(number_of_searches_per_batch_per_thread*number_of_threads)) * 100

    # probability of a thread fiding a prime after all batches are complete
    probability_ = (1-(1-odds_of_prime)**(number_of_searches_per_batch_per_thread*number_of_threads*number_of_batches)) * 100 

    # show probabilities on screen
    print('(there is a ' + str(round(probability, 2)) + '% chance of finding a prime per batch,', end='')
    print(' ' + str(round(probability_, 2)) + '% chance after all batches)\n\n')


# check if text file exists, and if not, create it
if not os.path.exists('resources/primes.txt'):
    with open('resources/primes.txt', 'w') as f:
        f.write('')
        


##````````````````````````````````````````````````````````````````````````````````````````````````````````````````##
#   main loop
#
#   batches are run to check for primality
#   any primes that are found are appended to
#   'resources/primes.txt'


if mode == 'random':                random_search(number_of_batches, number_of_digits, number_of_searches_per_batch_per_thread, number_of_threads)
elif mode == 'mersenne':            mersenne_search(number_of_batches, number_of_digits, number_of_searches_per_batch_per_thread, number_of_threads)
elif mode == 'bounded mersenne':    bounded_mersenne_search(lower, upper)



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
        
        # print('twin prime found')
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
            
            # print('triplet prime found')
            with open('resources/twin_primes.txt', 'a') as f:
                f.write('```````````````````\ntriplet primes:\n\n')
                f.write(str(primelist[i-2]) + '\n\n')
                f.write(str(primelist[i-1]) + '\n\n')
                f.write(str(primelist[i]) + '\n\n')
                f.write('```````````````````\n\n\n\n')
    
    pcounter += 1

# keep window open when code finishes running
_ = input()