from collections import Counter, defaultdict

class Tree(object):
    """A class containing a rooted Tree"""
    
    def __compute_root(self):
        """Computes the root of the Tree"""
        (self.__root,) = self.__names - set(self.__parent.keys())
        
    def __compute_children(self):
        """Computes the children of each city in the Tree"""
        self.__children = {city:[] for city in self.__names}
        for children, parent in self.__parent.iteritems():
            self.__children[parent].append(children)
    
    def __init__(self, size):
        """Inits Tree with a given size.
        
        The Tree is read from the standard input.
        """
        self.__parent = {}
        for i in range(size - 1):
            parent, child = raw_input().split()
            self.__parent[child] = parent
        self.__names = set(self.__parent.keys()) | set(self.__parent.values())
        self.__compute_root()
        self.__compute_children()
    
    def getParent(self, city):
        """Returns the parent of a city"""
        return self.__parent[city]
    
    def getNames(self):
        return self.__names
    
    def isRoot(self, city):
        return self.__root == city
    
    def getLevels(self):
        """For each level returns the cities in this level
        
        Returns:
            A list of lists. In position i, starting from 0, there will be
            a list of strings indicating the names of the cities at level i,
            in no particular order.
        """
        
        levels = [[self.__root]]
        while levels[-1]:  # While the last iteration gives new children:
            levels.append([])
            for city in levels[-2]:
                levels[-1] += self.__children[city]
                
        levels.pop()  # Last iteration gave no children
        return levels
    
    def compare(self, virus):
        """Checks whether there exists an equivalence between two virus
        
        If there is such an equivalence, it returns it. If there are many,
        cities with lower lexicographical order are related with cities
        with lower lexicographical order.
        
        Args:
            virus: A Tree to be compared with self.
        
        Returns:
            A dictionary. It will be empty if there is not correspondence.
            Otherwise, the keys will be cities in self, and the values the
            corresponding cities in virus.
        """
        
        level1 = self.getLevels()
        level2 = virus.getLevels()
        if len(level1) != len(level2):
            return {}
        names1 = self.getNames()
        names2 = virus.getNames()
        if len(names1) != len(names2):  # Should not happen
            return {}
        
        # For each city we keep an encoding of its children
        encoding1 = {city:Counter() for city in names1}
        encoding2 = {city:Counter() for city in names2}
        
        relations = {}
        for level in reversed(range(len(level1))):
            # codesi is a dictionary from a hash to a list of cities where
            # the corresponding encoding has this hash
            codes1 = defaultdict(list)
            codes2 = defaultdict(list)
            for city in level1[level]:
                encoded = hash(tuple(sorted(encoding1[city].items())))
                codes1[encoded].append(city)
                if not self.isRoot(city):
                    encoding1[self.getParent(city)][encoded] += 1
            for city in level2[level]:
                encoded = hash(tuple(sorted(encoding2[city].items())))
                codes2[encoded].append(city)
                if not virus.isRoot(city):
                    encoding2[virus.getParent(city)][encoded] += 1
                
            if sorted(codes1.keys()) != sorted(codes2.keys()):
                return {}
            
            for key, cities1 in codes1.iteritems():
                cities2 = codes2[key]
                if len(cities1) != len(cities2):
                    return {}
                for city1, city2 in zip(sorted(cities1), sorted(cities2)):
                    relations[city1] = city2
                    
        return relations
            

def main():
    N = int(raw_input())
    original = Tree(N)
    T = int(raw_input())
    for case_num in range(1, T+1):
        virus = Tree(N)
        relation = original.compare(virus)
        if relation:
            output = ' '.join('/'.join(cities) 
                              for cities in sorted(relation.iteritems()))
        else:
            output = 'NO'
        print 'Case #%d: %s' % (case_num, output)

if __name__ == '__main__':
    main()