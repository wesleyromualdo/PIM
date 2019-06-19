<?php
//echo 'bla';DIE;
ob_start();


include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

if( $_REQUEST['acao_download'] == 'downloadAnexo'){
            
    $arqid      = $_REQUEST['arqid'];
    $campos = array();

    if ($arqid != null) {
        $file = new FilesSimec('assessoramento_historico',$campos, 'sase');
        $ok = $file->getDownloadArquivo($arqid);
        if($ok === false){
            echo "<script>
                    setTimeout(function(){ 
                        window.location.href = 'sase.php?modulo=principal/assessoramento&acao=A&aba=lista'; }, 40)
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
include_once '../../sase/classes/Assessoramento.class.inc';
include_once '../../sase/classes/SituacaoAssessoramento.class.inc';
include_once '../../sase/classes/Model/Assessoramentohistorico.class.inc';


$assessoramentoHistorico = new Sase_Assessoramento_Historico();

if ($_REQUEST['acao']){
    switch($_REQUEST['acao']){
        case 'salva_arquivo':
            if(empty($_REQUEST['assid'])){
                echo "<script>
                        setTimeout(function(){ 
                            alert('Aviso:Selecione um Município na aba Lista.');
                            window.location.href = 'sase.php?modulo=principal/assessoramento&acao=A'; }, 4)
                     </script>";
                exit();
            }
            if (($_FILES['filepne']['size'] > 0) || ($_FILES['fileadesao']['size'] > 0) ){
                $Assessoramento = new Assessoramento();
                $Assessoramento->carregarPorId($_REQUEST['assid']);
                $file              = new FilesSimec("assessoramento", $campos, 'sase');
                $assid             = $_REQUEST['assid'];
                $descricaopne      = explode(".", $_FILES['filepne']['name']);
                $descricaoadesao   = explode(".", $_FILES['fileadesao']['name']);
                $campos            = array();
                
                if($Assessoramento->arAtributos['assleipne'] != null || $Assessoramento->arAtributos['assadesao'] != null){
                    
                    $lei     = ($_FILES['filepne']['size'] > 0) ? "assleipne = null" : '';
                    $termo   = ($_FILES['fileadesao']['size'] > 0) ? "assadesao = null" : '';
                    $virgula = (($_FILES['filepne']['size'] > 0) && ($_FILES['fileadesao']['size'] > 0)) ? ',' : '';
                    
                    $sql = "update sase.assessoramento set
                            {$lei}
                            {$virgula}
                            {$termo}
                            where assid = {$assid}";
                    $res = $db->executar($sql);
                    
                    $sqlHistorico = "INSERT INTO sase.assessoramento_historico 
                                    (hascpf, hasacao, hasdtacao, usucpf, assadesao,assleipne,docid,stacod,muncod,estuf,assid)
                                VALUES ('{$_SESSION['usucpf']}',
                                       'UPDATE',
                                       '" . date('Y-m-d H:m:s') . "',
                                       '".($Assessoramento->arAtributos['usucpf'] ? $Assessoramento->arAtributos['usucpf'] : $_SESSION['usucpf'])."',
                                       ".($Assessoramento->arAtributos['assadesao'] ? $Assessoramento->arAtributos['assadesao'] : 'NULL').",
                                       ".($Assessoramento->arAtributos['assleipne'] ? $Assessoramento->arAtributos['assleipne'] : 'NULL').",
                                       '{$Assessoramento->arAtributos['docid']}',
                                       '{$Assessoramento->arAtributos['stacod']}',
                                       '{$Assessoramento->arAtributos['muncod']}',
                                       '{$Assessoramento->arAtributos['estuf']}',
                                       '{$assid}')";
                    $db->executar($sqlHistorico);
                    
                    //não irá excluir do public.arquivo, permitindo download do arquivo.
//                    $file->setPulaTableEschema(true);
//                    if($lei){
//                        $file->setRemoveUpload($Assessoramento->arAtributos['assleipne']);
//                    }
//                    if($termo){
//                        $file->setRemoveUpload($Assessoramento->arAtributos['assadesao']);
//                    }
                }

                $i = 0;
                if($_FILES['filepne']['size'] > 0){
                    $filepne     = new FilesSimec("assessoramento", $campos, 'sase');
                    $nomeArquivo = $filepne->setUpload($_FILES['filepne']['name'], 'filepne', false);
                    $assleipne   = $filepne->getIdArquivo();
                    if($nomeArquivo) {
                        $sqlpne = "update sase.assessoramento set
                                  assleipne = {$assleipne}
                                where assid = {$assid}";
                        $respne = $db->executar($sqlpne);
                        $db->commit();
                        $i++;
                    }
                }

                if($_FILES['fileadesao']['size'] > 0){
                    $fileadesao  = new FilesSimec("assessoramento", $campos, 'sase');
                    $nomeArquivo = $fileadesao->setUpload($_FILES['fileadesao']['name'], 'fileadesao', false);
                    $assadesao   = $fileadesao->getIdArquivo();
                    if($nomeArquivo) {
                        $sqladesao = "update sase.assessoramento set
                                  assadesao = {$assadesao}
                                where assid = {$assid}";
                        $resadesao = $db->executar($sqladesao);
                        $db->commit();
                        $i++;
                    }
                    
                }

                if($i > 0){
                    echo <<<HTML
                    <script>
                        alert('Arquivos salvo com sucesso.');
                        window.location.href = 'sase.php?modulo=principal/assessoramento&acao=A&aba=edicao&assid={$assid}';
                    </script>
HTML;
                }
            }
            break;

        case 'apaga_arquivo':
            $Assessoramento = new Assessoramento();
            $Assessoramento->carregarPorId($_REQUEST['assid']);
            
            if($Assessoramento->arAtributos['assleipne'] != null || $Assessoramento->arAtributos['assadesao'] != null ){
                $asslei    = ($_REQUEST['arqidpne'] != null) ? "assleipne = null" : '';
                $assatermo = ($_REQUEST['arqidadesao'] != null) ? "assadesao = null" : '';
                $virgula   = (($_REQUEST['arqidpne'] != null) && ($_REQUEST['arqidadesao'] != null)) ? ',' : '';
                $assid     = $_REQUEST['assid'];
                $sql = "update sase.assessoramento set
                            {$asslei}
                            {$virgula}
                            {$assatermo}
                        where assid = {$assid}";
                $res = $db->executar($sql);
                $db->commit();
            }
            if($_REQUEST['arqidpne'] != null){
//                $campos = array();
//                $file   = new FilesSimec("assessoramento", $campos, 'sase');
//                $file->setPulaTableEschema(true);
//                $file->setRemoveUpload($_REQUEST['arqidpne']);
                $sqlHistorico = "INSERT INTO sase.assessoramento_historico 
                                    (hascpf, hasacao, hasdtacao, usucpf, assadesao,assleipne,docid,stacod,muncod,estuf,assid)
                                VALUES ('{$_SESSION['usucpf']}',
                                       'UPDATE',
                                       '" . date('Y-m-d H:m:s') . "',
                                       '" . ($Assessoramento->arAtributos['usucpf'] ? $Assessoramento->arAtributos['usucpf'] : $_SESSION['usucpf']) . "',
                                        " . ($Assessoramento->arAtributos['assadesao'] ? $Assessoramento->arAtributos['assadesao'] : 'NULL') . ",
                                        " . ($Assessoramento->arAtributos['assleipne'] ? $Assessoramento->arAtributos['assleipne'] : 'NULL') . ",
                                       '{$Assessoramento->arAtributos['docid']}',
                                       '{$Assessoramento->arAtributos['stacod']}',
                                       '{$Assessoramento->arAtributos['muncod']}',
                                       '{$Assessoramento->arAtributos['estuf']}',
                                       '{$assid}')";
                $db->executar($sqlHistorico);
                echo <<<HTML
                    <script>
                        alert('Arquivo Lei PNE apagado com sucesso.');
                        window.location.href = 'sase.php?modulo=principal/assessoramento&acao=A&aba=edicao&assid={$assid}';
                    </script>
HTML;
            }
            if($_REQUEST['arqidadesao'] != null){
//                $campos = array();
//                $file   = new FilesSimec("assessoramento", $campos, 'sase');
//                $file->setPulaTableEschema(true);
//                $file->setRemoveUpload($_REQUEST['arqidadesao']);
                                $sqlHistorico = "INSERT INTO sase.assessoramento_historico 
                                    (hascpf, hasacao, hasdtacao, usucpf, assadesao,assleipne,docid,stacod,muncod,estuf,assid)
                                VALUES ('{$_SESSION['usucpf']}',
                                       'UPDATE',
                                       '" . date('Y-m-d H:m:s') . "',
                                       '" . ($Assessoramento->arAtributos['usucpf'] ? $Assessoramento->arAtributos['usucpf'] : $_SESSION['usucpf']) . "',
                                        " . ($Assessoramento->arAtributos['assadesao'] ? $Assessoramento->arAtributos['assadesao'] : 'NULL') . ",
                                        " . ($Assessoramento->arAtributos['assleipne'] ? $Assessoramento->arAtributos['assleipne'] : 'NULL') . ",
                                       '{$Assessoramento->arAtributos['docid']}',
                                       '{$Assessoramento->arAtributos['stacod']}',
                                       '{$Assessoramento->arAtributos['muncod']}',
                                       '{$Assessoramento->arAtributos['estuf']}',
                                       '{$assid}')";
                $db->executar($sqlHistorico);
                echo <<<HTML
                    <script>
                        alert('Arquivo Termo de Adesão apagado com sucesso.');
                        window.location.href = 'sase.php?modulo=principal/assessoramento&acao=A&aba=edicao&assid={$assid}';
                    </script>
HTML;
            }
            break;

        case 'download_arquivo':
            //$Assessoramento->carregarPorId($_REQUEST['assid']);
            $campos = array();
            $file   = new FilesSimec("assessoramento", $campos, 'sase');
            if($_REQUEST['arqidpne'] != null){
                $file->getDownloadArquivo($_REQUEST['arqidpne']);
            }
            if($_REQUEST['arqidadesao'] != null){
                $file->getDownloadArquivo($_REQUEST['arqidadesao']);
            }
        break;

        case 'download_zip':
            ob_clean();
            $zip = new ZipArchive();
            $zname = date('d-m-Y H-i-s').".zip";
            $res = $zip->open($zname, ZipArchive::CREATE);

            $sql = "select assleipne, arq.arqdescricao from sase.assessoramento ass inner join public.arquivo arq on ass.assleipne = arq.arqid where assleipne is not null";
            $arqs = $db->carregar($sql);
            $i = 1;
            foreach ($arqs as $arq) {
                $caminho = APPRAIZ . 'arquivos/sase/' . floor($arq['assleipne'] / 1000) . '/' . $arq['assleipne'];
                if(file_exists($caminho)){
                    $zip->addFromString(basename($caminho), file_get_contents($caminho));
                    $zip->renameName($arq['assleipne'], $arq['arqdescricao']);
                }
                $i++;
            }
//            ver(filesize($zname), d);
            $zip->close();
            header('Content-Type: application/zip');
            header('Content-disposition: attachment; filename='.$zname);
            readfile($zname);
            exit(0);
            
        case 'historico':
            $cabecalho = array("Data/Hora","CPF","Usuário","Lei PNE","Termo Adesão");
            $alinhamento = array('left','left','left','left','left');
            $larguras = array('20%','20%','20%','20%','20%');
            $sql = $assessoramentoHistorico->montaListaHistorico( $_POST['assid'],trim($_POST['estuf']),trim($_POST['muncod']) );
            $db->monta_lista($sql,$cabecalho,30,5,'N','','N','listaAssessoramentoHistorico',$larguras,$alinhamento);
            die;
        break;
        
        
            
    
    }
}

include APPRAIZ . 'includes/cabecalho.inc';
// include APPRAIZ . 'www/sase/_funcoes.php';
// --

$perfis = pegaPerfilGeral( $_SESSION['usucpf'] );

// --------------- Decisão da Página
$pagina = "";
if( !$_GET['aba'] || $_GET['aba'] == 'lista' ) $pagina = "listaassessoramento";
if( $_GET['aba'] == 'edicao' ) $pagina = "edicaoassessoramento";
if( $_GET['aba'] == 'mapa' ) $pagina = "mapa";
// --

if( array_search(PFLCOD_SASE_CONSULTA, $perfis) !== false
	&& array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false )
	$pagina = 'mapa';

// --------------- Cabeçalho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino.";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação.";
 $titulo = 'Assistência Técnica <br/> Município';
//$titulo = 'Assistência Técnica';
 
?>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>

<div id='cabecalhoSistema'>
	<?='<h6><small>'.implode('</small></h6><h6><small>', $cabecalhoSistema).'</small></h6>'?>
	<center><h3><?=$titulo?></h3></center>
</div>

<div id="menu_sistema">
	<ul class="nav nav-tabs">
		<?php if( (array_search(PFLCOD_SASE_CONSULTA, $perfis) !== false
				  && array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false) ){ }else{ ?>

	  		<li <?=($pagina=='listaassessoramento')?'class="active"':''?>><a href="sase.php?modulo=principal/assessoramento&acao=A&aba=lista">Lista</a></li>

	  	<?php } ?>

        <?php if( (array_search(PFLCOD_SASE_CONSULTA, $perfis) !== false
                && array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false) || (array_search(PFLCOD_SASE_CONSULTA_LEI_PEE_PME, $perfis)) ){ }else{ ?>
	  	
	  		<li <?=($pagina=='edicaoassessoramento')?'class="active"':''?>><a href="sase.php?modulo=principal/assessoramento&acao=A&aba=edicao">Assistência</a></li>
	  		
	  	<?php } ?>

        <?php if (!array_search(PFLCOD_SASE_CONSULTA_LEI_PEE_PME, $perfis)){ ?>
	  	<li <?=($pagina=='mapa')?'class="active"':''?>><a href="sase.php?modulo=principal/assessoramento&acao=A&aba=mapa">Mapa</a></li>
        <?php } ?>
	</ul>
</div><?php
// --

// --------------- Inclusão da Página
include APPRAIZ . "sase/modulos/principal/assessoramento/" . $pagina . ".inc";
// --

