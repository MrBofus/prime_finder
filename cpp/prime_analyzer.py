
with open('rpi/twin_prime_candidates.txt') as f:
    lines = f.readlines()




# split strings into list of primes
primelist = []
for i in range(1, len(lines), 2): 
    primelist.append(int(lines[i]))



# iterate through each prime stored in the list of primes
pcounter = 0
for i in range(1, len(primelist)):
    # if there is a twin prime, append it to the twin primes text file
    if primelist[i] - primelist[i-1] == 2:
        
        print('twin prime found')
        with open('rpi/worth_saving/random_twin_primes.txt', 'a') as f:
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
            with open('rpi/worth_saving/random_twin_primes.txt', 'a') as f:
                f.write('```````````````````\ntriplet primes:\n\n')
                f.write(str(primelist[i-2]) + '\n\n')
                f.write(str(primelist[i-1]) + '\n\n')
                f.write(str(primelist[i]) + '\n\n')
                f.write('```````````````````\n\n\n\n')
    
    pcounter += 1