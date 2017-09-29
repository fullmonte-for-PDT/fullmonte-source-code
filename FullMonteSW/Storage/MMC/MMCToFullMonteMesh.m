addpath('../../matlab/'); 

load Digimouse_Mesh_1L.mat
fileID = fopen('digimouse.mesh', 'w');
fprintf(fileID, '%d\r\n', length(node));
fprintf(fileID, '%d\r\n', length(elem));
fprintf(fileID, '%3.6f %3.6f %3.6f\r\n', transpose(node));
fprintf(fileID, '%d %d %d %d %d\r\n', round(transpose(elem)));
fclose(fileID);