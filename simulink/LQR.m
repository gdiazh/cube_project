%% Controlador LQR
% A = [0 1 0; 84.4950 -0.5888 0.0289; -84.4950 0.5888 -1.0289];
% B = [0 577 20577]';
C = [1 0 0; 0 1 0; 0 0 1];
Q = [200 0 0; 0 300 0; 0 0 300];
D = [0; 0; 0];
% C = [1 0; 0 1];
% Q = [200 0; 0 300];
% D = [0; 0];
R = 1;
K = lqr(A,B,Q,R);

% Ac = (A-B*K);
% Bc = B;
% Cc = C;
% Dc = D;
% X0 = [0;4*pi/3;0;0];
% states = {'x' 'theta' 'x_dot' 'theta_dot'};
% inputs = {'r'};
% outputs = {'x','theta'};

% sys_cl = ss(Ac,Bc,Cc,Dc,'statename',states,'inputname',inputs,'outputname',outputs);
% sys_tf =tf(sys_cl)

% t = 0:0.01:5;
% r =10*ones(size(t));

% [y,t,x]=lsim(sys_cl,r,t,X0);
% %plot(t,y(:,1))
% y(:,2) = 180/pi.*y(:,2);

% [AX,H1,H2] = plotyy(t,y(:,1),t,y(:,2),'plot');
% xlabel('Time [s]')
% set(get(AX(1),'Ylabel'),'String','cart position (m)')
% set(get(AX(2),'Ylabel'),'String','pendulum angle (grades)')

% title('Step Response with LQR Control with initial condition 4pi/3')