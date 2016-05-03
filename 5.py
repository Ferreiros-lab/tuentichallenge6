from collections import Counter
import telnetlib

HOST = '52.49.91.111'
PORT = '9988'
file_dict = 'words.txt'

def read_dictionary():
    """Reads a dictionary from file
    
    Returns:
        A list of strings with the words in the given file
    """
    
    with open(file_dict, 'r') as f:
        return f.read().splitlines()

def matches(word, pattern):
    """Checks whether a given word matches with a pattern
    
    The pattern consists of lowercase letters and undescores,
    that work as wildcards.
    
    Args:
        word: A string to be compared with the pattern
        pattern: A string consisting of lowercase letters, undescores
        and possibly spaces, to be removed
    """
    
    new_pattern = pattern.replace(' ', "")
    for w, p in zip(word, new_pattern):
        if p != '_' and w != p:
            return False
    return True
    
    

def play_game(dictionary):
    """Plays a hangman game with a given dictionary
    
    Returns:
        A boolean indicating whether the game was successful
    """
    
    tn = telnetlib.Telnet(HOST, PORT)
    tn.read_until('...')
    while True:  # Each iteration is a successful level
        tn.write('\n')
        answer = tn.read_until('>')
        initial_word = answer.splitlines()[-3]  # Initially spaces and _
        word_length = initial_word.count('_')
        candidates = filter(lambda w: len(w) == word_length, dictionary)
        current_word = initial_word  # used to keep the list of guessed letters
        mistakes = 0
        finished = False
        while not finished:  # Guesses in each level
            occurrences = Counter()
            for word in candidates:
                occurrences += Counter(set(word))
            guess = next(g for g in occurrences.most_common()
                        if g[0] not in current_word)[0]  # Most common unused
            tn.write(guess+ '\n')
            answer = tn.read_until('>',1)
            if 'Noob' in answer:
                print answer
            if 'Master' in answer:
                print answer
                return True
            if answer[-1] == '.':  # Finished level
                finished = True
            else:    
                current_word = answer.splitlines()[-3]
                if guess in current_word:
                    candidates = filter(lambda w: matches(w, current_word),
                                        candidates)
                else:
                    candidates = filter(lambda w: guess not in w, candidates)
                    mistakes += 1
                if mistakes >= 6:
                    return False
    return True
    

def main():
    dictionary = read_dictionary()
    while not play_game(dictionary):
        pass

if __name__ == '__main__':
    main()