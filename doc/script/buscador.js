pcomunes = new Array ();
cresultado = "";
function showcreditos(url)
{
  l=(window.screen.availWidth-520)/2;
  t1=(window.screen.availHeight-340)/2;
  nueva=window.open(url,"ventana","height=340,width=520,alwaysRaised,dependent,scrollbars,top="+t1+",left="+l);
  nueva.focus();
};
function showcolaboradores(url)
{
  l=(window.screen.availWidth-520)/2;
  t1=(window.screen.availHeight-340)/2;
  nueva=window.open(url,"ventana","height=340,width=520,alwaysRaised,dependent,scrollbars,top="+t1+",left="+l);
  nueva.focus();
};
function showautores(url)
{
  l=(window.screen.availWidth-520)/2;
  t1=(window.screen.availHeight-340)/2;
  nueva=window.open(url,"ventana","height=340,width=520,alwaysRaised,dependent,scrollbars,top="+t1+",left="+l);
  nueva.focus();
};
function showayuda(url)
{
  l=(window.screen.availWidth-520)/2;
  t1=(window.screen.availHeight-340)/2;
  nueva=window.open(url,"ventana","height=340,width=520,alwaysRaised,dependent,scrollbars,top="+t1+",left="+l);
  nueva.focus();
};
function actualizar_reproductor_imagen()
{
  if (imagenes.length>0)
    {
	  document.getElementById("miniatura1").src=imagenes [indice_imagen];
	  document.getElementById("aminiatura1").href=imagenes [indice_imagen];
	  document.getElementById("miniatura1").alt=descripciones [indice_imagen];
	 };
  if (imagenes.length>1)
    {
	  document.getElementById("miniatura2").src=imagenes [indice_imagen+1];
	  document.getElementById("aminiatura2").href=imagenes [indice_imagen+1];
	  document.getElementById("miniatura2").alt=descripciones [indice_imagen+1];
	};
  if (imagenes.length>2)
    {
	  document.getElementById("miniatura3").src=imagenes [indice_imagen+2];
	  document.getElementById("aminiatura3").href=imagenes [indice_imagen+2];
	  document.getElementById("miniatura3").alt=descripciones [indice_imagen+2];
	};
  if (imagenes.length) 
    {
      if (imagenes.length>2) 
	    {
		  document.getElementById("mostrando").innerHTML='Mostrando de '+ (indice_imagen+1)+' a '+ (indice_imagen+3) +' de '+ imagenes.length+' im&aacute;genes.';  
		}
	  else
		{
		  document.getElementById("mostrando").innerHTML='Mostrando de 1 a '+ imagenes.length +' de '+ imagenes.length+' im&aacute;genes.';  
		};    
	};  
};
function inicio_imagen()
{
  if (indice_imagen>0) indice_imagen=0;  
  actualizar_reproductor_imagen();   
};
function atras_imagen()
{
  if (indice_imagen>0) indice_imagen--;  
  actualizar_reproductor_imagen();  
};
function adelante_imagen()
{
  if (indice_imagen+2<imagenes.length-1) indice_imagen++;  
  actualizar_reproductor_imagen();  
};
function final_imagen()
{
  if (imagenes.length>3) indice_imagen=imagenes.length-3;
  actualizar_reproductor_imagen();
};
function max()
{
  if (estado)
    {
	  document.getElementById("externo").innerHTML=contenido;
	  estado=false;
	  document.getElementById("max").innerHTML='max';
	}
  else
    {
	  contenido=document.getElementById("externo").innerHTML;
	  document.getElementById("externo").innerHTML=document.getElementById("interno").innerHTML
	  document.getElementById("max").innerHTML='min';
	  estado=true;
	};
};
function show_descrip(descrip)
{
  document.getElementById('descrip').innerHTML='Descripci&oacute;n: '+descrip;
};
function comun(s)
{
  r=false;
  for (e=0;e<pcomunes.length;e++)
    if (s==pcomunes[e])
      r=true;
  return(r); 
};
function depurar(t)
{
  for (i=0;i<t.length;i++)
    { 
	  if ((t[i].length<3) | (comun(t[i])))
	    {
		  t.splice(i,1);
		  i--;
	    } 
	}
};

