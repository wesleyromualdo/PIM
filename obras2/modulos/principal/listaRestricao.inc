<?php
    require APPRAIZ . 'obras2/includes/principal/listaRestricao/ctrl.php';

if($_GET['acao'] != 'L'){

switch ( $_REQUEST['requisicao'] ){
    case 'apagar': 
        $restricao = new Restricao( $_REQUEST['rstid'] );
            
        $restricao->rststatus = 'I';        
        $restricao->salvar();
        
        if ( ($restricao->usucpfsuperacao || $restricao->rsqid || $restricao->qtsid) && possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC) ) ){
            
            $procedencia = ( $restricao->rsqid || $restricao->qtsid ) ? 'Questionário (cadastro automático)' : 'Cadastro Manual';
            $superado    = ( $restricao->usucpfsuperacao ) ? 'Sim' : 'Não';
            $dtSuperado  = ( $restricao->rstdtsuperacao ) ? 'Em: ' . formata_data( $restricao->rstdtsuperacao ) : '';
            $item        = ( $restricao->rstitem == 'R' ) ? 'Restrição' : 'Inconformidade';
            
            $faseRestricao = new FaseRestricao( $restricao->fsrid );
            $tipoRestricao = new TipoRestricao( $restricao->tprid );
            
            $regAtividade                 = new RegistroAtividade();
            $arDado                       = array();
            $arDado['obrid']              = $obrid;
            $arDado['usucpf']             = $_SESSION['usucpf'];
            $arDado['rgaautomatica']      = 't';
            $arDado['rgadscsimplificada'] = 'RESTRIÇÃO E/OU INCONFORMIDADE APAGADA';
            $arDado['rgadsccompleta']     = "CPF: {$_SESSION['usucpf']}
                                         Quando: " . date('d/m/Y') . "
                                         Procedência: {$procedencia}
                                         Item: {$item}
                                         Situação da Obra na Ocorrência: {$faseRestricao->fsrdsc}
                                         Tipo: {$tipoRestricao->tprdsc}
                                         Descrição: {$restricao->rstdsc}
                                         Previsão da Providência: " . formata_data( $restricao->rstdtprevisaoregularizacao ) . "
                                         Providência: {$restricao->rstdscprovidencia}
                                         Superado: {$superado} {$dtSuperado}";

            $regAtividade->popularDadosObjeto( $arDado )
                         ->salvar(); 
                                
        }elseif ( ($restricao->usucpfsuperacao || $restricao->rsqid || $restricao->qtsid) ){
            $db->rollback();
            die("<script>
                    alert('Operação não realizada!\n Usuário sem permissão para realização da operação.'); 
                    window.location = '?modulo=principal/listaRestricao&acao={$_GET['acao']}';
                </script>");
        }
        
        $db->commit();
        die("<script>
                alert('Operação realizada com sucesso!'); 
                window.location = '?modulo=principal/listaRestricao&acao={$_GET['acao']}';
            </script>");
}


if( $_GET['acao'] != 'V' ){
    include  APPRAIZ."includes/cabecalho.inc";
    echo "<br>";
    if ( $_GET['acao'] == 'A' ){
        // empreendimento || obra || orgao
        verificaSessao( 'empreendimento' );
        
        $db->cria_aba(ID_ABA_EMP_CADASTRADO,$url,$parametros);
        
        $empreendimento = new Empreendimento( $empid );
        $empreendimento->montaCabecalho();
        
    }else{
        // empreendimento || obra || orgao
        verificaSessao( 'obra' );
        
        if( $_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ){
            $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros);
        }else{
            $db->cria_aba(ID_ABA_OBRA_CADASTRADA,$url,$parametros);
        }
        
        echo cabecalhoObra($obrid);
                
    }
    
    $habilitado = true;
    $habilita   = 'S';
    
}
else{

    verificaSessao( 'obra' );
?>  
    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <?php
    $db->cria_aba($abacod_tela,$url,$parametros);
    echo cabecalhoObra($obrid);

    if(possui_perfil(array(PFLCOD_EMPRESA_VISTORIADORA_FISCAL, PFLCOD_EMPRESA_VISTORIADORA_GESTOR))){
            $habilitado = true;
        }else{
            $habilitado = false;
        }
    $habilita   = 'N';
    $habilitada_insercao = false;
}


    if( possui_perfil( array(PFLCOD_CONSULTA_UNIDADE, PFLCOD_CONSULTA_ESTADUAL, PFLCOD_CALL_CENTER, PFLCOD_CONSULTA_TIPO_DE_ENSINO) ) ){
        $habilitado = false;
        $habilita = 'N';
    }


if( possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC) ) ){
    $habilitada_insercao = true;

}



