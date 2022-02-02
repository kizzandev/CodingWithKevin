import random
import json


def is_correct(value, user_answer):
    if value == user_answer:
        return True
    else:
        return False


def show_score(correct_guesses, incorrect_guesses, amount_questions):
    try:
        correct_percentage = correct_guesses / amount_questions * 100
        incorrect_percentage = incorrect_guesses / amount_questions * 100
        if correct_percentage % 2 == 0 or correct_percentage == 0:
            print(f'{correct_percentage:0.0f}% of you answers were correct.')
        else:
            print(f'{correct_percentage:0.2f}% of you answers were correct.')
        if incorrect_percentage % 2 == 0 or incorrect_percentage == 0:
            print(f'{incorrect_percentage:0.0f}% of you answers were incorrect.')
        else:
            print(f'{incorrect_percentage:0.2f}% of you answers were incorrect.')
    except ZeroDivisionError as e:
        print(e)
        pass


def restart_game(amount_questions, questions, answers):
    aux = input('Would you like to play again? Type \'yes\' to do so: ')
    if aux == 'yes':
        start_game(amount_questions, questions, answers, False)


def start_game(amount_questions=0, questions=None, answers=None, first_game=True):
    # initializing
    if answers is None:
        answers = []
    if questions is None:
        questions = {}
    guesses = []  # list
    correct_guesses = 0
    incorrect_guesses = 0
    question_num = 0
    ans_count = 1
    add_q = True

    if first_game is True:
        aux_choice = input('Would you like to load a previously saved file? ')
        if aux_choice == 'yes':
            questions, answers, amount_questions = load_from_file()
            aux_choice = input('Would you like to add additional questions? ')
            if aux_choice != 'yes':
                add_q = False
        else:
            try:
                ans_count = int(input('Amount of possible answers: '))
            except ValueError as e:
                print(e)

    # generation of questions
    if first_game is False:
        aux_choice = input('Would you like to add more questions? Type \'yes\' to do so: ')
        if aux_choice == 'yes':
            first_game = True

    while first_game is True and add_q is True:
        add_questions(questions, answers, ans_count)
        amount_questions += 1
        aux_choice = input('Would you like to add another question? Type \'yes\' to add more: ').lower()
        if aux_choice != 'yes':
            aux_choice = input('Would you like to save these questions? ')
            if aux_choice == 'yes':
                save_to_file(questions, answers, amount_questions)
            break

    lst_of_questions = shuffle_questions(questions)

    for qtn in lst_of_questions:
        question_num += 1
        print(qtn)
        possible_answers = shuffle_answers(qtn, answers)
        answer_num = 0
        for ans in possible_answers:
            answer_num += 1
            print(f'{answer_num}.', ans, sep=' ', end='\n')
        value = None
        for key, val in questions.items():
            if key == qtn:
                value = val

        correct_guesses, incorrect_guesses = choose_answer(possible_answers, value, guesses, correct_guesses,
                                                           incorrect_guesses)

    show_score(correct_guesses, incorrect_guesses, amount_questions)

    restart_game(amount_questions, questions, answers)


def choose_answer(possible_answers, value, guesses, correct_guesses, incorrect_guesses):
    while True:
        try:
            user_answer = int(input('Choose the correct answer by it\'s number: '))
            guesses.append(possible_answers[user_answer - 1])

            if is_correct(value, possible_answers[user_answer - 1]):
                print('correct')
                correct_guesses += 1
            else:
                print('incorrect')
                incorrect_guesses += 1
            return correct_guesses, incorrect_guesses
        except ValueError as e:
            print(e)
            print('Please enter a number.')
        except IndexError as e:
            print(e)
            print('You may only choose 1, 2, 3 or 4 as possible answers')
        except Exception as e:
            print(e)


def shuffle_answers(key, answers):
    for lst in answers:
        if lst[0] == key:
            ans_lst = lst[1:]
            random.shuffle(ans_lst)
            return ans_lst
    return ['Answers', 'Not', 'Found.']


def shuffle_questions(questions):
    lst = []
    for key in questions:
        lst.append(key)
    random.shuffle(lst)
    return lst


def add_questions(questions, answers, amount_possible_ans):
    user_question = input('Enter the question: ')
    user_answer = input('Enter the correct answer: ')
    questions[user_question] = user_answer  # added to dictionary
    answers_group = [f'{user_question}', user_answer]  # list group of the index(question) and the answers
    for x in range(amount_possible_ans - 1):
        user_answer = input('Enter another answer: ')
        answers_group.append(user_answer)
    answers.append(answers_group)  # added to the answers list


def save_to_file(questions, answers, amount_questions):
    file_name = input('Please write the name of the file without extension: ')
    to_save = [questions, answers, amount_questions]
    with open(f'{file_name}.json', 'w', encoding='utf-8') as f:
        json.dump(to_save, f, ensure_ascii=False, indent=4)


def load_from_file():
    file_name = input('Name of the file without extension (must be in folder): ')
    with open(f'{file_name}.json', 'r') as f:
        to_get = json.load(f)
    questions = to_get[0]
    answers = to_get[1]
    amount_questions = to_get[2]
    return questions, answers, amount_questions
