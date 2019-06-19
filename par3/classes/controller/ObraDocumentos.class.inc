<?php
/**
 * Classe de controle do  obra_documentos
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   RENNER NASCENTES TANIZAKI <rennertanizaki@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 10-07-2017
 * @link     no link
 */



/**
 * Par3_Controller_Obra_documentos
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 10-07-2017
 * @link     no link
 */
class Par3_Controller_ObraDocumentos
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_ObraDocumentos($_GET['odoid']);
        $this->modelHistorico = new Par3_Model_ObraDocumentosHistorico();
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($arrPost)
    {
        $url = '';
        if(isset($_REQUEST['toReturn'])){
            $url = "par3.php?modulo=principal/planoTrabalho/obras&acao=A&menu=documento_anexo&inuid={$arrPost['inuid']}&inpid={$arrPost['inpid']}&obrid={$arrPost['obrid']}&toReturn={$_REQUEST['toReturn']}";

        }else{
            $url = "par3.php?modulo=principal/planoTrabalho/obras&acao=A&menu=documento_anexo&inuid={$arrPost['inuid']}&inpid={$arrPost['inpid']}&obrid={$arrPost['obrid']}";

        }

        if($_FILES['arqid']['error'] == UPLOAD_ERR_NO_FILE){
            simec_redirecionar($url, 'error', "Adicione um arquivo");
        }
        $mdTipoDoc = new Par3_Model_ObraTipoDocumento($arrPost['otdid']);

        $tamMin                         = $mdTipoDoc->otdtamanho_minimo*1000;
        $mdTipoDoc->otdtamanho_minimo   = $mdTipoDoc->otdtamanho_minimo ? $mdTipoDoc->otdtamanho_minimo : 0;
        $tamMin                         = $tamMin ? $tamMin : 0;

        $mdTipoDoc->otdtamanho_maximo   = $mdTipoDoc->otdtamanho_maximo ? $mdTipoDoc->otdtamanho_maximo : 0;
        $tamMax                         = $mdTipoDoc->otdtamanho_maximo*1000;

        if ($_FILES['arqid']['name']) {
            if(
                ($_FILES['arqid']['size'] < $tamMin || $_FILES['arqid']['size'] > $tamMax) &&
                ($tamMin > 0 || $tamMax > 0)
            ){
                simec_redirecionar($url, 'error', "O tamanho do arquivo deve estar entre {$mdTipoDoc->otdtamanho_minimo}KB e {$mdTipoDoc->otdtamanho_maximo}KB.");
                die();
            }

            $_FILES['arqid']['name'] = remove_acentos($_FILES['arqid']['name']);

            $file = new FilesSimec("documentoarquivo", [], 'par3');

            $file->setUpload(null, 'arqid', false);
            $arqid = $file->getIdArquivo();
        }

        $arrayDados['obrid'] = $arrPost['obrid'];
        $arrayDados['arqid'] = $arqid;
        $arrayDados['otdid'] = $arrPost['otdid'];
        $arrayDados['odostatus'] = 'A';

        try {
            $this->model->popularDadosObjeto($arrayDados);
            $novoid = $this->model->salvar();
            $this->model->commit();

            $modelo = new Par3_Model_ObraDocumentos($novoid);

            $this->modelHistorico->gravarHistorico($modelo,'UPDATE');

            $this->atualizarPendenciaAnaliseDocumento($arrPost['obrid'], $arrPost['otdid']);

            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
        if ($novoid) {
            simec_redirecionar($url, 'success');
        }
        simec_redirecionar($url, 'success');
    }

    public function remover($arrPost)
    {
        $arrayDados['odoid'] = $arrPost['odoid'];
        $arrayDados['odostatus'] = 'I';

        try {
            $this->model->popularDadosObjeto($arrayDados);
            $otdid = $this->model->otdid;
            $novoid = $this->model->salvar();
            $this->model->commit();

            $modelo = new Par3_Model_ObraDocumentos($novoid);

            $this->modelHistorico->gravarHistorico($modelo,'UPDATE');

            $this->atualizarPendenciaAnaliseDocumento($arrPost['obrid'], $modelo->otdid);

            return 'true';
        } catch (Simec_Db_Exception $e) {
            return 'false';
        }
    }
   /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['odoid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&odoid={$id}";
        $obra_documentos = new Par3_Model_Obra_documentos($id);
        try{
             $obra_documentos->Xstatus = 'I';
             $obra_documentos->Xdtinativacao = date('Y-m-d H:i:s');
             $obra_documentos->Xusucpfinativacao = $_SESSION['usucpf'];

             $obra_documentos->salvar();
             $obra_documentos->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }

    public function listaDocumentosFNDE($arrPost) {
    	global $disabled;
    	$model = new Par3_Model_ObraDocumentos ();
    
    	$sql = $model->montarSQLSimplesNotaFNDE ( $arrPost );
    
    	$cabecalho = array (
    			'Documento de propriedade',
    			'Nota PF-FNDE',
    			'Parecer',
    			'Situação',
//     			'O que aconteceu',
    			'Quem fez',
    			'Quando fez'
    	);
    	$esconderColunas = array (
    			'otdid',
    			'arqid',
    			'arqid_nota',
    			'adpid'
    	);

    	$arPerfil = array(
    		PAR3_PERFIL_ADMINISTRADOR, 
    		PAR3_PERFIL_SUPER_USUARIO, 
    		PAR3_PERFIL_ANALISTA_PROFE, 
    		PAR3_PERFIL_PROCURADOR_PROFE,
    		PAR3_PERFIL_ANALISTA_CGEST,
    		PAR3_PERFIL_COORDENADOR_CGEST
    	);
    	
    	if ( !possuiPerfil( $arPerfil ) ){
    		unset($cabecalho[4]);
    		$esconderColunas[] = 'usunome';
    	}
    	
    	$tratamentoListaVazia = ($arrPost ['req'] == 'xls' || $arrPost ['req'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;
    
    	$listagem = new Simec_Listagem ( Simec_Listagem::RELATORIO_PAGINADO );
    	$listagem->setCabecalho ( $cabecalho );
    	$listagem->setId ( 'obraDocumentoFNDE' );
    	$listagem->esconderColunas ( $esconderColunas );
    	$listagem->setQuery ( $sql );
    	$listagem->setTotalizador ( Simec_Listagem::TOTAL_QTD_REGISTROS );
    
    	$listagem->addCallbackDeCampo('arquivo', 'formatarDocumentoObraAnexo');
    	$listagem->addCallbackDeCampo('arquivo_nota', 'formatarDocumentoNotaFNDE');
    	
    	$listagem->render ( $tratamentoListaVazia );
    }    
    
    public function listaDocumentos($arrPost)
    {
        global $disabled;
        $model   = new Par3_Model_ObraDocumentos();
        $mObra = new Par3_Model_Obra();
        $edicaoHabilitada = $mObra->habilitarEdicaoPorPerfilEstadoDocumento($_GET['obrid']);
        $analise = new Par3_Model_AnaliseEngenhariaObra();
        //VERIFICAR SE PODE HABILITAR PARA MOSTRAR ANÁLISE
        $habilitarAnalise = $analise->permiteEscreverAnalise($_GET['obrid']);



        $sql = $model->montarSQLSimples($arrPost);

        $cabecalho = array('Item', 'Descrição', 'Anexo(s)', 'Ações', 'Ajuda', 'Modelo');
        $esconderColunas = array('arqid', 'obrid', 'arqdata', 'odoid','obratipo');

        $cabecalho       = !$edicaoHabilitada? array('Item', 'Descrição', 'Anexo(s)', 'Ajuda', 'Modelo'): $cabecalho;
        $esconderColunas = !$edicaoHabilitada? array('arqid', 'obrid', 'arqdata', 'odoid','obratipo','otdid'): $esconderColunas;

        if($habilitarAnalise){
            $cabecalho[]       = 'Análise';
            $key = array_search('obratipo',$esconderColunas);
            unset($esconderColunas[$key]);

        }

        $tratamentoListaVazia = ($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('obraDocumento');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);

        if($edicaoHabilitada){
            $listagem->addCallbackDeCampo('otdid', 'linkAnexarDocumentoObra');
        }
        $listagem->addCallbackDeCampo('otdidajuda', 'linkArquivoDocumentoObraAjuda');
        $listagem->addCallbackDeCampo('arquivo', 'linkArquivoDocumentoObraAnexados');
        $listagem->addCallbackDeCampo('arqidmodelo', 'linkArquivoDocumentoObra');
        if($habilitarAnalise){
            $listagem->addCallbackDeCampo('obratipo', 'linkAnaliseTipoDocumento');
        }
        $listagem->render($tratamentoListaVazia);
    }

    public function retornaDadosTipoDocumento($arrPost){
        $model = new Par3_Model_ObraDocumentos();
        $sql = $model->montarSQLDadosTipoDocumento($arrPost);
        $dados = $model->pegaLinha($sql);
        return simec_json_encode($dados);
    }

    public function atualizarPendenciaAnaliseDocumento($obrid, $otdid){
        $modelAnaliseEngenharia = new Par3_Model_AnaliseEngenhariaObra();
        $analise = $modelAnaliseEngenharia->recuperarAnalisePorObra($obrid);
        $modelAnaliseResposta = new Par3_Model_AnaliseEngenhariaObraDocumentosRespostas($analise['aegid'], $otdid);
        if($modelAnaliseResposta->aed_pendencia === "S"){
            $modelAnaliseResposta->atualizarPendenciaCorrigida($modelAnaliseResposta->aedid);
        }
    }
    
    public function montaModalAnaliseProfe( $obrid, $inuid ){
        $controllerObra = new Par3_Controller_Obra($inuid, $obrid);
        $mObra = new Par3_Model_Obra($obrid);
        $mdMunicipio = new Territorios_Model_Municipio($mObra->muncod);
        
        $mdPlanejamento = new Par3_Model_IniciativaPlanejamento($mObra->inpid);
        $mdIniciativa = new Par3_Model_Iniciativa($mdPlanejamento->iniid);
        $mdIniciativaDsc = new Par3_Model_IniciativaDescricao($mdIniciativa->indid);
        
        $arrPerfil = pegaPerfils($_SESSION['usucpf']);
        ?>
    	<div class="ibox">
        	<div class="ibox-title">
                <div class="row">
                    <div class="col-md-12">
                        <h2 class="center">Análise da PF-FNDE</h2>
                    </div>
                </div>
            </div>
        	<div class="ibox-content">
        		<div class="col-lg-6">
            		<div class="row">
            			<div class="form-group " style="text-align: right;">
            				<label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Esfera: </label>
                            <div class="col-sm-7 col-md-7 col-lg-7" style="text-align: left;">
                            	<?php 
                            	if( empty($controllerObra->model->muncod) ){
                            	    echo 'Estadual';
                            	} else {
                            	    echo 'Municipal';
                            	}
                            	?>
                            </div>
            				<div style="clear:both"></div>
            			</div>
            		</div>
            		<div class="row">
            			<div class="form-group " style="text-align: right;">
            				<label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">UF: </label>
                            <div class="col-sm-7 col-md-7 col-lg-7" style="text-align: left;">
                            	<?php echo $mdMunicipio->estuf;?>
                            </div>
            				<div style="clear:both"></div>
            			</div>
            		</div>
            		<div class="row">
            			<div class="form-group" style="text-align: right;">
            				<label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Município: </label>
                            <div class="col-sm-7 col-md-7 col-lg-7" style="text-align: left;">
                            	<?php echo $mdMunicipio->mundescricao;?>
                            </div>
            				<div style="clear:both"></div>
            			</div>
            		</div>
            		<div class="row">
            			<div class="form-group" style="text-align: right;">
            				<label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Processo: </label>
                            <div class="col-sm-7 col-md-7 col-lg-7" style="text-align: left;">
                            	<?php 
                            	echo $this->model->pegaUm("select formata_num_processo(o.pronumeroprocesso) from par3.processo o
                                                            inner join par3.processoobracomposicao po on po.proid = o.proid and po.pocstatus = 'A'
                                                            where po.obrid = $obrid");
                            	?>
                            </div>
            				<div style="clear:both"></div>
            			</div>
            		</div>
            		<div class="row">
            			<div class="form-group" style="text-align: right;">
            				<label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">ID Obra: </label>
                            <div class="col-sm-7 col-md-7 col-lg-7" style="text-align: left;"><?php echo $obrid; ?></div>
            				<div style="clear:both"></div>
            			</div>
            		</div>
            		<div class="row">
            			<div class="form-group " style="text-align: right;">
            				<label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Iniciativa: </label>
                            <div class="col-sm-7 col-md-7 col-lg-7" style="text-align: left;">
                            	<?php echo $mdIniciativaDsc->inddsc ?>
                            </div>
            				<div style="clear:both"></div>
            			</div>
            		</div>
        		</div>
        		<div class="col-lg-6">
            		<div class="row">
            			<div class="form-group " style="text-align: right;">
            				<label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Tipo de Obra: </label>
                            <div class="col-sm-7 col-md-7 col-lg-7" style="text-align: left;">
                            	<?php echo $controllerObra->getComboTipoObra()[$controllerObra->model->otpid];?>
                            </div>
            				<div style="clear:both"></div>
            			</div>
            		</div>
            		<div class="row">
            			<div class="form-group" style="text-align: right;">
            				<label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Beneficiada: </label>
                            <div class="col-sm-7 col-md-7 col-lg-7" style="text-align: left;">
                            	<?php echo $controllerObra->model->obrdsc; ?>
                            </div>
            				<div style="clear:both"></div>
            			</div>
            		</div>
            		<div class="row">
            			<div class="form-group" style="text-align: right;">
            				<label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Logradouro: </label>
                            <div class="col-sm-7 col-md-7 col-lg-7" style="text-align: left;">
                            	<?php echo $controllerObra->model->obrlogradouro;?>
                            </div>
            				<div style="clear:both"></div>
            			</div>
            		</div>
            		<div class="row">
            			<div class="form-group" style="text-align: right;">
            				<label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Número: </label>
                            <div class="col-sm-7 col-md-7 col-lg-7" style="text-align: left;"><?php echo $controllerObra->model->obrnumero; ?></div>
            				<div style="clear:both"></div>
            			</div>
            		</div>
            		<div class="row">
            			<div class="form-group" style="text-align: right;">
            				<label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Complemento: </label>
                            <div class="col-sm-7 col-md-7 col-lg-7" style="text-align: left;"><?php echo $controllerObra->model->obrcomplemento; ?></div>
            				<div style="clear:both"></div>
            			</div>
            		</div>
            		<div class="row">
            			<div class="form-group" style="text-align: right;">
            				<label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Bairro: </label>
                            <div class="col-sm-7 col-md-7 col-lg-7" style="text-align: left;"><?php echo $controllerObra->model->obrbairro; ?></div>
            				<div style="clear:both"></div>
            			</div>
            		</div>
        		</div>
        	</div>
        	<div class="ibox-content">
        	<?php 
        	$sql = $this->model->sqlDocumentoAnaliseSalvo($obrid);
        	$arrRegistro = $this->model->pegaLinha($sql);
        	?>
        		<form name="form-propriedade" id="form-propriedade" class="form-horizontal" method="post" enctype="multipart/form-data">
            		<input type="hidden" name="requisicao" id="requisicao" value=""/>
            		<input type="hidden" name="method" id="method" value=""/>
            		<input type="hidden" name="adpid" id="adpid" value="<?php echo $arrRegistro['adpid']?>"/>
            		<input type="hidden" name="obrid" id="obrid" value="<?php echo $obrid;?>"/>
            		
        		<div class="row">
        			<div class="form-group" style="text-align: right;">
        				<label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Documento de propriedade: </label>
                        <div class="col-sm-7 col-md-7 col-lg-7" style="text-align: left;">
                        <?php
                        $sql = $this->model->sqlDocumentoAnalise($obrid);
                        $arrDados = $this->model->carregar($sql);
                        ?>
                        <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="tb_render" data-qtd-acoes="1">
                        	<thead>
                        		<tr>
                        			<th class="">&nbsp;</th>
                        			<th class="">Arquivo</th>
                        			<th class="">Data</th>
                        		</tr>
                        	</thead>
                        	<tbody>
<?php                           if($arrDados){
                                    foreach ($arrDados as $v) { ?>
										<tr>
                                			<td class="text-center" style="width:33px">
                                				<a href="javascript:downloadDocumento(<?php echo $v['arqid']; ?>);" title="Download do arquivo">
                                					<span class="btn btn-warning btn-sm glyphicon glyphicon-download-alt"></span>
                                				</a>
                                				<input type="hidden" name="arqid_prop[]" id="arqid_prop" value="<?php echo $v['arqid']; ?>"/>
                                			</td>
                                			<td><?php echo $v['arquivo']; ?></td>
                                			<td><?php echo $v['arqdata']; ?></td>
                                		</tr>
<?php                               }
                                }else{?>
                                	<tr class="lista-vazia">
                            			<td colspan="3">
                            				<div class="alert alert-info col-lg-8 col-lg-offset-2" role="alert">Nenhum registro encontrado</div>
                            			</td>
                            		</tr>
<?php                           }?>
                        	</tbody>
                        </table>
                        
                        </div>
        			</div>
        		</div>
        		<div class="row">
            		<div class="form-group" style="text-align: right;">
                        <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Parecer PF-FNDE: </label>
                        <div class="col-sm-7 col-md-7 col-lg-7" style="text-align: left;">
                            <textarea name="adpparecer" id="adpparecer" type="text" class="form-control" maxlengh="1000" label-for="adpparecer"><?php echo $arrRegistro['adpparecer']; ?></textarea>
                        </div>
                    </div>
                </div>
        		<div class="row">
            		<div class="form-group" style="text-align: right;">
                        <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Nota PF-FNDE: </label>
                        <div class="col-sm-7 col-md-7 col-lg-7" style="text-align: left;">
                        	<a href="par3.php?modulo=principal/listaObras&acao=A&method=download&arqid=<?php echo $arrRegistro['arqid_nota']; ?>"><?php echo $arrRegistro['arquivo']; ?></a>
                            <?php if($arrRegistro['arqid_nota']):?>
                                <input  type="hidden" name="arqid_nota" value="<?php echo $arrRegistro['arqid_nota'];?>"/>
                            <?php endif;?>
                            <input name="arqid" id="arqid" type="file" value="" class="form-control">
                        </div>
                    </div>
                </div>
                </form>
        	</div>
<?php       if ( in_array(PAR3_PERFIL_ANALISTA_PROFE, $arrPerfil) || in_array(PAR3_PERFIL_PROCURADOR_PROFE, $arrPerfil) || in_array(PAR3_PERFIL_SUPER_USUARIO, $arrPerfil) ) {?>
        	<div class="ibox-footer notprint" style="text-align: center;">
				<button type="button" id="btn-recusar-analise" data-dismiss="modal" class="btn btn-warning" onclick="salvarDocumentoAnalise('R')"><i class="fa fa-times"></i> Recusar</button>
				<button type="button" id="btn-salvar-analise" data-dismiss="modal" class="btn btn-primary" onclick="salvarDocumentoAnalise('S')"><i class="fa fa-save"></i> Salvar</button>
				<button type="button" id="btn-aprovar-analise" data-dismiss="modal" class="btn btn-success" onclick="salvarDocumentoAnalise('A')"><i class="fa fa-check"></i> Aprovar</button>
				<button type="button" id="btn-fechar" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Fechar</button>
			</div>
<?php       } else {?>
			<div class="ibox-footer notprint" style="text-align: center;">
				<button type="button" id="btn-fechar" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Fechar</button>
			</div>
<?php       }?>
        </div>
        <?php 
    }
    
    public function recusarAnaliseDocumento( $arrPost = array() ){
        
        $arrPost['adpsituacao'] = 'R';
        $this->salvarAnaliseDocumento($arrPost);
        
        $arrParamWork = array('obrid' => $arrPost['obrid']);        
        $docid = $this->model->pegaUm("select o.docid from par3.obra o where o.obrid = ".$arrPost['obrid']);
        
        $aedid = 5094; #Enviar para Em Revisão de Análise / Retorno da PROFE
        
        if(wf_alterarEstado( $docid, $aedid, 'Fluxo de Obras do PAR3', $arrParamWork )){
//            simec_redirecionar('par3.php?modulo=principal/listaObras&acao=A', 'success');
            return true;
        } else {
//            simec_redirecionar('par3.php?modulo=principal/listaObras&acao=A', 'error');
            return false;
        }

    }

    public function salvarAnaliseDocumento( $arrPost = array() ) {
        $arqid = '';
        if ($_FILES['arqid']['name']) {
            $_FILES['arqid']['name'] = remove_acentos($_FILES['arqid']['name']);
            $file = new FilesSimec("obraanalisedocumentopropriedade", [], 'par3');
            $file->setUpload(null, 'arqid', false);
            $arqid = $file->getIdArquivo();
        }

        $arrPost['arChavePrimaria']     = 'adpid';
        $arrPost['stNomeTabela']        = 'par3.obraanalisedocumentopropriedade';
        $arrPost['arAtributosTabela'] 	= array('adpid', 'obrid', 'arqid_nota', 'usucpf', 'adpparecer', 'adpdata', 'adpsituacao');

        $arrPost['arqid_nota']  = $arqid ?:$arrPost['arqid_nota'];
        $arrPost['usucpf']      = $_SESSION['usucpf'];
        $arrPost['adpdata']     = 'now()';
        $adpid = trata_salvar($arrPost);

        if( is_array($arrPost['arqid_prop']) ){
            $arrPostAnexo['arChavePrimaria']    = 'adaid';
            $arrPostAnexo['stNomeTabela']       = 'par3.obraanalisedocumentopropriedadeanexo';
            $arrPostAnexo['arAtributosTabela']  = array('adaid', 'adpid', 'arqid');

            $arrPostAnexo['adpid'] = $adpid;

            $arrAnexo = array();
//            //IMPLEMENTAÇÃO FEITA PARA EVITAR DUPLICAÇÃO DE ARQUIVOS
            if($adpid){
                $arrArqid = implode(',',$arrPost['arqid_prop']);
                $sql = "select * from par3.obraanalisedocumentopropriedadeanexo where adpid = {$adpid} and arqid in ($arrArqid) ";
                $arrAnexo = $this->model->carregar($sql);
            }
//            //*****************************************************
            if(!$arrAnexo){
                foreach ($arrPost['arqid_prop'] as $arqid_prop) {
                    $arrPostAnexo['arqid'] = $arqid_prop;
                    trata_salvar($arrPostAnexo);
                }
            }
        }

        return $this->model->commit();
    }

    public function aprovarAnaliseDocumento( $arrPost = array() ){
        $arrPost['adpsituacao'] = 'A';
        $this->salvarAnaliseDocumento($arrPost);

        $arrParamWork = array('obrid' => $arrPost['obrid']);
        $docid = $this->model->pegaUm("select o.docid from par3.obra o where o.obrid = ".$arrPost['obrid']);

        $aedid = 5249; #Retornar para Validação de deferimento / retorno da PROFE

        if(wf_alterarEstado( $docid, $aedid, 'Fluxo de Obras do PAR3', $arrParamWork )){
//            simec_redirecionar('par3.php?modulo=principal/listaObras&acao=A', 'success');
            return true;
        } else {
//            simec_redirecionar('par3.php?modulo=principal/listaObras&acao=A', 'error');
            return false;
        }
    }
}