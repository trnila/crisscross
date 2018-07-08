import random

EMPTY = ''
END = '#'

class Puzzle:
    def __init__(self, words, rows, cols, total_words):
        self.word_dict = words
        self.rows = rows
        self.cols = cols

        self.grid = [[EMPTY] * cols for i in range(0, rows)]
        self.words = []

        random.shuffle(self.word_dict)
        dirs = [[1, 0], [0, 1]]

        ss = 0
        for w in self.word_dict:
            available = []

            for r in range(0, rows):
                for c in range(0, cols):
                    for d in dirs:
                        if self.get(r - d[0], c - d[1]) != EMPTY:
                            continue
                        if self.get(r + len(w) * d[0] + d[0], c + len(w) * d[1] + d[1]) != EMPTY:
                            continue

                        score = 0
                        for i, l in enumerate(w):
                            x = r + i * d[0]
                            y = c + i * d[1]
                            cell = self.get(x, y)
                            score += 1

                            if cell != EMPTY and cell != l:
                                score = 0
                                break

                            if self.get(x + d[1], y + d[0]) != EMPTY or self.get(x - d[1], y - d[0]) != EMPTY:
                                if cell == EMPTY:
                                    score = 0
                                    break
                                else:
                                    score += 200

                            if cell == l:
                                score += 100

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
                self.words.append((r, c, d, w))

            if len(self.words) >= total_words:
                break

        self.legend = {}
        for word in sorted(self.words, key=lambda x: (len(x[3]), x[3])):
            k = len(word[3])
            if k not in self.legend:
                self.legend[k] = []
            self.legend[k].append(word)


    def get(self, r, c):
        if r < 0 or c < 0 or r >= self.rows or c >= self.cols:
            return END
        return self.grid[r][c]




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

random.seed(0)
p = Puzzle(load_words("./words"), 35, 33, 70)
print(render("template.j2", puzzle=p, style=open("style.css").read()))

