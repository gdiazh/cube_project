%{%%
 % @brief Continuously plot file data
 %}
 
% Author: Gustavo Diaz

close all;
clc

cube_angle = 0;
cube_vel = 0;
torque_input = 0;
output_pwm = 1400;
output_dir = 0;

figure(1)
fig1 = axes;
plot(cube_angle, 'b')
grid on
hold on
plot(cube_vel, 'r')
plot(torque_input, 'g')
ylim([-250 270])
set(gca,'fontsize',14)
title('Prueba de control con posición inicial 0°', 'fontsize', 14)
xlabel('Tiempo [s]', 'fontsize', 14); ylabel('Variables de estado', 'fontsize', 14);
% legend('Ángulo del Cubo [°]', 'Velocidad del Cubo [°/s]', 'Torque aplicado [Nm]')

figure(2)
fig2 = axes;
% subplot(2,1,1)
plot(output_pwm, 'r')
grid on
set(gca,'fontsize',14)
title('Señal PWM para el motor BLDC', 'fontsize', 14)
xlabel('Tiempo [s]', 'fontsize', 14); ylabel('Ciclo de trabajo [ms]', 'fontsize', 14);
ylim([1394 1415])

figure(3)
fig3 = axes;
% subplot(2,1,2)
plot(output_dir, 'b')
grid on
set(gca,'fontsize',14)
title('Señal de cambio de giro para el motor BLDC', 'fontsize', 14)
xlabel('Tiempo [s]', 'fontsize', 14); ylabel('Valor Lógico', 'fontsize', 14);
ylim([-.5 1.5])

while 1
	data = dlmread('data.txt');
	cube_angle = data(:,1);
	cube_vel = data(:,2);
	torque_input = data(:,3);
	output_pwm = data(:,4);
	output_dir = data(:,5);

	plot(fig1, cube_angle, 'b')
	grid on
	hold on
	plot(fig1, cube_vel, 'r')
	plot(fig1, torque_input, 'g')

	% figure(2)
	% subplot(2,1,1)
	plot(fig2, output_pwm, 'r')
	grid on

	% subplot(2,1,2)
	plot(fig3, output_dir, 'b')
	grid on
	pause(1)
end