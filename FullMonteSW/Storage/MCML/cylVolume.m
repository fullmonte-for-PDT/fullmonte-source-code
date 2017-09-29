% Calculates the volume of each cylindrical slice in an MCML output
%
% for i=1..N    slice runs from r ( i-1, i ) with volume pi*dz*dr^2*(2i-1)
%
% Ev = cylVolume(dr,dz,M,Npkt);
% 
% Arguments
%   dr      Radial sample spacing
%   dz      Depth sample spacing
%   M       Matrix of results M(r,z)
%   Npkt    Number of packets launched
%
% Returns
%   Ev      Energy per unit volume

function Ev = cylVolume(dr,dz,M,Npkt);

[r,c] = size(M);

i = (1:r)';
V = (2*i-1)*pi*dr^2*dz;

Ev = M ./ repmat(V,[1 c]) / Npkt;