monta_titulo($titulo_modulo, '<div class="row" style="background: #b94a48; color: #fff"><img src="/imagens/atencao.png" /> 
                Somente as RESTRIÇÕES não superadas impedem o repasse de recursos. Inconformidades, caso não sejam superadas, serão analisadas na
                 prestação de contas ao final da obra. A superação das inconformidades está condicionada à correção, conforme projeto pactuado com o
                  FNDE.</div>');



?>
        

        
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
    <tr>
        <td colspan="2" align="center">
            <?php if($habilitada_insercao && $habilitado && !possui_perfil( array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_GESTOR_UNIDADE, PFLCOD_EMPRESA_MI_FISCAL, PFLCOD_EMPRESA_MI_GESTOR))): ?>
                <input type="button" 
                       value="Inserir Nova Restrição e Inconformidade" 
                       style="padding: 1px; padding-left: 20px; padding-right: 20px; margin-top:3px; margin-bottom:3px;" 
                       onclick="janela = window.open('?modulo=principal/cadRestricao&acao=<?php echo $_GET['acao'] ?>', 'inserirRestricao', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=1050,height=550' ); janela.focus();"/>
            <?php endif; ?>
        </td>
    </tr>
    <tr><td colspan="2" style="text-align: center"></td></tr>
    <tr>
        <td style="text-align: left; width: 70px">
            <span><b>Providência</b> : </span>
        </td>
        <td style="text-align: left">
            <span><img style=" vertical-align:middle" src="/imagens/0_alerta.png" title="Aguardando Providência ou Aguardando Correção" /> Aguardando Providência ou Aguardando Correção | </span>
            <span><img style=" vertical-align:middle" src="/imagens/0_inativo.png" title="Aguardando Correção ou Aguardando Correção há mais de 15 dias" /> Aguardando Providência ou Aguardando Correção há mais de 15 dias | </span>
            <span><img style=" vertical-align:middle" src="/imagens/0_ativo.png" title="Aguardando Análise FNDE"/> Aguardando Análise FNDE | </span>
            <span><img style=" vertical-align:middle" src="/imagens/0_concluido.png" title="Superada" /> Superada | </span>
            <span><img style=" vertical-align:middle" src="/imagens/0_concluido_2.png" title="Justificada" /> Justificada | </span>
            <span><img style=" vertical-align:middle" src="/imagens/0_inexistente.png" title="Cancelada" /> Cancelada </span>
        </td>
    </tr>
</table>
        
<?php
$restricao = new Restricao();
if ( $_GET['acao'] == 'A' ){
	$param = array('empid' => $_SESSION['obras2']['empid']);
}else{
	$param = array('obrid' => $_SESSION['obras2']['obrid']);
}

if ( $habilitado == false ){
	$param['block_imgs_acao'] = true;
}else{
	$param['block_imgs_acao'] = false;    
}

$sql 	   = $restricao->listaSql( $param );


$cabecalho = array("Ação", "Providência", "ID Item", "Item", "Fase","Tipo", "Data da Inclusão", "Descrição", "Providência", "Previsão da Providência", "Criado Por", "Superação", "Situação Atual", "Superado Por", "Ressalva", "Último Tramite", "Data do Último Tramite");
$db->monta_lista($sql,$cabecalho,100,5,'N','center','N', 'N');

