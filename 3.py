from yaml import load
import sys

class TuringMachine(object):
    """Implementation of a Turing Machine"""
    
    initial_state = 'start'
    final_state = 'end'
    offset = {'right': 1, 'left': -1}
    
    def __init__(self, info):
        """Inits TuringMachine with the information of a Turing Machine."""
        self.__code = info['code']
        self.__tapes = info['tapes']
        
    
    def __run_tape(self, tape_content):
        """Runs the Turing Machine with a given initial tape tape_content
        
        Args:
            tape_content: The initial content of the tape
        
        Returns:
            A string containing the final content of the tape
        """
        tape = list(tape_content)  # Using a mutable object
        state = self.initial_state
        position = 0
        while state != self.final_state:
            while position >= len(tape):  # Out of the tape
                tape.append(' ')
            instructions = self.__code[state][tape[position]]
            if 'write' in instructions:
                tape[position] = instructions['write']
            if 'move' in instructions:
                position += self.offset[instructions['move']]
            if 'state' in instructions:
                state = instructions['state']
        return ''.join(tape)
            
    
    def run_all_tapes(self):
        """Runs all the tapes of the Turing Machine.
        
        The function prints the result using the standard output
        """
        
        for num_tape, tape_content in sorted(self.__tapes.items()):
            print 'Tape #%d: %s' % (num_tape, self.__run_tape(tape_content))


def main():
    data = load(sys.stdin)
    turing_machine = TuringMachine(data)
    turing_machine.run_all_tapes()

if __name__ == '__main__':
    main()