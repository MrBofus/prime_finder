import math
import lib.prime_core as pc
import time

candidate = 2**(94513) - 1

print(int(math.log10( candidate ) + 1))

t_to_start = time.monotonic()
print(pc.LucasLehmer(94513))
t_to_end = time.monotonic()

print(t_to_end - t_to_start)