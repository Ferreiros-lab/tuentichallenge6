import ahocorasick


# First and last combos are useless since they are equivalent to the
# second and fourth ones
combos = ['DTRP', 'RDTP', 'DNLK']
combo_prefixes = [c[:-1] for c in combos]
normalization = {'RU': 'S', 'RD': 'T', 'LU': 'M', 'LD': 'N'}

def add_words(words, automaton):
    """Add words from a list to an automaton from ahocorasick module
    
    Args:
        words: A list of words
        automaton: an automaton from ahocorasick module
    """
    for word in words:
        automaton.add_word(word, (0, word))
    
def create_automaton(words):
    """Creates an automaton from ahocorasick module from a list of words
    
    Args:
        words: A list of words
        
    Returns
        An automaton from ahocorasick module, initialized 
    """
    automaton = ahocorasick.Automaton()
    add_words(words, automaton)
    automaton.make_automaton()
    return automaton

def normalize(session):
    """Normalizes a string containing a training session
    
    Creates a new string from a training session that will be used to
    compute the number of occurrences of each combo. Two operations
    will be performed: movements consisting of more than one letter
    will be replaced with a single letter, according to the dictionary
    'normalization', and dashes will be removed.
    
    Args:
        session: A string consisting in a sequence of movementsseparated 
            with dashes.
    
    Returns:
        A single string containing one char for each movement in the input.
    """
    
    move_list = session.split('-')
    for i, m in enumerate(move_list):
        if m in normalization:
            move_list[i] = normalization[m]
    return ''.join(move_list)

def count_occurrences(moves, automaton):
    """Counts the number of occurrences of a list of string in another string
    
    The list of strings is given in the internal trie of an automaton of
    the ahocorasick module.
    
    Args:
        moves: A string in which the function will find occurrences.
        automaton: An automaton from the ahocorasick module containing the
            list of substrings
    
    Returns:
        An integer with the total number of occurrences of strings
        in the automaton in the string moves.
    """
    return sum(1 for _ in automaton.iter(moves))

def main():
    combo_automaton = create_automaton(combos)
    prefix_automaton = create_automaton(combo_prefixes)
    N = int(raw_input())
    for case_num in range(1, N+1):
        moves = normalize(raw_input())
        almost_combos = (count_occurrences(moves, prefix_automaton) - 
                         count_occurrences(moves, combo_automaton))
        print 'Case #%d: %s' % (case_num, almost_combos)
    
    

if __name__ == '__main__':
    main()