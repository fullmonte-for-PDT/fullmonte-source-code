fid =fopen('mouse.mesh', 'rt');
nodeCount = str2double({fgetl(fid)});
elemCount = str2double({fgetl(fid)});

node = textscan(fid, '%f %f %f', nodeCount);
node = cell2mat(node);
elem = textscan(fid, '%f %f %f %f %f', elemCount);
elem = cell2mat(elem);

save 'Mouse_Mesh.mat' node elem;