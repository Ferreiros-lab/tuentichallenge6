import math
import primefac
from collections import Counter

def fast_exponentiation(a, b, mod):
    """Computes a^b%mod in time log(b)"""
    
    # I could't find whether python implements pow as fast exponentiation
    # or ordinary exponentiation and I don't want to take the risk.
    
    if b == 0:
        return 1
    aux = fast_exponentiation(a, b/2, mod)
    aux *= aux
    if b % 2:
        aux *= a
    return aux % mod

def phi(n):
    """Computes the function phi of euler of n"""
    
    factorization = Counter(primefac.primefac(n))
    ans = 1
    for p, e, in factorization.iteritems():
        ans *= (p-1) * p**(e-1)
    return ans

def order(g, n):
    """Smallest k>=0 such that g^k===1(%n)"""
    phi_n = phi(n)
    factorization = Counter(primefac.primefac(phi_n))
    ans = 1
    for p, e in factorization.iteritems():
        ans *= pow(p, e-max([k for k in range(e + 1)
                            if fast_exponentiation(g, phi_n/p**k, n) == 1]))
    return ans

def next_ones(N):
    """Next multiple of N of the form 11...11"""
    
    return order(10, 9*N)

def divide(a, b):
    """Returns (c, k) such that a=c*b^k and k is maximum"""
    
    k = 0
    while a % b == 0:
        a /= b
        k += 1
    return (a, k)

def main():
    C = int(raw_input())
    for case_num in range(1, C + 1):
        N = int(raw_input())
        N, factor2 = divide(N, 2)
        N, factor5 = divide(N, 5)
        print "Case #%d: %d %d" % (case_num,
                                   next_ones(N),
                                   max(factor2, factor5))                                   
    
if __name__ == '__main__':
    main()