function mostrar_resultados(lista,indice,cant)
{
  path1=path;
  if (path1=="") { path1="../";} else {path1="";};
  temp=document.getElementById("texto").value;
  salida = "";
  if (!lista.length)
    {
	  cant=1;
	  indice=-1;
	  salida='<br><table width="100%" border="1" cellspacing="0" cellpadding="5"><tr><td><table width="100%" border="0" cellspacing="0" cellpadding="0"><tr><td nowrap="nowrap">Mostrando de '+(indice+1)+' a '+(indice+cant)+' de '+lista.length+' Resultados de la B&uacute;squeda:</td><td width="100%"><div align="left">&nbsp;<b>'+temp+'</b></div></td></tr><tr><td> No hay coincidencias. </td></tr></table></td></tr><tr><td align="right"><a href="javascript:cerrarbusqueda();" class="menu1_link">Cerrar</a></td></tr></table>';	}
  else
    {
	   if (indice+cant>lista.length) cant=lista.length-indice;
	   salida='<br><table width="100%" border="1" cellspacing="0" cellpadding="5"><tr><td><table width="100%" border="0" cellspacing="0" cellpadding="0"><tr><td nowrap="nowrap">Mostrando de '+(indice+1)+' a '+(indice+cant)+' de '+lista.length+' Resultados de la B&uacute;squeda:</td><td width="100%"><div align="left">&nbsp;<b>'+temp+'</b></div></td></tr><tr><td  colspan="2">';
	   if (indice>0) salida=salida+'<a href="javascript:mostrar_resultados(resultados,'+(indice-10)+',10)" class="menu1_link">Anterior</a>';
       if ((indice>0) & (indice+cant<lista.length)) salida=salida+'<span class="menu1"> - </span>';
	   if (indice+cant<lista.length) salida=salida+'<a href="javascript:mostrar_resultados(resultados,'+(indice+10)+',10)" class="menu1_link">Siguiente</a>';
	   if ((indice>0) | (indice+cant<lista.length)) salida=salida;
	   if (lista.length>10) salida=salida+'<br>';
	   salida=salida+'</td></tr><tr><td colspan="2">&nbsp;<p><table width="100%" border="0" cellspacing="0" cellpadding="0">'
       for (i=indice;((i<indice+cant) & (i<lista.length));i++)
         {
	      salida=salida+'<tr><td width="0"><img src="'+path1+'imagenes/iconos/articulo.gif" width="32" height="32" hspace="2" vspace="2"></td><td width="100%" valign="middle"><div align="justify" class="menu1_link"><a href="'+lista[i][0]+'" class="menu1_link">'+lista[i][2]+'</a></div><div align="justify"><span class="desactivado">'+lista[i][3]+'</span></div><br></td></tr>';
		  };
		salida=salida+'</table></td></tr></table></td></tr><tr><td align="right"><a href="javascript:cerrarbusqueda();" class="menu1_link">Cerrar</a></td></tr></table>';
	}; 
  if (cresultado=="") cresultado=document.getElementById("resultado").innerHTML;
  document.getElementById("resultado").innerHTML ='';
  document.getElementById("resultado").innerHTML = salida;  
};
function buscar(st)
{
  temp=st.toLowerCase();
  palabras=temp.split(" ");
  //depurar(palabras);
  resultados = new Array();
  if (palabras.length)
    for (i=0; i<t.length; i++)
      {
       	esta=false;
		lc=t[i][0]+t[i][1]+t[i][2]+t[i][3]+t[i][4]+t[i][5]+t[i][6];
		lc.toLowerCase();
		todo=true;
       	for (k=0; k<palabras.length; k++)
	  		if (lc.indexOf(palabras[k])==-1) todo=false;
		if (todo) esta=true;  
        if (esta) 
		{
			rc=t[i][3]; 
			if (path=="") rc = rc.replace('"medias','"../medias');
			resultados[resultados.length] = new Array( 	 path+t[i][0], 
													 	 t[i][1],
													     t[i][2],
													     rc 
													 );
		}
      } 
  mostrar_resultados(resultados,0,10);
};
function cerrarbusqueda()
{
	document.getElementById("resultado").innerHTML = cresultado;
}
t = new Array();
for (i=0;i<23;i++)
  t[i] = new Array();
t[0][0] = ('articulo_2.htm');
t[0][1] = ('licencia gnu gpl fsf');
t[0][2] = ('Licencia P&uacute;blica General de GNU (GNU/GPL)');
t[0][3] = ('');
t[0][4] = ('');
t[0][5] = ('licencia pública general de gnu (gnu/gpl)');
t[0][6] = ('');

t[1][0] = ('articulo_3.htm');
t[1][1] = ('');
t[1][2] = ('Apariencia');
t[1][3] = ('Describe como definir la apariencia de la aplicaci&oacute;n.');
t[1][4] = ('');
t[1][5] = ('apariencia');
t[1][6] = ('describe como definir la apariencia de la aplicación.');

