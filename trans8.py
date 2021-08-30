import pandas as pd

raw = pd.read_csv("data01.csv", header=None)
data = []

print(raw)

for ind, row in raw.iterrows():
    data.append(int(raw.iloc[ind,0].replace(' ', '').replace('+', '')))
    temp1 = raw.iloc[ind,1].split('+')
    data.append(int(temp1[0].replace(' ', '').replace('-', '')))
    data.append(int(temp1[1].replace(' ', '').replace('+', '')))
    temp2 = raw.iloc[ind,2].split('+')
    data.append(int(temp2[0].replace(' ', '').replace('-', '')))
    data.append(int(temp2[1].replace(' ', '').replace('+', '')))
    temp3 = raw.iloc[ind,3].split('+')
    data.append(int(temp3[0].replace(' ', '').replace('-', '')))
    data.append(int(temp3[1].replace(' ', '').replace('+', '')))
    if raw.iloc[ind,4]==raw.iloc[ind,4]:
        data.append(int(raw.iloc[ind,4].replace(' ', '').replace('-', '')))

print(data)