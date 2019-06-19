<?php
require APPRAIZ . 'demandas/model/GrupoInsumo.class.inc';

class GrupoInsumo extends mvc\Controle
{
	public function iniciarFluxo()
	{	    
	    switch ($_REQUEST['requisicao']) {
	        case 'salvarGrupoInsumo':
	            $this->acaoSalvar();
	            $this->sucesso('demandas.php?modulo=grupoInsumo/listar');
	            die;
	        case 'deletarGrupoInsumo':
	            $this->acaoDeletar($_REQUEST);
	            $this->sucesso('demandas.php?modulo=grupoInsumo/listar');
	            die;

	    }
	}	
	
	public function acaoListar()
	{    
	    
	    $moGrupoInsumo = new GrupoInsumo_Model();

	    $arDado = array();
	    $_POST['returnsql'] = true;
	    
	    $arDado['sql_grupo_insumo'] = $moGrupoInsumo->listaGrupoInsumo($_POST);

	    // lista de grupos de insumos
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Código', 'Grupo Insumo', 'Código grupo pai', 'Grupo insumo pai');
        $esconderColunas = array('co_grupo_insumo');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($arDado['sql_grupo_insumo']);
        $listagemSimec->addAcao('edit', array('func' => 'abrirGrupoInsumo'));
        $listagemSimec->addAcao('delete', array('func' => 'deletarGrupoInsumo'));
        $listagemSimec->setFormFiltros('formulario-grupo-insumo');
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
	    
	    $moGrupoInsumo = new GrupoInsumo_Model();
	    
	    $arDado = array();
	    
	    if($_REQUEST['co_grupo_insumo']) {
	        $arDado = $moGrupoInsumo->getGrupoInsumoPorId($_REQUEST['co_grupo_insumo']);
            $arDado['sql_grupo_insumo_filho'] = $moGrupoInsumo->listaGrupoInsumoFilhoPorPaiID($_REQUEST['co_grupo_insumo']);
        }

        $arDado['sql_grupo_insumo_pai'] = $moGrupoInsumo->listaGrupoInsumoPai($_POST);
	    //ver($arDado);
	    $this->visao->carregarDados($arDado);
	    
	    $this->visao->setarTituloCabecalho(' ');
	    $this->visao->incluirVisao('cadastro');
	}

	
	public function acaoDeletar()
	{
	    $moGrupoInsumo = new GrupoInsumo_Model();
	    
	    $_REQUEST['co_status'] = '0';

	    $moGrupoInsumo->popularDadosObjeto($_REQUEST)->salvar();
	    
	    $moGrupoInsumo->commit();
	    
	}
	
	public function acaoSalvar()
	{
	    $moGrupoInsumo = new GrupoInsumo_Model();

        $_REQUEST['nu_cpf_inclusao'] = $_SESSION['usucpf'];

	    $moGrupoInsumo->popularDadosObjeto($_REQUEST)->salvar();
	    
	    $moGrupoInsumo->commit();
	    
	}
	
	
}