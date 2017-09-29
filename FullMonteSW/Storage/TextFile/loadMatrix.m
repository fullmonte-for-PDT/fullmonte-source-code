% Loads an N-D matrix from a text file
%
% File format requires 
%
% <D> <dim0> <dim1> <...>
% <values>
%
% File is presumed to be stored in row-major order, ie. M[1][1] M[1][2] .. M[1][N] M[2][1]

function M = loadMatrix(fn)

fid = fopen(fn,'r');
if (fid == -1)
    error "Failed to open file"
end

D = fscanf(fid,'%d',1);
dim = fscanf(fid,'%d ',[D(1);1]);

printf('%d-D matrix\n');

for i=2:length(dim)
end

% read the values in order
T = fscanf(fid,'%f',prod(dim));
fclose(fid);

if (D == 1)
    M = T;
elseif (D == 2)
    M = reshape(T,dim)';
end
