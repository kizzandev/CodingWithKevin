if __name__ == '__main__':
    lst = []

    for _ in range(int(input())):
        name = input()
        score = float(input())

        lst.append([name, score])

    second_lowest_score = sorted(set(score for name, score in lst))[1]

    for name, score in sorted(lst):
        if score == second_lowest_score:
            print(name)
