
#랜덤 포레스트 모델    
def randomforest(traindata):
    data = pd.read_csv(traindata, sep=",")
    data = data.dropna()
    data = data.loc[1:1000]

    nulldata = data[data['reusedistance_std']=='#NAME?'].index
    data = data.drop(nulldata)
    nulldata = data[data['reusedistance_mean']=='#NAME?'].index
    data = data.drop(nulldata)

    x = data.iloc[:,[0,1,2,3,4]]
    y = data.iloc[:,[5]]

    x_train, x_test, y_train, y_test = train_test_split(x, y, test_size=0.1, random_state=321)
    #y_train = y_train.astype('int')
    #y_test = y_test.astype('int')

    forest = RandomForestClassifier(n_estimators= 5 ,n_jobs = 18, random_state=2)
    forest.fit(x_train, y_train)
    joblib.dump(forest, './ml_model/randomforestModel.pkl')

    y_pred=forest.predict(x_test)

    plt.figure(figsize=(5, 5))
    predict=plt.scatter(x=x_test.index, y=y_test,s=5,c='magenta')
    real=plt.scatter(x=x_test.index, y=y_pred,s=5)
    plt.xlabel('access time(count)')
    plt.ylabel('Reuse distance')
    plt.legend((predict,real),('predict','real'),loc='best', ncol=1)
    plt.savefig("./result/rf.png",dpi=200)

if (model_name == "RF"):
  randomforest(traindata_path)
  
elif (model_name == "LSTM"):
  LSTM(traindata_path)
