import pandas as pd
from numpy import mean
from numpy import std
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import accuracy_score
from sklearn.model_selection import RepeatedStratifiedKFold
from sklearn.model_selection import cross_val_score
# from imblearn.over_sampling import SMOTEN
# from imblearn.over_sampling import SMOTENC
from imblearn.over_sampling import SMOTE



BRIX_MAX_UNDERRIPE = 11.5       # under 11.5: underripe
BRIX_MIN_OVERRIPE = 12.1        # over 12.1: overripe

# lambda function to make new column, 'Result': 
#   0 if underripe  (Brix < 11.5)
#   1 if good       (11.5 <= Brix <= 12.1)
#   2 if overripe   (12.1 < Brix)


ripenessFunc = lambda brix: 0 if (brix < BRIX_MAX_UNDERRIPE) else (1 if (brix >= 11.5 and brix <=12.1) else 2)

df = pd.read_csv('nir_to_brix_data.csv')
df['Result'] = df['Brix'].apply(ripenessFunc)
# print(df)

X = df[['R', 'S', 'T', 'U', 'V', 'W']]
y = df['Result']

# expand dataset
# oversample = SMOTENC(categorical_features=[0,1,2], random_state=1)
# oversample = SMOTE(sampling_strategy="all", k_neighbors = 2, random_state=1)
# X, y = oversample.fit_resample(X, y)

print("df: X.shape:", X.shape, "\ty.shape:", y.shape)

X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.2, random_state=1002)

print("X_train:\n", X_train)
print("y_train:\n", y_train)

print("\nX_test:\n", X_test)
print("y_test:\n", y_test)

# define multinomial logistic regression model using a solver that 
# supports multinomial logistic regression, such as lbfgs
model = LogisticRegression(multi_class='multinomial', solver='newton-cg', max_iter=1000)

# cross validation
cv = RepeatedStratifiedKFold(n_splits=2, n_repeats=2, random_state=428)
n_scores = cross_val_score(model, X, y, scoring='accuracy', cv=cv, n_jobs=-1)

print('Mean Accuracy: %.3f (%.3f)' % (mean(n_scores), std(n_scores)))

lr = model.fit(X_train, y_train)


X_pred = lr.predict(X_test)

accuracytrain = lr.score(X_train, y_train)
accuracytest = lr.score(X_test, y_test)

print("Train score:", accuracytrain)
print("Test score:", accuracytest)
# accuracy_train = model.score(X_train,y_train)
# accuracy_test  = model.score(X_test,y_test)
# coefs = model.coef_
# # prediction = reg.predict(y_test)
# x_pred_train = model.predict(X_train)
# x_pred_test = model.predict(X_test)

# print("\n\nLinearRegression train score:", accuracy_train)
# print("LinearRegression test score: ", accuracy_test)
# print("LinearRegression coefs:", coefs)

# print("\n\nLinearRegression x_prediction_train:", x_pred_train)
# print("LinearRegression x_prediction_test :", x_pred_train)

# print("\n\nLinearRegression accuracy_train:", accuracy_train)
# print("LinearRegression accuracy_test :", accuracy_test)

