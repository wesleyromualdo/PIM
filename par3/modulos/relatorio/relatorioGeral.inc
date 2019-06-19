<?php
/**
 * @author  Thiago Tasca Barbosa <thiagotasca@gmail.com>
 * @license  GNU simec.mec.gov.br
 * @version  SVN:
 * @link     no link
 */

ini_set("memory_limit","500M");
set_time_limit(0);

# Classes
$obrelatorioGeralPAR 		= new Par3_controller_RelatorioGeralPar();
$obModelrelatorioGeralPAR 	= new Par3_Model_RelatorioGeralPar();
$estado 					= new Territorios_Model_Estado();
$municipio      			= new Territorios_Model_Municipio();


switch ($_REQUEST['requisicao']) {
	case 'exportar':
	    ini_set('memory_limit', '-1');
        $obrelatorioGeralPAR->carregaRelatorioGeralPar($_REQUEST);
		die;
		break;
    case 'carregaMunicipios':
		$municipio = new Territorios_Model_Municipio();
		print simec_json_encode($municipio->lista(array('muncod', 'mundescricao'), array("estuf = '{$_REQUEST['estuf']}'"))); die;
        break;
    case 'carregarMunicipiosAnalise':
        //lista todos os municípios relacionados ao Estado(estuf) informado
        ob_clean();
        $municipio = new Territorios_Model_Municipio();//instancia a classe município
        echo simec_json_encode($municipio->carregar($municipio->pegarSQLSelect($_GET['estuf'])));
        die;
        break;
    case 'carregarTodosMunicipios':
        //lista todos os municípios relacionados ao Estado(estuf) informado
        ob_clean();
        $municipio = new Territorios_Model_Municipio();//instancia a classe município
        $arrMun = $municipio->recuperarListaMunicipios();
        echo simec_json_encode($arrMun);
        die;
        break;
    case 'carregarIniciativa':
        ob_clean();
        echo $_REQUEST['objetof']; die();
        $arrIniciativa = $obModelrelatorioGeralPAR->sqlCarregaIniciativasPorObj($_GET['objetof'] );
        echo simec_json_encode($arrIniciativa);
        die;
        break;
}
 
#Includes
require APPRAIZ . 'includes/cabecalho.inc';


# SQL para campos carregados -- ANALISANDO AINDA DESEMPENHO E OUTRA FORMA DE USAR O COMPONENTE DO SIMEC
$sqlSituacoesI      = "SELECT esdid AS codigo, esdordem||' - '||esddsc AS descricao FROM workflow.estadodocumento WHERE tpdid=".PAR3_WORKFLOW_FLUXO_INICIATIVAS." AND esdstatus='A' ORDER BY esdordem";
$sqlSituacoesO      = "SELECT esdid AS codigo, esdordem||' - '||esddsc AS descricao FROM workflow.estadodocumento WHERE tpdid=".PAR3_WORKFLOW_FLUXO_OBRAS." AND esdstatus='A' ORDER BY esdordem";
$sqlTiposObjeto     = "SELECT intoid AS codigo, intoid||' - '||intodsc AS descricao FROM par3.iniciativa_tipos_objeto WHERE intostatus='A' AND intosituacao='A' ORDER BY 1";
$sqlDimensoes       = "SELECT DISTINCT dimcod AS codigo, dimcod||' - '||dimdsc AS descricao FROM par3.dimensao WHERE dimstatus='A' ORDER BY dimcod";
$sqlCategoriaO      = "SELECT octid AS codigo, octid||' - '||octdsc AS descricao FROM par3.obra_categoria WHERE octstatus = 'A' ORDER BY 1";
$sqlLocalO          = array('13, 14, 25' => 'Cobertura', '1, 2, 19, 20, 21, 23' => 'Creche', '4, 5, 6, 7, 8, 9, 10, 11, 15, 16, 17, 18, 22' => 'Escola', '12, 24' => 'Quadra');
$sqlAno             = 'SELECT DISTINCT obrano AS codigo, obrano AS descricao FROM par3.obra UNION SELECT DISTINCT ipiano AS codigo, ipiano AS descricao FROM par3.iniciativa_planejamento_item_composicao ORDER BY codigo DESC';
$sqlDocumentoTermo  = "SELECT dotid AS codigo, dotnumero AS descricao FROM par3.documentotermo WHERE dotstatus = 'A' ORDER BY codigo DESC";
$sqlProcesso        = "SELECT proid AS codigo, formata_num_processo( pronumeroprocesso ) AS descricao FROM par3.processo";
$sqlEmpenho         = "SELECT proid AS codigo, formata_num_processo( pronumeroprocesso ) AS descricao FROM par3.processo";
$sqlPagamentoSituacao = "SELECT * FROM (VALUES (1, '100% Pago'), (2, 'Pagamento parcial'), (3, 'Sem pagamento')) AS t (codigo,descricao);";
$arrMunicipio       = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa(true);
$obPagamento        = new Par3_Controller_Pagamentos();
$sqlIniciativa      = "  SELECT ini.iniid AS codigo, 
                                idesc.inddsc AS descricao 
                        FROM par3.iniciativa ini
                        INNER JOIN par3.iniciativa_descricao idesc ON idesc.indid = ini.indid 
                        -- WHERE inistatus = 'A' 
                        ORDER BY descricao ";
