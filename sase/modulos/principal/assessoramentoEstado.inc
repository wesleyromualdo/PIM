<?php

ob_start();

include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

if($_REQUEST['acao_download'] ==  'downloadAnexo'){
    $arqid  = $_REQUEST['arqid'];
    $campos = array();

    if ($arqid != null) {
        $file = new FilesSimec('assessoramentoestado_historico',$campos, 'sase');
        $ok = $file->getDownloadArquivo($arqid);
        if($ok === false){
            echo "<script>
                    setTimeout(function(){ 
                        window.location.href = 'sase.php?modulo=principal/assessoramentoEstado&acao=A&aba=lista'; }, 40)
                 </script>";
        exit();
        }
    }
    die;
}
// --------------- Dependências
include_once '../../sase/classes/Mapa/MetaDados.class.inc';
include_once '../../sase/classes/Mapa/Poligonos.class.inc';
include_once '../../sase/classes/Mapa/Mapas.class.inc';
include_once '../../sase/classes/AssessoramentoEstado.class.inc';
include_once '../../sase/classes/SituacaoAssessoramento.class.inc';
include_once '../../sase/classes/Model/Assessoramentoestadohistorico.class.inc';

$assessoramentoHistorico = new Sase_AssessoramentoEstado_Historico();

if ($_REQUEST['acao']){
    switch($_REQUEST['acao']){
        case 'salva_arquivo':
            if(empty($_REQUEST['aseid'])){
                echo "<script>
                        setTimeout(function(){ 
                                        alert('Aviso:Selecione um Estado na aba Lista.');
                                        window.location.href = 'sase.php?modulo=principal/assessoramentoEstado&acao=A'; }, 4)
                     </script>";
                exit();
            }
            if (($_FILES['filepne']['size'] > 0) || ($_FILES['fileadesao']['size'] > 0)){
                $AssessoramentoEstado = new AssessoramentoEstado();
                $AssessoramentoEstado->carregarPorId($_REQUEST['aseid']);
                $aseid           = $_REQUEST['aseid'];
                $nomeArquivo     = $_REQUEST['aiddsc'];
                $descricaopne    = explode(".", $_FILES['filepne']['name']);
                $descricaoadesao = explode(".", $_FILES['fileadesao']['name']);
                $campos          = array();
                $file            = new FilesSimec("assessoramentoEstado", $campos, 'sase');
                
                if($AssessoramentoEstado->arAtributos['aseleipne'] != null || $AssessoramentoEstado->arAtributos['aseadesao'] != null){
                    $lei     = ($_FILES['filepne']['size'] > 0) ? "aseleipne = null" : '';
                    $termo   = ($_FILES['fileadesao']['size'] > 0) ? "aseadesao = null" : '';
                    $virgula = (($_FILES['filepne']['size'] > 0) && ($_FILES['fileadesao']['size'] > 0)) ? ',' : '';
                    $sql = "update sase.assessoramentoEstado set
                            {$lei}
                            {$virgula}
                            {$termo}
                            where aseid = {$aseid}";
                    $res = $db->executar($sql);
                                        
                    $sqlHistorico = "INSERT INTO sase.assessoramentoestado_historico 
                                        (hascpf, hasacao, hasdtacao, usucpf, aseadesao,aseleipne,docid,stacod,estuf,aseid)
                                    VALUES ('{$_SESSION['usucpf']}',
                                        'UPDATE',
                                        '" . date('Y-m-d H:m:s') . "',
                                        '" . ($AssessoramentoEstado->arAtributos['usucpf'] ? $AssessoramentoEstado->arAtributos['usucpf'] : $_SESSION['usucpf']) . "',
                                         " . ($AssessoramentoEstado->arAtributos['aseadesao'] ? $AssessoramentoEstado->arAtributos['aseadesao'] : 'NULL') . ",
                                         " . ($AssessoramentoEstado->arAtributos['aseleipne'] ? $AssessoramentoEstado->arAtributos['aseleipne'] : 'NULL') . ",
                                        '{$AssessoramentoEstado->arAtributos['docid']}',
                                        '{$AssessoramentoEstado->arAtributos['stacod']}',
                                        '{$AssessoramentoEstado->arAtributos['estuf']}',
                                        '{$aseid}')";
                    $db->executar($sqlHistorico);
                    //não irá excluir do public.arquivo, permitindo download do arquivo.
//                    $file->setPulaTableEschema(true);
//                    if($lei){
//                        $file->setRemoveUpload($AssessoramentoEstado->arAtributos['aseleipne']);
//                    }
//                    if($termo){
//                        $file->setRemoveUpload($AssessoramentoEstado->arAtributos['aseadesao']);
//                    }
                }
                $i = 0;
                if($_FILES['filepne']['size'] > 0){
                    $filepne     = new FilesSimec("assessoramento", $campos, 'sase');
                    $nomeArquivo = $filepne->setUpload($_FILES['filepne']['name'], 'filepne', false);
                    $aseleipne   = $filepne->getIdArquivo();
                        if($nomeArquivo) {
                            $sql = "update sase.assessoramentoEstado set
                                    aseleipne = {$aseleipne}
                                    where aseid = {$aseid}";
                            $res = $db->executar($sql);
                            $db->commit();
                            $i++;
                        }
                }
                if($_FILES['fileadesao']['size'] > 0){
                    $fileadesao  = new FilesSimec("assessoramento", $campos, 'sase');
                    $nomeArquivo = $fileadesao->setUpload($_FILES['fileadesao']['name'], 'fileadesao', false);
                    $aseadesao   = $fileadesao->getIdArquivo();
                        if($nomeArquivo) {
                            $sql = "update sase.assessoramentoEstado set
                                    aseadesao = {$aseadesao}
                                    where aseid = {$aseid}";
                            $res = $db->executar($sql);
                            $db->commit();
                            $i++;
                        }
                }
                if($i > 0){
                    echo <<<HTML
                    <script>
                        alert('Arquivo salvo com sucesso.');
                        window.location.href = 'sase.php?modulo=principal/assessoramentoEstado&acao=A&aba=edicao&aseid={$aseid}';
                    </script>
HTML;
                }
            }
            break;

        case 'apaga_arquivo':
            $AssessoramentoEstado = new AssessoramentoEstado();
            $AssessoramentoEstado->carregarPorId($_REQUEST['aseid']);
            
            if($AssessoramentoEstado->arAtributos['aseleipne'] != null || $AssessoramentoEstado->arAtributos['aseadesao'] != null){
                $file        = new FilesSimec("assessoramentoEstado", $campos, 'sase');
                $file->setPulaTableEschema(true);
                $campos    = array();
                $aselei    = ($_REQUEST['arqidpne'] != null) ? "aseleipne = null" : '';
                $assatermo = ($_REQUEST['arqidadesao'] != null) ? "aseadesao = null" : '';
                $virgula   = (($_REQUEST['arqidpne'] != null) && ($_REQUEST['arqidadesao'] != null)) ? ',' : '';
                $aseid     = $_REQUEST['aseid'];
                $sql       = "update sase.assessoramentoEstado set
                                    {$aselei}
                                    {$virgula}
                                    {$assatermo}
                              where aseid = {$aseid}";
                $res = $db->executar($sql);
                $db->commit();
                
                if($_REQUEST['arqidpne'] != null){
//                  $file->setRemoveUpload($_REQUEST['arqidpne']);
                    $sqlHistorico = "INSERT INTO sase.assessoramentoestado_historico 
                                        (hascpf, hasacao, hasdtacao, usucpf, aseadesao,aseleipne,docid,stacod,estuf,aseid)
                                    VALUES ('{$_SESSION['usucpf']}',
                                        'UPDATE',
                                        '" . date('Y-m-d H:m:s') . "',
                                        '" . ($AssessoramentoEstado->arAtributos['usucpf'] ? $AssessoramentoEstado->arAtributos['usucpf'] : $_SESSION['usucpf']) . "',
                                         " . ($AssessoramentoEstado->arAtributos['aseadesao'] ? $AssessoramentoEstado->arAtributos['aseadesao'] : 'NULL') . ",
                                         " . ($AssessoramentoEstado->arAtributos['aseleipne'] ? $AssessoramentoEstado->arAtributos['aseleipne'] : 'NULL') . ",
                                        '{$AssessoramentoEstado->arAtributos['docid']}',
                                        '{$AssessoramentoEstado->arAtributos['stacod']}',
                                        '{$AssessoramentoEstado->arAtributos['estuf']}',
                                        '{$aseid}')";
                    $db->executar($sqlHistorico);
                    echo <<<HTML
                        <script>
                            alert('Arquivo Lei PNE apagado com sucesso.');
                            window.location.href = 'sase.php?modulo=principal/assessoramentoEstado&acao=A&aba=edicao&aseid={$aseid}';
                        </script>
HTML;
                }
                if($_REQUEST['arqidadesao'] != null){
                   // $file->setRemoveUpload($_REQUEST['arqidadesao']);
                    $sqlHistorico = "INSERT INTO sase.assessoramentoestado_historico 
                                        (hascpf, hasacao, hasdtacao, usucpf, aseadesao,aseleipne,docid,stacod,estuf,aseid)
                                    VALUES ('{$_SESSION['usucpf']}',
                                        'UPDATE',
                                        '" . date('Y-m-d H:m:s') . "',
                                        '" . ($AssessoramentoEstado->arAtributos['usucpf'] ? $AssessoramentoEstado->arAtributos['usucpf'] : $_SESSION['usucpf']) . "',
                                         " . ($AssessoramentoEstado->arAtributos['aseadesao'] ? $AssessoramentoEstado->arAtributos['aseadesao'] : 'NULL') . ",
                                         " . ($AssessoramentoEstado->arAtributos['aseleipne'] ? $AssessoramentoEstado->arAtributos['aseleipne'] : 'NULL') . ",
                                        '{$AssessoramentoEstado->arAtributos['docid']}',
                                        '{$AssessoramentoEstado->arAtributos['stacod']}',
                                        '{$AssessoramentoEstado->arAtributos['estuf']}',
                                        '{$aseid}')";
                    $db->executar($sqlHistorico);
                    echo <<<HTML
                        <script>
                            alert('Arquivo Termo de Adesão apagado com sucesso.');
                            window.location.href = 'sase.php?modulo=principal/assessoramentoEstado&acao=A&aba=edicao&aseid={$aseid}';
                        </script>
HTML;
                }
                
            }
        break;

        case 'download_arquivo':
            //$Assessoramento->carregarPorId($_REQUEST['aseid']);
            $campos = array();
            $file   = new FilesSimec("assessoramento", $campos, 'sase');
            if($_REQUEST['arqidpne'] != null){
                $file->getDownloadArquivo($_REQUEST['arqidpne']);
                
            }
            if($_REQUEST['arqidadesao'] != null){
                $file->getDownloadArquivo($_REQUEST['arqidadesao']);
                
            }
            die;
        break;
        
        case 'historico':
            $cabecalho = array("Data/Hora","CPF","Usuário","Lei PNE","Termo Adesão");
            $alinhamento = array('left','left','left','left','left');
            $larguras = array('20%','20%','20%','20%','20%');
            $sql = $assessoramentoHistorico->montaListaHistorico( $_POST['aseid'],trim($_POST['estuf']) );
            $db->monta_lista($sql,$cabecalho,30,5,'N','','N','listaAssessoramentoHistorico',$larguras,$alinhamento);
            die;
        break;
        
    }
}

