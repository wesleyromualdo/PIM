<?php

class Par3_Model_CicloParController extends Controle
{
    public function __construct()
    {
        $cicid = validarInteiro($_POST['cicid'])? $_POST['cicid']:'';
        $this->model = new Par3_Model_CicloPar($cicid);
    }

    public function form($arrPost)
    {
        $anos = array();
        $anoAtual = '2016';
        $anoFim   = $anoAtual + 15;
        for($anoAtual;$anoAtual < $anoFim;$anoAtual++)
        {
            $anos += array($anoAtual => $anoAtual);
        }

        global $simec;
        echo '<input type="hidden" name="cicid" value="'.$this->model->cicid.'"/>';
        echo '<div class="form-group"><label class="col-lg-3 control-label">Código:</label>';
        echo '<div class="col-lg-2"><input type="text" disabled="" value="'.$this->model->cicid.'" class="form-control"></div>';
        echo '</div>';
        echo $simec->input('cicdsc', 'Descrição', $this->model->cicdsc, array('maxlength' => '100','required'));
//        echo $simec->input('cicdtinicio', 'Início', $this->model->cicdtinicio, array('max' => '4','type' => 'number','required','title' => 'Insira o ano de início do Ciclo.'),array('class' => 'col-md-3'));
        echo $simec->select('cicdtinicio', 'Início',  !$this->model->cicdtinicio ? date(Y): $this->model->cicdtinicio ,$anos, array('maxlength' => '255','required','title' => 'Insira o ano de início do Ciclo.'),array('class' => 'col-md-3'));
        echo $simec->select('cicdtfim', 'Fim', !$this->model->cicdtfim ? date(Y) + 4 : $this->model->cicdtfim ,$anos, array('maxlength' => '255','required','title' => 'Insira o ano que encerra o Ciclo.'));
//        echo $simec->input('cicdtfim', 'Fim', $this->model->cicdtfim, array('max' => '3000','type' => 'number','required','title' => 'Insira o ano que encerra o Ciclo.'));
        $vigencia =  array('VIGENTE' => 'Vigente', 'FECHADO' => 'Fechado','NAO_INICIADO' =>'Não Iniciado');
        echo $simec->radio('cicvigencia', 'Vigência',$this->model->cicid?$this->model->cicvigencia:'VIGENTE',$vigencia, array($disabled), array('type' => 'radio radio-info radio', 'style' => 'inline'));
//        if ($arrPost['cicid']){
            echo $simec->radio('cicsituacao', 'Situação',(!($this->model->cicsituacao)?'A':$this->model->cicsituacao), array('A' => 'Ativo', 'I' => 'Inativo'), array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));
//        }
    }

    public function salvar($arrPost)
    {
        //define acão para guardar em histórico
        $acao = Par3_Model_CicloParHistorico::CREATE;
        if($arrPost['cicid']){
            $arrayCiclo['cicid'] = trim($arrPost['cicid']);
            $acao = Par3_Model_CicloParHistorico::UPDATE;
        }

        $arrayCiclo['cicsituacao']    = $arrPost['cicsituacao'];
        $arrayCiclo['cicstatus']      = 'A';
        $arrayCiclo['cicdsc']         = trim(($arrPost['cicdsc']));
        $arrayCiclo['ciccpfinclusao'] = str_replace(array('.', '-', '/', ' '), '',$_SESSION['usucpf']);
        $arrayCiclo['cicvigencia']    = $arrPost['cicvigencia'];
        $arrayCiclo['cicdtinicio']    = trim($arrPost['cicdtinicio']) ? trim($arrPost['cicdtinicio']) : null;
        $arrayCiclo['cicdtfim']       = trim($arrPost['cicdtfim']) ? trim($arrPost['cicdtfim']) : null;
        if(empty($arrPost['cicid'])){$arrayCiclo['cicdtinclusao']  = date('Y-m-d H:m:s');}//só grava data de inclusão ao criar

        //Valida campos
        $erro = $this->model->validarInput($arrayCiclo);
        if($erro){return $erro;}

        //validar antes de realizar operação
        $arrayCiclo['cicanos']        = implode(',',range((int)$arrPost['cicdtinicio'],(int)$arrPost['cicdtfim']));
        if($arrayCiclo['cicdtinicio'] == null || $arrayCiclo['cicdtfim'] == null){
            $arrayCiclo['cicduracao'] = 0;
        }else{
            $arrayCiclo['cicduracao']     = ((int)$arrPost['cicdtfim']  + 1) - (int)$arrPost['cicdtinicio'];
        }

        try {
            $this->model->popularDadosObjeto($arrayCiclo);
            $novoCiclo =  $this->model->salvar(true,true,array('cicdtinicio','cicdtfim'));//grava ciclo
            $this->model->commit();

            $modelCiclo = new Par3_Model_CicloPar($novoCiclo);//recupera ciclo

            $modelHistorico = new Par3_Model_CicloParHistorico();
            $modelHistorico->gravarHistorico($modelCiclo,$acao);//grava em hitórico
            $modelHistorico->salvar();
            $modelHistorico->commit();
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($novoCiclo) {
            return $novoCiclo;
        }
        return 'erro';
    }

    public function remover($arrPost)
    {
        $arrayCiclo['cicid']             = $arrPost['cicid'];
        $arrayCiclo['cicstatus']         = 'I';
        $arrayCiclo['cicsituacao']       = 'I';
        $arrayCiclo['cicdtinativacao']   = date('Y-m-d H:m:s');
        $arrayCiclo['ciccpfinativacao']  = $_SESSION['usucpf'];

        try {
            $this->model->popularDadosObjeto($arrayCiclo);
            $sucesso = $this->model->salvar();
            $this->model->commit();

            $modelCiclo = new Par3_Model_CicloPar($arrPost['cicid']);

            $modelHistorico = new Par3_Model_CicloParHistorico();
            $modelHistorico->gravarHistorico($modelCiclo,Par3_Model_CicloParHistorico::DELETE);
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($sucesso) {
            return $sucesso;
        }
        return 'erro';
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }

    public function listar($arrPost)
    {
        global $disabled;
        $modelCiclo = new Par3_Model_CicloPar();

        $campo   = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'ASC'? 'ASC':'DESC';

        $arrQuery = $arrPost;
        $arrQuery['ordenacao']['sentido'] = $sentido;
        switch($campo){//filtros de ordenação da listagem
            case 'codigo':      $arrQuery['ordenacao']['campo'] = 'cicid';break;
            case 'descricao':   $arrQuery['ordenacao']['campo'] = 'cicdsc';break;
            case 'cicsituacao': $arrQuery['ordenacao']['campo'] = 'cicsituacao';break;
            case 'cicdtinicio': $arrQuery['ordenacao']['campo'] = 'cicdtinicio';break;
            case 'cicdtfim':    $arrQuery['ordenacao']['campo'] = 'cicdtfim';break;
            case 'cicduracao':  $arrQuery['ordenacao']['campo'] = 'cicduracao';break;
            case 'cicanos':     $arrQuery['ordenacao']['campo'] = 'cicanos';break;
            case 'cicvigencia': $arrQuery['ordenacao']['campo'] = 'cicvigencia';break;
            default:            $arrQuery['ordenacao'] = '';break;
        }

        $sql = $modelCiclo->montarSQLSimples($arrQuery);

        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';

        $cabecalho = array('Código', 'Ciclo','Início','Fim','Duração Ciclo','Anos','Vigência','Situação');
        $esconderColunas = array('id','qtd');

        if($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir'){
            $esconderColunas = array('id');
            array_unshift($cabecalho,'QTD');
            $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;
        }

        $tipoRelatorio = ($arrPost['req'] == 'xls') ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;
        $tratamentoListaVazia = ($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('cicloPar');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
//        $listagem->setTamanhoPagina(10);
        $listagem->addCallbackDeCampo('cicsituacao','formata_status');
        $listagem->addCallbackDeCampo('cicvigencia','formata_vigencia');
        if($disabled == ''){
            $listagem->addAcao('remove', 'js_verificaRemoverCiclo');
            $listagem->addAcao('edit', 'editarCiclo');
        }
        if($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir'){
            $listagem->addCallbackDeCampo('cicanos','formata_separados_virgula_xls');
        }
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }

    public function verificaLigacaoPlanejamento($cicid){
        $result = $this->model->verificaLigacaoPlanejamento($cicid);
        return $result;
    }
}