$sqlItens           = " SELECT it.itcid AS codigo, it.itcdsc as descricao
                        FROM par3.itenscomposicao it
                        WHERE it.itcstatus = 'A' 
                        ORDER BY descricao";
?>

<style type="text/css">
    .contordemcol {
        border-style: solid;
        border-width: 1px;
        border-color: #d3d3d3;
        margin-top: 5px;
        margin-bottom: 5px;
        margin-right: 5px;
        margin-left: 5px;
        display: inline-block; 
        padding: 2px 5px 2px 5px;
        background-color: #E6E6E6;
        border-radius: 5px;
        }
        
    .contordemcolInterna{
        margin-top: 5px;
        margin-bottom: 5px;
        margin-right: 5px;
        margin-left: 5px;
        padding: 0px 0px 0px 0px;
        min-height: 0px;

        background-color: #E6E6E6;
        border-color: #e7eaec;
        border-image: none;
        border-style: none;
        border-width: 0px 0px 0px;
        color: #676a6c;
    }

    .ordemcolunas { 
        border-style: solid;
        border-width: 1px;
        border-color: #d3d3d3;
        
    }    
</style>

<div id="ibox">
    <!-- TITULO -->
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
               <h2 class="center"> Relatório Geral do PAR</h2>
            </div>
        </div>
    </div>

    <!-- início DO FORM -->
    <form method="post" name="formulario-relatorio" id="formulario-relatorio" class="form form-horizontal">
    <input type="hidden" name="requisicao" value="<?php echo $_REQUEST['requisicao'] ?>">
    <div class="ibox-title"> 
            <h3 class="font-bold no-margins" >Escolha as colunas e o tipo de relatório </h3> 
            <small>Você pode cruzar informações dos campos abaixo</small>
      
        <div class="ibox-tools">
                    <a class="collapse-link">
                        <i class="fa fa-chevron-up"></i>
                    </a>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row" >
            <div class="col-md-4" > 
                <div>
                    <div class="form-group">
                        <?php  
                                $dadosentidades = array('coluf' =>'UF' , 
                                                        'colmuncod' =>'IBGE', 
                                                        'colentidade' =>'Entidade',
                                                        'colesddsc' =>'Fase do PAR' );
                            echo $simec->select('colunasentidades', 'Dados da Entidade', $_REQUEST['colunasentidades'], $dadosentidades, array('multiple'));
                        
                            $plaentidade = array(   'id.tipoobjeto' =>'Objeto' ,
                                                    'id.iniid' => 'Cód da Iniciativa',
                                                    'p.inpid' =>'Código do Planejamento', 
                                                    'id.inddsc' =>'Iniciativa',
                                                    'moddsc' =>'Modalidade',
                                                    'it.item' =>'Item Solicitado', 
                                                    'p.qtditem' =>'QTD do item solicitado', 
                                                    'p.valoritem' =>'Valor do Item',
                                                    'vlrXqtdItem' =>'Valor Total Solicitado (QTD X Valor)',
                                                    'p.anoitem'=>'Ano do Item',
                                                    'id.cronograma' =>'Cronograma',
                                                    'id.tipoassistencia' =>'Tipo de assistência', 
                                                    'idobra' => 'Id da Obra',
                                                    'nomeobras' => 'Nome da Obra',
                                                    'au.autnome' => 'Autor da Emenda' );
                                                    
                                                    //'id.nomeiniciativa' =>'Tipo da Iniciativa',

                            echo $simec->select('planejamentoentidade', 'Planejamento da Entidade', $_REQUEST['planejamentoentidade'], $plaentidade, array('multiple')); 

                            $dadosanalise = array(  'codanalise' => 'Código da Análise',
                                                    'situacaoanalise' => 'Fase da Análise',
                                                    'p.itemaprovado' =>'Situação do Item', 
                                                    'p.ipiquantidadeaprovada' =>'Quantidade Aprovada do Item',
                                                    'valoraprovadoitem' =>'Valor aprovado do Item'
                                                     );
                                                     
                                                    //'situacaoiniciativa' =>'Situação da Iniciativa',
                                                    //'justificativa' =>'Justificativa'
                            echo $simec->select('analisefnde', 'Dados de Análises do FNDE', $_REQUEST['analisefnde'], $dadosanalise, array('multiple')); 
                           
                        ?> 
                    </div>
                </div>
            </div>
            <div class="col-md-4" > 
                <div>
                    <div class="form-group">
                        <?php 
                            $colProcessos = array(  'numprocesso' =>'Número do Processo' , 
                                                    'tipoprocesso' =>'Tipo de Processo',
                                                    'tipoobjeto' =>'Tipo Objeto', 
                                                    'anoprocesso' =>'Ano do Processo',
                                                    'valorprocesso' => 'Valor do Processo' );
                            echo $simec->select('colunasprocessos', 'Processos', $_REQUEST['colunasprocessos'], $colProcessos, array('multiple'));

                            $colEmpenho = array(    'ne' =>'NE - Número do Empenho' , 
                                                    'anoempenho' =>'Ano do Empenho', 
                                                    'valorempenho' =>'Valor do Empenho', 
                                                    'situacaoempenho' => 'Situação do Empenho',
                                                    'ptres' =>'PTRES', 
                                                    'fonte' =>'Fonte'
                                                 );
                            echo $simec->select('colunasempenhos', 'Empenhos', $_REQUEST['colunasempenhos'], $colEmpenho, array('multiple'));
                            
                        ?>
                    </div>
                </div>
            </div>
            <div class="col-md-4" > 
                <div>
                    <div class="form-group">
                        <?php 
                                $colTermos = array( 'numtermo' =>'Número do Termo' , 
                                                     'tipotermo' =>'Tipo de Termo', 
                                                     'valortermo' =>'Valor do Termo', 
                                                     'valorfnde' =>'Valor FNDE', 'valorcontrapartida' =>'Valor Contarpartida' );
                                //'anotermo' =>'Ano do Termo',
                                echo $simec->select('colunastermos', 'Termos', $_REQUEST['colunastermos'], $colTermos, array('multiple'));

                                $colFinanceiro = array( 'ob' =>'Número da OB', 
                                                        'numseqpagamento' =>'Número Seguencial do Pagamento' , 
                                                        'anoefetivacaopagamento' =>'Ano da efetivação do Pagamento',  
                                                        'vlrpago' =>'Valor Pago', 
                                                        'sitpagamento' =>'Situação do Pagamento' );
                                echo $simec->select('colunafinanceiro', 'Pagamentos', $_REQUEST['colunafinanceiro'], $colFinanceiro, array('multiple'));
                        ?>
                    </div> 
                </div>
            </div>
            
        </div>
            <h5 class="font-bold no-margins" >Ordenação de Colunas</h5>
            <small>Após a seleção das colunas neste campo você pode ordena-las.</small>
            <div class="ordemcolunas" id="ordemcolunas" > </div>
            <?php 
                echo $simec->input('selctordemcolunas', '', $_REQUEST['selctordemcolunas'],  array('class' => 'inteiro','type' => 'hidden'), array('input-size' => 4) );
                echo $simec->input('selctordemcolunastext', '', $_REQUEST['selctordemcolunastext'],  array('class' => 'inteiro','type' => 'hidden'), array('input-size' => 4) ); 
            ?>
    </div>   
