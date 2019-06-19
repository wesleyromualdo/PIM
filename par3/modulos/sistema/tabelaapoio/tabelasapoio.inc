<?php
if (!$_REQUEST['menu']) {
    $_REQUEST['menu'] = 'descricao';
}
//ver($_REQUEST,d);
//$acessoCPF = array('98143760430','13665252806','00707786126','35918489134', '05646593638', '04133589173', '10163112657', '78151597100','04315043184', '00158596145');
$arrPerfil = pegaPerfils($_SESSION['usucpf']);
$boVisualiza = false;
if( in_array(PAR3_PERFIL_SUPER_USUARIO_DESENVOLVEDOR, $arrPerfil) ){
    $boVisualiza = true;
}


if ($_REQUEST['carregarDesbloqueioJudicial'] == 'S') {
    $sql = "SELECT dejid, inuid, usucpf, to_char(dejdatalimite , 'DD/MM/YYYY' ) as dejdatalimite, dejcomentario, arqid
  			FROM par3.desbloqueio_juridico where dejid = {$_REQUEST['dejid']}";
    
    $arDados = $db->pegaLinha($sql);
    
    $arInstrumento = $db->pegaLinha("select i.estuf, i.muncod, i.itrid from par3.instrumentounidade i where inuid = {$arDados['inuid']}");

    $arqid = $arDados['arqid'];
    $estuf_s = $arInstrumento['estuf'];
    $muncod_s = $arInstrumento['muncod'];
    $itrid_s = $arInstrumento['itrid'];
    $pepdatalimite_s = $arDados['dejdatalimite'];
    $comentario = $arDados['dejcomentario'];
    
    $nomeAnexo = '';
    if( $arqid ){
        $file = new FilesSimec();
        $nomeAnexo = $file->getNomeArquivo($arqid);
    }
    $dados = "{\"nomeAnexo\": \"$nomeAnexo\", \"arqid\": \"$arqid\", \"estuf_s\": \"$estuf_s\", \"muncod_s\" : \"$muncod_s\", \"itrid_s\" : \"$itrid_s\", \"pepdatalimite_s\" : \"$pepdatalimite_s\", \"comentario\" : \"$comentario\"}";
    echo $dados;
    die;
} 

switch ($_REQUEST['requisicao']){
	case 'desativaVinculoRegra':
		ob_clean();
		$sql = "UPDATE par3.pendecia_planejamento SET pepstatus = 'I' WHERE pepid = {$_REQUEST['pepid']}";
		$db->executar($sql);
		return $db->commit();
		die;
	break;
	case 'ativaVinculoRegra':
		ob_clean();
		$sql = "UPDATE par3.pendecia_planejamento SET pepstatus = 'A' WHERE pepid = {$_REQUEST['pepid']}";
		$db->executar($sql);
		return $db->commit();
		die;
	break;
	case 'carregaMunicipios':
		ob_clean();
		$municipio = new Territorios_Model_Municipio();
		print simec_json_encode($municipio->lista(array('muncod', 'mundescricao'), array("estuf = '{$_REQUEST['estuf']}'")));
		die;
	break;
	case 'visualizarLog':
		ob_clean();
		$sql = "SELECT lgwurl, lgwrequest, lgwresponse, lgwmethod
				FROM par3.logwssigef
				WHERE lgwid = {$_REQUEST['lgwid']}";
		$arRetorno = $db->pegaLinha($sql);
		?>
		<table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="tb_render" data-qtd-acoes="1" style="text-align: left !important;">
			<tr>
				<th class="subtituloesquerda">method</th>
				<td style="text-align: left;"><?php echo $arRetorno['lgwmethod']; ?></td>
			</tr>
			<tr>
				<th class="subtituloesquerda">URL</th>
				<td style="text-align: left;"><?php echo $arRetorno['lgwurl']; ?></td>
			</tr>
			<tr>
				<th class="subtituloesquerda">Request</th>
				<td style="text-align: left;"><?php echo '<pre>'; echo simec_htmlentities( formatXmlString($arRetorno['lgwrequest']) ) ?></td>
			</tr>
			<tr>
				<th class="subtituloesquerda">Response</th>
				<td style="text-align: left;"><?php echo '<pre>'; echo simec_htmlentities( formatXmlString($arRetorno['lgwresponse']) ); ?></td>
			</tr>
		</table>
		<?php
		die;
	break;
	case 'visualizarLogSigarp':
		ob_clean();
		$sql = "SELECT lgwurl, lgwrequest, lgwresponse, lgwmethod
				FROM par3.logwssigarp
				WHERE lgwid = {$_REQUEST['lgwid']}";
		$arRetorno = $db->pegaLinha($sql);
		
		$xml = new SimpleXMLElement($arRetorno['lgwresponse']);
		?>
		<table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="tb_render" data-qtd-acoes="1" style="text-align: left !important;">
			<tr>
				<th class="subtituloesquerda">method</th>
				<td style="text-align: left;"><?php echo $arRetorno['lgwmethod']; ?></td>
			</tr>
			<tr>
				<th class="subtituloesquerda">URL</th>
				<td style="text-align: left;"><?php echo $arRetorno['lgwurl']; ?></td>
			</tr>
			<tr>
				<th class="subtituloesquerda">Request</th>
				<td style="text-align: left;"><?php echo '<pre>'; echo simec_htmlentities( formatXmlString($arRetorno['lgwrequest']) ) ?></td>
			</tr>
			<tr>
				<th class="subtituloesquerda">Response</th>
				<td style="text-align: left;"><?php echo '<pre>'; echo simec_htmlentities( formatXmlString($arRetorno['lgwresponse']) ); ?></td>
			</tr>
		</table>
		<?php
		die;
	break;
}

