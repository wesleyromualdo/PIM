<?php
require APPRAIZ . 'demandas/model/Insumo.class.inc';
require APPRAIZ . 'demandas/model/GrupoInsumo.class.inc';
require APPRAIZ . 'demandas/model/TipoInsumo.class.inc';
require APPRAIZ . 'demandas/model/PisoInsumo.class.inc';
require APPRAIZ . 'demandas/model/UnidadeMedida.class.inc';

class Insumo extends mvc\Controle
{
	public function iniciarFluxo()
	{	    
	    switch ($_REQUEST['requisicao']) {
	        case 'salvarInsumo':
	            $this->acaoSalvar();
	            $this->sucesso('demandas.php?modulo=insumo/listar');
	            die;
	        case 'deletarInsumo':
	            $this->acaoDeletar($_REQUEST);
	            $this->sucesso('demandas.php?modulo=insumo/listar');
	            die;

	    }
	}	
	
	public function acaoListar()
	{    
	    
	    $moInsumo = new Insumo_Model();

	    $arDado = array();
	    $_POST['returnsql'] = true;
	    
	    $arDado['sql_insumo'] = $moInsumo->listaInsumo($_POST);

	    // lista de grupos de insumos
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Código', 'Insumo', 'Unidade de medida', 'Tipo de insumo', 'Grupo insumo');
        $esconderColunas = array('co_insumo');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($arDado['sql_insumo']);
        $listagemSimec->addAcao('edit', array('func' => 'abrirInsumo'));
        $listagemSimec->addAcao('delete', array('func' => 'deletarInsumo'));
        $listagemSimec->setFormFiltros('formulario-insumo');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setTamanhoPagina(50);
        $listagemSimec->setCampos($arrayCabecalho);
        $this->visao->listagemSimec = $listagemSimec;

	    $this->visao->carregarDados($arDado);
	    
	    $this->visao->setarTituloCabecalho(' ');
	    $this->visao->incluirVisao('lista');
	}
	
	public function acaoCadastrar()
	{
	    
	    $moInsumo = new Insumo_Model();
        $moUnidadeMedida = new UnidadeMedida_Model();
        $moGrupoInsumo = new GrupoInsumo_Model();
        $moTipoInsumo = new TipoInsumo_Model();
        $moPisoInsumo = new PisoInsumo_Model();

	    $arDado = array();
	    
	    if($_REQUEST['co_insumo']) {
	        $arDado = $moInsumo->getInsumoPorId($_REQUEST['co_insumo']);
            //$arDado['sql_grupo_insumo_filho'] = $moInsumo->listaInsumoFilhoPorPaiID($_REQUEST['co_grupo_insumo']);
        }

        $arDado['sql_unidade_medida'] = $moUnidadeMedida->getComboUnidadeMedida();
        $arDado['sql_tipo_insumo'] = $moTipoInsumo->getComboTipoInsumo();
        $arDado['sql_piso_insumo'] = $moPisoInsumo->getComboPisoInsumo();
        $arDado['sql_grupo_insumo'] = $moGrupoInsumo->getComboGrupoInsumo();

	    //ver($arDado);
	    $this->visao->carregarDados($arDado);
	    
	    $this->visao->setarTituloCabecalho(' ');
	    $this->visao->incluirVisao('cadastro');
	}

	
	public function acaoDeletar()
	{
	    $moInsumo = new Insumo_Model();
	    
	    $_REQUEST['co_status'] = '0';

	    $moInsumo->popularDadosObjeto($_REQUEST)->salvar();
	    
	    $moInsumo->commit();
	    
	}
	
	public function acaoSalvar()
	{
	    $moInsumo = new Insumo_Model();

        $_REQUEST['nu_cpf_inclusao'] = $_SESSION['usucpf'];

	    $moInsumo->popularDadosObjeto($_REQUEST)->salvar();
	    
	    $moInsumo->commit();
	    
	}
	
	
}