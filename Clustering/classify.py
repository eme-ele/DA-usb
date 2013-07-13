import re
import optparse

def calculateSimilarity(setTrain,setTest):
	sorted = 0.00
	for example in setTrain:
		if example in setTest:
			sorted += 1.00
	return 100*(sorted/(len(setTrain)))

def getSubsets(set,numclusters):
	subsets = [[] for x in xrange(numclusters)]
	subsetsWithClass = [[] for x in xrange(numclusters)]

	for i in range(1,numclusters+1):
		for x in set:
			if (int(x.split(",")[-1])==i):
				example = re.sub(r',\d+\n','',x)
				subsets[i-1].append(example)
	return subsets 

def getSimilarSubsets(trainSet,testSet,numclusters):
	trainSubsets = getSubsets(trainSet,numclusters)
	testSubsets = getSubsets(testSet,numclusters)

	similarities = []

	for trainSubset in trainSubsets:
		for testSubset in testSubsets:
			similarity = calculateSimilarity(trainSubset,testSubset)
			similarities.append(similarity)

	finalSimilarity = []
	for i in range(0,numclusters):
		inicio = i*numclusters
		fin = numclusters+(i*numclusters)
		bestSimilar = -1;
		indexSimilar = -1;
		for j in range(inicio,fin):
			if(similarities[j]>= bestSimilar):
				indexSimilar = j%numclusters
				bestSimilar = similarities[j]
		finalSimilarity.append(indexSimilar)

	finalTestSubsets = [[] for x in xrange(numclusters)]
	for i in range(0,numclusters):
		index = finalSimilarity[i];
		finalTestSubsets[i] = testSubsets[index]
	return (trainSubsets,finalTestSubsets)

def calculatePrecision(training,testing):
	correct = 0
	incorrect = 0
	for (tr,ot) in zip(training, testing):
		for t in ot:
			if t in tr:
				correct += 1.00
			else:
				incorrect += 1.00

	print correct
	print incorrect

	print 100*(correct/(incorrect+correct))

def main():
	# parser for console input options
	parser = optparse.OptionParser()
	parser.add_option('-t', help='training data', type='string', dest='train_file')
	parser.add_option('-o', help='clustering output', type='string', dest='out_file')
	parser.add_option('-c', help='number of clusters', type='int', dest='numclusters')
	#parser.add_option('-l', help='label to evaluate. if missing, test all', type='string', dest='label')

	(opts, args) = parser.parse_args()

	# Making sure all options appear
	mandatories = ['train_file', 'out_file', 'numclusters']
	for m in mandatories:
		if not opts.__dict__[m]:
			print "Mandatory option is missing\n"
			parser.print_help()
			exit(-1)

	trnfile = open(opts.train_file, 'r')
	outfile = open(opts.out_file, 'r')
	numclusters = opts.numclusters

	trainset = []
	testset = []
	for (t,o) in zip(trnfile, outfile):
		trainset.append(t)
		testset.append(o)

	(training,testing) = getSimilarSubsets(trainset,testset,numclusters)

	calculatePrecision(training,testing);

if __name__ == "__main__":
	main()