require APPRAIZ . 'includes/cabecalho.inc';
?>
<script src="js/par3.js"></script>
<link rel="stylesheet" type="text/css" href="css/menu_tabela_apoio.css" />

<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <div class="ibox float-e-margins">
            <input type="hidden" name="menu" id="menu" value="<?php echo $_REQUEST['menu']; ?>"/>
            <div class="ibox-title">
                <h5 style="font-size: 18px; font-weight: normal;">Tabelas de Apoio</h5>
            </div>
            <div class="ibox-content">
                <div class="ibox-content-round-gray ibox-content">
                    <h5 id="btn-menu-tabelaapoio" style="cursor: pointer;">
                        Tabelas de Apoio
                        <i class="fa fa-bars" title="clique para esconder/mostar os menus"></i>
                    </h5>
                    <div class="row">
                        <div id="side-menu-tabelaapoio" class="col-lg-2 col-md-2 colunaMenu noprint">
                            <div class="space-25"></div>
                          
                            
                            <!--Guia de Iniciativas-->
                            <h5 class="menuopcoes" id="titulo-iniciativas"><b><i class="fa fa-plus"></i> Guia Iniciativas</b></h5>
                            <div id="div-iniciativasmenu" class="guiamenu col-md-offset-1 col-lg-offset-1 col-sm-offset-1" style="display: none;">
                                 <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'areas' ? 'menuSelecionado' : '') ?>" id="areas" tipo="areas">Áreas</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'areasrelacionadas' ? 'menuSelecionado' : '') ?>" id="areasrelacionadas" tipo="areasrelacionadas">Áreas Relacionadas</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'ciclo' ? 'menuSelecionado' : '') ?>" id="cicloPar" tipo="ciclo">Ciclos do PAR</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'descricao' ? 'menuSelecionado' : '') ?>" id="descricao" tipo="descricao">Descrição de Iniciativas</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'desdobramentos' ? 'menuSelecionado' : '') ?>" id="desdobramentos" tipo="desdobramentos">Desdobramentos</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'modalidades' ? 'menuSelecionado' : '') ?>" id="modalidades" tipo="modalidades">Modalidades</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'programas' ? 'menuSelecionado' : '') ?>" id="programas" tipo="programas">Programas</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'projetos' ? 'menuSelecionado' : '') ?>" id="projetos" tipo="projetos">Projetos</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'tiposatendimento' ? 'menuSelecionado' : '') ?>" id="tiposatendimento" tipo="tiposatendimento">Tipos de atendimento</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'tiposassistencia' ? 'menuSelecionado' : '') ?>" id="tiposassistencia" tipo="tiposassistencia">Tipos de assistência</div>
                                <br/>
                            </div>


                            <!--Guia de Itens de Composição-->
                            <h5 id="titulo-itenscomposicao" class="menuopcoes"><b><i class="fa fa-plus"></i> Guia Itens de Composição</b></h5>
                            <div id="div-itenscomposicaomenu" class="col-md-offset-1 col-lg-offset-1 col-sm-offset-1" style="display: none;" tipo="itenscomposicaomenu" >
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'atas' ? 'menuSelecionado' : '') ?>" id="atas" tipo="atas">Atas</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'itens' ? 'menuSelecionado' : '') ?>" id="itens" tipo="itens">Itens</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'grupoitens' ? 'menuSelecionado' : '') ?>" id="grupoitens" tipo="grupoitens">Grupo de Itens</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'tiposobjeto' ? 'menuSelecionado' : '') ?>" tipo="tiposobjeto">Tipos de Objeto</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'unidademedida' ? 'menuSelecionado' : '') ?>" id="unidademedida" tipo="unidademedida">Unidade de Medida</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'categoriaSigarp' ? 'menuSelecionado' : '') ?>" id="categoriaSigarp" tipo="categoriaSigarp">SIGARP - Categoria</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'itensComposicaoSigarp' ? 'menuSelecionado' : '') ?>" id="itensComposicaoSigarp" tipo="itensComposicaoSigarp">SIGARP - Itens  </div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'pregaoSigarp' ? 'menuSelecionado' : '') ?>" id="pregaoSigarp" tipo="pregaoSigarp">SIGARP - Pregão</div>
                                <br/>
                            </div>

                            <!--Guia de Obras-->
                            <h5 id="titulo-itensobras" class="menuopcoes"><b><i class="fa fa-plus"></i> Guia de Obras</b></h5>
                            <div id="div-itensobrasmenu" class="col-md-offset-1" style="display: none;" tipo="itensobrasmenu" >
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'agrupador' ? 'menuSelecionado' : '') ?>" id="documentoAnexo" tipo="agrupador">Agrupador</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'coordenador' ? 'menuSelecionado' : '') ?>" id="coordenador" tipo="coordenador">Coordenador</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'documentosApoio' ? 'menuSelecionado' : '') ?>" id="documentosApoio" tipo="documentosApoio">Documentos de Apoio</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'documentoAnexo' ? 'menuSelecionado' : '') ?>" id="documentoAnexo" tipo="documentoAnexo">Documentos Anexos</div>
                                <div class="menuopcoes menuopcao  <?php echo ( $_REQUEST['menu'] === 'listar_tipo_obra' || $_REQUEST['menu'] === 'tipo_obra' ? 'menuSelecionado' : '') ?>" id="listar_tipo_obra" tipo="listar_tipo_obra">Tipo de Obra</div>
                                <?php if($_REQUEST['menu'] === 'tipo_obra' ): ?>
                                    <div class="menuopcoes menuopcao col-md-offset-1 <?php echo ( $_REQUEST['menu'] === 'tipo_obra'  ? 'menuSelecionado' : '') ?>" id="tipo_obra" tipo="tipo_obra">Cadastrar Tipo Obras</div>
                                <?php endif; ?>
                                <div class="menuopcoes menuopcao col-md-offset-1 col-lg-offset-1 col-sm-offset-1 <?php echo ( in_array($_REQUEST['menu'], array('configurar_abas', 'cad_configurar_abas') ) ? 'menuSelecionado' : '') ?>" id="configurar_abas" tipo="configurar_abas"> Configurar Abas </div>
                                <div class="menuopcoes menuopcao col-md-offset-1 col-lg-offset-1 col-sm-offset-1 <?php echo ( in_array($_REQUEST['menu'], array('planilha_orcamentaria', 'cad_planilha_orcamentaria') ) ? 'menuSelecionado' : '') ?>" id="planilha_orcamentaria" tipo="planilha_orcamentaria"> Planilha Orçamentária</div>
                                <br>
                            </div>
