<?php
require APPRAIZ . 'demandas/model/Demandantes.class.inc';

class Demandantes extends mvc\Controle
{
	public function iniciarFluxo()
	{
        $this->visao->setCabecalho('includes/cabecalho_bootstrap_v4.inc', true);
        $this->visao->setRodape('includes/rodape_bootstrap_v4.inc', true);
        $this->visao->setarIncludeJS('/estrutura/js/funcoes.js');
        $this->visao->setarIncludeJS('/includes/bootstrap-validator/validator.js');
        $this->visao->setarIncludeJS('/includes/bootstrap-validator/validator.min.js');
        
	    switch ($_REQUEST['requisicao']) {
	        case 'acessarDemandante':
	            $this->acessarDemandante();
	            die;
	        case 'abreListaGetor':
                ob_clean();
                $_REQUEST['gestor'] = 'S';
                $this->carregarListaGestor($_REQUEST);
                die;
	        case 'carregarCardapio':
	            ob_clean();
	            $this->carregarCardapio($_REQUEST);
	            die;
	        case 'carregarItensCardapio':
	            ob_clean();
	            $this->carregarItensCardapio($_REQUEST);
	            die;
	        case 'salvar_demanda_itens':
	            $demid = $this->salvarDemanda( $_REQUEST );
	            $url = 'demandas.php?modulo=demandantes/cadastarDemanda&acao=A&ddtid='.$_GET['ddtid'].'&demid='.$demid;
	            $this->sucesso($url);
	            die;
	        case 'incluirLinhaItensDemanda':
	            $this->incluirLinhaItensDemanda( $_REQUEST );
	            die;
	        case 'salva_pessoa_juridica':
	            $this->salva_pessoa_juridica( $_REQUEST );
	            if( $_REQUEST['enfid'] && $_REQUEST['tipo'] ){
	                $url = 'demandas.php?modulo=demandantes/verDadosDemandante&acao=A&ddtid='.$_GET['ddtid'].'&enfid='.$_GET['enfid'].'&tipo='.$_GET['tipo'].'&funcao='.$_GET['funcao'];
	            } else {
	                $url = 'demandas.php?modulo=demandantes/verDadosDemandante&acao=A&ddtid='.$_GET['ddtid'];
	            }
	            $this->sucesso($url);
	            die;
	        case 'salva_pessoa_fisica':
	            $this->salva_pessoa_fisica( $_REQUEST );
	            $url = 'demandas.php?modulo=demandantes/verDadosDemandante&acao=A&ddtid='.$_GET['ddtid'].'&enfid='.$_GET['enfid'].'&tipo='.$_GET['tipo'].'&funcao='.$_GET['funcao'];
	            $this->sucesso($url);
	            die;
	        case 'excluirPessoaFisica':
	            $this->excluir_pessoa_fisica( $_GET['ensid'] );
	            $url = 'demandas.php?modulo=demandantes/verDadosDemandante&acao=A&ddtid='.$_GET['ddtid'].'&enfid='.$_GET['enfid'].'&tipo='.$_GET['tipo'].'&funcao='.$_GET['funcao'];
	            $this->sucesso($url);
	            die;
	        case 'excluirPessoaJuridica':
	            $this->excluir_pessoa_juridica( $_GET['enjid'] );
	            $url = 'demandas.php?modulo=demandantes/verDadosDemandante&acao=A&ddtid='.$_GET['ddtid'].'&enfid='.$_GET['enfid'].'&tipo='.$_GET['tipo'].'&funcao='.$_GET['funcao'];
	            $this->sucesso($url);
	            die;
	        case 'montar_tela_pessoa':
	            ob_clean();
	            
	            if( $_REQUEST['tipo'] == 'F' ){
	                echo $this->monta_tela_pessoafisica( $_REQUEST );
	            } else {
	                $_REQUEST['enjid'] = $_REQUEST['enjid_pai'];
	                $_REQUEST['enfid'] = $_REQUEST['enfid_pai'];
	                $_REQUEST['ejfid'] = $_REQUEST['ejfid_pai'];
	               echo $this->monta_tela_pessoajuridica( $_REQUEST );
	            }
	            die;
	    }
	}	
	
