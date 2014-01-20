function eArrayTnorm = mask( magB,f )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here

%--------------------------------------------------------------
%calculate bark from frequency vector
z = 13*atan(.00076*f) + 3.5*atan((f./7500).^2);

z = floor(z);

magB = magB(1:length(z),:);

eArray=zeros(1,25);
for i=1:256
        for bark=0:24
            if (z(i)==bark)
                eArray(bark+1)=eArray(bark+1)+magB(i);
            end
        end
end        
    
eArrayT=zeros(1,256);
for i=1:256
    for k=0:24
        if z(i)==k
            eArrayT(i)=eArray(k+1);
        end
    end
end

for bark1 = -5:5
    Be(bark1+6)=15.91+7.5*(bark1+0.474)-17.5*sqrt(1+(bark1+0.474).^2);
end

x=[-5:5];
y=10.^(Be/20);
y=y.^2;

Sm=conv(eArray,y);

SmT=conv(eArrayT,y);

SmTbis=zeros(1,256);
for i=1:256
SmTbis(i)=SmT(i+5);
end
Smbis=zeros(1,26);
for i=1:26
Smbis(i)=Sm(i+5);
end

SmArrayT=zeros(1,256);
for i=1:256
    for k=0:24
        if z(i)==k
            SmArrayT(i)=Smbis(k+1);
        end
    end
end


%----------------------------------------------------------------
%masking threshold estimate
SFM=10.*log10(((prod(eArray(:)))/((1/25)*sum(eArray(:))))^(1/25));

alpha=min((SFM/(-60)),1);

for bark2 = 0:24 
    O(bark2+1) = alpha*(14.5+bark2)+(1-alpha)*5.5;
    Traw(bark2+1)=10.^(log10(Smbis(bark2+1))-((O(bark2+1))./10));
end

eArrayTraw=zeros(1,256);
for i=1:256
    for k=0:24
        if z(i)==k
            eArrayTraw(i)=Traw(k+1);
        end
    end
end



%----------------------------------------------------------------
%Tnorm
Tnorm=zeros(1,25);
Pz=zeros(1,25);
for i=1:256
    for k=0:24
        if z(i)==k
            %eArrayTnorm(i)=Traw(k+1);
            Pz(k+1)=Pz(k+1)+1;
            Tnorm(k+1)=Traw(k+1)/Pz(k+1);
        end
    end
end
eArrayTnorm=zeros(1,256);
for i=1:256
    for k=0:24
        if z(i)==k
            eArrayTnorm(i)=Tnorm(k+1);
        end
    end
end

end

