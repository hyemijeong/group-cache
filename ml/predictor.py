from sklearn.ensemble import RandomForestClassifier
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier
from sklearn.metrics import accuracy_score
import pickle
import joblib

# predict_joblib = joblib.load('./ml/predict_model.pkl')    
    
def predictor(model_path,avg_freq,avg_reuse,avg_recency,std_freq,std_reuse,std_recency):
    predict_joblib = joblib.load(model_path)
    p_data = pd.DataFrame({
      'avg_freq' : [avg_freq],
      'avg_reuse' : [avg_reuse],
      'avg_recency' : [avg_recency],
      'std_freq' : [std_freq],
      'std_reuse' : [std_reuse],
      'std_recency' : [std_recency]
    })
    y_pred = 123
    # y_pred = predict_joblib.predict(p_data)
    return y_pred