	public function acaoListar()
	{    
	    
	    if( $_REQUEST['enfid'] ){
	        $_POST['enfid'] = $_REQUEST['enfid'];
	    } else {
	        $_POST['enfid'] = 1;
	    }
	    
	    $moDemandante = new Demandantes_Model();
	    $arDado = $moDemandante->getFuncaoEntidade($_POST['enfid']);
	    $arDado['enfid'] = $_REQUEST['enfid'];
	    
	    $arDado['abas'] = criaAbaListaDemandantes();	    
	    
	    if( $_POST['pesquisar'] ){
	        $arDado['sql_demandantes'] = $moDemandante->listaEntidade($_POST);
	    }
	    
	    $this->visao->carregarDados($arDado);
	    
	    $this->visao->setarTituloCabecalho(' ');
	    $this->visao->incluirVisao('formListaDemandantes');
	}
	
	public function acaoVerPainel()
	{
	    global $db;
	    
	    $moDemandante = new Demandantes_Model();
	    
	    $arDado = $db->pegaLinha($moDemandante->listaEntidade($_REQUEST));
	    
	    $this->visao->carregarDados($arDado);
	    
	    $this->visao->setarTituloCabecalho(' ');
	    $this->visao->incluirVisao('formPainelDemandante');
	}
	
	public function acaoVerDadosDemandante()
	{	    
	    $moDemandante = new Demandantes_Model();
	    
	    $arDado['demandante'] = $moDemandante->getVariaveisPessoaJuridicaPai( $_REQUEST['ddtid'] );
	    
	    $arDado['funcao'] = $_REQUEST['funcao'];
	    
	    /*$arDado['verifica_gestor'] = '';
	    if( $_REQUEST['enfid'] ){
	        $arRepresentante = $moDemandante->getDadosRepresentante($_REQUEST['enfid']);
	        $arDado['qtd_representante'] = $arRepresentante['enrqtdcadastrados'];
	        $arDado['enrtipo'] = $arRepresentante['enrtipo'];
	        
	        $arDado['verifica_gestor'] = $moDemandante->verificaExisteGestor($_REQUEST['enfid']);
	    }
	    
	    $arDado['municipio'] = array();
	    $arDado['prefeitura'] = array();
	    if( $_REQUEST['tipo'] == 'F' ){
	        $arDado['sql_pessoafisica'] = $moDemandante->getDadosPessoaFisica($_REQUEST['ddtid'], $_REQUEST['ensid'], true, $_REQUEST['enfid'], false, $arDado['enrtipo']);
	        $arDado['qtd_ja_cadastrado'] = $moDemandante->getDadosPessoaFisica($_REQUEST['ddtid'], $_REQUEST['ensid'], false, $_REQUEST['enfid'], true, $arDado['enrtipo']);
	        
	        $arDado['ejfid'] = $moDemandante->getIdVinculoDemandante($_REQUEST['ddtid']);
	        
	    } elseif( $_REQUEST['tipo'] == 'J' && !in_array($_REQUEST['enfid'], array(1) ) ){
	        $arDado['sql_pessoajuridica'] = $moDemandante->getDadosRepresentanteJuridica($_REQUEST['ddtid'], true, $_REQUEST['enjid'], $_REQUEST['enfid']);
	        $arDado['qtd_ja_cadastrado'] = $moDemandante->getDadosRepresentanteJuridica($_REQUEST['ddtid'], false, $_REQUEST['enjid'], $_REQUEST['enfid'], true);
	        $arDado['ejfid_pai'] = $arDado['ejfid'];
	    } else {
	        $arDado['sql_pessoajuridica'] = $moDemandante->getDadosPessoaJuridica($_REQUEST['ddtid'], true, $_REQUEST['enjid'], $_REQUEST['enfid']);
	        $arDado['qtd_ja_cadastrado'] = $moDemandante->getDadosPessoaJuridica($_REQUEST['ddtid'], false, $_REQUEST['enjid'], $_REQUEST['enfid'], true);
	    }*/  
	    
	    if( $_REQUEST['enfid'] ){
	        $arRepresentante = $moDemandante->getDadosRepresentante($_REQUEST['enfid']);
	        $arDado['qtd_representante'] = $arRepresentante['enrqtdcadastrados'];
	        $arDado['enrtipo'] = $arRepresentante['enrtipo'];
	        
	        $arDado['verifica_gestor'] = $moDemandante->verificaExisteGestor($_REQUEST['enfid']);
	    }
	    
	    if( $_REQUEST['tipo'] == 'F' )
	    {
	        $arrParam = array('ddtid'          => $_REQUEST['ddtid'], 
	                           'ensid'         => '', 
	                           'retornoSQL'    => true, 
	                           'enfid'         => $_REQUEST['enfid'], 
	                           'retornoCount'  => false, 
	                           'enrtipo' => $arDado['enrtipo'], 
	                           'funcao' => $_REQUEST['funcao'], 
	                           'enrgestor'    => false, 
	                           'enjid'         => $arDado['demandante']['enjid']);
	        
	        $arDado['sql_pessoafisica'] = $moDemandante->getDadosPessoaFisica( $arrParam );
	        
	        $arrParam['retornoSQL'] = false;
	        $arrParam['retornoCount'] = true;	        
	        $arDado['qtd_ja_cadastrado'] = $moDemandante->getDadosPessoaFisica( $arrParam );
	        $arFuncao = $moDemandante->getFuncaoEntidade($_GET['enfid']);
	    }else{
	        
    	    $arrParam = array('ddtid' => $_REQUEST['ddtid'], 
    	                      'retornoSQL' => true, 
    	                      'enjid' => $arDado['demandante']['enjid'], 
    	                      'enfid' => $arDado['demandante']['enfid'], 
    	                      'enrtipo' => $arDado['enrtipo'],
    	                      'funcao' => $_REQUEST['funcao'], 
    	                      'retornoCount' => false);
    	    
    	    if( $_REQUEST['funcao'] == 'R' ) #Carrega informações do representante
    	    {
    	        $arrParam['enjid'] = '';
    	        $arrParam['enfid'] = $_GET['enfid'];
    	        $arDado['sql_pessoajuridica'] = $moDemandante->getDadosRepresentanteJuridica( $arrParam );
    	        $arrParam['retornoSQL'] = false;
    	        $arrParam['retornoCount'] = true;
    	        $arDado['qtd_ja_cadastrado'] = $moDemandante->getDadosRepresentanteJuridica( $arrParam );
    	        $arFuncao = $moDemandante->getFuncaoEntidade($_GET['enfid']);
    	        
    	    }else{
                
        	    $arDado['sql_pessoajuridica'] = $moDemandante->getDadosPessoaJuridica( $arrParam );    	    
        	    $arrParam['retornoSQL'] = false;
        	    $arrParam['retornoCount'] = true;
        	    $arDado['qtd_ja_cadastrado'] = $moDemandante->getDadosPessoaJuridica( $arrParam );
        	    
        	    $arFuncao = $moDemandante->getFuncaoEntidade($arDado['demandante']['enfid']);   	    
    	    }
	    }
	    
	    $arDado['qtd_ja_cadastrado'] = ($arDado['qtd_ja_cadastrado'] ? (int)$arDado['qtd_ja_cadastrado'] : (int)0);
	    
	    $arDado['funcao_entidade'] = $moDemandante->getDadosFuncaoEntidade($_REQUEST['ddtid']);
	    
	    $arDado['entidades'] = $arFuncao['enfdsc'];
	    
	    $this->visao->carregarDados($arDado);
	    
	    $this->visao->setarTituloCabecalho(' ');
	    $this->visao->incluirVisao('formDadosDemandante');
	}
	
