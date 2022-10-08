from collections import deque

# input -> int: test cases
n_cases = int(input())

for _ in range(n_cases):
    # input -> int: number of cubes
    n_cubes = int(input())
    # input -> deque: [length_1, length_2, ...]
    blocks = deque(map(int, input().split()))
    
    for _ in range(n_cubes - 1):
        # branch: pile up the greater number
        # if first >= last -> remove first
        # else remove last
        a = blocks.popleft() if blocks[0] >= blocks[-1] else blocks.pop()
        
        # if first or last element is greater than the value of a, then cannot be pile up
        if blocks[0] > a or blocks[-1] > a:
            break

    if not len(blocks) - 1:
        # all elements are pile up!
        print('Yes')
    else:
        # not piled up
        print('No')
