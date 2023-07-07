from threading import Thread
import random
import time



def trial_composite(a, d, n, s):
    if pow(a, d, n) == 1:
        return False
    for i in range(s):
        if pow(a, 2**i * d, n) == n-1:
            return False
    return True


def is_Prime(n):
    
    l = n%10
    
    if (l == 2 or l == 4 or l == 5 or l == 6 or l == 8 or l == 0):
        return False
    
    s = 0
    d = n-1
    while d%2==0:
        d>>=1
        s+=1
 
    for i in range(8): # number of trials 
        a = random.randrange(2, n)
        if trial_composite(a, d, n, s):
            return False
 
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
    def __init__(self, i, r, mode, ep):
        
        Thread.__init__(self)
        self.n = r[0]
        self.m = r[1]
        self.i = i

        self.mode = mode
        self.ep = ep
        
        self.primelist = []
    
    def run(self):
        if self.mode == 0:
            for i in range(self.n, self.m):
                if is_Prime(i):
                    # print(str(i) + ' is prime, found with thread #' + str(self.i))
                    self.primelist.append(i)
            
            if len(self.primelist) == 1: plural = ''
            else: plural = 's'
            
            print('\tthread #' + str(self.i) + ' finished;\tfound ' 
                + str(len(self.primelist)) + ' prime' + plural + ' of ' 
                + str(self.m - self.n) + ' candidates')
            
        elif self.mode == 1:
            for i in range(len(self.ep)):
                p_ = 2**self.ep[i] + 1
                if is_Prime(p_):
                    # print(str(p_) + ' is prime, found with thread #' + str(self.i))
                    self.primelist.append(p_)
            
            if len(self.primelist) == 1: plural = ''
            else: plural = 's'
            
            print('\tthread #' + str(self.i) + ' finished;\tfound ' 
                + str(len(self.primelist)) + ' prime' + plural + ' of ' 
                + str(len(self.ep)) + ' candidates')




def findPrimes(lower_bound, upper_bound, number_of_threads):
    ranges = divide_into_range(lower_bound, upper_bound, number_of_threads)
    
    threads = [None]*number_of_threads
    for t in range(number_of_threads):
        threads[t] = CustomThread(t, ranges[t], 0, 0)
        threads[t].start()
        # print('starting thread #' + str(t))
    
    for t in range(number_of_threads):
        threads[t].join()
    
    primes = []

    for T in threads:
        for i in range(len(T.primelist)):
            primes.append(T.primelist[i])
    
    return primes



def findMersennePrimes(primelist, number_of_threads):

    threads = [None]*number_of_threads
    for t in range(number_of_threads):
        l_ = (t/number_of_threads) * len(primelist)
        u_ = ((t+1)/number_of_threads) * len(primelist)
        threads[t] = CustomThread(t, [0, 1], 1, primelist[int(l_):int(u_)])
        threads[t].start()
        # print('starting thread #' + str(t))
    
    for t in range(number_of_threads):
        threads[t].join()
    
    primes = []

    for T in threads:
        for i in range(len(T.primelist)):
            primes.append(T.primelist[i])
    
    return primes


var = '2'

'''
number_of_digits =  500000
number_of_threads = 40
number_of_batches = 1000
number_of_searches_per_batch_per_thread = 10
'''

number_of_digits =  5000
number_of_threads = 40
number_of_batches = 10
number_of_searches_per_batch_per_thread = 10

odds_of_prime = 1/number_of_digits
probability = (1-(1-odds_of_prime)**(number_of_searches_per_batch_per_thread*number_of_threads)) * 100
probability_ = (1-(1-odds_of_prime)**(number_of_searches_per_batch_per_thread*number_of_threads*number_of_batches)) * 100 

if var == '1':
    print('\n\n\n````````````````````````````````````````````\nlet\'s find some primes...')
    
    flag = input('Find a random huge prime or enter a custom range?\n\t(yes for random, no for custom range)\t[y]/[N]\t')
    
    if flag == 'y' or flag == 'Y' or flag == 'yes' or flag == 'Yes':
        lower_bound = random.randint(10**400, 10**401)
        upper_bound = lower_bound + 2000
        n_threads = 20
        
    else:
        print('Enter a lower and upper bound of where to search for prime numbers.')
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
            f.write('no primes found in time, please try again')



elif var == '2':
    print('\n\nSearching for twin primes...')
    print('(there is a ' + str(round(probability, 2)) + '% chance of finding a prime per batch,')
    print(' ' + str(round(probability_, 2)) + '% chance after all batches)\n\n')
    
    # lower_bound = random.randint(10**400, 10**401)
    lower_bound = random.randint(10**number_of_digits, 10**(number_of_digits+1))
    upper_bound = lower_bound + number_of_searches_per_batch_per_thread*number_of_threads
    n_threads = number_of_threads

    with open('primes.txt', 'w') as f:
        f.write('')
    
    counter = 0
    while counter < number_of_batches:
        print('\t\t\t==starting batch ' + str(counter+1) + '==\n\n')
        
        primes = findPrimes(int(lower_bound), int(upper_bound), int(n_threads))
        
        for p in primes:
            with open('primes.txt', 'a') as f:
                # f.write(str(p) + ' is prime\n\n')
                f.write(str(p) + '\n')
        
        print('\n\n\t\t\t==batch ' + str(counter+1) + ' complete==')
        
        if len(primes) == 1: plural = ''
        else: plural = 's'
        
        print('\t\t(' + str(len(primes)) + ' prime' + plural + ' found this batch)\n\n')
        lower_bound += number_of_searches_per_batch_per_thread*number_of_threads
        upper_bound += number_of_searches_per_batch_per_thread*number_of_threads
        counter += 1

elif var == '3':
    print('\n\nSearching for mersenne primes...')
    
    # lower_bound = random.randint(10**400, 10**401)

    with open('3_digit_primes.txt', 'r') as f:
        established_primes = f.readlines()
    
    for i in range(len(established_primes)):
        established_primes[i] = int(established_primes[i])

    n_threads = number_of_threads

    with open('mersenne_primes.txt', 'w') as f:
        f.write('')
    
    counter = 0
    while counter < number_of_batches:

        shortlist = established_primes[int((counter/number_of_batches) * len(established_primes)): 
                                       int(((counter+1)/number_of_batches) * len(established_primes))]
        print('\t\t\t==starting batch ' + str(counter+1) + '==\n\n')
        
        primes = findMersennePrimes(shortlist, number_of_threads)
        
        for p in primes:
            with open('mersenne_primes.txt', 'a') as f:
                # f.write(str(p) + ' is prime\n\n')
                f.write(str(p) + '\n')
        
        print('\n\n\t\t\t==batch ' + str(counter+1) + ' complete==')
        
        if len(primes) == 1: plural = ''
        else: plural = 's'
        
        print('\t\t(' + str(len(primes)) + ' prime' + plural + ' found this batch)\n\n')

        counter += 1