
%load('tension_hysteresis\0.5kgfirsttest.mat','angle_value','axisx','loadcell_mean','tension_value')
load('tension_hysteresis\0.5kgsecondtest.mat','angle_mean_value','axisx','loadcell_mean')
angle_value=angle_filt_value;
%data of the forearm
Lf=0.28; %length of the forearm
Lh=0; %0.10; %lenght of the hand
Dcf=0.0475;   %clamping of the forearm distance
Dca=0.0575;   %clamping of the arm distance
m_exo=0.01; %despreciable
dcmf=1; %0.682; %forearm distance in percent of the center of mass
wcmf=1; %0.022;  %forearm weight in percent
Sw=0.5;     %subject weigth
Wp=0;       %perturbance weight
g=9.8;
dp=Lf+Lh;
Wf=Sw*wcmf;

%create estructure of the gradient data
%filtered signal
fs=10;
%[a,b]=butter(4,0.25*2/fs,'low'); %design of the low passband filter
%vector=filtfilt(a,b,angle_value)*pi/180; %implementation of the filter and filtered signal
%vector=aux;
vector=angle_value;
%vector=data3;
axisx(:,1)=(1:size(vector))*0.1;
grad_state=struct('ek_1',vector(2,1),'state',[3,0,0],'gradient',0,'quantizer',3);

gradient=0;
gradient(1,1)=0;
gradient(2,1)=0;

v_speeds=0;
v_speeds(1,1)=0;
v_speeds(2,1)=0;
ek_0=vector(2,1);
vk_0=0;
t=1;            %requiere confirmación del valor
for i=3:size(vector,1)
    ek_1=ek_0;
    ek_0=vector(i,1);
    [aux_grad,grad_state]=calculate_gradient(ek_0,grad_state);
    gradient(i,1)=aux_grad;
    vk_0=gradient(i,1)*abs(ek_0-ek_1)/t;
    v_speeds(i,1)=(vk_0+v_speeds(i-1,1)+v_speeds(i-2,1))/3;
end
aux_speed_calc=diff(vector)./diff(axisx);
speed_calc=zeros(size(aux_speed_calc,1)+1,1);
speed_calc(2:end,1)=-aux_speed_calc(:,1);
aux_accel=diff(speed_calc(2:end,1))./diff(axisx(2:end,1));
accel_calc=zeros(size(aux_accel,1)+2,1);
accel_calc(3:end,1)=aux_accel(:,1);

figure
plot(axisx,vector,axisx,gradient,axisx,v_speeds,axisx,speed_calc,axisx,accel_calc)
Inertia_exo=m_exo*(Lf^2)/3;
Inertia_arm=((Lf+Lh)*dcmf)^2*Wf;
I=Inertia_exo+Inertia_arm;

%calculation of tensor
x=sqrt(Dca^2+Dcf^2-2*Dca*Dcf*cos(vector));
%angles calculations
teta_fc=asin((Dca*sin(vector))./x);
teta_ac=asin((Dcf*sin(vector))./x);
A=0.1;
B=2;
Tt=I*accel_calc./(Dcf*sin(teta_ac));
Torq_f=Wf*(Lf+Lh)*dcmf*g*sin(vector)./(Dcf*sin(teta_ac));   %torque of the forearm
Torq_p=dp*Wp./(Dcf*sin(teta_ac));
Friction=A*Wf*g*sin(vector).*speed_calc./(Dcf*sin(teta_ac));
Fs=B*Tt+Torq_f+Torq_p+Friction;
figure
plot(axisx,vector,axisx,Fs,axisx,loadcell_mean,axisx,speed_calc)
