# Team 6 - Perceptron

![Alt Text](ezgif-6-85ba56436fb9.gif)

# Motivation
The ultimate goal of this project was the design, construct and train a neural network to play the ancient chinese strategy board game Go on a 9x9 board featuring traditional rules. The net should play at a proficiency level that encourages the human player to employ strategic counters of their own in pursuit of forcing the net to pass or otherwise passing themselves should they find they are unable. To this end, we have accomplished our goal in creating a convolutional neural network and it can be played against as demonstrated by this demo and by you. 

# Preliminary Steps:

It is necessary to install JupyterLab. Instructions and documentation are available here: https://jupyterlab.readthedocs.io/en/stable/

## Necessary Downloads

The git page contains four necessary files that must be downloaded for the network to function (the driver, network, datasets, and pretrained weights, respectively):
  1. exp_drivers.ipynb
  2. exp_GOnet.ipynb
  3. go9.zip
  4. mini_weights.h5

*From this point it is necessary to:*
  1. *Open the terminal*
  2. *Navigate to the location of downloaded files*
  3. *Run the command:*
   ```unzip go9.zip```
   
 # Open exp_GOnet.ipynb and run the code blocks as directed:
 
 # Load Drivers
 The code below links the necessary components to our file. Whether you're attempting to play with the network or train the network, you'll need to run this cell.
 ```#necessary imports
import pandas
import numpy as np
import os
import tensorflow.keras as keras
from keras.models import Model
from keras.layers import Dense, Input
from IPython.display import display
import sympy as sp
sp.init_printing(use_latex = True)
import math

import matplotlib.pyplot as plt
%matplotlib inline

%run exp_Drivers.ipynb

EMPTY = 1;
COLOR = 0;

BLACK = -1;
WHITE = 1;

WIDTH = 9;
```
# Data Categorization and Assignment 
If you're training the network, you need to run this code, as it converts the sgf games in the database into positions the network can read.
```
   Boards = []
Moves = []
def Main():
    path = "./go9"
    counter = 0
    for entry in os.scandir(path): #I changed my mind i love python
        Go = True
        Board = createEmptyBoard() # 0 - 80 = [color, empty], 81 = [turn, turn]
        with open(entry) as f:
            if Go:
                for line in f:
                    if line[0] == ';': # this is the line with all the moves.
                        Go = False
                        copy = ""
                        for c in line:
                            if c != "[" and c != "]" and c != ")":
                                copy += c
                        arr = copy[1:].split(';')
                        for a in arr:
                            int_move = Decode_Move(a[1:])
                            move = index_to_coordinate(int_move)
                            color = 1
                            if(a[0] == 'B'):
                                color = -1
                            Boards.append(Board)
                            Moves.append(int_move)
                            if int_move < 81:
                                Board = Move(Board, move[1], move[0])[1]
Main()
Boards = np.array(Boards)
print(Moves[0])
Moves = np.array(Moves)

print(Boards.shape)
```
40
```
print(Boards.shape)
```
(414124, 9, 9, 2)
```
# Example Position:
printBoard(Boards[22], -1)
printBoard(Boards[23], 1)
```
#  A B C D E F G H I
1  . . . . . . . . .
2  . . . . . . . . .
3  . . O . @ . @ . .
4  . . . O @ @ O @ .
5  . . @ O @ O O @ .
6  . . O . @ O . . .
7  . . . . @ O . . .
8  . . . @ O O . . .
9  . . . . . . . . .

#  A B C D E F G H I
1  . . . . . . . . .
2  . . . . . . . . .
3  . . O . @ . @ . .
4  . . . O @ @ O @ .
5  . . @ O @ O O @ .
6  . . O @ @ O . . .
7  . . . . @ O . . .
8  . . . @ O O . . .
9  . . . . . . . . .

```
X = Boards
Y = keras.utils.to_categorical(Moves)

training_samples = int(0.9 * X.shape[0])
X_train, X_test = X[:training_samples], X[training_samples:] # Inputs
Y_train, Y_test = Y[:training_samples], Y[training_samples:] # Outputs

print(X.shape)
print(Y.shape)
print(Moves)
```
(414124, 9, 9, 2)
(414124, 82)
[40 49 41 ... 75 18 36]

```
model = keras.models.Sequential()

model.add(keras.layers.Dense(2, activation = 'relu', input_shape = (9, 9, 2)))
model.add(keras.layers.Conv2D(81, (3, 3), activation = 'relu'))
model.add(keras.layers.Conv2D(81, (3, 3), activation = 'relu'))
model.add(keras.layers.Conv2D(81, (3, 3), activation = 'relu'))
model.add(keras.layers.Conv2D(81, (3, 3), activation = 'relu'))
model.add(keras.layers.Flatten())
model.add(keras.layers.Dense(82, activation = 'relu'))
model.add(keras.layers.Dropout(.25))
model.add(keras.layers.Dense(82, activation = 'softmax'))


model.compile(loss=keras.losses.CategoricalCrossentropy(), optimizer = keras.optimizers.Adam(), metrics = [keras.metrics.CategoricalAccuracy()])
print(X_train.shape)

model.summary()
```
(372711, 9, 9, 2)
Model: "sequential_1"
_________________________________________________________________
Layer (type)                 Output Shape              Param #   
=================================================================
dense_3 (Dense)              (None, 9, 9, 2)           6         
_________________________________________________________________
conv2d_4 (Conv2D)            (None, 7, 7, 81)          1539      
_________________________________________________________________
conv2d_5 (Conv2D)            (None, 5, 5, 81)          59130     
_________________________________________________________________
conv2d_6 (Conv2D)            (None, 3, 3, 81)          59130     
_________________________________________________________________
conv2d_7 (Conv2D)            (None, 1, 1, 81)          59130     
_________________________________________________________________
flatten_1 (Flatten)          (None, 81)                0         
_________________________________________________________________
dense_4 (Dense)              (None, 82)                6724      
_________________________________________________________________
dropout_1 (Dropout)          (None, 82)                0         
_________________________________________________________________
dense_5 (Dense)              (None, 82)                6806      
=================================================================
Total params: 192,465
Trainable params: 192,465
Non-trainable params: 0
_________________________________________________________________

If you already have a .h5 weights file, then you can run this cell to load those weights.

```
# Load Weights
model.load_weights('mini_weights.h5')
```
# Training
Here is where the training is conducted. If you sipmly want to play against the neural net, skip to the last cell.
```
#Train the model
history = model.fit(X_train, Y_train, batch_size = 32, epochs = 6, workers = 10, verbose = 1, validation_data = (X_test, Y_test))
```
```
# Save Weights
model.save_weights('mini_weights.h5')
```

# Results
```
plt.figure(1)

plt.subplot(211)
plt.plot(history.history['categorical_accuracy'])
plt.plot(history.history['val_categorical_accuracy'])
plt.title('model accuracy')
plt.ylabel('accuracy')
plt.xlabel('epoch')
plt.legend(['train', 'val'], loc='upper left')

plt.subplot(212)
plt.plot(history.history['loss'])
plt.plot(history.history['val_loss'])
plt.title('model loss') 

plt.ylabel('loss')
plt.xlabel('epoch')
plt.legend(['train', 'val'], loc='upper left')
plt.tight_layout()
plt.show()
```
![image](https://user-images.githubusercontent.com/74555083/116799267-88776900-aabd-11eb-833b-43e4417c0b18.png)

# Play
```
Play()
```
