import re
import optparse
import os 
import classify 
import pandas


	###### PARSING #######


parser = optparse.OptionParser()
parser.add_option('-d', help='[Mandatory] DATASET file', dest='dataset_file', type='string')
parser.add_option('-c', help='[Mandatory] expected number of CLUSTERS ', dest='num_clusters', type='int')
parser.add_option('-f', help='[Mandatory] number of FEATURES ', dest='num_feats', type='int')
parser.add_option('-m', help='[Mandatory] run MODE: kmeans, rls, ils or ga ', dest='mode', type='string')
parser.add_option('-t', help='[Mandatory] TEST destination path', dest='test_path', type='string')
parser.add_option('-i', help='number of iterations', dest='num_iter', type='int')


KM = optparse.OptionGroup(parser, 'KMEANS Options')
KM.add_option('-o', help='boolean option: start from dataset solution', dest='km_sol', default=False, action='store_true')

GA = optparse.OptionGroup(parser, 'GA Options')
GA.add_option('-n', help='population size', dest='tam_pob', type='int')
GA.add_option('--pc', help='crossover probability', dest='prob_cross', type='float')
GA.add_option('--pm', help='mutation probability', dest='prob_mutar', type='float')

parser.add_option_group(KM)
parser.add_option_group(GA)

(opts, args) = parser.parse_args()

# Making sure all options appear
mandatories = ['dataset_file', 'num_clusters', 'num_feats', 'mode', 'test_path']
for m in mandatories:
	if not opts.__dict__[m]:
		print "\nMandatory option is missing\n"
		parser.print_help()
		exit(-1)

if (opts.mode not in ['kmeans', 'ils', 'rls', 'ga']):
	print "\nInvalid argument for option -m\n"
	parser.print_help()
	exit(-1)

if (opts.mode == "rls" or opts.mode == "ils") and not opts.__dict__['num_iter']:
	print "\nmode " + opts.mode + " requires option -i\n"
	parser.print_help()
	exit(-1)

ga_mandatories = ['num_iter', 'tam_pob', 'prob_cross', 'prob_mutar']
if (opts.mode == "ga" ):
	for m in ga_mandatories:
		if not opts.__dict__[m]:
			print "\nga mode requires options -i, -n, --pc, --pm\n"
			parser.print_help()
			exit(-1)
			

	###### CREATING OUTPUT DIRECTORIES #######

def ensure_dir(f):
	d = os.path.dirname(f)
	if not os.path.exists(d):
		os.makedirs(d)

filename = opts.dataset_file[15:-5]
path = opts.test_path+"/"+filename+"/"+opts.mode+"/"
print path
ensure_dir(path)

	###### EXECUTING #######

if opts.mode == "kmeans":
	command = "./kmeans " + str(opts.num_clusters) + " " + opts.dataset_file + " " + str(opts.num_feats) 

if opts.mode == "rls":
	command = "./rls " + str(opts.num_clusters) + " " + opts.dataset_file + " " + str(opts.num_feats) + " " + str(opts.num_iter)

if opts.mode == "ils":
	command = "./ils " + str(opts.num_clusters) + " " + opts.dataset_file + " " + str(opts.num_feats) + " " + str(opts.num_iter)

if opts.mode == "ga":
	command = "./ga " + str(opts.num_clusters) + " " + opts.dataset_file + " " + str(opts.num_feats) + " " + str(opts.num_iter) + \
			  " " + str(opts.tam_pob) + " " + str(opts.prob_cross) + " " + str(opts.prob_mutar)


print command
for i in range(1,11):
	spec_path = path +opts.mode+ str(i) + ".out"
	run = command +  " > " + spec_path
	print run 
	os.system(run)


	###### TESTING #######



trnfile = open(opts.dataset_file, 'r')
trainset = []
for t in trnfile:
		trainset.append(t)

out_files = [f for f in os.listdir(path) if f.endswith('.out')]
print out_files

precision = []
tiempo = []
fitness = []

comentario = re.compile(r'%.*(\n|$)')
time_reg = re.compile(r'% Time .*')
fitn_reg = re.compile(r'% Min .*')


for f in out_files:
	print path+f
	outfile = open(path+f, 'r')

	testset = []

	for o in outfile:
		if (re.match(comentario, o) is None):
			testset.append(o)
		else:
		 	if (re.match(time_reg, o) is not None):
		 		time = re.sub(r'[^\d\.]+', '', o)
				tiempo.append(float(time))	
			if (re.match(fitn_reg, o) is not None):
				fitn = re.sub(r'[^\d\.]+', '', o)
				fitness.append(float(fitn))

	(training,testing) = classify.getSimilarSubsets(trainset,testset,opts.num_clusters)
	
	prec = classify.calculatePrecision(training, testing)
	precision.append(prec)

if len(fitness) == 0:
	fitness = [0 for x in xrange(0, 10)]


print precision
print tiempo
print fitness

data = [precision, fitness, tiempo]
title = ['Precision', 'Fitness', 'Tiempo']
print pandas.DataFrame(data, title)



def average(lista):
	acum = 0.0
	for e in lista:
		acum+= e
	return acum/float(len(lista))

def best(precision, fitness, tiempo):
	best = 0.0
	for i in range(len(precision)):
		if precision[i] > best:
			best = precision[i]
			fit = fitness[i]
			t = tiempo[i]
	return (best, fit, t)

def worst(precision, fitness, tiempo):
	worst = 100.0
	for i in range(len(precision)):
		if precision[i] < worst:
			worst = precision[i]
			fit = fitness[i]
			t = tiempo[i]
	return (worst, fit, t)

		



print "\nPrec Avg: " + str(average(precision))+"%"
print "Fitness Avg: "  + str(average(fitness))
print "Tiempo Avg: "  + str(average(tiempo))+"\n"

best_res = best(precision, fitness, tiempo)
print "Best\n\tPrec: "+str(best_res[0])+"%\n\tFitness: "+str(best_res[1])+"\n\tTiempo: "+str(best_res[2])+"\n"

worst_res = worst(precision, fitness, tiempo)
print  "Worst\n\tPrec: "+str(worst_res[0])+"%\n\tFitness: "+str(worst_res[1])+"\n\tTiempo: "+str(worst_res[2])+"\n"


		


