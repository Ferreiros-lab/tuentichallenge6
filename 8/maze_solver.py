import telnetlib

HOST = '52.49.91.111'
PORT = '1986'


directions = ['u', 'r', 'd', 'l']
movex = [0, 1, 0, -1]
movey = [-1, 0, 1, 0]

def next_blocked(maze, direction):
    """Checks whether it is possible to move in the given direction
    
    Args:
        maze: A string depicting a section of the maze, containing once the
            symbol 'x', which represents the current position
        direction: an integer from 0 to 3 representing the direction as in the
            list 'directions'
            
    Returns:
        A boolean stating whether the next block is blocked, i.e., contains
        a # symbol.
    """
    
    for index, line in enumerate(maze):
        if 'x' in line:
            position = (index, line.index('x'))
    new_position = (position[0] + movey[direction],
                    position[1] + movex[direction])
    return maze[new_position[0]][new_position[1]] == '#'

def rotate(direction, clockwise):
    """Rotates the in the given direction"""
    if clockwise:
        return (direction + 1) % 4
    else:
        return (direction + 3) % 4

def update_maze(maze_map, maze, position):
    """Updates the maze map from a section"""
    for i in range(7):
        for j in range(7):
            maze_map[position[0]+i-3][position[1]+j-3] = maze[i][j]

def main():
    tn = telnetlib.Telnet(HOST, PORT)
    direction = 0
    position = (100,100)
    maze_map = [[' ']*200 for i in range(200)]
    for movement in range(100000):
        maze = []
        for i in range(7):
            maze.append(tn.read_until('\n',0.5)[:-1])
        update_maze(maze_map, maze, position)
        direction = rotate(direction, clockwise = True)
        while next_blocked(maze, direction):
            direction = rotate(direction, clockwise = False)
        tn.write(directions[direction] + '\n')
        position = (position[0] + movey[direction],
                    position[1] + movex[direction])
        if movement % 100 == 0:
            print '\n'.join([''.join(line) for line in maze_map])

if __name__ == '__main__':
    main()