t[2][0] = ('articulo_4.htm');
t[2][1] = ('licencia fsf gnu');
t[2][2] = ('Licencia de Documentaci&oacute;n Libre de GNU (GNU/FDL)');
t[2][3] = ('');
t[2][4] = ('');
t[2][5] = ('licencia de documentación libre de gnu (gnu/fdl)');
t[2][6] = ('');

t[3][0] = ('articulo_5.htm');
t[3][1] = ('autor creditos');
t[3][2] = ('Cr&eacute;ditos');
t[3][3] = ('');
t[3][4] = ('');
t[3][5] = ('créditos');
t[3][6] = ('');

t[4][0] = ('articulo_6.htm');
t[4][1] = ('nuevo');
t[4][2] = ('Nueva Enciclopedia');
t[4][3] = ('Describe c&oacute;mo se crea una nueva enciclopedia.');
t[4][4] = ('');
t[4][5] = ('nueva enciclopedia');
t[4][6] = ('describe cómo se crea una nueva enciclopedia.');

t[5][0] = ('articulo_7.htm');
t[5][1] = ('abrir enciclopedia');
t[5][2] = ('Abrir enciclopedia');
t[5][3] = ('Describe como abrir una enciclopedia creada con anterioiridad');
t[5][4] = ('');
t[5][5] = ('abrir enciclopedia');
t[5][6] = ('describe como abrir una enciclopedia creada con anterioiridad');

t[6][0] = ('articulo_8.htm');
t[6][1] = ('');
t[6][2] = ('Estad&iacute;sticas de la enciclopedia');
t[6][3] = ('Breve resumen estad&iacute;stico.');
t[6][4] = ('');
t[6][5] = ('estadísticas de la enciclopedia');
t[6][6] = ('breve resumen estadístico.');

t[7][0] = ('articulo_9.htm');
t[7][1] = ('');
t[7][2] = ('Propiedades');
t[7][3] = ('Muestra las propiedades de la enciclopedia.');
t[7][4] = ('');
t[7][5] = ('propiedades');
t[7][6] = ('muestra las propiedades de la enciclopedia.');

t[8][0] = ('articulo_10.htm');
t[8][1] = ('');
t[8][2] = ('Configuraci&oacute;n');
t[8][3] = ('Configuraci&oacute;n de la enciclopedia');
t[8][4] = ('');
t[8][5] = ('configuración');
t[8][6] = ('configuración de la enciclopedia');

t[9][0] = ('articulo_11.htm');
t[9][1] = ('');
t[9][2] = ('Actores');
t[9][3] = ('Describe c&oacute;mo insertar actores en la enciclopedia.');
t[9][4] = ('');
t[9][5] = ('actores');
t[9][6] = ('describe cómo insertar actores en la enciclopedia.');

t[10][0] = ('articulo_12.htm');
t[10][1] = ('');
t[10][2] = ('Medias');
t[10][3] = ('Describe c&oacute;mo registrar medias en la enciclopedia.');
t[10][4] = ('');
t[10][5] = ('medias');
t[10][6] = ('describe cómo registrar medias en la enciclopedia.');

t[11][0] = ('articulo_13.htm');
t[11][1] = ('');
t[11][2] = ('Usuarios');
t[11][3] = ('Crear nuevos usuarios');
t[11][4] = ('');
t[11][5] = ('usuarios');
t[11][6] = ('crear nuevos usuarios');

t[12][0] = ('articulo_14.htm');
t[12][1] = ('');
t[12][2] = ('Plantillas');
t[12][3] = ('Administraci&oacute;n de las plantillas que utiliza la aplicaci&oacute;n para el proceso de generaci&oacute;n.');
t[12][4] = ('');
t[12][5] = ('plantillas');
t[12][6] = ('administración de las plantillas que utiliza la aplicación para el proceso de generación.');

t[13][0] = ('articulo_15.htm');
t[13][1] = ('');
t[13][2] = ('Manejo de art&iacute;culos');
t[13][3] = ('Describe c&oacute;mo se hace la gesti&oacute;n de art&iacute;culos (insertar, editar, eliminar y mover) en la aplicaci&oacute;n.');
t[13][4] = ('');
t[13][5] = ('manejo de artículos');
t[13][6] = ('describe cómo se hace la gestión de artículos (insertar, editar, eliminar y mover) en la aplicación.');

t[14][0] = ('articulo_16.htm');
t[14][1] = ('temas insertar editar mover borrar');
t[14][2] = ('Manejo de temas');
t[14][3] = ('Describe c&oacute;mo gestionar (insertar, eliminar, editar, ordenar y mover) los temas relacionados en la enciclopedia.');
t[14][4] = ('');
t[14][5] = ('manejo de temas');
t[14][6] = ('describe cómo gestionar (insertar, eliminar, editar, ordenar y mover) los temas relacionados en la enciclopedia.');

