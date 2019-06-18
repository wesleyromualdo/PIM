<?php
class Par3_Controller_IniciativaAreasRelacionadasProgramas extends Controle
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_IniciativaAreasRelacionadasProgramas($_REQUEST['iapid']);
    }

    public function recuperarPrograma()
    {
         return $this->model->recuperarPrograma($this->model->iapid);

    }

    public function desvincular($iapid)
    {
        return $this->model->desvincularAreaRelacionadaProgramas($iapid);
    }

    public function salvar($arrPost)
    {
        return $this->model->salvarAreaRelacionadaProgramas($arrPost);
    }

    public function marcarOpcao($esdid,$iapid)
    {
        $mIaw = new Par3_Model_IniciativaAreasRelacionadasProgramasEstadoDocumento();
        $mHistorico = new Par3_Model_IniciativaAreasRelacionadasProgramasEstadoDocumentoHistorico();

        $arr['esdid'] = $esdid;
        $arr['iapid'] = $iapid;
        $arr['iawdtinclusao']  = date('Y-m-d H:m:s');
        $arr['iawcpfinclusao'] = $_SESSION['usucpf'];

        $rsEstadoDocumento = $mIaw->recuperarAreaRelaciondada($arr);
        if($rsEstadoDocumento['iawid']){
            $iawstatus = 't';
            if($rsEstadoDocumento['iawstatus'] == 't'){
                $iawstatus = 'f';
            }
            $rsEstadoDocumento['iawstatus'] = $iawstatus;
            $mIaw->popularDadosObjeto($rsEstadoDocumento);
            $iaw = $mIaw->salvar();
            $mIaw->commit();

            $model = new Par3_Model_IniciativaAreasRelacionadasProgramasEstadoDocumento($iaw);
            $mHistorico->gravarHistorico($model,$mHistorico::UPDATE);
            $mHistorico->commit();
            return $iaw;
        }

        $rsEstadoDocumento['iapid'] = $iapid;
        $rsEstadoDocumento['esdid'] = $esdid;
        $rsEstadoDocumento['iawstatus'] = 't';
        $mIaw->popularDadosObjeto($rsEstadoDocumento);
        $iaw = $mIaw->salvar();
        $mIaw->commit();

        $model = new Par3_Model_IniciativaAreasRelacionadasProgramasEstadoDocumento($iaw);
        $mHistorico->gravarHistorico($model,$mHistorico::CREATE);
        $mHistorico->commit();
        return $iaw;
    }

    public function listarProgramas($arrPost)
    {
        global $disabled;
        $modelAreas = new Par3_Model_IniciativaAreasRelacionadasProgramas();

        $campo   = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'DESC'? 'DESC':'ASC';

        $arrQuery = $arrPost;
        $arrQuery['iuoid'] = $arrPost['tipodesc'];
        $arrQuery['ordenacao']['sentido'] = $sentido;

        $sql = $this->model->dataGridSQL($arrPost);

        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
        $cabecalho = array('Programa','Unidade Orçamentária','Área Relacionada');
        $esconderColunas = array('id');

        $mEstadoDocumento  = new Workflow_Model_EstadoDocumento();
        $rsEstadoDocumento = $mEstadoDocumento->recuperarTodos('esdid,esddsc',array('tpdid ='.Par3_Model_Iniciativa::WORKFLOW_TIPO_DOCUMENTO_INICIATIVA));
        $arrEstadosDocumentos = array();
        foreach($rsEstadoDocumento as $esd){
            $arrEstadosDocumentos[] = $esd;
        }
        foreach($arrEstadosDocumentos as $class){
            $cabecalho[] = $class['esddsc'];
        }//Adiciona a descrição das classes  ao cabeçalho da listagem

        if($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir'){
            $esconderColunas = array('id');
            array_unshift($cabecalho,'QTD');
            $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;
        }

        $tipoRelatorio = ($arrPost['req'] == 'xls') ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;
        $tratamentoListaVazia = ($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('areasrelacionadastable');
        $listagem->setQuery($sql);
        $listagem->esconderColunas($esconderColunas);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        if($disabled == ''){
            $listagem->addAcao('unlinkfa', 'desvincularPrograma');
        }
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }
}