	public function acaoListarDemanda()
	{
	    global $db;
	    
	    $moDemandante = new Demandantes_Model();
	    $arDado = $db->pegaLinha($moDemandante->listaEntidade($_REQUEST));
	    
	    if( $_REQUEST['requisicao'] == 'pesquisar-demanda' ){
	        $arDado['sql_demandar'] = $moDemandante->listarDemandas($_REQUEST);
	    }
	    $arDado['abasDemanda'] = criaAbaDemanda( $_REQUEST['ddtid'] );
	    $this->visao->carregarDados($arDado);
	    
	    $this->visao->setarTituloCabecalho(' ');
	    $this->visao->incluirVisao('formListaDemandas');
	}
	
	public function acaoCadastarDemanda()
	{
	    global $db, $arDado;
	    
	    $moDemandante = new Demandantes_Model();
	    
	    $arDado = $db->pegaLinha($moDemandante->listaEntidade($_REQUEST));
	    $arrDemanda = $moDemandante->getDemandaPorID($_REQUEST['demid']);
	    
	    $arDado['dcoid'] = $arrDemanda['dcoid'];
	    $arDado['dctid'] = $arrDemanda['dctid'];
	    $arDado['demid'] = $arrDemanda['demid'];
	    
	    $arDado['arrItensCardapio'] = $moDemandante->carregarItensCardapio($arDado['dcoid']);
	    
	    if( $_POST['pesquisar'] ){
	        $arDado['sql_demandar'] = $moDemandante->listaEntidade($_POST);
	    }
	    
	    $arDado['arrItensDemanda'] = array();
	    if( $arDado['demid'] ) $arDado['arrItensDemanda'] = $moDemandante->carregarItensVinculadosDemandas($arDado['demid']);
	    
	    $arDado['abasDemanda'] = criaAbaDemanda( $_REQUEST['ddtid'] );
	    
	    $arDado['html_itens'] = $this->visao->capturarHtmlVisaoAcessoria('formDemandaItens');
	    
	    $this->visao->carregarDados($arDado);
	    $this->visao->setarTituloCabecalho(' ');
	    $this->visao->incluirVisao('formCadastroDemandas');
	}
	