</div>

    <div class="row">
        <div class="col-lg-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h3 class="font-bold no-margins" >Filtros </h3>
                        <small>Selecione o Filtro desejado.</small>
                    <div class="ibox-tools">
                        <a class="collapse-link">
                            <i class="fa fa-chevron-up"></i>
                        </a>
                    </div>
                </div>
                <div class="ibox-content">
                    <div class="row">
                        <div class="col-lg-6">
                            <h4>Filtros sobre Entidade</h4>
                            <?php 
                                echo $simec->radio('itrid', 'Esfera', $_REQUEST['itrid'] ? $_REQUEST['itrid'] : -1, array('-1' => 'Todos', '1' => 'Estadual', '2' => 'Municipal'));
                                echo $simec->select('estuf', 'Estados', $_REQUEST['estuf'], $estado->pegarSQLSelect( $_REQUEST ), array('multiple'), array('input-size' => '7')); 
                           
                                $_REQUEST['muncod'] = tratarArrayParaMultiSelect($_REQUEST['muncod']);
                                echo $simec->select('muncod[]', 'Município', $_REQUEST['muncod'], $arrMunicipio, array('maxlength' => '255','multiple' => 'multiple' ,'onclick'   => 'listarMunicipios()'), array('input-size' => '7')); 
                           
                                $mEstadoDocumento   = new Workflow_Model_EstadoDocumento();
                                $rsEsd              = $mEstadoDocumento->getArrayEstadoDocumentoPAR3();
                                $arrAttr            = array ( 'data-placeholder' => 'Situação' );
                                echo $simec->select('esdidFiltros', 'Fase do PAR', $_POST ['esdidFiltros'], $rsEsd, $arrAttr, array('input-size' => '7'));
                            ?>
                            <div class="hr-line-dashed"></div>
                            <h4>Filtros de Orçamento</h4>
                            <?php 
                                echo $simec->radio('situacaoempenho', 'Empenho', $_REQUEST['situacaoempenho']? $_REQUEST['situacaoempenho'] : 0, array( '0' => 'Todos', '1'=>'Total', '2' => 'Parcial', '3'=> 'Nenhum'));
                                echo $simec->input('pronumeroprocesso', 'Processo', $_REQUEST['pronumeroprocesso'],  array('class' => 'inteiro','type' => 'number'), array('input-size' => 4) );
                                echo $simec->input('filtroNE', 'NE', $_REQUEST['filtroNE'],  array('class' => 'text','type' => 'text'), array('input-size' => 4) );
                                echo $simec->radio('situacaopagamento', 'Pagamento', $_REQUEST['situacaopagamento']? $_REQUEST['situacaopagamento'] : 0, array( '0' => 'Todos', '1'=>'Total', '2' => 'Parcial', '3'=> 'Nenhum')); 
                            ?>
                            <div class="hr-line-dashed"></div>
                            <h4>Filtros Diversos</h4>
                            <?php 
                                echo $simec->input('codplafiltro', 'Código do Planejamento', $_REQUEST['codplafiltro'],  array('class' => 'inteiro','type' => 'number'), array('input-size' => 4) );
                                echo $simec->input('emendafiltro', 'Código da Emenda', $_REQUEST['emendafiltro'],  array('class' => 'inteiro','type' => 'number'), array('input-size' => 4) );
                                echo $simec->input('obridfiltro', 'Código da Obra', $_REQUEST['obridfiltro'],  array('class' => 'inteiro','type' => 'number'), array('input-size' => 4) );
                            ?>   
                        </div>

                        <div class="col-lg-6">
                            <h4>Filtros sobre o Planejamento</h4>
                            <?php 
                                echo $simec->select('tipoobjetoFiltro', 'Tipo de Objeto', $_POST['tipoobjetoFiltro'], $sqlTiposObjeto, array('multiple'), array('input-size' => '7'));
                                echo $simec->select('iniciativaFiltro', 'Iniciativa', $_POST['iniciativaFiltro'], $sqlIniciativa, array('multiple'), array('input-size' => '7'));
                                echo $simec->select('itemFiltros', 'Itens', $_POST['itemFiltros'], $sqlItens, array('multiple'), array('input-size' => '7'));
                                echo $simec->select('ano', 'Ano', $_POST['ano'], $sqlAno, null, array('input-size' => '7'));
                                echo $simec->radio('tipoassistencia', 'Tipo de Assistência', $_REQUEST['tipoassistencia']? $_REQUEST['tipoassistencia'] : 0, array('0' => 'Todos', '1' => 'PAR', '2' => 'Emendas')); 
                            ?>
                            
                            <div class="hr-line-dashed"></div>
                            <h4>Filtros sobre Termos de Compromisso</h4>
                            <?php 
                                echo $simec->select('dotid', 'Termo', $_POST['dotid'], $sqlDocumentoTermo, array('multiple'), array('input-size' => '7'));
                                echo $simec->radio('situacaoTermo', 'Situação do Termo', $_REQUEST['situacaoTermo']? $_REQUEST['situacaoTermo'] : 0, array('0'=>'Selecione', '1' => 'Pendente', '2'=>'Validado', '3' => 'Rejeitado'));
                                echo $simec->radio('termoComContato', 'Termos com Contrato', $_REQUEST['termoComContato']? $_REQUEST['termoComContato'] : 0, array('0'=>'Todos', '1' => 'Sim', '2'=>'Não'));
                                echo $simec->radio('TermoComNota', 'Termos com nota ', $_REQUEST['TermoComNota']? $_REQUEST['TermoComNota'] : 0, array('0'=>'Todos', '1' => 'Sim', '2'=>'Não')); 
                            ?>
                            
                            
                            <div class="hr-line-dashed"></div>
                            <h4>Filtros de Análise do PAR</h4>
                            <?
                                $sql ="select esdid as codigo, esddsc as descricao from workflow.estadodocumento where tpdid = 310";
                                echo $simec->select('esdidAnalise', 'Situação da Análise', $_POST ['esdidAnalise'], $sql, array('maxlength' => '255'), array('input-size' => '7'));
                                echo $simec->radio('itemaprovado', 'Itens Aprovados', $_REQUEST['itemaprovado']? $_REQUEST['itemaprovado'] : 0, array('0' => 'Todos', '1' => 'Sim', '2'=>'Não'));
                                echo $simec->radio('itemadesao', 'Itens com Adesão', $_REQUEST['itemadesao']? $_REQUEST['itemadesao'] : 0, array('0' => 'Todos', '1' => 'Sim', '2'=> 'Não')); 
                            ?>
                        </div>
                    </div>

            <div class="row">
        <div class="col-lg-12" >
            <div class="ibox-content">
                <div class="form-horizontal">
                    <div class="form-group">
                        <div class="center">
                            <button type="button" id="btn-pesquisar" data-dismiss="modal" class="btn btn-primary btn-pesquisar" data-loading-text="Pesquisando, aguarde ..."><i class="fa fa-search"></i> Pesquisar</button>
                            <button type="button" class="btn btn-primary btn-export" ><i class="fa fa-list"></i> Exportar Excel</button>
                            <button type="button" class="btn btn-success" id="limpar"><i class="fa fa-eraser"></i> Limpar</button>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
