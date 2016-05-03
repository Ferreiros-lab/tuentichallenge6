from collections import Counter

def main():
    text_name = 'corpus.txt'
    with open(text_name, 'r') as f:
        word_list = f.read().split()
        
    N = int(raw_input())
    for case_num in range(1, N+1):
        A, B = map(int, raw_input().split())
        occurrences = Counter(word_list[A-1:B])
        top_frequent = occurrences.most_common()[:3]
        string_frequent = [' '.join(map(str, frequent))
                           for frequent in top_frequent]
        print 'Case #%d: %s' % (case_num, ','.join(string_frequent))
    

if __name__ == '__main__':
    main()