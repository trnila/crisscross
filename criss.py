import random
import locale

EMPTY = ''
END = '#'
TRIES = 300

class Puzzle:
    def __init__(self, words, rows, cols):
        self.word_dict = words
        self.rows = rows
        self.cols = cols

        self.grid = [[EMPTY] * cols for i in range(0, rows)]
        self.words = [[] for i in range(0, 20)]

        self._generate()
        
        for words in self.words:
            words.sort(key=lambda w: locale.strxfrm(w[3]))


    def _generate(self):
        random.shuffle(self.word_dict)
        dirs = [[1, 0], [0, 1]]

        ss = 0
        nothing = 0
        for w in self.word_dict:
            available = []

            for r in range(0, self.rows):
                for c in range(0, self.cols):
                    for d in dirs:
                        score = self.calculate_score(r, c, d, w)

                        if score > ss:
                            available.append((r, c, d, score))
                            ss = 200

#            print(w)
#            print(available)
#            print()
            if available:
                max_score = max([x[3] for x in available])
                available = list(filter(lambda w: w[3] == max_score, available))
                chosen = random.choice(available)

                r = chosen[0]
                c = chosen[1]
                for l in w:
                    self.grid[r][c] = l
                    r += chosen[2][0]
                    c += chosen[2][1]

                self.words[len(w)].append((r, c, d, w))
                nothing = 0
            else:
                nothing += 1

            if nothing >= TRIES:
                break

    def get(self, r, c):
        if r < 0 or c < 0 or r >= self.rows or c >= self.cols:
            return END
        return self.grid[r][c]

    def calculate_score(self, r, c, d, w):
        # occupied before word
        if self.get(r - d[0], c - d[1]) != EMPTY:
            return 0

        # occupied after word
        if self.get(r + (len(w) - 1) * d[0] + d[0], c + (len(w) - 1) * d[1] + d[1]) != EMPTY:
            return 0

        score = 0
        for i, l in enumerate(w):
            is_first = i == 0
            is_last = i + 1 == len(w)

            x = r + i * d[0]
            y = c + i * d[1]
            cell = self.get(x, y)
            score += 1

            # already occupied by different letter
            if cell != EMPTY and cell != l:
                return 0

            # neighour cell
            if self.get(x + d[1], y + d[0]) != EMPTY or self.get(x - d[1], y - d[0]) != EMPTY:
                if cell == EMPTY:
                    return 0
                else:
                    score += 600

            # shared letter with another word
            if cell == l:
                if is_first or is_last:
                    score += 100
                else:
                    score += 200

        return score




def load_words(path, min_length=4, max_length=15):
    with open(path) as f:
        words = [s.strip() for s in f.readlines()]
        return list(filter(lambda w: len(w) >= min_length and len(w) <= max_length, words))

def render(template, **kwargs):
    from jinja2 import FileSystemLoader, Environment
    import os
    env = Environment(loader=FileSystemLoader(os.path.dirname(template)))
    template = env.get_template(os.path.split(template)[-1])
    return template.render(**kwargs)

# TODO: why it is not working from ENV?
locale.setlocale(locale.LC_COLLATE, 'cs_CZ.UTF8')
random.seed(0)
p = Puzzle(load_words("./words"), 35, 33)
print(render("template.j2", puzzle=p, style=open("style.css").read()))