</form>
</div>

<div class="float-e-margins" id="listagem">
    <div class="ibox-content" id="div-listagem" style="display: block;">
        <div class="table-responsive" >
            <div>
                <div id="resposta">
                <?php
                if ($_REQUEST['requisicao'] == 'pesquisar') {
                   $obrelatorioGeralPAR->carregaRelatorioGeralPar($_REQUEST);
                }
                ?>
                </div>
            </div>
        </div>
    </div>
</div>
 
 <script src="../includes/jquery-ui/jquery-ui-sortable.min.js"></script>
 <script>

 jQuery('#limpar').click(function(){
	window.location.href = window.location;
});

 function WinMoveRel() {
    var element = "[class*=ordemcolunas]";
    var handle = ".ibox-title";
    var connect = "[class*=col]";
    jQuery(element).sortable(
        {
            handle: handle,
            connectWith: connect,
            tolerance: 'pointer',
            forcePlaceholderSize: true,
            opacity: 0.8
        })
        .disableSelection();
}

jQuery('#estuf').change(function(){
        if($('[name=itrid]:checked').val() == 1){
            return;
        }else{
            if(!Array.isArray($(this).val())){
                listarMunicipios();
            }else{
                carregarMunicipioAnalise($('#estuf').val(),$('#muncod').val());
            }
        }
});