?>
<script type="text/javascript">
<!--
function abreHistoricoRest( rstid ){
	janela = window.open('?modulo=principal/popUpHistoricoRest&acao=A&rstid=' + rstid, 'Histórico Restrição', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=1050,height=550' );
	janela.focus();
}

function alterarRest( rstid ){
	janela = window.open('?modulo=principal/cadRestricao&acao=<?php echo $_GET['acao'] ?>&rstid=' + rstid, 'inserirRestricao', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=1050,height=550' ); 
	janela.focus();
}


function excluirRest( rstid ){
        <?
        $objObras = new Obras();
        $objObras->carregarPorIdCache($obrid);
        
        $blockEdicao = $objObras->verificaObraVinculada();
        if($blockEdicao){
            echo 'var blockEdicao = true;';
        }else{
            echo 'var blockEdicao = false;';
        }
        ?>
                
	if ( confirm('Deseja apagar esta restrição?') ){
                if(blockEdicao){
                    alert('Você não pode editar os dados da Obra Vinculada.');
                }else{
                    location.href = '?modulo=principal/listaRestricao&acao=<?php echo $_GET['acao'] ?>&rstid=' + rstid + '&requisicao=apagar';
                }
	}
}
//-->
</script>

<?php

//end if $_GET['acao'] != 'L'
}
elseif ($_GET['acao'] == 'L'){
    
    if($_GET['novoDoc'] == true){
       atualizaDocidNullRestricao($_GET['rstid']); 
    }
    
    ?>
        
    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
        
    <?php
    
    include  APPRAIZ."includes/cabecalho.inc";

    $titulo_modulo    = 'Lista de Restrições e Inconformidades';
    $subtitulo_modulo = 'Pesquisa dos registros de todas as Obras';
    monta_titulo_obras($titulo_modulo, $subtitulo_modulo);
    


    //Fluxo de Restrição/Inconformidade
    $tpdid = TPDID_RESTRICAO_INCONFORMIDADE;
    
    ?>
    
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
    <script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
    <script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>
   
<form name="formulario" id="formulario" action="" method="post">

    <input type="hidden" name="form" value="1" /> 
    <input type="hidden" name="tipo_relatorio" id="tipo_relatorio" value="" /> 

    <div class="form-filters">
        <div class="row">
            <div class="col-lg-2">
                Nome da Obra / ID:
            </div>
            <div class="col-lg-10">
            <?php 
                $val = (!empty($_POST['obrbuscatexto'])) ? $_POST['obrbuscatexto'] : '';
                echo campo_texto('obrbuscatexto', 'N', 'S', '', 70, 80, '', '', '', '', '', 'id="obrbuscatexto"', '', $val); 
            ?>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                ID da Restrição/Inconformidade:
            </div>
            <div class="col-lg-10">
            <?php
                $val = (!empty($_POST['rstid'])) ? $_POST['rstid'] : '';
                echo campo_texto('rstid', 'N', 'S', '', 70, 80, '', '', '', '', '', 'id="rstid"', '', $val);
            ?>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Tipologia:
            </div>
            <div class="col-lg-10">
                <select name="tpoid[]" id="tpoid" class="chosen-select" multiple data-placeholder="Selecione">
                <?php   $tipologiaObra = new TipologiaObra();
                    $param = array("orgid" => $_SESSION['obras2']['orgid']);
                    $dados = $tipologiaObra->listaCombo($param, false);
                        foreach ($dados as $key) {
                ?>
                        <option value="<?php echo $key['codigo'] ?>" <?php if ($tpoid && in_array($key['codigo'], $tpoid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                <?php } ?>
                </select>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Item:
            </div>
            <div class="col-lg-10">
                <div class="btn-group" data-toggle="buttons">
                    <label class="btn btn-default">
                        <input type="radio" name="item_restrict" value="R" <?= ($_POST["item_restrict"] == "R" ? "checked" : "") ?>> Restrição
                    </label>
                    <label class="btn btn-default">
                        <input type="radio" name="item_restrict" value="I" <?= ($_POST["item_restrict"] == "I" ? "checked" : "") ?>> Inconformidade
                    </label>
                    <label class="btn btn-default <?= ($_POST['item_restrict'] == "" || $_POST['item_restrict'] == "T" ? "active" : "") ?>">
                        <input type="radio" name="item_restrict" value="T" <?= ($_POST["item_restrict"] == "T" ? "checked" : "") ?>> Todos
                    </label>
                </div>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Tipo:
            </div>
            <div class="col-lg-10">
                <select name="tprid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    $tipoRetricao = new TipoRestricao();
                    foreach ($tipoRetricao->listaCombo() as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['tprid']) && in_array($key['codigo'], $_POST['tprid'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Situação da Restrição/Inconformidade:
            </div>
            <div class="col-lg-10">
                <?php

                $dados = $db->carregar(getSqlEstadoDoc(TPDID_RESTRICAO_INCONFORMIDADE));
                $dados = $dados ? $dados : array();
                ?>
                <select name="esdid_ri[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    foreach ($dados as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['esdid_ri']) && in_array($key['codigo'], $_POST['esdid_ri'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                        <?php } ?>
                </select>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Situação da Obra:
            </div>
            <div class="col-lg-10">
                <?php
                $sql_obr = "";

                $dados = $db->carregar(getSqlEstadoDoc(TPDID_OBJETO));
                $dados = $dados ? $dados : array();
                ?>
                <select name="strid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    foreach ($dados as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['strid']) && in_array($key['codigo'], $_POST['strid'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Municípios:
            </div>
            <div class="col-lg-10">
                <select name="muncod[]" class="chosen-select municipios" multiple data-placeholder="Selecione">
                    <?php
                    $municipio = new Municipio();
                    foreach ($municipio->listaComboMulti() as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['muncod']) && in_array($key['codigo'], $_POST['muncod'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Programa:
            </div>
            <div class="col-lg-10">
                <select name="prfid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php  $programa = new ProgramaFonte();
                    $param = array("orgid" => $_SESSION['obras2']['orgid']);
                    foreach ($programa->listacombo($param, false) as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['prfid']) && in_array($key['codigo'], $_POST['prfid'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Fonte:
            </div>
            <div class="col-lg-10">
                <select name="tooid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php  $tipoOrigemObra = new TipoOrigemObra();
                    $param = array();
                    foreach ($tipoOrigemObra->listaCombo(true, $param, false) as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['tooid']) && in_array($key['codigo'], $_POST['tooid'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Data de Cadastro:
            </div>
            <div class="col-lg-10">
                <?php 
                    $data_de  = '';
                    $data_ate = '';
                ?>
                de: <input type="text" id="rstdtinclusao_de" name="rstdtinclusao_de" value="<?php echo $data_de;?>" size="15" maxlength="10" class="normal" > 
                
                 até: <input type="text" id="rstdtinclusao_ate" name="rstdtinclusao_ate" value="<?php echo $data_ate;?>" size="15" maxlength="10" class="normal">
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Data de Superação:
            </div>
            <div class="col-lg-10">
                <?php 
                    $data_de  = '';
                    $data_ate = '';
                ?>
                de: <input type="text" id="rstdtsuperacao_de" name="rstdtsuperacao_de" value="<?php echo $data_de;?>" size="15" maxlength="10" class="normal"> 
                 
                até: <input type="text" id="rstdtsuperacao_ate" name="rstdtsuperacao_ate" value="<?php echo $data_ate;?>" size="15" maxlength="10" class="normal">
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Data do Último Tramite:
            </div>
            <div class="col-lg-10">
                <?php
                    $ultimotramite_de  = $_REQUEST['ultimotramite_de'];
                    $ultimotramite_ate = $_REQUEST['ultimotramite_ate'];
                ?>
                de: <input type="text" id="ultimotramite_de" name="ultimotramite_de" value="<?php echo $ultimotramite_de;?>" size="15" maxlength="10" class="normal">
                 
                até: <input type="text" id="ultimotramite_ate" name="ultimotramite_ate" value="<?php echo $ultimotramite_ate;?>" size="15" maxlength="10" class="normal">
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Existe Ressalva:
            </div>
            <div class="col-lg-10">
                <div class="btn-group" data-toggle="buttons">
                    <label class="btn btn-default">
                        <input type="radio" name="rstflressalva" value="S" <?= ($_POST["rstflressalva"] == "S" ? "checked" : "") ?>> Sim
                    </label>
                    <label class="btn btn-default">
                        <input type="radio" name="rstflressalva" value="N" <?= ($_POST["rstflressalva"] == "N" ? "checked" : "") ?>> Não
                    </label>
                    <label class="btn btn-default <?= ($_POST['rstflressalva'] == "" || $_POST['rstflressalva'] == "T" ? "active" : "") ?>">
                        <input type="radio" name="rstflressalva" value="T" <?= ($_POST["rstflressalva"] == "T" ? "checked" : "") ?>> Todos
                    </label>
                </div>
            </div>
        </div>
    </div>

    <div class="row form-filters text-center">
        <div class="col">
            <button type="button" name="btn_filtrar" id="btn_filtrar" class="btn btn-success" onclick="javascript:getLista('visual');">
                <span class="glyphicon glyphicon-search"></span> Filtrar
            </button>
            <button type="button" name="btn_excel" id="btn_excel" class="btn btn-primary" onclick="javascript:getLista('xls');">
                <span class="glyphicon glyphicon-download-alt"></span> Gerar Excel
            </button>
        </div>
    </div>
</form>

<div class="row form-filters text-center">
    <div class="col-lg-12">
        <b>Providência: </b>
        <span><img style=" vertical-align:middle" src="/imagens/0_alerta.png" title="Aguardando Providência ou Aguardando Correção" /> Aguardando Providência ou Aguardando Correção | </span>
        <span><img style=" vertical-align:middle" src="/imagens/0_inativo.png" title="Aguardando Correção ou Aguardando Correção há mais de 15 dias" /> Aguardando Providência ou Aguardando Correção há mais de 15 dias | </span>
        <span><img style=" vertical-align:middle" src="/imagens/0_ativo.png" title="Aguardando Análise FNDE"/> Aguardando Análise FNDE | </span>
        <span><img style=" vertical-align:middle" src="/imagens/0_concluido.png" title="Superada" /> Superada | </span>
        <span><img style=" vertical-align:middle" src="/imagens/0_concluido_2.png" title="Justificada" /> Justificada | </span>
        <span><img style=" vertical-align:middle" src="/imagens/0_inexistente.png" title="Cancelada" /> Cancelada </span>
    </div>
</div>



    
<?php

if($_REQUEST['form'] == '1' || ( ($_GET['acao'] == 'L') AND possui_perfil(array(PFLCOD_GESTOR_UNIDADE, PFLCOD_SUPERVISOR_UNIDADE)) ) ){
    montaLista($_REQUEST['tipo_relatorio']);
}


}// END - elseif ($_GET['acao'] == 'L')

require APPRAIZ . 'obras2/includes/principal/listaRestricao/js.php';



    
    


?>