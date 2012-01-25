function [w, x] = bow( b, h, L, E, P, samples )

    %bow Summary of this function goes here
    %   Detailed explanation goes here

    I = (b*h^3)/12; %Tröghetsmoment
    x = 0:L/samples:L; 
    x1 = x(1:size(x,2)/2);
    x2 = x(size(x,2)/2: size(x,2));

    w1 = -(P.*x1 .*(4.*x1.^2 - 3.*L.^2))/(48*E*I);
    w2 = (P*(x2-L).*(L^2 - 8*L.*x2 + 4.*x2.^2))/(48*E*I);

    w = [w1 w2]
    w = w - max(w)

end

