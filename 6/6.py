import os

def main():
    # Reflect the given image and adapt its format
    os.system('convert -flop alice_shocked.png alice_shockedreflected.ppm')
    # Compile a Piet interpreter
    os.system('make npiet > /dev/null')
    # Execute it
    os.system('./npiet alice_shockedreflected.ppm')

if __name__ == '__main__':
    main()