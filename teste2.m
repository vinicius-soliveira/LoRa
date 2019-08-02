clear all
delete(instrfindall);  % deletando portas seriais abertas no Matlab
s = instrfind('Port','COM43'); %instanciando a porta serial COM31

if isempty(s) %verificando a disponibilidade da porta escolhida, caso livre configura
s = serial('COM43');
set(s,'BaudRate', 9600);
set(s,'DataBits', 8);
set(s,'StopBits', 1);
set(s,'Parity', 'none');
set(s, 'Timeout', 12);
set(s, 'FlowControl', 'hardware');
set(s, 'InputBufferSize', 512);
else
    fclose(s);
    s = s(1);
end

fopen(s);

%configurando a imagem dos gráfico
figureHandle1 = figure('NumberTitle','off','Name','Curva de Temperatura','Color',[1 1 1],'Visible','off');

% configurando os eixos
axesHandle1 = axes('Parent',figureHandle1,'YGrid','on','YColor',[0 0 0],'XGrid','on','XColor',[0 0 0],'Color',[1 1 1]);

hold on
plotHandle1 = plot(axesHandle1,NaN,NaN,'LineWidth',1,'Color',[1 0 0]);
plotHandle2 = plot(axesHandle1,NaN,NaN,'LineWidth',1,'Color',[0 1 0]);
plotHandle3 = plot(axesHandle1,NaN,NaN,'LineWidth',1,'Color',[0 0 1]);
plotHandle4 = plot(axesHandle1,NaN,NaN,'LineWidth',1,'Color',[0 0 0]);

grid on

xlabel('Time','FontWeight','bold','FontSize',14,'Color',[0 0 0]);

ylabel('Temperature(°C)','FontWeight','bold','FontSize',14,'Color',[0 0 0]);

%title('Curvas de Temperatura','FontSize',15,'Color',[0 0 0]);

legend({'bme', '100 mm','75 mm','50 mm'} ,'FontSize',8,'FontWeight','bold');

while(1)
 pause(2);
 
 dado=fscanf(s);

 if dado(1)=='#'
 temp100 = str2double(dado(2:6));
 temp75 = str2double(dado(8:12));
 temp50 = str2double(dado(14:18));
 bme = str2double(dado(20:24));
 
 enviar = [bme temp100, temp75, temp50]; %dados a serem enviados
 dlmwrite('calibre.csv', enviar, '-append');%criando arquivo .csv
  time =now;
  
  set(plotHandle1,'XData', [get(plotHandle1, 'XData'), time],'YData', [get(plotHandle1, 'YData'),bme]);
  set(plotHandle2,'XData', [get(plotHandle2, 'XData'), time],'YData', [get(plotHandle2, 'YData'),temp100]);
  set(plotHandle3,'XData', [get(plotHandle3, 'XData'), time],'YData', [get(plotHandle3, 'YData'),temp75]);
  set(plotHandle4,'XData', [get(plotHandle4, 'XData'), time],'YData', [get(plotHandle4, 'YData'),temp50]);
  datetick(axesHandle1,'x',13);
  set(figureHandle1,'Visible','on');
  dado = 0;
 end
end

