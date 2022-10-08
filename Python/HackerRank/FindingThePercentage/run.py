if __name__ == '__main__':
    n = int(input())
    student_marks = {}
    for _ in range(n):
        name, *line = input().split()
        scores = list(map(float, line))
        student_marks[name] = scores
    query_name = input()
    
    # The provided code stub will read in a dictionary containing key/value pairs of name:[marks] for a list of students.
    # Print the average of the marks array for the student name provided, showing 2 places after the decimal.

    # input
    # name score_1 score_2 score_3 ...
    
    # student_marks[name] = [score_1, score_2, score_3, ...]
    # student_marks = {name:[score_1, score_2, score_3, ...]}
    
    # query_name = name
    
    # output
    # avg of the marks ->  (score_1 + score_2 + score_3 + ...) / n
    # only 2 decimal values
    
    print(f'{(sum(student_marks[query_name]) / len(student_marks[query_name])):.2f}')