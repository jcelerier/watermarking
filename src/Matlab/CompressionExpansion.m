clear all
close all

[word, fs] = wavread('/Users/marcqueirossoares/Documents/watermarking/output/input_mono.wav');

nfft = 512;
window = diag(hanning(512)');
windowInv=diag(1./hanning(512)');
Noverlap = nfft/2;

%Segment in frames and overlap--------------------------------------------

% Y = frame(X=word,W=nfft,H=Noverlap)
% Return Y as a set of columns as W-point segments of X stepped
% by H. 

lx = length(word);
nh = 1+ceil((lx-nfft)/Noverlap);
% Pad X to an integral number of windows (add 0)
Xp = [word(:)',zeros(1, (nfft+(nh-1)*Noverlap)-lx)];

% Index-fu:
% We build a matrix of indices that pull out the values we want...
% (columns of 1:W, with successive multiples of H added on)
winix = repmat(Noverlap*[0:(nh-1)],nfft,1)+repmat([1:nfft]',1,nh);
% A=repmat(Noverlap*[0:(nh-1)],nfft,1);
% C=repmat([1:nfft]',1,nh);
% D=A+C;
% .. then the output is just the input matrix indexed by these indices
Y = Xp(winix);
Ybis=reshape(Y,1,[]);

%Apply hanning window-----------------------------------------------------
Yf=window*Y;

Yfreshaped=reshape(Yf,1,[]);
figure 
plot(Yfreshaped)

%Apply dct----------------------------------------------------------------
Ydct=dct(Yf);

%calculate mask-----------------------------------------------------------
window2 = hamming(512);
[B,f] =spectrogram(word,window2,Noverlap,nfft,fs);

for i=1:343
magB(:,i)=zeros(257,1);  
magB(:,i)=abs(B(:,i));
magB(:,i)=(magB(:,i)).^2;

Masking(:,i)=zeros(256,1);
Masking(:,i)=mask(magB(:,i),f);
end

%compression expansion between first two frames---------------------------

V1=Ydct(:,1)';
V1=Ydct(1:504);
vec=Ydct(505:512);%8 last coefficients under mask
V2=[vec,Ydct(:,2)'];

Ydct2=Ydct(:,[3:344]);

Ydct2R=reshape(Ydct2,1,[]);

YdctEnc=[V1,V2,Ydct2R];

%retour temporel----------------------------------------------------------
V1inv=idct(V1);
V2inv=idct(V2);

Yinv=idct(Ydct2);

Yinv=reshape(Yinv,1,[]);
Yinv=[V1inv,V2inv,Yinv];
figure
plot(Yinv)

YinvN=idct(Ydct);
YinvN=windowInv*YinvN;

diff=Yfreshaped-Yinv;
figure
plot(diff)

% Retrouver signal original
% for i=1:2:343
%     wordInv(:,i)=YinvN(:,i); 
% end
% k=0;
% for i=1:172
%     wordInv2(:,i)=wordInv(:,i+k);
%     k=k+1;
% end
% wordInv2=reshape(wordInv2,1,[]);
% wordInv2=[wordInv2,YinvN([257:512],344)'];
% wordInv2=wordInv2(1:88200);
% figure
% plot(wordInv2)

%decodage-----------------------------------------------------------------
%x=xcorr(signalDiamonds,triangle);


