import string

# Too late in the morning for improving the legibility of my code, sorry :(
probables = map(chr, range(0, ord('~')+1))

def move(k, c, l):
    if ord(c) % 3 == 0:
        return (k + 1) % l
    elif ord(c) % 3 == 1:
        return (k - 1) % l
    else:
        return l // 2


def backtracking(ans, k, a, text):
    if a == len(ans):
        return True
    if ans[a] != '':
        if ans[k] != '':
            if ord(ans[a]) ^ ord(ans[k]) != text[a]:
                return False            
            return backtracking(ans, move(k, ans[a], len(ans)), a+1, text)
        else:
            ans[k] = chr(ord(ans[a]) ^ text[a])
            if ans[k] in probables and backtracking(ans, move(k, ans[a], len(ans)), a+1, text):
                return True
            ans[k] = ''
            return False
    else:
        if ans[k] != '':
            ans[a] = chr(ord(ans[k]) ^ text[a])
            if ans[a] in probables and backtracking(ans, move(k, ans[a], len(ans)), a+1, text):
                return True
            ans[a] = ''
            return False
        else:
            for i in probables:
                ans[k] = i
                ans[a] = chr(ord(ans[k]) ^ text[a])
                if (ans[a] in probables and
                    ans[k] in probables and
                    backtracking(ans, move(k, ans[a], len(ans)), a+1, text)):
                    return True
            ans[a] = ''
            ans[k] = ''
            return False
    

text = raw_input()
t2 = bytearray.fromhex(text)
l = len(t2)
k = l // 2
ans = [''] * l


ans[21:26] = 'crypt'
# How did I guess that? Well, I tried many other possiblities. But:
# There are many 59. They might be spaces xored with the middle symbol.
# If so, the middle symbol is a 'y'. This also implies that two symbols
# after it we have a t. Why? Well, the encoded symbol at the right of the 
# middle is 00, and the encoded symbols of the middle and the one two
# positions at right is the same (0d). This suggests that 0d is the xor 
# of both positions. Now, there is a word with two letters between two
# spaces. The first letter is a 'i'. The second one could be an 's'.
# If so, the symbol between the 'y' and the 't' must be a 'p'.
# Now we have 'ypt'... Which words have those letters? Well, since
# this is a problem about cryptography, not so many. Backtracking for
# the rest and we have it :)

backtracking(ans, k, 0, t2)
print ''.join(ans)