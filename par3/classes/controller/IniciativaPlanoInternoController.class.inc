<?php

class Par3_Controller_IniciativaPlanoInternoController extends Controle
{
    private $model;
    private $modelHistorico;
    public function __construct()
    {
        $inplintid = validarInteiro($_REQUEST['inplintid'])? $_REQUEST['inplintid']:'';
        $this->model = new Par3_Model_IniciativaPlanoInterno($inplintid);
        $this->modelHistorico = new Par3_Model_IniciativaPlanoInternoHistorico();
    }

    public function form($arrPost)
    {
        global $simec;
        $programa = new Par3_Model_IniciativaProgramas();

      /*  $sql = "SELECT
                    pgoid as codigo,
                    pgodsc as descricao
                FROM par3.programaorigem
               ";*/
//ver($this->model,d);
        echo '<input type="hidden" name="inplintid" value="'.$this->model->inplintid.'"/>';
        echo '<div class="form-group"><label class="col-lg-3 control-label">Código:</label>';
        echo '<div class="col-lg-2"><input type="text" disabled="" value="'.$this->model->inplintid.'" class="form-control"></div>';
        echo '</div>';
        //echo $simec->select("pgoid", 'Tipo programa', $this->model->pgoid, $sql, array());
        echo $simec->select('prgPlano', 'Programa', $this->model->inplinprog, $programa->pegarSQLSelectComboSigla(), array('maxlength' => '255','placeholder'=>'Selecione o Programa')); 
        echo $simec->input('ptres', 'PTRES', $this->model->inplintptres,  array('type' => 'number','required','max' => '999999','placeholder'=>'Informe o PTRES')); 
        echo $simec->input('planoIntern', 'Plano Interno', $this->model->inplintdsc, array('maxlength' => '255', 'required','placeholder'=>'Informe o Plano Interno', 'maxlength' => 11)); 
        echo $simec->select('ano', 'Ano', $this->model->inplintano, pegaAnoAFrente(5), array('maxlength' => '255', 'required', 'placeholder'=>'Selecione o Ano')); 
    }

    public function salvar($arrPost, $updateAction = false)
    {
        
        $acao = Par3_Model_IniciativaPlanoInternoHistorico::CREATE;
        if( !empty($arrPost['inplintid']) ) {
            $arrayPlanoInterno['inplintid']      = $arrPost['inplintid'];
            $acao   = Par3_Model_IniciativaPlanoInternoHistorico::UPDATE;
        }else {
            //$arrayPlanoInterno['prgdtinclusao']  = date('Y-m-d H:m:s');
        }

        $arrayPlanoInterno['inplinprog']        = $arrPost['prgPlano'];
        $arrayPlanoInterno['inplintptres']      = $arrPost['ptres'];
        $arrayPlanoInterno['inplintdsc']        = strtoupper(trim(($arrPost['planoIntern'])));
        $arrayPlanoInterno['inplintano']        = $arrPost['ano'];
        $arrayPlanoInterno['inplintdtcriacao']  = date('Y-m-d H:m:s');
        $arrayPlanoInterno['inplintcpfcriacao'] = str_replace(array('.', '-', '/', ' '), '', $_SESSION['usucpf']);
        $arrayPlanoInterno['inplintstatus']     = 'A';
        $arrayPlanoInterno['inplintsituacao']   = 'A';

        //Valida campos
        $erro = $this->model->validarInput($arrayPlanoInterno); 
        if($erro) return $erro;


        //verifica se existe fonte de recurso viculada com plano interno que vai ser editadas
        if($acao == "UPDATE" && !$updateAction){
            $sqlConsultaFonte = "SELECT * FROM par3.iniciativa_fonte_recurso WHERE inplintid = {$arrayPlanoInterno['inplintid']}
            AND ifrstatus = 'A'";
            $cont = $this->model->pegaum($sqlConsultaFonte);
            if($cont != ''){ 
                return $retorno = ["status" => 1 ];
            } 
        }

        //consulta se já existe esse plano interno com os dados informados
        $sqlConsultaExistencia = "SELECT * FROM par3.iniciativa_plano_interno WHERE inplinprog = {$arrPost['prgPlano']}
        and inplintptres = '".$arrPost['ptres']."' 
        and inplintdsc = '".strtoupper(trim(($arrPost['planoIntern'])))."'
        and inplintano = {$arrPost['ano']} 
        and inplintstatus='A'";
        $cont = $this->model->pegaum($sqlConsultaExistencia);
        if($cont != ''){ 
           return $retorno = ["status" => 0 ];
        }   

        try {
            $this->model->popularDadosObjeto($arrayPlanoInterno);
            $id  = $this->model->salvar(); 
            $this->model->commit();

            $modelPlano = new Par3_Model_IniciativaPlanoInterno($id);//recupera programa
            $this->modelHistorico->gravarHistorico($modelPlano,$acao);//grava em hitórico
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }

        if ($id) {
            return $id;
        }

        return 'erro';
    }

    public function recuperar()
    {
        return $this->model;     
    }

    public function recuperarCompleto($arrDados)
    {
           global $db;
           $sql = "SELECT ifr.*,pr.prgdsc FROM par3.iniciativa_plano_interno ifr INNER JOIN par3.programa pr ON pr.prgid = ifr.inplinprog
           WHERE ifr.inplintid = {$arrDados['inplintid']}";
           $result = $db->executar($sql);
           $array = pg_fetch_assoc($result);
           return $array;
    }

    public function remover($arrPost){
        return $this->model->removerPlano($arrPost);
    }

    public function listar($arrPost)
    {
        global $disabled;

        $arrQuery = $arrPost;
        if ($arrPost['requisicao'] == 'imprimir' || $arrPost['requisicao'] == 'xls') {
            $sql = $this->model->montarSQLSimples($arrQuery,true);
        }else{
            $sql = $this->model->montarSQLSimples($arrQuery);
        }
        
        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';

        $cabecalho = array('Código', 'Programa', 'PTRES', 'Plano Interno', 'Ano', 'Situação');
        $esconderColunas = array('inplintid','inplintsituacao');

        if ($arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir') {

            $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;
            $disabled = true;
        }

        $tipoRelatorio = ($arrPost['requisicao'] == 'xls') ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;
        if ($arrPost['requisicao'] == 'imprimir') {
            $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;
            $disabled = true;
        }
        ///desabilitar opções para a impressão
        $tratamentoListaVazia = ($arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('codigo');
        
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->setTamanhoPagina(50);
        
        $listagem->addCallbackDeCampo('situacao','adiciona_switchery');

        if ($arrPost['requisicao'] == 'imprimir' || $arrPost['requisicao'] == 'xls') {
            $listagem->addCallbackDeCampo('situacao','formata_status');
            $esconderColunas = array('inplintid');
        }

        if($disabled == ''){
            $listagem->addAcao('remove', 'js_RemoverPlanoInterno');
            $listagem->addAcao('edit', 'editar');
            $listagem->turnOnPesquisator();
        }

        $listagem->esconderColunas($esconderColunas);
        $listagem->render($tratamentoListaVazia);
    }

    public function alterarSituacao($arrPost){
        return $this->model->alterarSituacao($arrPost['inplintid']);
    }


}