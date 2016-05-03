from collections import Counter, defaultdict
import bisect


def main():
    text_name = 'corpus.txt'
    with open(text_name, 'r') as f:
        word_list = f.read().split()
    frequencies = defaultdict(list)
    for i, w in enumerate(word_list):
        frequencies[w].append(i)
    
    ordered_list = Counter(word_list).most_common()
    
    
    N = int(raw_input())
    for case_num in range(1, N+1):
        A, B = map(int, raw_input().split())
        occurrences = Counter()
        for word, frequency in ordered_list:
            if occurrences and frequency <= occurrences.most_common()[-1][1]:
                break
            # Binary search
            num_occurrences = (bisect.bisect_right(frequencies[word],B-1) -
                               bisect.bisect_right(frequencies[word],A-2))
            occurrences[word] = num_occurrences
            if len(occurrences) > 3:
                del occurrences[occurrences.most_common()[-1][0]]
        
        top_frequent = occurrences.most_common()
        string_frequent = [' '.join(map(str, frequent))
                           for frequent in top_frequent]
        print 'Case #%d: %s' % (case_num, ','.join(string_frequent))
    

if __name__ == '__main__':
    main()