jQuery('#tipoobjetoFiltro').change(function(){
    return false;
});

function listarMunicipios(){
    console.log('testes');
    var options = jQuery('#muncod');
    options.empty();
    options.trigger('chosen:updated');
    // options.append(new Option('', ''));
    jQuery.ajax({
        type: "GET",
        url: window.location.href,
        data: { requisicao:'carregarTodosMunicipios'},
        success: function(retorno){
            $.each(JSON.parse(retorno), function() {
                options.append(new Option(this.descricao, this.codigo));
            });
            options.val(muncod);
            options.trigger('chosen:updated');
        }
    });
}
 
function carregarMunicipioAnalise(estuf, muncod) {
    if(estuf != '' && $('[name=itrid]:checked').val() != 1) {
        var options = jQuery('#muncod');
        options.empty();
        options.trigger('chosen:updated');
        if(!Array.isArray(estuf)){
            return;
        }
        $('#loading').hide();
        // options.append(new Option('', ''));
        jQuery.ajax({
            type: "GET",
            url: window.location.href,
            data: { requisicao:'carregarMunicipiosAnalise',estuf : estuf },
            success: function(retorno){
                // options.append(new Option('', ''));
                $.each(JSON.parse(retorno), function() {
                    options.append(new Option(this.descricao, this.codigo));
                });
                options.focus();
                options.val(muncod);
                options.trigger('chosen:updated');
            }
        });
    }
}

