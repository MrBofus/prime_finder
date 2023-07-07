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



number_of_digits =  100000
number_of_threads = 40
number_of_batches = 1000
number_of_searches_per_batch_per_thread = 5

odds_of_prime = 1/number_of_digits
probability = (1-(1-odds_of_prime)**(number_of_searches_per_batch_per_thread*number_of_threads)) * 100
probability_ = (1-(1-odds_of_prime)**(number_of_searches_per_batch_per_thread*number_of_threads*number_of_batches)) * 100 




print('(there is a ' + str(round(probability, 2)) + '% chance of finding a prime per batch,', end='')
print(' ' + str(round(probability_, 2)) + '% chance after all batches)\n\n')

lower_bound = random.randint(10**number_of_digits, 10**(number_of_digits+1))
upper_bound = lower_bound + number_of_searches_per_batch_per_thread*number_of_threads
n_threads = number_of_threads

with open('resources/primes.txt', 'w') as f:
    f.write('')
    
    
counter = 0
while counter < number_of_batches:
    print('\t  == starting batch ' + str(counter+1) + ' ==')
    print('   checking ' + str(number_of_searches_per_batch_per_thread) 
            + ' candidates per thread\n\n')

    t_to_start = time.monotonic()
    primes = pc.findPrimes(int(lower_bound), int(upper_bound), int(n_threads))
    t_to_end = time.monotonic()
    
    for p in primes:
        with open('resources/primes.txt', 'a') as f:
            f.write(str(p) + ' is prime\n\n')
    
    if len(primes) == 1: plural = ''
    else: plural = 's'
    
    print('\t(' + str(len(primes)) + 
            ' prime' + plural + ' found this batch)') # \n\n\n\n')
    print('\t(' + str(t_to_end - t_to_start) + 's per batch)\n\n\n\n')

    lower_bound += number_of_searches_per_batch_per_thread*number_of_threads
    upper_bound += number_of_searches_per_batch_per_thread*number_of_threads
    counter += 1


with open('resources/primes.txt') as f:
    lines = f.readlines()

primelist = []
for i in range(0, len(lines), 2): 
    primelist.append(int(lines[i].split(' ')[0]))

pcounter = 0
for i in range(1, len(primelist)):
    if primelist[i] - primelist[i-1] == 2:
        
        print('twin prime found')
        with open('resources/twin_primes.txt', 'a') as f:
            f.write('```````````````````\ntwin primes:\n\n')
            f.write(str(primelist[i-1]) + '\n\n')
            f.write(str(primelist[i]) + '\n\n')
            f.write('```````````````````\n\n\n\n')
    
    
    
    if pcounter > 0:
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

    _ = input()