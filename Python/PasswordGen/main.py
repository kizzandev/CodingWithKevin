import requests
import random
import string
import re


def random_string(to_print):
    """Returns a random set of characters that has at least 1 of each: digit, undercase, uppercase, symbol
    (Allows the user to select how many characters they want)."""
    choose = 0
    while choose < 8:
        choose = int(input('Select amount of characters (>7): '))

    while True:
        to_print = ''.join(random.SystemRandom().choice(
            string.ascii_letters + string.digits + r'#$/()=!%&@?-') for _ in range(choose))

        if None not in (re.search(r"[0-9]", to_print), re.search(r"[a-z]", to_print),
                        re.search(r"[A-Z]", to_print), re.search(r"[#\$/\(\)=!%&@?\-]", to_print)):  # regex
            break

    return to_print


def random_words(to_print):
    """Returns a random set of words provided by https://www.mit.edu/~ecprice/wordlist.10000
    (Allows the user to select how many words they want)."""
    words = 'https://www.mit.edu/~ecprice/wordlist.10000'
    response = requests.get(words)
    words = response.content.splitlines()

    choose = 0
    while choose < 4:
        choose = int(input('Select amount of characters (>3): '))

    for _ in range(choose):
        to_print += words[random.SystemRandom().randint(0, 9999)
                          ].decode('utf-8') + ' '

    return to_print


def main():
    to_print = ''
    choose = int(input(
        """1. Create a random string
2. Create a random set of words
3. Exit
"""))

    if choose == 1:
        to_print = random_string(to_print)
    elif choose == 2:
        to_print = random_words(to_print)
    elif choose == 3:
        quit()
    else:
        main()

    print(f"\n{to_print}\n")
    to_print = ''
    main()


if __name__ == "__main__":
    main()