$('#muncod').one('focus',function() {
    console.log('on focus');
    if(Array.isArray($('#estuf').val())){
        return;
    }
    listarMunicipios();
});

jQuery(".btn-export").click(function () {
    //$btn = jQuery(this).button('loading');
    
    var itens = '';
    $(".ordemcolunas").children("div").each(function() {
        itens +=  $(this).attr('id') + ',';
        $('#selctordemcolunas').val(itens);
    });
    jQuery('[name="requisicao"]').val('exportar');
    jQuery('[name="formulario-relatorio"]').submit();
});

jQuery(".btn-pesquisar").click(function () {
    var itens = '';
    var texto = '';
    $('.loading-dialog-par3').show();
    $(".ordemcolunas").children("div").each(function() {
        itens +=  $(this).attr('id') + ',';
        $('#selctordemcolunas').val(itens);
    });
    
    $(".ordemcolunas").children("div").each(function() {
        texto +=  $(this).text() + ',';
        $('#selctordemcolunastext').val(texto);
    });

    jQuery(this).button('loading');
    jQuery('[name="requisicao"]').val('pesquisar');
    jQuery('[name="formulario-relatorio"]').submit();
});

jQuery(document).ready(function(){
    WinMoveRel();
    var dadoscol = $('#selctordemcolunas').val();
    var divOrdenacao = jQuery('#ordemcolunas');
    var dadoscoltext = $('#selctordemcolunastext').val(); 
    if(dadoscol){
        var seletorcol = dadoscol.split(',');
        var seletorcoltext = dadoscoltext.split(',');
        jQuery.each( seletorcol, function( i, val ) {
            if(val != ''){
                divOrdenacao.append('<div class="ibox contordemcol" id="' + val + '" > <div class="ibox-title contordemcolInterna " >' + seletorcoltext[i] +  '</div> </div> ');
            }
        });
    }
});

