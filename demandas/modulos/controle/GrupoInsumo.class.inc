<?php
require APPRAIZ . 'demandas/model/GrupoInsumo.class.inc';
require APPRAIZ . 'demandas/model/Insumo.class.inc';

class GrupoInsumo extends mvc\Controle
{
    private $modelGrupoInsumo;
    private $modelInsumo;

	public function iniciarFluxo()
	{
        $this->modelGrupoInsumo = new GrupoInsumo_Model();
        $this->modelInsumo = new Insumo_Model();

	    switch ($_REQUEST['requisicao']) {
	        case 'salvarGrupoInsumo':
	            $this->acaoSalvar();
                //$this->sucesso('demandas.php?modulo=grupoInsumo/listar');
	            die;
	        case 'deletarGrupoInsumo':
	            $this->acaoDeletar($_REQUEST);
                $this->sucesso('demandas.php?modulo=grupoInsumo/listar');
                die;
            case 'validarExclusaoGrupoInsumo':
                ob_clean();
                $resSubgrupo = $this->modelGrupoInsumo->listaSubgrupoInsumoGrupoID($_REQUEST['co_grupo_insumo']);
                // se ja existir um subgrupo não deixa deletar
                if( !empty($resSubgrupo) && is_array($resSubgrupo) ){
                    echo 'false';
                }
                die;
            case'validarExclusaoSubgrupoInsumo':
                ob_clean();
                $resInsumo = $this->modelInsumo->listaInsumo($_REQUEST['co_grupo_insumo']);
                // se ja existir insumos vinculados ao subgrupo não deixa deletar vinculado ao subgrupo
                if( !empty($resInsumo) && is_array($resInsumo) ){
                    echo 'false';
                }
                die;
            case'validarCodigoGrupoInsumo':
                ob_clean();
                $resGrupo = $this->modelGrupoInsumo->getGrupoInsumoPorCodigoId($_REQUEST);
                // se ja existir o código inserido não deixa o usuário continuar
                if( !empty($resGrupo) && is_array($resGrupo) ){
                    echo 'false';
                }
                die;
            case 'listarSubgrupoInsumo':
                $filtro = array('co_grupo_insumo' => $_REQUEST['dados'][0]);
                $this->modelGrupoInsumo->listaSubgrupoInsumoPorGrupo($filtro)->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
                die;
                break;
            case 'listarInsumoSubgrupo':
                $filtro = array('co_grupo_insumo' => $_REQUEST['dados'][0]);
                $this->modelInsumo->listaInsumoPorSubgrupo($filtro)->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
                die;
                break;

	    }
	}	
	
	public function acaoListar()
	{
	    $arDado = array();
	    $_REQUEST['returnsql'] = true;
        $pesquisar = $_POST['pesquisar'];
        $this->visao->pesquisar = $pesquisar;
	    
	    $arDado['sql_grupo_insumo'] = $this->modelGrupoInsumo->listaGrupoInsumo($_REQUEST);

        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Código', 'Descrição do grupo');
        $esconderColunas = array('co_grupo_insumo', 'co_status');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOffForm();
        $listagemSimec->setQuery($arDado['sql_grupo_insumo']);
        $listagemSimec->addAcao('plus', array('func' => 'listarSubgrupoInsumo'));
        $listagemSimec->addAcao('edit', array('func' => 'abrirGrupoInsumo'));
        $listagemSimec->addAcao('delete', array('func' => 'deletarGrupoInsumo'));
        $listagemSimec->setAcaoComoCondicional('plus', array(array('campo' => 'co_status', 'valor' => 0, 'op' => 'diferente')) );
        $listagemSimec->setAcaoComoCondicional('delete', array(array('campo' => 'co_status', 'valor' => 0, 'op' => 'diferente')) );
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
	    $arDado = array();

	    if ($_REQUEST['co_grupo_insumo']) {
	        $arDado = $this->modelGrupoInsumo->getGrupoInsumoPorId($_REQUEST['co_grupo_insumo']);
            $arDado['sql_grupo_insumo_filho'] = $this->modelGrupoInsumo->listaSubgrupoInsumoGrupoID($_REQUEST['co_grupo_insumo']);
        }
        $arDado['sql_grupo_insumo_pai'] = $this->modelGrupoInsumo->listaGrupoInsumoPai($_REQUEST);

	    $this->visao->carregarDados($arDado);
	    $this->visao->setarTituloCabecalho(' ');
	    $this->visao->incluirVisao('cadastro');
	}

	
	public function acaoDeletar()
	{
	    $_REQUEST['co_status'] = '0';
	    $this->modelGrupoInsumo->popularDadosObjeto($_REQUEST)->salvar();
	    $this->modelGrupoInsumo->commit();
	}
	
	public function acaoSalvar()
    {
        $co_grupo_insumo = $_REQUEST['co_grupo_insumo'];
        $_REQUEST['nu_cpf_inclusao'] = $_SESSION['usucpf'];

        $valida = $this->modelGrupoInsumo->validaCadastroGrupoInsumo($_REQUEST);

        if (!empty($valida['erro']) && isset($valida['erro']) && ($valida['erro'] == false) ) {
            $this->modelGrupoInsumo->popularDadosObjeto($_REQUEST)->salvar();
            $this->modelGrupoInsumo->commit();
            $this->sucesso('demandas.php?modulo=grupoInsumo/listar');
        } else {
            if ($co_grupo_insumo){
                $this->erro("demandas.php?modulo=grupoInsumo/cadastrar&co_grupo_insumo={$co_grupo_insumo}", $valida['msg']);
            } else {
                $this->erro('demandas.php?modulo=grupoInsumo/cadastrar', $valida['msg']);
            }
            $this->erro($valida['msg']);
        }
	}
	
	
}