t[15][0] = ('articulo_17.htm');
t[15][1] = ('buscar niveles artículos');
t[15][2] = ('Proceso de B&uacute;squeda');
t[15][3] = ('Breve explicaci&oacute;n de c&oacute;mo la aplicaci&oacute;n realiza la b&uacute;squeda de contenido.');
t[15][4] = ('');
t[15][5] = ('proceso de búsqueda');
t[15][6] = ('breve explicación de cómo la aplicación realiza la búsqueda de contenido.');

t[16][0] = ('articulo_18.htm');
t[16][1] = ('importar enciclopedias');
t[16][2] = ('Importar enciclopedias.');
t[16][3] = ('La importaci&oacute;n de enciclopedias es una &uacute;til funcionalidad que permite sobre todas las cosas el trabajo en colectivo, pues por medio de esta opci&oacute;n es posible el intercambio de contenido (enciclopedias) entre varios usuarios de Alarife IV de una manera arm&oacute;nica y total compatibilidad y complementariedad.');
t[16][4] = ('');
t[16][5] = ('importar enciclopedias.');
t[16][6] = ('la importación de enciclopedias es una útil funcionalidad que permite sobre todas las cosas el trabajo en colectivo, pues por medio de esta opción es posible el intercambio de contenido (enciclopedias) entre varios usuarios de alarife iv de una manera armónica y total compatibilidad y complementariedad.');

t[17][0] = ('articulo_19.htm');
t[17][1] = ('exportar enciclopedia');
t[17][2] = ('Exportar enciclopedias.');
t[17][3] = ('La exportaci&oacute;n de enciclopedias es una &uacute;til funcionalidad que permite no s&oacute;lo la protecci&oacute;n del proyecto (salva); sino, el trabajo en colectivo, pues por medio de esta opci&oacute;n es posible el intercambio de contenido.');
t[17][4] = ('');
t[17][5] = ('exportar enciclopedias.');
t[17][6] = ('la exportación de enciclopedias es una útil funcionalidad que permite no sólo la protección del proyecto (salva); sino, el trabajo en colectivo, pues por medio de esta opción es posible el intercambio de contenido.');

t[18][0] = ('articulo_20.htm');
t[18][1] = ('generar enciclopedia.');
t[18][2] = ('Generar enciclopedia');
t[18][3] = ('Breve descripci&oacute;n del proceso de generaci&oacute;n de enciclopedias o sitios web.');
t[18][4] = ('');
t[18][5] = ('generar enciclopedia');
t[18][6] = ('breve descripción del proceso de generación de enciclopedias o sitios web.');

t[19][0] = ('articulo_21.htm');
t[19][1] = ('edición artículos ortografía propiedades insertar medias editar');
t[19][2] = ('Editor de Alarife');
t[19][3] = ('Describe como crear un art&iacute;culo y las acciones b&aacute;sicas que se ejecutan: definir las propiedades, editarlo, guadarlo, insertar medias y emplear el corrector ortogr&aacute;fico.');
t[19][4] = ('');
t[19][5] = ('editor de alarife');
t[19][6] = ('describe como crear un artículo y las acciones básicas que se ejecutan: definir las propiedades, editarlo, guadarlo, insertar medias y emplear el corrector ortográfico.');

t[20][0] = ('articulo_22.htm');
t[20][1] = ('teclas generalidades');
t[20][2] = ('Combinaci&oacute;n de teclas &quot;Hot Key&quot;');
t[20][3] = ('');
t[20][4] = ('');
t[20][5] = ('combinación de teclas &quot;hot key&quot;');
t[20][6] = ('');

t[21][0] = ('articulo_23.htm');
t[21][1] = ('');
t[21][2] = ('Alarife por primera vez');
t[21][3] = ('Descripci&oacute;n de la interfaz del Software Alarife IV, para una mayor comprensi&oacute;n del funcionamiento de esta herramienta.');
t[21][4] = ('');
t[21][5] = ('alarife por primera vez');
t[21][6] = ('descripción de la interfaz del software alarife iv, para una mayor comprensión del funcionamiento de esta herramienta.');

t[22][0] = ('articulo_24.htm');
t[22][1] = ('demo ejemplo enciclopedia');
t[22][2] = ('Ejemplos');
t[22][3] = ('');
t[22][4] = ('');
t[22][5] = ('ejemplos');
t[22][6] = ('');


