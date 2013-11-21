Y = load('../Y.debug');
Z = linkage(Y, 'ward', 'euclidean');
dlmwrite('../Z.matlab.debug', Z, 'delimiter', ' ');
exit(0);