jQuery('#colunasentidades').on('change', function(){
    jQuery('#ordemcolunas').html('');
    jQuery('#colunasentidades option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = jQuery(this).text();
       id = jQuery(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    jQuery('#planejamentoentidade option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = jQuery(this).text();
       id = jQuery(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });
    
    // COLUNAS ANALISE FNDE
    jQuery('#analisefnde option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = jQuery(this).text();
       id = jQuery(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS PROCESSOS
    jQuery('#colunasprocessos option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = jQuery(this).text();
       id = jQuery(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS EMPENHOS
    jQuery('#colunasempenhos option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = jQuery(this).text();
       id = jQuery(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS TERMOS
    jQuery('#colunastermos option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = jQuery(this).text();
       id = jQuery(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS PAGAMENTO
    jQuery('#colunafinanceiro option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = jQuery(this).text();
       id = jQuery(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

});

jQuery('#planejamentoentidade').on('change', function(){
    jQuery('#ordemcolunas').html('');

    //COLUNAS ENTIDADES
    jQuery('#colunasentidades option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = jQuery(this).text();
       id = jQuery(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS PLANEJAMENTO
    jQuery('#planejamentoentidade option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = jQuery(this).text();
       id = jQuery(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS ANALISE FNDE
    jQuery('#analisefnde option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = jQuery(this).text();
       id = jQuery(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS PROCESSOS
    jQuery('#colunasprocessos option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = jQuery(this).text();
       id = jQuery(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS EMPENHOS
    $('#colunasempenhos option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS TERMOS
    jQuery('#colunastermos option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = jQuery(this).text();
       id = jQuery(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS PAGAMENTO
    jQuery('#colunafinanceiro option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = jQuery(this).text();
       id = jQuery(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

});


 $('#planejamentoentidade').on('change', function(){
    $('#ordemcolunas').html('');

    //COLUNAS ENTIDADES
    $('#colunasentidades option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS PLANEJAMENTO
    $('#planejamentoentidade option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS ANALISE FNDE
    $('#analisefnde option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS PROCESSOS
    $('#colunasprocessos option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS EMPENHOS
    $('#colunasempenhos option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS TERMOS
    $('#colunastermos option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS PAGAMENTO
    $('#colunafinanceiro option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

});

$('#analisefnde').on('change', function(){
    $('#ordemcolunas').html('');

    //COLUNAS ENTIDADES
    $('#colunasentidades option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS PLANEJAMENTO
    $('#planejamentoentidade option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS ANALISE FNDE
    $('#analisefnde option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS PROCESSOS
    $('#colunasprocessos option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS EMPENHOS
    $('#colunasempenhos option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS TERMOS
    $('#colunastermos option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS PAGAMENTO
    $('#colunafinanceiro option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

});


$('#colunasprocessos').on('change', function(){
    $('#ordemcolunas').html('');

    //COLUNAS ENTIDADES
    $('#colunasentidades option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS PLANEJAMENTO
    $('#planejamentoentidade option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS ANALISE FNDE
    $('#analisefnde option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS PROCESSOS
    $('#colunasprocessos option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS EMPENHOS
    $('#colunasempenhos option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS TERMOS
    $('#colunastermos option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS PAGAMENTO
    $('#colunafinanceiro option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

});


$('#colunasempenhos').on('change', function(){
    $('#ordemcolunas').html('');

    //COLUNAS ENTIDADES
    $('#colunasentidades option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS PLANEJAMENTO
    $('#planejamentoentidade option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS ANALISE FNDE
    $('#analisefnde option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS PROCESSOS
    $('#colunasprocessos option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS EMPENHOS
    $('#colunasempenhos option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS TERMOS
    $('#colunastermos option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS PAGAMENTO
    $('#colunafinanceiro option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

});


$('#colunastermos').on('change', function(){
    $('#ordemcolunas').html('');

    //COLUNAS ENTIDADES
    $('#colunasentidades option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS PLANEJAMENTO
    $('#planejamentoentidade option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS ANALISE FNDE
    $('#analisefnde option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS PROCESSOS
    $('#colunasprocessos option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS EMPENHOS
    $('#colunasempenhos option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS TERMOS
    $('#colunastermos option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS PAGAMENTO
    $('#colunafinanceiro option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

});


jQuery('#colunafinanceiro').on('change', function(){
    jQuery('#ordemcolunas').html('');

    //COLUNAS ENTIDADES
    jQuery('#colunasentidades option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS PLANEJAMENTO
    jQuery('#planejamentoentidade option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS ANALISE FNDE
    jQuery('#analisefnde option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS PROCESSOS
    jQuery('#colunasprocessos option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS EMPENHOS
    jQuery('#colunasempenhos option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS TERMOS
    jQuery('#colunastermos option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

    // COLUNAS PAGAMENTO
    jQuery('#colunafinanceiro option:selected').each(function() {
       var divOrdenacao = jQuery('#ordemcolunas');
       valor = $(this).text();
       id = $(this).val();
       divOrdenacao.append('<div class="ibox contordemcol" id="' + id + '" > <div class="ibox-title contordemcolInterna " >' + valor +  '</div> </div> ');
    });

});

</script>
