#!/user/bin/env python

import optuna
import subprocess
import sys

is_goal=False
goal_value=-1000.0

def objective(trial):

	global is_goal

	if is_goal :
		return 0;

	B = trial.suggest_uniform('B',0,5)
	C = trial.suggest_uniform('C',0,10)
	D = trial.suggest_uniform('D',0,1)
	E = trial.suggest_uniform('E',0,1)
	F = trial.suggest_uniform('F',0,5)

	subprocess.run(["sh", "data/jobscript.sh", str(0), str(B), str(C), str(D), str(E), str(F), str(0)])
	subprocess.run(["sh", "exp/jobscript.sh", str(1), str(0)])

	f = open("exp/score")
	line = f.readline()

	subprocess.run(["rm", "data/data"])
	subprocess.run(["rm", "exp/score"])

	ret = float(line)
	if ret <= goal_value :
		is_goal = True

	return ret

if __name__ == "__main__":
	study_name = sys.argv[1]
	storage_url = sys.argv[2]
	output_file = sys.argv[3]

	study = optuna.create_study(study_name=study_name, storage=storage_url, load_if_exists=True)

	study.optimize(objective, n_trials=100)

	f = open(output_file, mode='a')
	f.write(str(study.best_value))

	for p in study.best_params.values():
		f.write("," + str(p))
	f.write("\n")
