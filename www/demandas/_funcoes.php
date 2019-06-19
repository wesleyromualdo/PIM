<?php 
function alignLeft($txt)
{
    return "<div style=\"text-align:left;\">{$txt}</div>";
}


function criaAbaTabelaApoio()
{
    global $db, $simec;
    
    $moduloparts = explode("/", $_GET['modulo']);
    
    $aba[] = array("descricao" => "Pesquisar", "link" => "demandas.php?modulo=".$moduloparts[0]."/listar");
    
    if($moduloparts[1]=='cadastrar') $aba[] = array("descricao" => "Cadastrar", "link" => "demandas.php?modulo=".$moduloparts[0]."/cadastrar");
    
    echo $simec->tab ($aba, "demandas.php?modulo=".$_GET['modulo']);
    
    
}


function criaAbaListaDemandantes()
{
    global $db;
    
    $sql = "select ef.enfid, ef.enfdsc from corporativo.entidades_funcao ef 
                inner join demandas.entidades_funcaoxpim fx on ef.enfid = fx.enfid 
            where enfstatus = 'A'";
    $arrDados = $db->carregar($sql);
    
    $abasPar = array();
    foreach ($arrDados as $v) {
        array_push($abasPar, array("descricao" => $v['enfdsc'], "link" => "demandas.php?modulo=demandantes/listar&acao=A&enfid=".$v['enfid']));
    }
    return $abasPar;
}

function criaAbaDemanda( $ddtid )
{    
    $abasDemanda = array();
    array_push($abasDemanda, array("descricao" => 'Lista', "link" => "demandas.php?modulo=demandantes/listarDemanda&acao=A&ddtid=".$ddtid));
    array_push($abasDemanda, array("descricao" => 'Cadastro', "link" => "demandas.php?modulo=demandantes/cadastarDemanda&acao=A&ddtid=".$ddtid));
        
    return $abasDemanda;
}

function montaCabecalhoDemanda( $ddtid ){
    
    $arrArquivoDemanda = array('demandantes/listarDemanda', 'demandantes/cadastarDemanda');
    
    echo '<div class="row">
        	<div class="col-md-2">
            	<button class="btn btn-outline btn-success dim btn-block '.($_REQUEST['modulo'] == 'demandantes/verPainel' ? 'active' : '').'" type="button" 
                    style="height: 50px;" onclick="javascript: window.location.href=\'demandas.php?modulo=demandantes/verPainel&acao=A&ddtid='.$ddtid.'\'"><i class="fa fa-line-chart"></i> Painel</button>
            </div>
            <div class="col-md-2">
                <button class="btn btn-outline btn-info dim btn-block '.($_REQUEST['modulo'] == 'demandantes/verDadosDemandante' ? 'active' : '').'" type="button" 
                    style="height: 50px;" onclick="javascript: window.location.href=\'demandas.php?modulo=demandantes/verDadosDemandante&acao=A&ddtid='.$ddtid.'\'"><i class="fa fa-user"></i> Demandante</button>
            </div>
            <div class="col-md-2">
                <button class="btn btn-outline btn-primary dim btn-block '.( in_array($_REQUEST['modulo'], $arrArquivoDemanda) ? 'active' : '').'" type="button" 
                    style="height: 50px;" onclick="javascript: window.location.href=\'demandas.php?modulo=demandantes/listarDemanda&acao=A&ddtid='.$ddtid.'\'"><i class="fa fa-bar-chart-o"></i> Demandar</button>
            </div>
            <div class="col-md-2">
                <button class="btn btn-outline btn-warning dim btn-block '.($_REQUEST['modulo'] == 'demandantes/analisar' ? 'active' : '').'" type="button" style="height: 50px;"><i class="fa fa-bolt"></i> Analisar</button>
            </div>
            <div class="col-md-2">
                <button class="btn btn-outline btn-success dim btn-block '.($_REQUEST['modulo'] == 'demandantes/emendas' ? 'active' : '').'" type="button" style="height: 50px;"><i class="fa fa-mortar-board"></i> Emendas</button>
            </div>
            <div class="col-md-2">
                <button class="btn btn-outline btn-danger dim btn-block '.($_REQUEST['modulo'] == 'demandantes/obras' ? 'active' : '').'" type="button" style="height: 50px;"><i class="fa fa-institution"></i> Obras</button>
            </div>
        </div>';
}

function formata_numero_cnpj_cpf($str)
{
    global $db;
    
    $sql = "select public.formata_cpf_cnpj('".$str."')";
    return $db->pegaUm($sql);
    
}
?>