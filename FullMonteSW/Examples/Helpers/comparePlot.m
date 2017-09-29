function comparePlot(varargin)
    if nargin ~= 2
        error('Incorrect number of arguments');
    end
    
    filename = varargin{1};
    figureName = varargin{2};
    fid = fopen(filename, 'rt');
    temp = textscan(fid, '%f %f %f');
    fclose(fid);

    energyMat = cell2mat(temp);
    energyMMCVector = energyMat(:,2);
    energyFMVector = energyMat(:,3);

    a = figure('visible','off');
    loglog(energyMMCVector, energyFMVector, '.', 'markersize', 1e-100);
    xlabel('MMC result');
    ylabel('FM result');
    axis([1.e-15 1.e-2 1.e-15 1.e-2]);
    saveas(a, strcat('./Result/comparison_plot_',figureName,'.png'));
    close(a);
end
