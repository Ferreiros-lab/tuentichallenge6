# Text found at the end of the code
text='<<B~<B~|BBf@BB@B@BZ|BB|BN~B@BB@|BBB@B$@D<BB~<'+chr(18)+'~B'

# The code splits the message in 6 chunks of 8 chars each
for i in range(6):
    print(''.join(format(ord(x), 'b').zfill(8) for x in text[i*8:i*8+8]))
    
# The message is GAMEOVER in ascii art