<?php
require APPRAIZ . 'demandas/model/GrupoServico.class.inc';

class GrupoServico extends mvc\Controle
{

    private $modelGrupoServico;



    public function iniciarFluxo()
    {   
        
        $this->modelGrupoServico = new GrupoServico_Model();

        switch ($_REQUEST['requisicao']) {
            case 'salvarGrupoServico':
                
                $this->acaoSalvar();
                $this->sucesso('demandas.php?modulo=GrupoServico/listar');
                die;
                break;

            case 'deletarGrupoServico':
                $this->acaoDeletar();
                $this->sucesso('demandas.php?modulo=GrupoServico/listar');
                die;
                break;

            case 'mostrarFilhos':
                 

                $filtro = array('co_grupo_servico_pai' => $_REQUEST['dados'][0]);

                $this->modelGrupoServico->listarGrupoServico($filtro)->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);


                //$this->sucesso('demandas.php?modulo=GrupoServico/listar');
                die;
                break;


        }
    }   
    
    public function acaoListar()
    {    
         
        
        $arDado = array('listagem' =>  $this->modelGrupoServico->listarGrupoServico($_POST));
        
        $this->visao->carregarDados($arDado);
        $this->visao->setarTituloCabecalho('');
        $this->visao->incluirVisao('lista');
    
    }
    

    public function acaoCadastrar()
    {
         
        $arDado = array('sqlSelect' => $this->modelGrupoServico->listaGrupoServicoPai());
        
        if($_REQUEST['co_grupo_servico']) {
            $arDado = $this->modelGrupoServico->getGrupoServicoPorId($_REQUEST['co_grupo_servico']);
            $arDado['sql_grupo_servico_filho'] = $this->modelGrupoServico->listaGrupoServicoFilhoPorPaiID($_REQUEST['co_grupo_servico']);
        }



        $arDado['sql_grupo_servico_pai'] = $this->modelGrupoServico->listaGrupoServicoPai($_POST);

        $this->visao->carregarDados($arDado);
        $this->visao->setarTituloCabecalho(' ');
        $this->visao->incluirVisao('cadastro');


    }

    
    public function acaoDeletar()
    { 

        $_REQUEST['co_status'] = '0';
        $this->modelGrupoServico->popularDadosObjeto($_REQUEST)->salvar();
        $this->modelGrupoServico->commit();
        
    }
    
    public function acaoSalvar()
    { 

        $_REQUEST['nu_cpf_inclusao'] = $_SESSION['usucpf']; 

        $arrCamposNull = array();
        if(empty($_REQUEST['co_grupo_servico_pai']))
            array_push($arrCamposNull, 'co_grupo_servico_pai'); 
         

        $this->modelGrupoServico->popularDadosObjeto($_REQUEST)->salvar(true, true, $arrCamposNull);
        $this->modelGrupoServico->commit();
        
    }
    
    
}