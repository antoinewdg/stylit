import cv2
import os

filename = '../files/models.png'
out_dir = '../files'
models = cv2.imread(filename)

names = ['hat', 'train', 'teacup', 'rabbit', 'hat_bis', 'hand', 'duck']
lpes = ['rendered', 'lde', 'lse', 'ld12e', 'l_dde']

for i in range(7):
    for j in range(5):
        row = (456 + 25) * i
        col = (600 + 25) * j

        dir_name = out_dir + '/' + names[i]
        out_name = dir_name + '/' + lpes[j] + '.png'
        if not os.path.isdir(dir_name):
            os.makedirs(dir_name)
        print(out_name)
        cv2.imwrite(out_name, models[row: row + 456, col:col + 600])