<?php                       if( $boVisualiza == true || in_array(PAR3_PERFIL_SUPER_USUARIO, $arrPerfil) ){?>
                              <!--Guia de Planejamento-->
                            <h5 class="menuopcoes" id="titulo-planejamento"><b><i class="fa fa-plus"></i> Guia Planejamento</b></h5>
                            <div id="div-planejamentomenu" class="guiamenu col-md-offset-1 col-lg-offset-1 col-sm-offset-1" style="display: none;">
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'pendenciaEntidade' ? 'menuSelecionado' : '') ?>" id="pendenciaEntidade" tipo="pendenciaEntidade">Desbloqueio por Entidade</div> <br/>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'desbloqueioJudicial' ? 'menuSelecionado' : '') ?>" id="desbloqueioJudicial" tipo="desbloqueioJudicial">Desbloqueio Judicial</div> <br/>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'cadastroUnidadeExecutora' ? 'menuSelecionado' : '') ?>" id="cadastroUnidadeExecutora" tipo="cadastroUnidadeExecutora">Entidade Executora</div> <br/>
                            </div>
<?php                       }
                            if( $boVisualiza == true  ){?>                            
                              <!--Guia de Bloqueio-->
                            <h5 class="menuopcoes" id="titulo-regrabloqueio"><b><i class="fa fa-plus"></i> Guia de Bloqueio</b></h5>
                            <div id="div-regrabloqueiomenu" class="guiamenu col-md-offset-1 col-lg-offset-1 col-sm-offset-1" style="display: none;">
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'regrasBloqueio' ? 'menuSelecionado' : '') ?>" id="regrasBloqueio" tipo="regrasBloqueio">Regras de Bloqueios</div> <br/>
                            </div>
<?php                       }?>
                            <!--Guia de Serviço WS SIGEF-->
                            <h5 class="menuopcoes" id="titulo-servivo-ws"><b><i class="fa fa-plus"></i> Serviço WS SIGEF</b></h5>
                            <div id="div-servico-wsmenu" class="guiamenu col-md-offset-1 col-lg-offset-1 col-sm-offset-1" style="display: none;">
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'servicoLogErro' ? 'menuSelecionado' : '') ?>" id="servicoLogErro" tipo="servicoLogErro">LOG  WS SIGEF</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'servicoLogErroSigarp' ? 'menuSelecionado' : '') ?>" id="servicoLogErroSigarp" tipo="servicoLogErroSigarp">LOG  WS SIGARP</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'processos' ? 'menuSelecionado' : '') ?>" id="processos" tipo="processos">Processos</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'remanejamentoEmpenho' ? 'menuSelecionado' : '') ?>" id="remanejamentoEmpenho" tipo="remanejamentoEmpenho">Remanejamento de Empenhos</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'remanejamentoPagamento' ? 'menuSelecionado' : '') ?>" id="remanejamentoPagamento" tipo="remanejamentoPagamento">Remanejamento de Pagamentos</div>
                                <br/>
                            </div>

                            <h5 class="menuopcoes" id="titulo-orcamento"><b><i class="fa fa-plus"></i> Guia Orçamento </b></h5>
                             <div id="div-orcamentomenu" class="guiamenu col-md-offset-1 col-lg-offset-1 col-sm-offset-1" style="display: none;">