include APPRAIZ . 'includes/cabecalho.inc';
// include APPRAIZ . 'www/sase/_funcoes.php';
// --


// --------------- Decisão da Página
$pagina = "";
if( !$_GET['aba'] || $_GET['aba'] == 'lista' ) $pagina = "listaassessoramentoEstado";
if( $_GET['aba'] == 'edicao' ) $pagina = "edicaoassessoramentoEstado";
if( $_GET['aba'] == 'mapa' ) $pagina = "mapaEstado";
// --

$perfis = pegaPerfilGeral( $_SESSION['usucpf'] );

if( in_array(PFLCOD_SASE_CONSULTA, $perfis) ){
	$pagina = 'mapaEstado';
}

// --------------- Cabeçalho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação";
// $titulo = 'Assessoramento Municipal.';
$titulo = 'Assistência Técnica <br/> Estado';
//$titulo = 'Assessoria Estado';

?>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>
<script>
jQuery("#loading").hide();
</script>
<div id='cabecalhoSistema'>
	<?='<h6><small>'.implode('</small></h6><h6><small>', $cabecalhoSistema).'</small></h6>'?>
	<center><h3><?=$titulo?></h3></center>
</div>

<div id="menu_sistema">
	<ul class="nav nav-tabs">
	  <?php if( !in_array(PFLCOD_SASE_CONSULTA, $perfis) ){ ?>
		  <li <?=($pagina=='listaassessoramentoEstado')?'class="active"':''?>><a href="sase.php?modulo=principal/assessoramentoEstado&acao=A&aba=lista">Lista</a></li>
		  <!-- <li <?=($pagina=='edicaoassessoramento')?'class="active"':''?>><a href="sase.php?modulo=principal/assessoramento&acao=A&aba=edicao">Assessoramento</a></li> -->
          <?php if(!in_array(PFLCOD_SASE_CONSULTA_LEI_PEE_PME, $perfis)){ ?>
		  <li <?=($pagina=='edicaoassessoramentoEstado')?'class="active"':''?>><a href="sase.php?modulo=principal/assessoramentoEstado&acao=A&aba=edicao">Assistência</a></li>
          <?php } ?>
	  <?}?>
    <?php if(!in_array(PFLCOD_SASE_CONSULTA_LEI_PEE_PME, $perfis)){ ?>
	  <li <?=($pagina=='mapaEstado')?'class="active"':''?>><a href="sase.php?modulo=principal/assessoramentoEstado&acao=A&aba=mapa">Mapa</a></li>
    <?php } ?>
	</ul>
</div><?php
// --

// --------------- Inclusão da Página
include APPRAIZ . "sase/modulos/principal/assessoramento/" . $pagina . ".inc";
// --

