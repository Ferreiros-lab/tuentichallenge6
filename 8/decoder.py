
def main():
    filename = 'maze'
    with open(filename, 'r') as f:
        maze = f.read().splitlines()
    up = maze[1][64:141]
    right = ''.join([line[140] for line in maze[2:-2]])
    down = maze[-2][140:59:-1]
    left = ''.join([line[60] for line in maze[-3:2:-1]])
    print up + right + down + left
if __name__ == '__main__':
    main()