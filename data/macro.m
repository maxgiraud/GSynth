duration = 20;
sample_rate = 44100;
step = 1/sample_rate;
t = [0:step:duration]';
rd = rand(size(t));
data = sin(2*pi*t.*(1000+10*sin(2*pi*t*100)))+sin(2*pi*t.*(1000-10*sin(2*pi*t*100)));
out = [t data];
save -ascii data/func.dat out
