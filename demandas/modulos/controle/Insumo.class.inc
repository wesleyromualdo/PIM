<?php
require APPRAIZ . 'demandas/model/Insumo.class.inc';
require APPRAIZ . 'demandas/model/GrupoInsumo.class.inc';
require APPRAIZ . 'demandas/model/TipoInsumo.class.inc';
require APPRAIZ . 'demandas/model/PisoInsumo.class.inc';
require APPRAIZ . 'demandas/model/UnidadeMedida.class.inc';

class Insumo extends mvc\Controle
{
    private $modelInsumo;

	public function iniciarFluxo()
	{
        $this->modelInsumo = new Insumo_Model();

	    switch ($_REQUEST['requisicao']) {
	        case 'salvarInsumo':
	            $this->acaoSalvar();
                //$this->sucesso('demandas.php?modulo=insumo/listar');
	            die;
	        case 'deletarInsumo':
	            ob_clean();
	            $this->acaoDeletar($_REQUEST);
                $this->sucesso('demandas.php?modulo=insumo/listar');
	            die;
            case'validarCodigoInsumo':
                ob_clean();
                $resInsumo = $this->modelInsumo->getInsumoCodigoSubgrupoId($_REQUEST);
                // se ja existir o código inserido não deixa o usuário continuar
                if( !empty($resInsumo) && is_array($resInsumo) ){
                    echo 'false';
                }
                die;

	    }
	}	
	
	public function acaoListar()
	{
	    $arDado = array();
	    $_REQUEST['returnsql'] = true;
        $pesquisar = $_POST['pesquisar'];
        $this->visao->pesquisar = $pesquisar;
	    
	    $arDado['sql_insumo'] = $this->modelInsumo->listaInsumo($_REQUEST);

        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Código', 'Insumo', 'Unidade de medida', 'Tipo', 'Subgrupo');
        $esconderColunas = array('co_insumo', 'co_status');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOffForm();
        $listagemSimec->setQuery($arDado['sql_insumo']);
        $listagemSimec->addAcao('edit', array('func' => 'abrirInsumo'));
        $listagemSimec->addAcao('delete', array('func' => 'deletarInsumo'));
        $listagemSimec->setAcaoComoCondicional('delete', array(array('campo' => 'co_status', 'valor' => 0, 'op' => 'diferente')) );
        $listagemSimec->setFormFiltros('formulario-insumo');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setTamanhoPagina(50);
        $listagemSimec->setCampos($arrayCabecalho);
        $this->visao->listagemSimec = $listagemSimec;

	    $this->visao->carregarDados($arDado);
	    
	    $this->visao->setarTituloCabecalho('Insumos');
	    $this->visao->incluirVisao('lista');
	}
	
	public function acaoCadastrar()
	{
        $moUnidadeMedida = new UnidadeMedida_Model();
        $moGrupoInsumo = new GrupoInsumo_Model();
        $moTipoInsumo = new TipoInsumo_Model();
        $moPisoInsumo = new PisoInsumo_Model();

	    $arDado = array();
	    
	    if($_REQUEST['co_insumo']) {
	        $arDado = $this->modelInsumo->getInsumoPorId($_REQUEST['co_insumo']);
        }

        $arDado['sql_unidade_medida'] = $moUnidadeMedida->getComboUnidadeMedida();
        $arDado['sql_tipo_insumo'] = $moTipoInsumo->getComboTipoInsumo();
        $arDado['sql_piso_insumo'] = $moPisoInsumo->getComboPisoInsumo();
        $arDado['sql_grupo_insumo_filho'] = $moGrupoInsumo->getComboGrupoInsumoFilho();

	    $this->visao->carregarDados($arDado);
	    $this->visao->setarTituloCabecalho('');
	    $this->visao->incluirVisao('cadastro');
	}

	
	public function acaoDeletar()
	{
	    $_REQUEST['co_status'] = '0';
	    $this->modelInsumo->popularDadosObjeto($_REQUEST)->salvar();
	    $this->modelInsumo->commit();
	}
	
	public function acaoSalvar()
	{
        $co_insumo = $_REQUEST['co_insumo'];
        $_REQUEST['nu_cpf_inclusao'] = $_SESSION['usucpf'];
	    //$moInsumo = new Insumo_Model();
	    //$resInsumo = $this->modelInsumo->salvarInsumo($_REQUEST);
        //$_REQUEST['co_piso_insumo'] = NULL;
        // @TODO validar regra ao alterar tipo de insumo para material
        if ( $_REQUEST['co_tipo_insumo'] == 2 ){
            $_REQUEST['co_piso_insumo'] = null;
        }

        // @TODO alterar regra de validação para executar no método antesSalvar();
//        $this->modelInsumo->popularDadosObjeto($_REQUEST);
//        $result = $this->modelInsumo->salvar(true, false, array('co_piso_insumo'), false);

        $valida = $this->modelInsumo->validaCadastroInsumo($_REQUEST);

        if (isset($valida['erro']) && ($valida['erro'] == false) ) {
            $this->modelInsumo->popularDadosObjeto($_REQUEST);
            $this->modelInsumo->salvar(false, false, array('co_piso_insumo'), false);
            $this->modelInsumo->commit();
            $this->sucesso('demandas.php?modulo=insumo/listar');
        } else {
            if ($co_insumo){
                $this->erro("demandas.php?modulo=insumo/cadastrar&co_insumo={$co_insumo}", $valida['msg']);
            } else {
                $this->erro('demandas.php?modulo=insumo/cadastrar', $valida['msg']);
            }
        }
	    
	}
	
	
}