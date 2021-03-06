#!/usr/bin/env python3
import random
import locale
from collections import namedtuple
import argparse
import os


EMPTY = ''
END = '#'
TRIES = 300

Item = namedtuple('Item', ['row', 'col', 'dir', 'score', 'word'])

class Puzzle:
    def __init__(self, words, rows, cols):
        self.word_dict = words
        self.rows = rows
        self.cols = cols

        self.grid = [[EMPTY] * cols for i in range(0, rows)]
        self.hints = [[False] * cols for i in range(0, rows)]
        self.starts = [[{'right': None, 'down': None} for j in range(cols)] for i in range(rows)]
        self.words = [[] for i in range(0, 20)]

        self._generate()
        self._generate_hints()
        
        for words in self.words:
            words.sort(key=lambda w: locale.strxfrm(w.word))

    @property
    def total_words(self):
        return sum([len(l) for l in self.words])

    def _generate_hints(self):
        remaining = self.total_words * 0.05

        while remaining > 0:
            r = random.randint(0, self.rows - 1)
            c = random.randint(0, self.cols - 1)

            if self.grid[r][c] not in [EMPTY, END]:
                self.hints[r][c] = True
                remaining -= 1


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
                            available.append(Item(r, c, d, score, w))
                            ss = 200

#            print(w)
#            print(available)
#            print()
            if available:
                max_score = max([x.score for x in available])
                available = list(filter(lambda w: w.score == max_score, available))
                chosen = random.choice(available)

                r = chosen.row
                c = chosen.col
                for l in w:
                    self.grid[r][c] = l
                    r += chosen.dir[0]
                    c += chosen.dir[1]

                self.words[len(chosen.word)].append(chosen)
                nothing = 0

                label = 'down' if chosen.dir[0] == 1 else 'right' 
                self.starts[chosen.row][chosen.col][label] = len(chosen.word)

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
        if self.get(r - d[0], c - d[1]) not in [EMPTY, END]:
            return 0

        # occupied after word
        if self.get(r + (len(w) - 1) * d[0] + d[0], c + (len(w) - 1) * d[1] + d[1]) not in [EMPTY, END]:
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
                    # penalty for multiple words that matches same place
                    ambiguous = [item for item in self.words[len(w)] if item.word[i] == l]
                    score += 600 - 4000 * len(ambiguous)

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


parser = argparse.ArgumentParser()
parser.add_argument("--words", help="path to file with words separated by newline", default="./words")
parser.add_argument("--css", default="./style.css")
parser.add_argument("--css-embed", help='embed css directly to html', action='store_true')
parser.add_argument("--rows", default=35, type=int)
parser.add_argument("--cols", default=31, type=int)
parser.add_argument("--seed")
parser.add_argument('--serialize', help='puzzle object will be serialized to this file')

options = parser.parse_args()

# TODO: why it is not working from ENV?
locale.setlocale(locale.LC_COLLATE, 'cs_CZ.UTF8')

if not options.seed:
    options.seed = random.randint(0, 65000)

random.seed(options.seed)
p = Puzzle(load_words(options.words), options.rows, options.cols)

variables = {
        "seed": options.seed
}

if options.css_embed:
    with open('style.css') as f:
        variables['style'] = f.read()
else:
    variables['css'] = options.css

print(render("template.j2", puzzle=p, **variables)) 

if options.serialize:
    import pickle

    try:
        os.mkdir(os.path.dirname(options.serialize))
    except FileExistsError as e:
        pass

    with open(options.serialize, "wb") as f:
        pickle.dump(p, f)

