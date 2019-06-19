<?php

class Programa extends mvc\Controle
{
    public function iniciarFluxo()
	{
	    switch ($_REQUEST['requisicao']) {
	        case 'salvarPrograma':
	            $this->acaoSalvar();
	            $this->sucesso('demandas.php?modulo=programa/listar');
	            die;
	        case 'deletarPrograma':
	            $this->acaoDeletar($_REQUEST);
	            $this->sucesso('demandas.php?modulo=programa/listar');
	            die;

	    }
    }	

    public function acaoListar()
	{    
	    $moPrograma = new Programa_Model();

	    $arDado = array();
        
        $_POST['returnsql'] = true;
	    
        $arDado['sql_programa'] = $moPrograma->listaPrograma($_POST);
        $arDado['sql_secretarias'] = $moPrograma->getComboSecretarias();

	    $this->visao->carregarDados($arDado);
	    
	    $this->visao->setarTituloCabecalho(' ');
	    $this->visao->incluirVisao('lista');
    }

    public function acaoCadastrar()
    {
	    $moPrograma = new Programa_Model();

        $arDado = array();

        if($_REQUEST['co_programa'])
        {
            $arDado = $moPrograma->getProgramaPorId($_REQUEST['co_programa']);
            $arDado['secretarias'] = $moPrograma->getSecretariasDoPrograma($_REQUEST['co_programa']);
        }

        $arDado['sql_secretarias'] = $moPrograma->getComboSecretariasTodas();

        $this->visao->carregarDados($arDado);
        $this->visao->setarTituloCabecalho(' ');
        $this->visao->incluirVisao('cadastrar');
    }

    public function acaoSalvar()
    {
        $moPrograma = new Programa_Model();
        $moProgSec = new ProgramaSecretaria_Model();
        
        $_POST['co_situacao'] = 1;

        $idx = $moPrograma->popularDadosObjeto($_POST)->salvar();
        $moPrograma->commit();

        $arrProgSec = array();

        if(isset($_POST['co_programa']))
            $moProgSec->limparParaId($_POST['co_programa']);

        foreach($_POST['enjid'] as $sec)
        {
            array_push($arrProgSec, array(
                'co_programa' => $idx,
                'co_secretaria' => $sec
            ));
        }
        $moProgSec->inserirAssociacao($arrProgSec);
        
    }

    public function acaoDeletar()
    {
        $moPrograma = new Programa_Model();
        
        $_REQUEST['co_situacao'] = '0';

        $moPrograma->popularDadosObjeto($_REQUEST)->salvar();

        $moPrograma->commit();
    }
}
?>