from collections import Counter
from fractions import Fraction
import numpy

def gcd(a, b):
    """Greatest common divisor of two numbers"""
    if b == 0: return a
    return gcd(b, a % b)

def lcm(a, b):
    """Least common multiple of two numbers"""
    return a * b / gcd(a, b)

def get_probability(matrix, exponent, initial):
    """Returns the probability of ending in the most likely position.
    
    It also returns an index with such a position.
    
    Args:
        matrix: A numpy matrix of probabilities. In position [i,j] there
            should be the probability of moving to j after being in i,
            times 100 to avodid fractions.
        exponent: the number of changes
        initial: the initial position
    Returns:
        A tuple with three elements. The first one is the last digit of the
        numerator of the probability, the second one is the last digit of the
        denominator of the probability, and the last one is the index of the 
        most likely position. If there is more than one such position it
        returns the largest one.
    """
    
    common = 100
    for r in matrix.tolist():
        for v in r:
            common = gcd(common, v)
    den = 100
    matrix /= common
    den /= common
    row = (matrix ** exponent)[initial,:].tolist()[0]
    max_value, max_index = max((val, idx) for (idx, val) in enumerate(row))
    f = Fraction(max_value, den**exponent)
    return (f.numerator % 10, f.denominator % 10, max_index)

def get_reachable(initial, graph):
    """Returns a list with the set of vertices reachable from the initial"""
    reachable = {initial}
    previous = set()
    while previous != reachable:
        previous = set(reachable)
        for i, row in enumerate(graph):
            if i in previous:
                for j, value in enumerate(row):
                    if value:
                        reachable.add(j)
    return sorted(list(reachable))

def get_submatrix(matrix, indices):
    """Computes the submatrix of matrix given by rows an columns in indices"""
    return [[matrix[i][j] for j in indices] for i in indices]

def main():
    C = int(raw_input())
    matrix = [[0] * C for i in range(C)]
    P = int(raw_input())
    graph = [[0] * C for i in range(C)]
    for i in range(P):
        origin, dest, probability = map(int, raw_input().split('/')[0].split())
        matrix[origin][dest] = probability
        if probability > 0:
            graph[origin][dest] = 1
    
    Q = int(raw_input())
    for num_case in range(1, Q + 1):
        initial, changes = map(int, raw_input().split())
        reachable = get_reachable(initial, graph)
        submatrix = get_submatrix(matrix, reachable)
        markov = numpy.matrix(submatrix, dtype = 'object')
        if changes < 200000:  # Small enough to compute the whole matrix
            numerator, denominator, index = get_probability(markov,
                                                            changes,
                                                            reachable.index(initial))
        else:
            cycleLength = 60
            exponent = changes % cycleLength + cycleLength
            numerator, denominator, index = get_probability(markov,
                                                            exponent,
                                                            reachable.index(initial))
            
        print 'Case #%d: Chair: %d Last digits: %d/%d' % (
            num_case, reachable[index], numerator, denominator)

if __name__ == '__main__':
    main()