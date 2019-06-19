<?
//Contador de Páginas
//Autor: Cristiano Cabral
//Data: 04/07/2003
//Arquivo: paginacao.inc
$url = str_replace('/simec/','',$_SERVER['REQUEST_URI']);
$posicao = strpos($url, '&numero=');
if ($posicao>0) $url = substr($url,0,$posicao);
//Registros por página (instanciado na chamada)
//$perpage = 15;
//Paginas que serão mostradas na barrinha (instanciado na chamada)
//$pages = 20;
//Total de Registros (instanciado na chamada)
//$total_reg = $nlinhas;

if (fmod($total_reg, $perpage)==0) $total_reg = $total_reg - 1;

//Página atual
$pag_atu = intval(($numero-1)/$perpage)+1;

//Página Max
$pag_max = intval(($total_reg+$perpage)/$perpage);

//Pagina Inicial
if ($pag_max<$pages) $p_ini = 1; else $p_ini = (intval(($pag_atu-1)/$pages)*$pages)+1;

//Pagina Final
if  ($pag_max > ($p_ini+$pages)) $p_fim = $p_ini + $pages - 1; else $p_fim = $pag_max;

//Montando a Barrrinha de Navegação
if ($total_reg>$perpage)
{
	print "<div class=\"row text-right view-table-pagination\">Páginas: ";
	if (intval($numero)>$perpage) print "<a href=\"javascript:pagina(".(intval($numero)-$perpage).");\" title=\"Página Anterior\">«</a>";
		for($n = $p_ini; $n < $p_fim+1; $n++)
		{
			if (($n*$perpage)>=$total_reg) $reg_fim = $nlinhas; else $reg_fim = $n*$perpage;
			$alt_txt=  "De ".($n*$perpage-$perpage+1)." até ".$reg_fim;
			if ($n == $pag_atu) print"<div class=\"view-table-pagination-current\">".$n."</div>";
			else print "<a href=\"javascript:pagina(".($n*$perpage-$perpage+1).");\" title=\"".$alt_txt."\">".$n."</a>";
		}
	if (intval($numero)<($total_reg-$perpage+1)) print "<a href=\"javascript:pagina(".(intval($numero)+$perpage).");\" title=\"Próxima Página\">»</a>";
	print "</div>";
}
?>