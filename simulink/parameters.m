%cube parameters

l  = 7e-2;      %[m]
lb = 6e-2;      %[m]
mb = 222.2e-3;  %[Kg]
mw = 22.9e-3;   %[Kg]
Ib = 1.62e-3;   %[Kg m^2]
Iw = 0.05e-3;   %[Kg m^2]
Cb = 1.02e-3;   %[Kg m^2 s^-1]
Cw = 0.05e-3;   %[Kg m^2 s^-1]
g  = 9.8;       %[m/s^2]
Km = 25.1e-3;

%Jumping Up velocity

JumpingUp_vel = sqrt((2-sqrt(2))*((Iw+Ib+mw*l^2)/Iw^2)*(mb*lb+mw*l)*g)

%Linear Model Parameters

a21 = g*(mb*lb+mw*l)/(Ib+mw*l^2);
a22 = -Cb/(Ib+mw*l^2);
a23 = Cw/(Ib+mw*l^2);

a31 = -g*(mb*lb+mw*l)/(Ib+mw*l^2);
a32 = Cb/(Ib+mw*l^2);
a33 = -Cw*(Iw+Ib+mw*l^2)/(Iw*(Ib+mw*l^2));

b2 = -Km/(Ib+mw*l^2);
b3 = Km*(Iw+Ib+mw*l^2)/(Iw*(Ib+mw*l^2));

A=[0 1 0; a21 a22 a23; a31 a32 a33]
B = [0; b2; b3]
% A=[0 1; a21 a22]
% B = [0; b2]