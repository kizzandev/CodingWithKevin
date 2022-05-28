import hashlib


def crack_sha1_hash(hash: str, use_salts=False):
    # get info from files
    with open('./top-10000-passwords.txt') as top:
        passwords = top.read().splitlines()
    if use_salts:
        with open('./known-salts.txt') as known_salts:
            salts = known_salts.read().splitlines()
        for password in passwords:
            for salt in salts:
                salted_password_1 = bytes(salt + password, 'utf-8')
                salted_password_2 = bytes(password + salt, 'utf-8')
                if hashlib.sha1(salted_password_1).hexdigest() == hash:
                    return password
                if hashlib.sha1(salted_password_2).hexdigest() == hash:
                    return password
        return 'PASSWORD NOT IN DATABASE'

    for password in passwords:
        if hashlib.sha1(bytes(password, 'utf-8')).hexdigest() == hash:
            return password
    return 'PASSWORD NOT IN DATABASE'
