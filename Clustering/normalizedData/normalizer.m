
% HOW TO NORMALIZE:
%      normalizer(<inputfile string name>, 
%                 <outputfile string name>, 
%                 <number of attributes to process>, 
%                 <starting index to read attributes>)
%
% EXAMPLES:  
%          normalizer('iris.data','irisNorm.data',4,1)
%          normalizer('wine.data','wineNorm.data',13,1)
%          normalizer('glass.data','glassNorm.data',9,2) (Skips first attribute)
%
% Important: Indexes in MatLab start from 1.


% Normalizes data in the input filename between 0 and 1
% Note: It does not normalize the class of the examples.
function [] = normalizer(filename,outputfile,numAttr,startIn)
    matrix = load(filename);

    attrAll = matrixAll(:,startIn:numAttr+startIn-1);

    i = 1;

    [m,n] = size(attrIris);
    matrixNormAll = zeros(m,n+1);
    
    while(i<=numAttr)
        attr = attrAll(:,i);
        normalized = normalizeMinMax(attr);
        matrixNormAll(:,i) = normalized;
        i++;
    end
    
    matrixNormAll(:,i) = matrixAll(:,numAttr+startIn);
    % disp(matrixNormAll);
    
    % Writes the matrix in a comma separated file
    % with the given outputfile name.
    dlmwrite(outputfile, matrixNormAll, 'delimiter', ',', 'precision', 4);
end



% MinMax normalization.
function vnorm = normalizeMinMax(v)
    maxv = max(v);
    minv = min(v);
    vnorm = (v-minv)/maxv;
end
