import os
import shutil
import time
import pandas as pd
from copy import deepcopy

n_experiments = 10
times = []
outfile_res = "results.csv" 

# Experiments:
experiments = dict()

for i in range(1,9):
    experiments[f"L{i}"] = f"output_L{i}.txt"

# Creating new directory for experiments
try:
    shutil.rmtree('experiments')
    print("Removing & Creating new direcory: experiments/")
except:
    print("Creating new direcory: experiments/")

os.mkdir('experiments')


# Run experiments
for e in experiments:
    print(" ~~~ ", e, " ~~~ ")
    tic = time.perf_counter()
    for seed in range(n_experiments):
        print("\tseed: ", seed)
        command = f"./bin/ggi -p bin/{e}/positive.txt -n bin/{e}/negative.txt " +\
                  f"-i {seed} -o experiments/{experiments[e]} >> experiments/{e}_log_{experiments[e]}"
        os.system(command)
    toc = time.perf_counter()
    print(f"{e} executed in {(toc - tic)/n_experiments:0.4f} seconds on average")
    times.append((toc - tic)/n_experiments)


print(times)
if(len(times)<n_experiments):
    times = [-1] * len(experiments)


# Read results
names=['generations', 'non-terminals', 'rules', 'gen_len', 'accuracy', 'TPR', 'TNR', 'none']
df = []
for e in experiments:
    file = f'experiments/output_{e}.txt'
    _df = pd.read_csv(file, sep=",", names=names, engine='python')
    _df = _df[names[:-1]]
    _df['Dataset'] = e
    df.append(deepcopy(_df))

df = pd.concat(df)
aggregations = dict()
for n in names[:-1]:
    aggregations[n] = ['min', 'mean', 'max']

df_res = df.groupby('Dataset').agg(aggregations).reset_index()
df_res['mean_time']  = times

df_res.to_csv(outfile_res, index = False)

print(df_res)

# Remove all files except results
try:
    shutil.rmtree('experiments')
    print("Removing direcory: experiments/")
except:
    pass