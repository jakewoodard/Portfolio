# Kenneth Woodard

# description: 

import pandas as pd
import numpy as np
import pylab as pl

from sklearn import svm, datasets
from sklearn.model_selection import train_test_split
from sklearn.model_selection import GridSearchCV
from sklearn.metrics import classification_report
from sklearn.metrics import confusion_matrix, accuracy_score
from sklearn.svm import SVC

filename = "mushrooms.csv"

# shroomset is my dataset
shrooms = pd.read_csv(filename)
data = pd.DataFrame(shrooms)
x = data.drop('class', axis=1)
y = data['class']

# Convert label column to number:
labels = set(data['label_col'])
label_to_num = { label : num for num, label in enumerate(labels) }
labels = data['label_col'].apply(lambda x, mapping = label_to_num : mapping[x])
new_df = {}
# Transform all categorical columns into numeric ones:
for col in data.columns:
    if col != 'label_col':
        dummies = pd.get_dummies(data[col], prefix = col)
        output_df.update({col : dummies[col] for col in dummies.columns})

x_train, x_text, y_train, y_test = train_test_split(x, y, test_size=0.2, random_state=0)
y_train_converted = y_train.values.ravel()

#svc = svm.SVC(kernel='linear').fit(x_train, y_train)

print(shroomset.keys())
print()
print(shroomset)