<!--                                --><?php // if ($_SESSION['baselogin'] == "simec_desenvolvimento") { ?>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'planoInterno' ? 'menuSelecionado' : '') ?>" id="planoInterno" tipo="planoInterno">Plano Interno</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'fonterecurso' ? 'menuSelecionado' : '') ?>" id="fonterecurso" tipo="fonterecurso">Fonte de Recurso</div>
<!--                                --><?php //} ?>
                            </div>
                            <h5 class="menuopcoes" id="titulo-documento"><b><i class="fa fa-plus"></i> Documentos </b></h5>
                            <div id="div-documentomenu" class="guiamenu col-md-offset-1 col-lg-offset-1 col-sm-offset-1" style="display: none;">
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'modelodocumento' ? 'menuSelecionado' : '') ?>" id="modelodocumento" tipo="modelodocumento">Modelo de Documento</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'documentosanexos' ? 'menuSelecionado' : '') ?>" id="documentosanexos" tipo="documentosanexos">Documentos Anexos</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'documentotermogerado' ? 'menuSelecionado' : '') ?>" id="documentotermogerado" tipo="documentotermogerado">Documentos Gerados</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'gerartermomanifestacao' ? 'menuSelecionado' : '') ?>" id="gerartermomanifestacao" tipo="gerartermomanifestacao">Gerar Termo Manifestação</div>
                            </div>
                            
                            <?php
                            $arPerfil = pegaArrayPerfil($_SESSION['usucpf']);
							if (in_array(PAR3_PERFIL_SUPER_USUARIO_DESENVOLVEDOR, $arPerfil)):
                            ?>
                            <h5 class="menuopcoes" id="titulo-configuracao"><b><i class="fa fa-plus"></i> Configuração </b></h5>
                            <div id="div-configuracaomenu" class="guiamenu col-md-offset-1 col-lg-offset-1 col-sm-offset-1" style="display: none;">
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'aviso' ? 'menuSelecionado' : '') ?>" id="aviso" tipo="aviso">Painel de Aviso</div>
								<div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'email' ? 'menuSelecionado' : '') ?>" id="email" tipo="email">Envio de email</div>
								<div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'email_nutricionista' ? 'menuSelecionado' : '') ?>" id="email_nutricionista" tipo="email_nutricionista">E-mails Nutricionistas</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'atualizar_historico' ? 'menuSelecionado' : '') ?>" id="atualizar_historico" tipo="atualizar_historico">Atualizar Histórico</div>
                                <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'listar_regra_termo' ? 'menuSelecionado' : '') ?>" id="listar_regra_termo" tipo="listar_regra_termo">Regra Termo</div>
                            </div>
                            <?php
                            endif;
                            ?>
                        </div>

                        <div class="col-lg-10 col-md-10 colunaForm" id="print_area" id="demo" class="collapse">
                            <?php
                            $pastaItensComposicao   = 'par3/modulos/sistema/tabelaapoio/itenscomposicao/';
                            $pastaIniciativas       = 'par3/modulos/sistema/tabelaapoio/iniciativas/';
                            $pastaErroWS       		= 'par3/modulos/sistema/tabelaapoio/errows/';
                            $pastaplanejamento      = 'par3/modulos/sistema/tabelaapoio/planejamento/';
                            $pastaObras             = 'par3/modulos/sistema/tabelaapoio/obras/';
                            $modelodocumento        = 'par3/modulos/sistema/tabelaapoio/modelodocumento/';
                            $documentosAnexos       = 'par3/modulos/sistema/tabelaapoio/documentosanexos/';
                            $documentotermogerado   = 'par3/modulos/sistema/tabelaapoio/documentotermogerado/';
                            $pastaAviso             = 'par3/modulos/sistema/tabelaapoio/aviso/';
                            $pastaEmail             = 'par3/modulos/sistema/tabelaapoio/email/';
                            $pastaConfiguracao      = 'par3/modulos/sistema/tabelaapoio/configuracao/';
                            $urlGuia                = '/par3/par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu='.$_REQUEST['menu'];
                            switch ($_REQUEST['menu']) {
                                case 'itens':
                                    if ($_REQUEST['submenu']) {
                                        if ($_REQUEST['submenu'] == 'form') {
                                            include APPRAIZ . $pastaItensComposicao . 'itens/form.inc';
                                            break;
                                        }
                                    }
                                    include APPRAIZ . $pastaItensComposicao . 'itens/itens.inc';
                                    break;
                                case 'atas':
                                    include APPRAIZ . $pastaItensComposicao . 'atas/atas.inc';
                                    break;
                                case 'grupoitens':
                                    if ($_REQUEST['submenu']) {
                                        if ($_REQUEST['submenu'] == 'form') {

                                            include APPRAIZ . $pastaItensComposicao . 'grupos/form.inc';
                                            break;
                                        }
                                    }
                                    include APPRAIZ . $pastaItensComposicao . 'grupos/grupos.inc';
                                    break;

                                case 'categoriaSigarp':
                                    if ($_REQUEST['submenu']) {
                                        if ($_REQUEST['submenu'] == 'form') {
                                            include APPRAIZ . $pastaItensComposicao . 'categoriaSigarp/form.inc';
                                            break;
                                        }
                                    }
                                    include APPRAIZ . $pastaItensComposicao . 'categoriaSigarp/categoriaSigarp.inc';
                                    break;

                                case 'itensComposicaoSigarp':
                                    if ($_REQUEST['submenu']) {
                                        if ($_REQUEST['submenu'] == 'form') {
                                            include APPRAIZ . $pastaItensComposicao . 'itensComposicaoSigarp/form.inc';
                                            break;
                                        }
                                    }
                                    include APPRAIZ . $pastaItensComposicao . 'itensComposicaoSigarp/itensComposicaoSigarp.inc';
                                    break;

                                case 'pregaoSigarp':
                                    if ($_REQUEST['submenu']) {
                                        if ($_REQUEST['submenu'] == 'form') {
                                            include APPRAIZ . $pastaItensComposicao . 'pregaoSigarp/form.inc';
                                            break;
                                        }
                                    }
                                    include APPRAIZ . $pastaItensComposicao . 'pregaoSigarp/pregaoSigarp.inc';
                                    break;
                                case 'unidademedida':
                                    include APPRAIZ . $pastaItensComposicao . 'unidademedida/unidademedida.inc';
                                    break;
                                case 'descricao':
                                    include APPRAIZ . $pastaIniciativas . 'descricaoIniciativas.inc';
                                    break;
                                case 'servicoLogErro':
                                    include APPRAIZ . $pastaErroWS . 'servicoLogErro.inc';
                                    break;
                                case 'servicoLogErroSigarp':
                                    include APPRAIZ . $pastaErroWS . 'servicoLogErroSigarp.inc';
                                    break;
                                case 'processos':
                                    include APPRAIZ . $pastaErroWS . 'processo.inc';
                                    break;
                                case 'remanejamentoEmpenho':
                                    include APPRAIZ . $pastaErroWS . 'remanejamentoEmpenho.inc';
                                    break;
                                case 'remanejamentoPagamento':
                                    include APPRAIZ . $pastaErroWS . 'remanejamentoPagamento.inc';
                                    break;
                                case 'ciclo':
                                    include APPRAIZ . $pastaIniciativas . 'ciclos_par.inc';
                                    break;
                                case 'planoInterno':
                                        include APPRAIZ . $pastaIniciativas . 'planoInterno.inc';
                                    break;
                                case 'fonterecurso':
                                    include APPRAIZ . $pastaIniciativas . 'fonterecurso.inc';
                                    break;
                                case 'pendenciaEntidade':
                                    if( $boVisualiza == false && !in_array(PAR3_PERFIL_SUPER_USUARIO, $arrPerfil) ){
                                        echo "<script>
                                                alert('Acesso não autorizado!');
                                                window.location='par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A';
                                              </script>";
                                    }else{
                                        include APPRAIZ . $pastaplanejamento . 'pendenciaEntidade.inc';
                                    break;
                                    }
                                case 'desbloqueioJudicial':
                                    if( $boVisualiza == false && !in_array(PAR3_PERFIL_SUPER_USUARIO, $arrPerfil) ){
                                        echo "<script>
                                                alert('Acesso não autorizado!');
                                                window.location='par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A';
                                              </script>";
                                    }else{
                                        include APPRAIZ . $pastaplanejamento . 'desbloqueioJudicial.inc';
                                        break;
                                    }
                                case 'cadastroUnidadeExecutora':
                                    if( $boVisualiza == false && !in_array(PAR3_PERFIL_SUPER_USUARIO, $arrPerfil) ){
                                        echo "<script>
                                                alert('Acesso não autorizado!');
                                                window.location='par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A';
                                              </script>";
                                    }else{
                                        include APPRAIZ . $pastaplanejamento . 'cadastroUnidadeExecutora.inc';
                                        break;
                                    }
                                case 'regrasBloqueio':
                                    if( $boVisualiza == false ){
                                        echo "<script>
                                                alert('Acesso não autorizado!');
                                                window.location='par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A';
                                              </script>";
                                    }else{
                                        include APPRAIZ . $pastaplanejamento . 'regrasBloqueio.inc';
                                    break;
                                    }

                                case 'programas':
                                    if ($_REQUEST['submenu']) {
                                        $controllerIniciativaProgramas = new Par3_Controller_IniciativaProgramasController();
                                        $programacao = $controllerIniciativaProgramas->recuperar($_REQUEST);
                                        include APPRAIZ . $pastaIniciativas . 'programas/atendimento_extraordinario.inc';
                                        break;
                                    }
                                    include APPRAIZ . $pastaIniciativas . 'programas.inc';
                                    break;
                                case 'projetos':include APPRAIZ . $pastaIniciativas . 'projetos.inc';
                                    break;
                                
                                case 'tiposatendimento':include APPRAIZ . $pastaIniciativas . 'tiposAtendimento.inc';
                                    break;
                                case 'tiposassistencia':
                                	include APPRAIZ . $pastaIniciativas . 'tiposAssistencia.inc';
                                    break;
                                case 'modelodocumento':include APPRAIZ . $modelodocumento . 'listaModeloTermo.inc';
                                    break;
                                case 'documentosanexos':include APPRAIZ . $documentosAnexos . 'documentosAnexos.inc';
                                    break;
                                case 'documentotermogerado':include APPRAIZ . $documentotermogerado . 'documentotermogerado.inc';
                                    break;
                                case 'gerartermomanifestacao':include APPRAIZ . $modelodocumento . 'gerarTermoManifestacao.inc';
                                    break;
                                case 'gerarmodelodocumento':include APPRAIZ . $modelodocumento . 'gerarModeloTermo.inc';
                                    break;
                                case 'modalidades':include APPRAIZ . $pastaIniciativas . 'modalidades.inc';
                                    break;
                                case 'tiposobjeto':include APPRAIZ . $pastaIniciativas . 'tiposObjeto.inc';
                                    break;
                                case 'desdobramentos':include APPRAIZ . $pastaIniciativas . 'desdobramentos.inc';
                                    break;
                                case 'areas':include APPRAIZ . $pastaIniciativas . 'areas.inc';break;
                                case 'areasrelacionadas':
                                    if($_REQUEST['submenu']){
                                        if($_REQUEST['submenu'] == 'form'){
                                            include APPRAIZ . $pastaIniciativas . 'areasrelacionadas/form.inc';break;
                                        }
                                    }
                                    include APPRAIZ . $pastaIniciativas . 'areasrelacionadas.inc';break;


                                case 'listar_tipo_obra':
                                case 'tipo_obra':
                                case 'configurar_abas':
                                case 'cad_configurar_abas':
                                case 'planilha_orcamentaria':
                                case 'cad_planilha_orcamentaria':
                                case 'documentosApoio':
                                case 'documentoAnexo':
                                case 'coordenador':
                                    include APPRAIZ . $pastaObras .$_REQUEST['menu']. '.inc';break;
                                case 'agrupador':
                                    include APPRAIZ . $pastaObras .$_REQUEST['menu']. '.inc';break;
                                case 'aviso':
                                    include APPRAIZ . $pastaAviso .'listaAviso.inc';break;
                                case 'cadastroAviso':
                                    include APPRAIZ . $pastaAviso .'cadastroAviso.inc';break;
                                case 'email':
                                    include APPRAIZ . $pastaEmail .'enviarEmail.inc';break;
                                case 'email_nutricionista':
                                    include APPRAIZ . $pastaEmail .'resetarEmailNutricionistas.inc';break;
                                case 'atualizar_historico':
                                    include APPRAIZ . $pastaEmail .'atualizarHistoricoanalises.php';break;
                                case 'listar_regra_termo':
                                    include APPRAIZ . $pastaConfiguracao .'listar_regra_termo.inc';break;
                                case 'cadastrar_regra_termo':
                                    include APPRAIZ . $pastaConfiguracao .'cadastrar_regra_termo.inc';break;
                            }

                            switch($_REQUEST['menu']){
                                case 'cad_planilha_orcamentaria':
                                    $_REQUEST['menu'] = 'planilha_orcamentaria';
                                    break;
                            }
                            ?>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<script>
    $(document).ready(function () {
        $('#btn-menu-tabelaapoio').on('click',function(evt) {
            evt.preventDefault();
            $('#side-menu-tabelaapoio').toggle("slide");
            $('#print_area').toggleClass(function() {
                if($(this).is('.col-lg-10')) {
                    $(this).removeClass('col-lg-10');
                    $(this).removeClass('col-md-10');
                    return 'col-lg-12 col-md-12';
                }
                $(this).removeClass('col-lg-12');
                $(this).removeClass('col-md-12');
                return 'col-lg-10 col-md-10';
            });
        });

        $('.menuopcao').click(function () {
            var menuAtual = '<?php echo $_REQUEST['menu']; ?>';
            var menu = $(this).attr('tipo');
            var url = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=' + menu;

            if (menu != menuAtual) {
                $(location).attr('href', url);
            }
        });
    });

    /*******************************************/
    $('[tipo=<?php echo $_REQUEST['menu']; ?>]').parent('#div-itenscomposicaomenu').show(function () {
        $('#titulo-itenscomposicao').find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });

    $('[tipo=<?php echo $_REQUEST['menu']; ?>]').parent('#div-iniciativasmenu').show(function () {
        $('#titulo-iniciativas').find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });

    $('[tipo=<?php echo $_REQUEST['menu']; ?>]').parent('#div-orcamentomenu').show(function () {
        $('#titulo-orcamento').find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });
        
	$('[tipo=<?php echo $_REQUEST['menu']; ?>]').parent('#div-documentomenu').show(function () {
        $('#titulo-documento').find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });
	$('[tipo=<?php echo $_REQUEST['menu']; ?>]').parent('#div-configuracaomenu').show(function () {
        $('#titulo-configuracao').find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });
    $('[tipo=<?php echo $_REQUEST['menu']; ?>]').parent('#div-servico-wsmenu').show(function () {
        $('#titulo-servivo-ws').find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });
    $('[tipo=<?php echo $_REQUEST['menu']; ?>]').parent('#div-planejamentomenu').show(function () {
        $('#titulo-planejamento').find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });
    $('[tipo=<?php echo $_REQUEST['menu']; ?>]').parent('#div-regrabloqueiomenu').show(function () {
        $('#titulo-regrabloqueio').find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });

    $('[tipo=<?php echo $_REQUEST['menu']; ?>]').parent('#div-itensobrasmenu').show(function () {
        $('#titulo-itensobras').find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });

    $('[tipo=<?php echo $_REQUEST['menu']; ?>]').parent('#div-itenstipoobrasmenu').show(function () {
        $('#titulo-itensobras, #titulo-tipoobra').each(function(){
            $(this).find('i').toggleClass(function () {
                if ($(this).is(".fa-plus")) {
                    return 'fa-minus';
                }
                return 'fa-plus';
            });
        });
        $('#div-itensobrasmenu').slideToggle();
    });






    /*********************************/

    $('#titulo-iniciativas').click(function () {
        $('#div-itenscomposicaomenu').slideUp(function () {
            $('#titulo-itenscomposicao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itensobrasmenu').slideUp(function () {
            $('#titulo-itensobras').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-orcamentomenu').slideUp(function () {
            $('#titulo-orcamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-documentomenu').slideUp(function () {
            $('#titulo-documento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-configuracaomenu').slideUp(function () {
            $('#titulo-configuracao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-planejamentomenu').slideUp(function () {
            $('#titulo-planejamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-regrabloqueiomenu').slideUp(function () {
            $('#titulo-regrabloqueio').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-servico-wsmenu').slideUp(function () {
            $('#titulo-servivo-ws').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-iniciativasmenu').slideToggle();
        $(this).find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });
    
    $('#titulo-servivo-ws').click(function () {
        $('#div-itenscomposicaomenu').slideUp(function () {
            $('#titulo-itenscomposicao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itensobrasmenu').slideUp(function () {
            $('#titulo-itensobras').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-planejamentomenu').slideUp(function () {
            $('#titulo-planejamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-regrabloqueiomenu').slideUp(function () {
            $('#titulo-regrabloqueio').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-iniciativasmenu').slideUp(function () {
            $('#titulo-iniciativas').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-orcamentomenu').slideUp(function () {
            $('#titulo-orcamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-documentomenu').slideUp(function () {
            $('#titulo-documento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-configuracaomenu').slideUp(function () {
            $('#titulo-configuracao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-servico-wsmenu').slideToggle();
        $(this).find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });
    
    $('#titulo-planejamento').click(function () {
        $('#div-itenscomposicaomenu').slideUp(function () {
            $('#titulo-itenscomposicao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itensobrasmenu').slideUp(function () {
            $('#titulo-itensobras').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-regrabloqueiomenu').slideUp(function () {
            $('#titulo-regrabloqueio').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-iniciativasmenu').slideUp(function () {
            $('#titulo-iniciativas').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-servico-wsmenu').slideUp(function () {
            $('#titulo-servivo-ws').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-orcamentomenu').slideUp(function () {
            $('#titulo-orcamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-documentomenu').slideUp(function () {
            $('#titulo-documento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-configuracaomenu').slideUp(function () {
            $('#titulo-configuracao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-planejamentomenu').slideToggle();
        $(this).find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });
    
    $('#titulo-regrabloqueio').click(function () {
        $('#div-itenscomposicaomenu').slideUp(function () {
            $('#titulo-itenscomposicao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-planejamentomenu').slideUp(function () {
            $('#titulo-planejamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itensobrasmenu').slideUp(function () {
            $('#titulo-itensobras').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-iniciativasmenu').slideUp(function () {
            $('#titulo-iniciativas').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-servico-wsmenu').slideUp(function () {
            $('#titulo-servivo-ws').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-orcamentomenu').slideUp(function () {
            $('#titulo-orcamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-documentomenu').slideUp(function () {
            $('#titulo-documento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-configuracaomenu').slideUp(function () {
            $('#titulo-configuracao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-regrabloqueiomenu').slideToggle();
        $(this).find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });

    $('#titulo-itenscomposicao').click(function () {
        $('#div-iniciativasmenu').slideUp(function () {
            $('#titulo-iniciativas').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itensobrasmenu').slideUp(function () {
            $('#titulo-itensobras').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-planejamentomenu').slideUp(function () {
            $('#titulo-planejamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-regrabloqueiomenu').slideUp(function () {
            $('#titulo-regrabloqueio').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-servico-wsmenu').slideUp(function () {
            $('#titulo-servivo-ws').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-orcamentomenu').slideUp(function () {
            $('#titulo-orcamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-documentomenu').slideUp(function () {
            $('#titulo-documento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-configuracaomenu').slideUp(function () {
            $('#titulo-configuracao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itenscomposicaomenu').slideToggle();
        $(this).find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });

    $('#titulo-itensobras').click(function () {
        $('#div-iniciativasmenu').slideUp(function () {
            $('#titulo-iniciativas').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itenscomposicaomenu').slideUp(function () {
            $('#titulo-itenscomposicao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-planejamentomenu').slideUp(function () {
            $('#titulo-planejamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-regrabloqueiomenu').slideUp(function () {
            $('#titulo-regrabloqueio').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-servico-wsmenu').slideUp(function () {
            $('#titulo-servivo-ws').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-orcamentomenu').slideUp(function () {
            $('#titulo-orcamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        
        $('#div-documentomenu').slideUp(function () {
            $('#titulo-documento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-configuracaomenu').slideUp(function () {
            $('#titulo-configuracao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itensobrasmenu').slideToggle();
        $(this).find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });


    $('#titulo-tipoobra').click(function () {
        $('#div-iniciativasmenu').slideUp(function () {
            $('#titulo-iniciativas').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itenscomposicaomenu').slideUp(function () {
            $('#titulo-itenscomposicao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-planejamentomenu').slideUp(function () {
            $('#titulo-planejamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-regrabloqueiomenu').slideUp(function () {
            $('#titulo-regrabloqueio').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-servico-wsmenu').slideUp(function () {
            $('#titulo-servivo-ws').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-orcamentomenu').slideUp(function () {
            $('#titulo-orcamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-documentomenu').slideUp(function () {
            $('#titulo-documento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-configuracaomenu').slideUp(function () {
            $('#titulo-configuracao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itenstipoobrasmenu').slideToggle();
        $(this).find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });

    $('#titulo-orcamento').click(function () {
        $('#div-itenscomposicaomenu').slideUp(function () {
            $('#titulo-itenscomposicao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itensobrasmenu').slideUp(function () {
            $('#titulo-itensobras').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-iniciativasmenu').slideUp(function () {
            $('#titulo-iniciativas').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-planejamentomenu').slideUp(function () {
            $('#titulo-planejamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-regrabloqueiomenu').slideUp(function () {
            $('#titulo-regrabloqueio').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-documentomenu').slideUp(function () {
            $('#titulo-documento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-configuracaomenu').slideUp(function () {
            $('#titulo-configuracao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-orcamentomenu').slideToggle();
        
        $(this).find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });
    
    $('#titulo-documento').click(function () {
        $('#div-itenscomposicaomenu').slideUp(function () {
            $('#titulo-itenscomposicao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itensobrasmenu').slideUp(function () {
            $('#titulo-itensobras').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-iniciativasmenu').slideUp(function () {
            $('#titulo-iniciativas').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-planejamentomenu').slideUp(function () {
            $('#titulo-planejamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-regrabloqueiomenu').slideUp(function () {
            $('#titulo-regrabloqueio').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-orcamentomenu').slideUp(function () {
            $('#titulo-orcamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-configuracaomenu').slideUp(function () {
            $('#titulo-configuracao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-documentomenu').slideToggle();

        $(this).find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });
    
    $('#titulo-configuracao').click(function () {
        $('#div-itenscomposicaomenu').slideUp(function () {
            $('#titulo-itenscomposicao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itensobrasmenu').slideUp(function () {
            $('#titulo-itensobras').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-iniciativasmenu').slideUp(function () {
            $('#titulo-iniciativas').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-planejamentomenu').slideUp(function () {
            $('#titulo-planejamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-regrabloqueiomenu').slideUp(function () {
            $('#titulo-regrabloqueio').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-servico-wsmenu').slideUp(function () {
            $('#titulo-servivo-ws').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-orcamentomenu').slideUp(function () {
            $('#titulo-orcamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-documentomenu').slideUp(function () {
            $('#titulo-documento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-configuracaomenu').slideToggle();

        $(this).find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });

</script>