	public function desenharTela($menu)
	{
	    $pastaDadosUnidade = 'demandas/modulos/visao/';
	    
	    switch ($menu) {
	        case'prefeito':
	            include APPRAIZ . $pastaDadosUnidade . 'prefeito.php';
	            break;
	        case 'prefeitura':
	            include APPRAIZ . $pastaDadosUnidade . 'prefeitura.php';
	            break;
	        case 'equipe':
	            include APPRAIZ . $pastaDadosUnidade . 'equipe.php';
	            break;
	        default:
	            include APPRAIZ . $pastaDadosUnidade . 'prefeitura.php';
	            break;
	    }
	    
	}
	
	public function carregarCardapio($arrPost) {
	    $moDemandante = new Demandantes_Model();
	    
	    $arrDados = $moDemandante->carregarCardapio($arrPost);
	    
	    echo simec_json_encode($arrDados);
	}
	
	public function carregarListaGestor($arrPost) {
	    global $db;
	    $moDemandante = new Demandantes_Model();
	    
	    $pastaDadosUnidade = 'demandas/modulos/visao/';
	    
	    $efjid = $arrPost['dados'][0];
	    $enjid = $arrPost['dados'][1];
	    
	    $arDado['funcao'] = $_REQUEST['funcao'];
	    	    
	    $arDado['ejfid'] = $efjid;
	    $enfidfilho = $moDemandante->verificaExisteGestor($arrPost['enfid']);
	    
	    $enfidpai = $arrPost['enfid'];
	    
	    $_REQUEST['enfid'] = $enfidfilho;
	    $_REQUEST['tipo'] = 'F';
	    $arrPost['enfid'] = $enfidfilho;
	    $arrPost['tipo'] = 'F';
	    
	    if( $arrPost['enfid'] ){
	        $arRepresentante = $moDemandante->getDadosRepresentante($arrPost['enfid'], $enfidpai);	        
	        $arDado['qtd_representante'] = $arRepresentante['enrqtdcadastrados'];
	        $arDado['enrtipo'] = $arRepresentante['enrtipo'];
	        $arDado['enrgestor'] = ($arRepresentante['enrgestor'] == true ? 'S' : 'N');
	    }
	    
	    $arFuncao = $moDemandante->getFuncaoEntidade($enfidfilho);
	    $arDado['entidades'] = $arFuncao['enfdsc'];	    
	    
	    $arrParam = array('ddtid'          => $arrPost['ddtid'],
                	        'ensid'         => $arrPost['ensid'],
                	        'retornoSQL'    => true,
	                        'enfid'         => $arrPost['enfid'],
                	        'retornoCount'  => false,
	                        'enrtipo' => $arDado['enrtipo'],
                	        'enrgestor'    => true,
	                        'enjid'         => $enjid);
	    
	    $arDado['sql_pessoafisica'] = $moDemandante->getDadosPessoaFisica( $arrParam );
	    $arrParam['retornoSQL'] = false;
	    $arrParam['retornoCount'] = true;
	    $arDado['qtd_ja_cadastrado'] = $moDemandante->getDadosPessoaFisica( $arrParam );
	    
	    $this->visao->carregarDados($arDado);
	    $this->visao->setarTituloCabecalho(' ');
	    echo $this->visao->incluirVisaoAcessoria('formListaPessoaFisica');
	}
	
