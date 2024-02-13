# Kenneth Woodard
# COSC 423

import sys
import heapq

class PathPlanner:
    def __init__(self):
        pass

    def getNeighbors(self, grid, x, y):
        result = []
        nRows = len(grid)
        nCols = len(grid[0])
        if x > 0:
            result.append((x - 1, y))
        if x < nCols - 1:
            result.append((x + 1, y))
        if y > 0:
            result.append((x, y - 1))
        if y < nRows - 1:
            result.append((x, y + 1))
        return result

    def printResult(self, start, goal, parent, nodesTraversed):
        solution = []
        if goal in parent:
            while goal != start:
                solution.append(goal)
                goal = parent[goal]
            solution.append(start)
            solution.reverse()
        print("Path:", solution)
        print("Traversed:", nodesTraversed)

    def breadth_first_search(self, start, goal, grid):
        q0 = [start]
        parent = {start: start} # to mark from what point we discovered each
        q1 = []
        nodesTraversed = 1
        while q0:
            for cur in q0:
                if cur == goal:
                    break # reached the goal
                else:
                    for nxt in self.getNeighbors(grid, cur[0], cur[1]):
                        if grid[nxt[1]][nxt[0]] == 0 and nxt not in parent:
                            parent[nxt] = cur
                            q1.append(nxt)
                            nodesTraversed += 1
            q0, q1 = q1, q0
            q1.clear()
        self.printResult(start, goal, parent, nodesTraversed)
        

    def depth_first_search(self, start, goal, grid):
        q0 = [start]
        parent = {start: start} # to mark from what point we discovered each
        nodesTraversed = 1
        while q0:
            cur = q0.pop()
            if cur == goal:
                break # reached the goal
            else:
                for nxt in self.getNeighbors(grid, cur[0], cur[1]):
                    if grid[nxt[1]][nxt[0]] == 0 and nxt not in parent:
                        parent[nxt] = cur
                        q0.append(nxt)
                        nodesTraversed += 1
        self.printResult(start, goal, parent, nodesTraversed)

    def estDistance(self, p1, p2):
        return abs(p1[0] - p2[0]) + abs(p1[1] - p2[1])

    def a_star_search(self, start, goal, grid):
                 # est total path    # traveled so far # location
        pq = [(self.estDistance(start, goal), 0, start)]
        parent = {start: start} # to mark from what point we discovered each
        nodesTraversed = 1
        while pq:
            est, traveled, cur = heapq.heappop(pq)
            if cur == goal:
                break
            else:
                traveled += 1 # making the next step
                for nxt in self.getNeighbors(grid, cur[0], cur[1]):
                    if grid[nxt[1]][nxt[0]] == 0 and nxt not in parent:
                        parent[nxt] = cur
                        heapq.heappush(pq, (traveled + self.estDistance(nxt, goal), traveled, nxt))
                        nodesTraversed += 1
        self.printResult(start, goal, parent, nodesTraversed)
        

# parse and error check command line arguments
n = len(sys.argv)
if n != 9:
  sys.exit("Usage: python main.py --input (filename) --start (0,0) --goal (n, m) --search (BFS || DFS || A* || ALL)")

A = "--input"
B = "--start"
C = "--goal"
D = "--search"

if sys.argv[1] != A:
  sys.exit("Usage: python main.py --input (filename) --start (0,0) --goal (n, m) --search (BFS || DFS || A* || ALL)")
if sys.argv[3] != B:
  sys.exit("Usage: python main.py --input (filename) --start (0,0) --goal (n, m) --search (BFS || DFS || A* || ALL)")
if sys.argv[5] != C:
  sys.exit("Usage: python main.py --input (filename) --start (0,0) --goal (n, m) --search (BFS || DFS || A* || ALL)")
if sys.argv[7] != D:
  sys.exit("Usage: python main.py --input (filename) --start (0,0) --goal (n, m) --search (BFS || DFS || A* || ALL)")

filename = sys.argv[2]
start = sys.argv[4]
goal = sys.argv[6]
option = sys.argv[8] 

if option != "BFS" and option != "DFS" and option != "A*" and option != "ALL":
  sys.exit("Usage: python main.py --input (filename) --start (0,0) --goal (n, m) --search (BFS || DFS || A* || ALL)")

# check start and goal values
c1 = 0  # comma check 1
c2 = 0  # comma check 2
S1 = ""
S2 = ""
S3 = ""
S4 = ""

for x in start:
  if ord(x) == 44:
    c1 = 1
  if c1 == 0:
     S1 += x
  if c1 == 1 and ord(x) != 44:
     S2 += x

for x in goal:
  if ord(x) == 44:
    c2 = 1
  if c2 == 0:
     S3 += x
  if c2 == 1 and ord(x) != 44:
     S4 += x

# error check again
if c1 == 0 or c2 == 0:
  sys.exit("Error in start/goal value input")

start_x = int(S1)
start_y = int(S2)
goal_x = int(S3)
goal_y = int(S4)



# open & read file
try:
  grid = []
  with open(filename, "r") as f:
    for line in f:
      grid.append(list(map(int, line.split(","))))
except IOError:
  sys.exit("Error: file does not exist")


nRows = len(grid)
nCols = len(grid[0])

# error check again
if goal_x >= nCols or goal_y >= nRows:
  sys.exit("Error: goal value out-of-bounds")
if start_x >= nCols or start_y >= nRows:
  sys.exit("Error: start value out-of-bounds")

planner = PathPlanner()
if option == "BFS" or option == "ALL":
  planner.breadth_first_search((start_x, start_y), (goal_x, goal_y), grid)
if option == "DFS" or option == "ALL":
  planner.depth_first_search((start_x, start_y), (goal_x, goal_y), grid)
if option == "A*" or option == "ALL":
  planner.a_star_search((start_x, start_y), (goal_x, goal_y), grid)
