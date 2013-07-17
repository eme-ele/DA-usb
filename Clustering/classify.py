import re

def calculateSimilarity(setTrain,setTest):
	 sorted = 0.00
	 for example in setTest:
		if example in setTrain:
			sorted += 1.00
	 return sorted	

def getSubsets(conjunto,numclusters):
	subsets = [[] for x in xrange(0,numclusters)]
	for i in range(1,numclusters+1):
		for x in conjunto:
			var = int(x.split(",")[-1])
			if (var==i):
				example = re.sub(r',\d+\n','',x)
				subsets[i-1].append(x[:-3])
	return subsets 
	 
def getFinalSimilarity(similarities,numclusters):
	 finalSimilarity = []
	 used = []
	 
	 for i in range(0,numclusters):
		 inicio = i*numclusters
		 fin = numclusters+(i*numclusters)
		 bestSimilar = -1;
		 indexSimilar = -1;
		 for j in range(inicio,fin):
			 if(similarities[j] > bestSimilar and j%numclusters not in used):
				 indexSimilar = j%numclusters
				 bestSimilar = similarities[j]
		 finalSimilarity.append(indexSimilar)
		 used.append(indexSimilar)
	 
	 return finalSimilarity
				 

def getSimilarSubsets(trainSet,testSet,numclusters):
	 trainSubsets = getSubsets(trainSet,numclusters)
	 testSubsets = getSubsets(testSet,numclusters)
	 
	 similarities = []

	 for i in xrange(0,numclusters):
		 for j in xrange(0,numclusters):
	 		 similarity = calculateSimilarity(trainSubsets[i],testSubsets[j])
			 similarities.append(similarity)
			 
	 
	 finalSimilarity = getFinalSimilarity(similarities,numclusters)


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

	 return 100*(correct/(incorrect+correct))