	public function incluirLinhaItensDemanda($arrPost) {
	    global $arDado;
	    
	    $moDemandante = new Demandantes_Model();
	    
	    $arrItensDemanda = $moDemandante->carregarLinhaItensDemandas($arrPost['cidid']);    	    
	    $arrPost['dciid_linha'] = $arrItensDemanda['dciid'];
	    $arrPost['dcivalor_linha'] = $arrItensDemanda['dcivalor'];
	    
	    $moDemandante->salvarItemCardapioDemandaLinha($arrPost);
	    $moDemandante->salvarItemCardapioDemanda($arrPost);
	    
	    $arDado['arrItensCardapio'] = $moDemandante->carregarItensCardapio($arrPost['dcoid']);
	    
	    $arDado['arrItensDemanda'] = array();
	    if( $arrPost['demid'] ) $arDado['arrItensDemanda'] = $moDemandante->carregarItensVinculadosDemandas($arrPost['demid']);
	    
	    $arDado['html_itens'] = $this->visao->capturarHtmlVisaoAcessoria('formDemandaItens');
	    echo $arDado['html_itens'];
	}
	
	public function monta_tela_pessoajuridica($arrPost) {
	    
	    $moDemandante = new Demandantes_Model();
	    
	    $arDado['demandante'] = $moDemandante->getVariaveisPessoaJuridicaPai( $arrPost['ddtid'] );
	    $arDado['pessoa_juridica'] = array();
	    
	    $arDado['funcao'] = $_GET['funcao'];
	    
	    //Verifica se a pessoa e juridica e se a funcao e de representante
	    if( $arDado['funcao'] == 'R' ){
	        $arFuncao = $moDemandante->getFuncaoEntidade( $arrPost['enfid'] );
	        
	        $arDado['ddtid'] = $arrPost['ddtid'];
	        $arDado['enjid'] = $arrPost['enjid'];
	        $arDado['enfid'] = $arrPost['enfid'];
	        $arDado['ejfid'] = $arrPost['ejfid'];
	           
	        $arDado['entidades'] = $arFuncao['enfdsc'];
	        $arDado['demandante']['enfid'] = $arrPost['enfid'];
	        
	        $arrParam = array('ddtid' => $arrPost['ddtid'],
            	              'retornoSQL' => false,
	                          'enjid' => $arrPost['enjid'],
	                          'enfid' => $arrPost['enfid'],
	                          'funcao' => $arrPost['funcao'],
            	              'retornoCount' => false);
	        if( $arrPost['enjid'] )
	           $arDado['pessoa_juridica'] = $moDemandante->getDadosRepresentanteJuridica( $arrParam );
	    } else {
	        
	        $arDado['ddtid'] = $_GET['ddtid'];
	        $arDado['enjid'] = $arDado['demandante']['enjid'];
	        $arDado['enfid'] = $arDado['demandante']['enfid'];
	        $arDado['ejfid'] = $arDado['demandante']['ejfid'];
	        
	        $arFuncao = $moDemandante->getFuncaoEntidade( $arDado['demandante']['enfid'] );
	        $arDado['entidades'] = $arFuncao['enfdsc'];
	        
	        $arrParam = array('ddtid' => $arrPost['ddtid'],
            	            'retornoSQL' => false,
            	            'enjid' => $arDado['demandante']['enjid'],
            	            'enfid' => $arDado['demandante']['enfid'],
	                        'funcao' => $arrPost['funcao'],
            	            'retornoCount' => false);
	        
	        if( $arrPost['enjid'] )
	           $arDado['pessoa_juridica'] = $moDemandante->getDadosPessoaJuridica( $arrParam );
	    }
	    
	    $arDado['funcao_entidade'] = $moDemandante->getDadosFuncaoEntidade($arrPost['ddtid']);
	    
	    $this->visao->carregarDados($arDado);
	    
	    $arDado['html_pessoa'] = $this->visao->capturarHtmlVisaoAcessoria('formPessoaJuridica');
	    
	    echo $arDado['html_pessoa'];
	}
	
