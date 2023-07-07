from threading import Thread
import random
import time
import queue
import os


def print_green(text):
    # print('\033[0;32m' + text + '\033[0;0m')
    print(text)
    # pass


def trial_composite(j, a, d, n, s):
    # print_green('checking null case')
    if pow(a, d, n) == 1:
        return False
    
    # print_green('iterating tests')
    for i in range(s):
        # print_green('     (node: ' + str(j+1) + ' -- checking ' + str(i+1) + '/' + str(s) + ')')

        v_ = pow(a, 2**i * d, n)
        if v_ == n-1:
            return False
    
    return True



def is_Prime(j, n, n_trials):
    # print_green('checking last digit...')
    # Miller-Rabin primality test
    
    l = n%10
    if (l == 2 or l == 4 or l == 5 
        or l == 6 or l == 8 or l == 0):
        return False
    
    # print_green('last digit verified')

    # print_green('starting bitshift')
    s = 0
    d = n - 1
    while d%2 == 0:
        d >>= 1
        s += 1
    
    # print_green('bitshifted')

    # print_green('starting trials')
    for i in range(n_trials):
        # print_green('\ttrial ' + str(i+1))
        a = random.randrange(2, n)
        if trial_composite(j, a, d, n, s):
            return False
    # print_green('trials finished')
    
    return True



def divide_into_range(lower_bound, upper_bound, steps):
    step_size = int((upper_bound - lower_bound)/steps)
    
    old = lower_bound
    ranges = []
    for i in range(steps):
        ranges.append([old, old+step_size])
        old += step_size
    
    return ranges



class CustomThread(Thread):
    def __init__(self, i, r, queue):
        
        Thread.__init__(self)
        self.n = r[0]
        self.m = r[1]
        self.i = i

        self.queue = queue
        
        self.primelist = []
        
    def run(self):
        
        # counter = 0
        for i in range(self.n, self.m):

            # sys.stdout.write('\r')
            # percentage = int( 100*counter / (self.m - self.n))
            # sys.stdout.write("    thread #1: %d%%" % (percentage))
            # sys.stdout.flush()

            # counter += 1

            if is_Prime(self.i, i, 8):
                self.primelist.append(i)
        
        # sys.stdout.flush()
        # sys.stdout.write('\r')

        if len(self.primelist) == 1: plural = ''
        else: plural = 's'
        
        if self.i < 9: extraspace = ' ' 
        else: extraspace = ''
        
        
        print('   thread #' + str(self.i+1) + ' finished; ' + extraspace + 'found ' 
            + str(len(self.primelist)) + ' prime' + plural)



def findPrimes(lower_bound, upper_bound, number_of_threads):

    q = queue.Queue()

    ranges = divide_into_range(lower_bound, upper_bound, number_of_threads)
    
    threads = [None]*number_of_threads
    for t in range(number_of_threads):
        threads[t] = CustomThread(t, ranges[t], q)
        threads[t].start()
        # print('\t   starting thread #' + str(t))
    
    # print('\n\n   activating workers...\n\n')
    # q.put('start')

    for t in range(number_of_threads):
        threads[t].join()
    
    primes = []
    for T in threads:
        for i in range(len(T.primelist)):
            primes.append(T.primelist[i])
    
    return primes


if not os.path.exists('resources/'):
    os.mkdir('resources/')



var = '2'



number_of_digits =  100000
number_of_threads = 40
number_of_batches = 1000
number_of_searches_per_batch_per_thread = 5

odds_of_prime = 1/number_of_digits
probability = (1-(1-odds_of_prime)**(number_of_searches_per_batch_per_thread*number_of_threads)) * 100
probability_ = (1-(1-odds_of_prime)**(number_of_searches_per_batch_per_thread*number_of_threads*number_of_batches)) * 100 





if var == '1':
    print('\n\n\n````````````````````````````````````\n' +
          'let\'s find some primes...')
    
    flag = input('Find a random huge prime or enter a custom range?' +
                 '\n\t(yes for random, no for custom range)\t[y]/[N]\t')
    
    if flag == 'y' or flag == 'Y' or flag == 'yes' or flag == 'Yes':
        lower_bound = random.randint(10**400, 10**401)
        upper_bound = lower_bound + 2000
        n_threads = 20
    
    else:
        print('Enter a lower and upper bound of ' + 
              'where to search for prime numbers.')
        lower_bound = input('Lower bound: ')
        upper_bound = input('Upper bound: ')
        n_threads = input('number of threads? ')
    
    t_to_start = time.monotonic()
    
    primes = findPrimes(int(lower_bound), int(upper_bound), int(n_threads))
    
    t_to_end = time.monotonic()
    
    print('\nfinished, writing to text file...')
    print('\t(calculation took ' + str(t_to_end - t_to_start) + ' seconds)')
    
    with open('primes.txt', 'w') as f:
        f.write('')
    
    if len(primes) > 0:
        for p in primes:
            with open('primes.txt', 'a') as f:
                f.write(str(p) + ' is prime\n\n')
    
    else:
        with open('primes.txt', 'a') as f:
            f.write('no primes found in range, please try again')



elif var == '2':
    # print('\n\nSearching for twin primes...')
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
        primes = findPrimes(int(lower_bound), int(upper_bound), int(n_threads))
        t_to_end = time.monotonic()
        
        for p in primes:
            with open('resources/primes.txt', 'a') as f:
                f.write(str(p) + ' is prime\n\n')
        
        if len(primes) == 1: plural = ''
        else: plural = 's'
        
        print('\t(' + str(len(primes)) + 
              ' prime' + plural + ' found this batch)') # \n\n\n\n')
        print('\t(' + str(t_to_end - t_to_start) + 's per batch)\n\n\n\n')
        # print('\n\t== batch ' + str(counter+1) + ' complete== \n\n\n\n')

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