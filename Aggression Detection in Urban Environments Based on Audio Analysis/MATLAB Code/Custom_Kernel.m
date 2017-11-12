% Create either a histogram intersection kernel or a chi-squared kernel
% If useweights = 1, then different weights are given to the values coming
% from different levels in the spatial pyramid
function K = Custom_Kernel(X,Y)
    kernel = @(x,Y) sum(bsxfun(@min,x,Y),2);
    K = pdist2(X,Y,kernel);
end