	public function monta_tela_pessoafisica($arrPost) {
	    
	    $moDemandante = new Demandantes_Model();
	    $arDado = array();
	    
	    $arDado['demandante'] = $moDemandante->getVariaveisPessoaJuridicaPai( $_REQUEST['ddtid'] );
	    
	    $arDado['funcao'] = $_GET['funcao'];
	    
	    $arFuncao = $moDemandante->getFuncaoEntidade( $arrPost['enfid'] );
	    
	    $arDado['entidades'] = $arFuncao['enfdsc'];
	    $arDado['demandante']['enfid'] = $_GET['enfid'];
	    $arDado['enrgestor'] = $arrPost['enrgestor'];
	    $enrgestor = ($arrPost['enrgestor'] == 'S' ? true : false);
	    
	    $arDado['prefeitura'] = array();
	    if( $arrPost['ensid']  ){
	        
	        $arrParam = array('ddtid'          => $_REQUEST['ddtid'],
	                           'ensid'         => $arrPost['ensid'],
                	           'retornoSQL'    => false,
                	           'enfid'         => $_REQUEST['enfid'],
                	           'retornoCount'  => false,
	                           'enrtipo' => $arrPost['enrtipo'],
	                           'enrgestor'    => $enrgestor,
	                           'enjid'         => $arrPost['enjid']);
	        
	        $arDado['pessoa_fisica'] = $moDemandante->getDadosPessoaFisica( $arrParam );
	        $arDado['pessoa_fisica'] = $arDado['pessoa_fisica'][0];
	    }
	    
	    $arDado['ddtid'] = $arrPost ['ddtid'];
	    $arDado['ensid'] = $arrPost ['ensid'];
	    $arDado['enfid'] = $arrPost ['enfid'];
	    if( $arrPost['enrgestor'] == 'S' ) $arDado['ejfid'] = ($arrPost['ejfid']);
	    else $arDado['ejfid'] = $arDado['pessoa_fisica']['ejfid'];
	    
	    $arDado['funcao_entidade'] = $moDemandante->getDadosFuncaoEntidade($arrPost['ddtid']);
	    $arDado['enrtipo'] = $arrPost['enrtipo'];
	    
	    $this->visao->carregarDados($arDado);
	    
	    $arDado['html_pessoa'] = $this->visao->capturarHtmlVisaoAcessoria('formPessoaFisica');
	    
	    echo $arDado['html_pessoa'];
	}
	
	public function carregarItensCardapio($arrPost) {
	    $moDemandante = new Demandantes_Model();
	    
	    $arrDados = $moDemandante->carregarItensCardapio($arrPost);
	    
	    echo simec_json_encode($arrDados);
	}
	
	public function salvarDemanda($arrPost) {
	    $moDemandante = new Demandantes_Model();
	    $arrPost['docid'] = $this->pegaIdWorkFlow($arrPost['demid']);
	    
	    if( $arrPost['demid'] ){
	        //$moDemandante->atualizarDemanda($arrPost);
	        if( $arrPost['deiid'] ){
	           $moDemandante->salvarItemCardapioDemanda($arrPost);
	        }
	        $demid = $arrPost['demid'];
	    } else {
	       $demid = $moDemandante->salvarDemanda($arrPost);
	    }
	    
	    return $demid;
	}
	
	public function pegaIdWorkFlow($demid = ''){
	    global $db;
	    
	    require_once APPRAIZ . '/includes/workflow.php';
	    
	    if ($demid) {
	        $moDemandante = new Demandantes_Model();
	        $arDados = $moDemandante->listarDemandas(array($demid));
	        if( is_array($arDados) ){
	            $docid = $arDados['docid'];
	        } else {
	            $arDados = $db->pegaLinha($arDados);
	            $docid = $arDados['docid'];
	        }
	        
	    } else {
	        $docdsc = "Fluxo de Demandas: demid - " . $demid;
	        $docid = wf_cadastrarDocumento(TPDID_FLUXO_DEMANDA, $docdsc);
	    }
	    return $docid;
	}

	public function salva_pessoa_juridica( $arrPost ){
	    
	    $moDemandante = new Demandantes_Model();
	    return $moDemandante->salva_pessoa_juridica( $arrPost );
	}
	
	public function salva_pessoa_fisica( $arrPost ){
	    
	    $moDemandante = new Demandantes_Model();
	    return $moDemandante->salva_pessoa_fisica( $arrPost );
	}
	
	public function excluir_pessoa_fisica( $ensid ){
	    
	    $moDemandante = new Demandantes_Model();
	    return $moDemandante->excluir_pessoa_fisica( $ensid );
	}
	
	public function excluir_pessoa_juridica( $enjid ){
	    
	    $moDemandante = new Demandantes_Model();
	    return $moDemandante->excluir_pessoa_juridica( $enjid );
	}	
}