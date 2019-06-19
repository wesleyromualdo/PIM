<?php
class Par3_Helper_PendenciasEntidade extends Controle
{

    public function __construct()
    {
        parent::__construct();
    }

    public function recuperarPendenciasEntidade($inuid, $semInstrumentoUnidade = false)
    {
        $oInstrumento = new Par3_Model_Pendencias();
        $instrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);
        $estuf = $instrumentoUnidade->estuf;
        $muncod = $instrumentoUnidade->muncod;
        $itrid = $instrumentoUnidade->itrid;

        $GLOBALS['estuf'] = $estuf;
        $GLOBALS['muncod'] = $muncod;

        $arr = $oInstrumento->recuperarPendenciasEntidade($inuid, $estuf, $muncod, $itrid);

        $arrAux = array();

        if($semInstrumentoUnidade){
            return $arr;
            exit;
        }

        if( is_array( $arr) ){

            foreach( $arr as $key => $inputAux ){

                $arrAux[$inputAux['pendencia']][$inputAux['estuf']][$inputAux['muncod']][$inputAux['obrid']] = $inputAux;
            }
        }


        return $arrAux;
    }

    public function recuperarPendenciasLista($inuid)
    {
        $oInstrumento = new Par3_Model_Pendencias();

        $arr = $oInstrumento->recuperarPendenciasEntidade($inuid);

        return $arr;
    }

    public function recuperarTiposPendencia()
    {
        $oInstrumento = new Par3_Model_Pendencias();

        $arr = $oInstrumento->recuperarTiposPendencia();

        return $arr;
    }
    
    public function existePendenciaPeloTipoPar($inuid){
        return $this->existePendenciaPorTipo('par', $inuid);
    }

    public function existePendenciaPorTipo($pendencia, $inuid)
    {
        $oInstrumento = new Par3_Model_Pendencias();
        $instrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);
        $estuf = $instrumentoUnidade->estuf;
        $muncod = $instrumentoUnidade->muncod;
        $itrid = $instrumentoUnidade->itrid;
        $modelRestricao = new Par3_Model_Restricao();
        $GLOBALS['estuf'] = $estuf;
        $GLOBALS['muncod'] = $muncod;

        $exists = $oInstrumento->existePendenciaPorTipo($pendencia, $inuid, $estuf, $muncod, $itrid);
        if($pendencia == 'par'){
            $existeRestricao = $modelRestricao->existeRestricaoInstrumentoUnidadeById($inuid); //se existir restrições que implicam em restringir o diagnóstico ou plenajamento
            if( $exists || $existeRestricao == true){
                return true;
            }else{
                return false;
            }
        }


        if( $exists){
            return true;
        }else{
            return false;
        }
        return $arr;
    }

    /**
     * Função: controletipoPendencia
     * -Retorna um array com a descrição e peculiaridades do tipo da pendência para a listagem em tela.
     * @return array
     */
    public function controleTipoPendencia($pendencia, $boExists=false){
        $arAux = array();

        switch($pendencia){

            case 'par':
                $arAux['description'] = "Pendências de Obras do PAR";
                $arAux['widget'] = "pendenciaPar";
                $arAux['icon'] = "fa-building";
                break;
            case 'recursos':
                $arAux['description'] = "Pendências nos Recursos";
                $arAux['widget'] = "lazur";
                $arAux['icon'] = "fa-database";
                break;
            case 'desembolso':
                $arAux['description'] = "Pendências de Desembolso";
                $arAux['widget'] = "pendenciaDesembolso";
                $arAux['icon'] = "fa-dollar";
                break;
            case 'preenchimento':
                $arAux['description'] = "Pendências no Preenchimento";
                $arAux['widget'] = "yellow";
                $arAux['icon'] = "fa-file-text";
                break;
        }
        if( $boExists )
            $arAux['thumb'] = "fa-warning";
        else
            $arAux['thumb'] = "fa-thumbs-up";

        return $arAux;
    }

    /**
     * Função: controleWidgetsPendencia
     * -Retorna um array com a descrição e peculiaridades do tipo da pendência para a listagem em tela.
     * @return array
     */
    public function controleWidgetsPendencia($pendencia, $boExists=false){
        $arAux = array();

        switch($pendencia){

            case 'par':
                $arAux['description'] = "Pendências de Obras do PAR";
                $arAux['widget'] = "#ed5565";
                $arAux['icon'] = "fa-building";
                break;
            case 'habilitacao':
                $arAux['description'] = "Habilitação";
                $arAux['widget'] = "#BDB76B";
                $arAux['icon'] = "fa-certificate";
                break;
            case 'cacs':
                $arAux['description'] = "<span style=\"font-size: 9pt;\">Cons. de Acomp. de Controle Social</span>";
                $arAux['widget'] = "#f8ac59";
                $arAux['icon'] = "fa-group";
                break;
            case 'cae':
                $arAux['description'] = "Conselho de Alimentação Escolar";
                $arAux['widget'] = "#EEC591";
                $arAux['icon'] = "fa-cutlery";
                break;
            case 'contas':
                $arAux['description'] = "Prestação de Contas";
                $arAux['widget'] = "#23c6c8";
                $arAux['icon'] = "fa-dollar";
                break;
            case 'monitoramento':
                $arAux['description'] = "Monitoramento PAR 2011-2014 e Termos de Compromisso";
                $arAux['widget'] = "#8B8989";
                $arAux['icon'] = "fa-binoculars";
                break;
            case 'siope':
                $arAux['description'] = "SIOPE";
                $arAux['widget'] = "#4A708B";
                $arAux['icon'] = "fa-desktop";
                break;
            case 'pne':
                $arAux['description'] = "Plano Municipal de Educação";
                $arAux['widget'] = "#BC8F8F";
                $arAux['icon'] = "fa-graduation-cap";
                break;

        }
        if( $boExists ){
            $arAux['thumb'] = "fa-warning";
            $arAux['boolean'] = 1;
        }else{
            $arAux['thumb'] = "fa-thumbs-up";
            $arAux['boolean'] = 0;
        }
        $arAux['type'] = $pendencia;

        return $arAux;
    }

    /**
     * Função: controleWidgetsPendencia
     * -Retorna um array com a descrição e peculiaridades do tipo da pendência para a listagem em tela.
     * @return array
     */
    public function controleWidgetsPendenciaPrincipal($pendencia, $boExists=false, $msgPendenciaSiope=null){
        $arAux = array();

        switch($pendencia){

            case 'par':
                $arAux['description'] = "Pendências de Obras do PAR";
                $arAux['widget'] = "#ed5565";
                $arAux['icon'] = "fa-building";
                $arAux['msgSucess'] = "Não existem pendências de Obras do PAR.";
                $arAux['msgFail'] = "Existem pendências no SIOPE.";
                break;
            case 'habilitacao':
                $arAux['description'] = "Habilitacao";
                $arAux['widget'] = "#BDB76B";
                $arAux['icon'] = "fa-certificate";
                $arAux['msgSucess'] = "Entidade encontra-se habilitada";
                $arAux['msgFail'] = "Entidade não encontra-se habilitada";
                break;
            case 'cacs':
                $arAux['description'] = "Conselho de Acompanhamento de Controle Social - CACS";
                $arAux['widget'] = "#f8ac59";
                $arAux['icon'] = "fa-group";
                $arAux['msgSucess'] = "A situação no CACS está REGULAR.";
                $arAux['msgFail'] = "A situação no CACS está IRREGULAR.";
                break;
            case 'cae':
                $arAux['description'] = "Conselho de Alimentação Escolar";
                $arAux['widget'] = "#EEC591";
                $arAux['icon'] = "fa-cutlery";
                $arAux['msgSucess'] = "A etapa de preenchimento do Conselho de Alimentação Escolar encontra-se preenchida.";
                $arAux['msgFail'] = "A etapa de preenchimento do Conselho de Alimentação Escolar não encontra-se preenchida.";
                break;
            case 'contas':
                $arAux['description'] = "Prestação de Contas";
                $arAux['widget'] = "#23c6c8";
                $arAux['icon'] = "fa-dollar";
                $arAux['msgSucess'] = "Não existem pendências na Prestação de contas.";
                $arAux['msgFail'] = "Existem pendências na Prestação de Contas.";
                break;
            case 'monitoramento':
                $arAux['description'] = "Monitoramento PAR";
                $arAux['widget'] = "#8B8989";
// 				$arAux['widget'] = "#1ab394";
                $arAux['icon'] = "fa-binoculars";
                $arAux['msgSucess'] = "Não existem pendencias no Monitoramento PAR.";
                $arAux['msgFail'] = "Existem pendências no Monitoramento PAR.";
                break;
            case 'siope':
                $arAux['description'] = "SIOPE";
                $arAux['widget'] = "#4A708B";
// 				$arAux['widget'] = "#1c84c6";
                $arAux['icon'] = "fa-desktop";
                $arAux['msgSucess'] = "Não existem pendências no SIOPE.";
                $arAux['msgFail'] = "Existem pendências no SIOPE. <br/>" . $msgPendenciaSiope;
                break;
            case 'pne':
                $arAux['description'] = "Plano Municipal de Educação";
                $arAux['widget'] = "#BC8F8F";
                $arAux['icon'] = "fa-graduation-cap";
                $arAux['link'] = "<span id=\"pneArquivo\" style=\"color: #53868B; font-weight: bold; \"> Clique Aqui </span> para download ";
                $arAux['msgSucess'] = "O arquivo com a lei Munucipal/Estadual encontra-se disponível. ";
                $arAux['msgFail'] = "O arquivo com a lei Munucipal/Estadual não encontra-se disponível.";
                break;

        }
        if( $boExists ){
            $arAux['thumb'] = "fa-warning";
            $arAux['boolean'] = 1;
            $arAux['msg'] = $arAux['msgFail'];
            $arAux['link'] = '';
        }else{
            $arAux['thumb'] = "fa-thumbs-up";
            $arAux['boolean'] = 0;
            $arAux['msg'] = $arAux['msgSucess'];
        }
        $arAux['type'] = $pendencia;

        return $arAux;
    }

    /**
     * Função: consultaHabilitaEntidade
     * - Retorna se a entidade está ou não habilitada.
     * @return array
     */
    public function consultaHabilitaEntidade($inuid)
    {
        $unidade = new Par3_Model_InstrumentoUnidade();

        $inuid_par = $unidade->recuperarInuidPar($inuid);

        $oPendencia = new Par3_Model_Pendencias();

        $cnpj = $oPendencia->recuperaCNPJEntidade($inuid_par);

        $obWsPendencia = new Par3_Ws_PendenciaHabilitacao();

        $strHabilitacao = $obWsPendencia->consultaHabilitaEntidade($cnpj);

        if( $strHabilitacao == 'Habilitado' ){

            return 0;
        }else{
            return 1;
        }
    }

    /**
     * Função: consultaPendenciaCACS
     * - Retorna se a entidade está ou não regular no CACS.
     * @return array
     */
    public function consultaPendenciaCACS($inuid){
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);
        $modelCACS = new Par3_Model_CACS();

        if($modelInstrumentoUnidade->itrid != '2'){
            $listaCACS = $modelCACS->listarConselheiros($modelInstrumentoUnidade->estuf, $inuid);
        }else{
            $listaCACS = $modelCACS->listarConselheiros($modelInstrumentoUnidade->muncod, $inuid);
        }

        $situacaoCacs = $listaCACS[0]['sit_conselheiro'];

        $arConfig = array();

        if( $situacaoCacs == 'REGULAR' ) {
            return 0;
        }elseif( ( $situacaoCacs != 'REGULAR' ) && ( $modelInstrumentoUnidade->muncod != '' || $modelInstrumentoUnidade->estuf != '' ) ){
            return 1;
        }elseif(count($listaCACS) == 0){ //modificado, para gerar pendência na tela planejamento pendência
            return 1;
        }
        //else comentado, pois desconhecido sua função "Sergio"
        /*		else{
                    return 0; //
                }*/
    }

    /**
     * Função: consultaHabilitaEntidade
     * - Retorna se a entidade está ou não habilitada.
     * @return array
     */
    public function consultaArquivoPNE($inuid){
        $oPendencia = new Par3_Model_Pendencias();

        $instrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);
        $estuf = $instrumentoUnidade->estuf;
        $muncod = $instrumentoUnidade->muncod;
        $itrid = $instrumentoUnidade->itrid;

        $GLOBALS['estuf'] = $estuf;
        $GLOBALS['itrid'] = $itrid;
        $GLOBALS['muncod'] = $muncod;

        $boExiste = $oPendencia->recuperaArquivoSasePne($inuid, $estuf, $muncod , $itrid );

        if( $boExiste ){

            return 0;
        }else{
            return 1;
        }
    }

    /**
     * Função: existeArquivoPNE
     * - Retorna se a entidade está ou não habilitada.
     * @return array
     */
    public function existeArquivoPNE($inuid){
        $oPendencia = new Par3_Model_Pendencias();

        $instrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);
        $estuf = $instrumentoUnidade->estuf;
        $muncod = $instrumentoUnidade->muncod;
        $itrid = $instrumentoUnidade->itrid;

        $GLOBALS['estuf'] = $estuf;
        $GLOBALS['itrid'] = $itrid;
        $GLOBALS['muncod'] = $muncod;

        $boExiste = $oPendencia->recuperaArquivoSasePne($inuid, $estuf, $muncod , $itrid );

        if( $boExiste ){

            return 1;
        }else{
            return 0;
        }
    }

    /**
     * Função: consultaHabilitaEntidade
     * - Retorna se a entidade está ou não habilitada.
     * @return array
     */
    public function recuperaArquivoPNE($inuid){

        $oPendencia = new Par3_Model_Pendencias();
        $instrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);
        $estuf = $instrumentoUnidade->estuf;
        $muncod = $instrumentoUnidade->muncod;
        $itrid = $instrumentoUnidade->itrid;
        $arqid = $oPendencia->recuperaArquivoSasePne($inuid, $estuf, $muncod , $itrid );

        if( $arqid ){
            if( $muncod != '' ){
                $file = new FilesSimec('assessoramento', array(), 'sase');
            }else{
                $file = new FilesSimec('assessoramentoestado', array(), 'sase');
            }ob_clean();
            $arquivo = $file->getDownloadArquivo($arqid);
        }else{
            return 1;
        }
    }
    public function carregaPendenciaContas($inuid){

        $oPendencia = new Par3_Model_Pendencias();

        if ($oPendencia->getPendenciaContas($inuid)) {
            return 1;
        } else {
            return 0;
        }
    }

    /**
     * Função: consultaHabilitaEntidade
     * - Retorna se a entidade está ou não habilitada.
     * @return array
     */
    public function controlePendenciasBox($arrNot = array()){
        global $db;
        $instrumentoUnidade = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
        $itrid = $instrumentoUnidade->itrid;
        
        $sql = "SELECT inuid, habilitacao, cae, contas, monitoramento_par, obras_par, cacs, pne, siope
                FROM par3.vm_relatorio_quantitativo_pendencias WHERE inuid = {$_REQUEST['inuid']}";
        $arViewPendencia = $db->pegaLinha($sql);
        
        //if(IS_PRODUCAO == TRUE) {
        if (!in_array('habilitacao', $arrNot)) {
            //$boExists = $this->consultaHabilitaEntidade($_REQUEST['inuid']);
            $boExists = ($arViewPendencia['habilitacao'] == 't' ? true: false);
            $arAuxHelperTipoPendencia = $this->controleWidgetsPendencia('habilitacao', $boExists);
            $arAux['habilitacao'] = $arAuxHelperTipoPendencia;
        }
        //}

        if(!in_array('cae', $arrNot)){
            //$boExists = $this->carregaPendenciaConselhoCAE( $_REQUEST['inuid'] );
            $boExists = ($arViewPendencia['cae'] == 't' ? true: false);
            $arAuxHelperTipoPendencia = $this->controleWidgetsPendencia('cae', $boExists);
            $arAux['cae'] = $arAuxHelperTipoPendencia;
        }

        if(!in_array('contas', $arrNot)){
            //$boExists = $this->carregaPendenciaContas( $_REQUEST['inuid'] );
            $boExists = ($arViewPendencia['contas'] == 't' ? true: false);
            $arAuxHelperTipoPendencia = $this->controleWidgetsPendencia('contas', $boExists );
            $arAux['contas'] = $arAuxHelperTipoPendencia;
        }

        if(!in_array('monitoramento', $arrNot)){
            //$boExists = $this->consultaPendenciaMonitoramentoPAR( $_REQUEST['inuid'] );
            $boExists = ($arViewPendencia['monitoramento_par'] == 't' ? true: false);
            $arAuxHelperTipoPendencia = $this->controleWidgetsPendencia('monitoramento', $boExists);
            $arAux['monitoramento'] = $arAuxHelperTipoPendencia;
        }

        if(!in_array('par', $arrNot)){
            //$boExists = $this->existePendenciaPorTipo('par', $_REQUEST['inuid'] );
            $boExists = ($arViewPendencia['obras_par'] == 't' ? true: false);
            $arAuxHelperTipoPendencia = $this->controleWidgetsPendencia('par', $boExists);
            $arAux['par'] = $arAuxHelperTipoPendencia;
        }

        if(!in_array('cacs', $arrNot)){
            //$boExists = $this->consultaPendenciaCACS( $_REQUEST['inuid'] );
            $boExists = ($arViewPendencia['cacs'] == 't' ? true: false);
            $arAuxHelperTipoPendencia = $this->controleWidgetsPendencia('cacs', $boExists);
            $arAux['cacs'] = $arAuxHelperTipoPendencia;
        }

        if(!in_array('pne', $arrNot)){
            //$boExists = $this->consultaArquivoPNE( $_REQUEST['inuid'] );
            $boExists = ($arViewPendencia['pne'] == 't' ? true: false);
            $arAuxHelperTipoPendencia = $this->controleWidgetsPendencia('pne', $boExists);
            $arAux['pne'] = $arAuxHelperTipoPendencia;
            if( $itrid == '2')
                $arAux['pne']['description'] = "Plano Municipal de Educação";
            else
                $arAux['pne']['description'] = "Plano Estadual de Educação";
        }

        if(!in_array('siope', $arrNot)){
            //$boExists = $this->carregaPendenciaSiope($_REQUEST['inuid']);
            $boExists = ($arViewPendencia['siope'] == 't' ? true: false);
            $arAuxHelperTipoPendencia = $this->controleWidgetsPendencia('siope', $boExists);
            $arAux['siope'] = $arAuxHelperTipoPendencia;
            $arrSorted = $this->agrupaArrayChave( $arAux, 'boolean' );
        }
        
        krsort($arrSorted);

        return $arrSorted;
    }
    
    public function carregaPendenciaSiope($inuid){
            $oSiope = new Par3_Model_Siope();

//Trecho de código comentado para retirar as validações de pendencias antigas do siope e considerar apenas a situação enviada pela carga
//historia 10818 link: https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=10818
//    		$cumprimento = $oSiope->listarCumprimentos($_REQUEST);
//    		$cumprimento = is_array($cumprimento) ? $cumprimento : array();
//    		$boExists = $instrumentoUnidade->verificaMunicipioSIOP($_REQUEST['inuid']);
//    		$arrSiope = array();
//    		foreach ($cumprimento as $v) {
//    			if( $v['an_exercicio_mde'] == '2015' ){
//    				if( $v['vl_perc_aplic_med'] < 25 || $v['vl_perc_aplic_mag_rpm'] < 60 ){
//    					$boExists = true;
//    				}
//    			}
//    		}

            $resultado = $oSiope->transmissaoSiope($_REQUEST);
            $transmissao = $resultado['0']['cod_situacao'];
            $msgPendenciaSiope = $resultado['0']['situacao_desc'];

            if($transmissao == 2){
                $boExists = true;
            }else{
                $boExists = false;
            }

        return $boExists;
    }


    public function controlePendenciasBoxImpeditivas($arrNot = array()){

        if(!in_array('contas', $arrNot)){
            $arAuxHelperTipoPendencia = $this->controleWidgetsPendencia('contas', false);
            $arAux['contas'] = $arAuxHelperTipoPendencia;
        }
        if(!in_array('siope', $arrNot)){
            $oSiope = new Par3_Model_Siope();

            $resultado = $oSiope->transmissaoSiope($_REQUEST);
            $transmissao = $resultado['0']['cod_situacao'];
            $msgPendenciaSiope = $resultado['0']['situacao_desc'];

            if($transmissao == 2){
                $boExists = true;
            }
            $arAuxHelperTipoPendencia = $this->controleWidgetsPendencia('siope', $boExists);
            $arAux['siope'] = $arAuxHelperTipoPendencia;
            $arrSorted = $this->agrupaArrayChave( $arAux, 'boolean' );
        }

        if($this->existePendenciaPorTipo('par', $_REQUEST['inuid'])){
            return true;
        }
        if($this->consultaPendenciaCACS( $_REQUEST['inuid'] )){
            return true;
        }
        if($this->consultaArquivoPNE( $_REQUEST['inuid'])) {
            return true;
        }
        if(IS_PRODUCAO == TRUE) {
            if ($this->consultaHabilitaEntidade($_REQUEST['inuid'])) {
                return true;
            }
        }
        krsort($arrSorted);
        return count($arrSorted[1]);
    }




    /**
     * Função: consultaHabilitaEntidade
     * - Retorna se a entidade está ou não habilitada.
     * @return array
     */
    public function carregaPendenciaConselhoCAE($inuid){
        $oPendencia = new Par3_Model_Pendencias();

        $modelCAE = new Par3_Model_CAE();

        $conselho = $modelCAE->carregarDadosCAE($inuid);
        $presidente = $modelCAE->carregarDadosPresidenteCAE($inuid);
        $conselheiros = $modelCAE->carregarConselheirosCAE($inuid);

        if (!empty($conselho->arqid) && !empty($presidente->capid) && is_array($conselheiros) && count($conselheiros) >= 6) {
            return 0;
        } else {
            return 1;
        }
    }

    /**
     * ordenaPorCampo - ordena um array por um de seus indices.
     * @param unknown $arAux
     * @param unknown $campo
     * @return Array ordenado
     */
    public function ordenaPorCampo($arAux, $campo){
        $sortArray = array();

        foreach($arAux as $dado){
            foreach($dado as $key=>$value){
                if(!isset($sortArray[$key])){
                    $sortArray[$key] = array();
                }
                $sortArray[$key][] = $value;
            }
        }

        $orderby = $campo;

        array_multisort($sortArray[$orderby],SORT_DESC,$arAux);

        return $arAux;
    }

    /**
     *
     * @param unknown $arr
     * @param unknown $chave
     */
    public function agrupaArrayChave($arr, $chave){

        $arrAux = array();

        foreach( $arr as $key => $inputAux ){

            $arrAux[$inputAux[$chave]][] = $inputAux;
        }
        return $arrAux;
    }

    /**
     * Função carregarWidgetPrincipal
     * - desenha a tela da unidade de acordo com o menu a ser preenchido
     *
     * @return escreve a tela.
     *
     */
    public function carregarWidgetPrincipal($pendencia)
    {
        $pastaPendencias = 'par3/modulos/principal/planoTrabalho/pendencias/';

        if($pendencia == 'obrasPar') {
            $pendencia = 'par';
        }

        switch($pendencia){
            case 'cae':
            case 'monitoramento':
            case 'habilitacao':
            case 'contas':
            case 'par':
            case 'cacs':
            case 'pne':
            case 'siope':
                include APPRAIZ.$pastaPendencias.$pendencia.'.php';
                break;
        }
    }

    /**
     * Função: consultaPendenciaMonitoramentoPAR
     * - Retorna se a entidade está ou não regular no CACS.
     * @return array
     */
    public function consultaPendenciaMonitoramentoPAR($inuid){

        $oPendencia = new Par3_Model_Pendencias();
        $instrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);
        $estuf    = $instrumentoUnidade->estuf;
        $muncod   = $instrumentoUnidade->muncod;
        $itrid    = $instrumentoUnidade->itrid;

        $boExisteRegraPagamento = $oPendencia->recuperaProcessosRegraPagamento($inuid, $estuf, $muncod , $itrid );

        $boExisteSaldoconta = $oPendencia->recuperaProcessosRegraSaldoConta($inuid, $estuf, $muncod , $itrid );

        $arrPendencia = $oPendencia->recuperaProcessosRegraMonitoramento2010($inuid);
        $boPendenciaMonitoramento = count($arrPendencia['pendencias']) > 0 ? true : false;

        $arrPendenciaTermos = $oPendencia->recuperaProcessosRegraMonitoramento2010Termos($inuid);

        $boPendenciaMonitoramentoTermos = count($arrPendenciaTermos['pendencias']) > 0 ? true : false;

        if( !$boExisteRegraPagamento && !$boExisteSaldoconta && !$boPendenciaMonitoramento && !$boPendenciaMonitoramentoTermos ) {
            return 0;
        }else{
            return 1;
        }
    }

    /**
     * Função formAdminstrarPendencia
     * - monta o formulario de dados de pessoa fisica
     *
     * @return string escreve a lista.
     *
     */
    public function formAdminstrarPendencia($disabled = 'disabled')
    {
        global $simec;

        /*
         * @todo Verificar tabela que possui estas informações
        */
        $classPendencia = array(
            'cae' => 'Conselho de Alimentação Escolar',
            'cacs' => 'Cons. de Acomp. de Controle Social',
            'pme' => 'Plano Municipal de Educação',
            'contas' => 'Prestação de Contas',
            'monitoramento' => 'Monitoramento PAR',
            'siope' => 'SIOPE',
            'par' => 'Obras do PAR',
            'habilitacao' => 'Habilitação'
        );

        $regras = array(
            '1' => 'Processos x Notas Fiscais',
            '2' => 'Termos x Saldo em conta'
        );

        echo $simec->select('class', 'Classificação da Pendência', '', $classPendencia , array('maxlength' => '255', $disabled, 'required'));
        echo $simec->select('regras', 'Regras', '', $regras , array('maxlength' => '255', $disabled, 'required'));

    }

    /**
     * Função formDetalheAdminstrarPendencia
     * - monta o formulario de dados de pessoa fisica
     *
     * @return string escreve a lista.
     *
     */
    public function formDetalheAdminstrarPendencia($disabled = 'disabled')
    {
        global $simec;

        $radioBoolean = array('t' => 'Sim', 'f' => 'Não');
        $tiposubacao = array(
            '0' => 'Selecione..',
            '1' => 'Tipo Subação 1',
            '2' => 'Tipo Subação 2'
        );

        echo $simec->textarea('descricaoregra', 'Descrição da Regra', $indjustificativa, array());
        echo $simec->select('tiposubacao', 'Tipo de Sub Ação', '', $tiposubacao , array('maxlength' => '255', $disabled, 'required'));

    }

    /**
     * Função formConfigAdminstrarPendencia
     * - monta o formulario de dados de pessoa fisica
     *
     * @return string escreve a lista.
     *
     */
    public function formConfigAdminstrarPendencia($disabled = 'disabled')
    {
        global $simec;
    }

    /**
     * Função formConfigAdminstrarPendencia
     * - monta o formulario de dados de pessoa fisica
     *
     * @return string escreve a lista.
     *
     */
    public function carregarPendenciasPreenchimento($inuid)
    {
        $instrumentoUnidadeController = new Par3_Controller_InstrumentoUnidade();
        $pneController = new Par3_Controller_Pne();

        $pendencias = array();
        $pendencias[] = array('descricao' => 'Dados da Unidade', 'itens' => $instrumentoUnidadeController->verificaPreencimentoUnidadeDetalhado($inuid));
        $pendencias[] = array('descricao' => 'Plano Nacional de Educação', 'itens' => $pneController->verificaPreenchimentoPNE($inuid, true));
        $pendencias[] = array('descricao' => 'Questões Estratégicas', 'itens' => $instrumentoUnidadeController->verificaPreencimentoQuestoes($inuid,true));
        $pendencias[] = array('descricao' => 'PNE: Questões Complementares', 'itens' => $instrumentoUnidadeController->verificaPreencimentoQuestoesPNE($inuid, true));

        return $pendencias;
    }

    /**
     * Função detalharPendencia
     * - gerencia tipos de detalhamento de pendencias
     *
     * @return string escreve a lista.
     *
     */
    public function detalharPendencia($dados)
    {
        switch($dados['tipo']){
            case 'monitoramento':
                $this->detalharPendenciaMonitoramento($dados);
                break;
        }
        return $pendencias;
    }//end detalharPendencia();

    /**
     * Função detalharPendenciaMonitoramento
     * - monta detalhamentos de monitoramento
     *
     * @return string escreve a lista.
     *
     */
    public function detalharPendenciaMonitoramento($dados)
    {
        $pendencias = new Par3_Model_Pendencias();

        $entidade = new Par3_Model_InstrumentoUnidade($inuid);

        $dados['inuid_par'] = $entidade->recuperarInuidPar($dados['inuid']);

        switch($dados['funcao']){
            case 'item_sem_contrato':
                $dados = $pendencias->buscaDetalheItemSemContrato($dados);
                break;
            case 'item_contrato_sem_qtd_recebido':
                $dados = $pendencias->buscaDetalheItemContratoSqmQtdRecebido($dados);
                break;
            case 'item_sem_detalhar':
                $dados = $pendencias->buscaDetalheItemSemDetalhar($dados);
                break;
            case 'item_detalhado_menor_recebido':
                $dados = $pendencias->buscaDetalheItemDetalhadoMenosRecebido($dados);
                break;
            case 'item_sem_nota':
                $dados = $pendencias->buscaDetalheItemSemNota($dados);
                break;
            case 'contrato_diligencia':
                $dados = $pendencias->buscaDetalheContratoDiligencia($dados);
                break;
            case 'nota_diligencia':
                $dados = $pendencias->buscaDetalheNotaDiligencia($dados);
                break;
            case 'termos_n_assinados':
                $dados = $pendencias->buscaDetalheTermosNAssinados($dados);
                break;
        }

        include APPRAIZ.'par3/modulos/principal/planoTrabalho/pendencias/monitoramento/detalheMonitoramento.php';
    }//end detalharPendenciaMonitoramento();


    /**
     * @author Leo Kenzley<leo.oliveira@cast.com.br>
     * @description <Esta função retorna os dados das restrições das obras onde a coluna R do Obras 2.0 está em vermelho Hisórioa Tuleap 12936>
     */
    public function recuperaRestricoesEntidade($inuid){
        if(isset($inuid) && $inuid != ''){

        }
    }//end method recuperaRestricoesEntidade();


    /**
     * @author Leo Kenzley<leo.oliveira@cast.com.br>
     * @param array $arrayObras
     * @return string
     * @description <Função monta o HTML Response de pendências de Obras>
     */
    public function montaPendenciasObras($arrayPendecias){
        $html = '';
        if(is_array($arrayPendecias) && count($arrayPendecias) > 0){

            foreach($arrayPendecias as $key => $array){
                $html .= "<a class=\"list-group-item active\" style=\"display:none;pointer-events: none;cursor: default;\" href=\"#\" >";
                $html .= "<h4 class=\"list-group-item-heading\">ID Obras2: {$array['obrid']} - {$array['obrnome']} </h4>";
                $html .= "<p class=\"list-group-item-text\"> Motivo: {$array['tipopendencia']}</p>";
            }

        }
        return $html;
    }

    /**
     * @author Leo Kenzley<leo.oliveira@cast.com.br>
     * @description <Esta função retorna os dados das restrições das obras onde a coluna R do Obras 2.0 está em vermelho Hisórioa Tuleap 12936>
     */
    public function montaRestricoesInstrumentoUnidade($arrObrasComRestricaoR){

        $modeloRestricao = new Par3_Model_Restricao();
        $html = '';
        if(isset($arrObrasComRestricaoR[0])){
            for($i = 0; $i < count($arrObrasComRestricaoR); $i++){
                $html .= '<a class="list-group-item active" style="display:none; pointer-events: none;cursor: default;" href="#">';
                $html .= '<h4 class="list-group-item-heading">'."RESTRIÇÃO:ID Obras2: ".$arrObrasComRestricaoR[$i]['obrid']." - ".$arrObrasComRestricaoR[$i]['obrnome'].'</h4>';
                if($modeloRestricao->existeRestricoesDaObra($arrObrasComRestricaoR[$i]['obrid']) == true){
                    $arrRestricoesDaObra = $modeloRestricao->getRestricoesDaObra($arrObrasComRestricaoR[$i]['obrid']);
                    foreach($arrRestricoesDaObra as $key => $value){
                        //
                        $html .= " - ".$value['rstdsc']."</p>";
                    }
                }
                $html .= '</a>';

            }

        }

        return $html;
    }


    /**
     *
     */
    public function montaPendeciasRestricoesExiste($htmlResponseContent){
        $html = "<div class=\"col-lg-12 par\">".
            "<div class='widget yellow-bg text-left' style='background-color: #ed5565;'>".
            "<div class=\"slim-scroll\">".
            "<i class=\"fa fa-warning fa-2x\"></i>".
            "<span class=\"font-bold no-margins\" style=\"font-size: 16pt;\">Pendências de Obras do PAR </span>".
            "<i class=\"fa fa-plus-circle abrir-pendencias\" style=\"cursor:pointer;\"> Exibir todas as pendências</i>".
            "<div class=\"list-pendencias-content\">".
            "$htmlResponseContent".
            "</div></div></div></div>";
        return $html;
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param array $array | dentro do array deve conter: (estuf 'sigla do estado', mundescricao 'Nome do município')
     * @description <Quando não houver nem pendências e nem restrições nas obras do município>
     */
    public function montaSemPendenciaSemRestricao($nomeEntidade){
        $html = '';
        if($nomeEntidade != ''){
            $html .= "<div class=\"col-lg-12 par\">";
            $html .= "<div class=\"widget yellow-bg text-left\" style=\"background-color: #ed5565;\">";
            $html .= "<div class=\"slim-scroll\">";
            $html .= "<i class=\"fa fa-thumbs-up fa-2x\"></i>";
            $html .= "<span class=\"font-bold no-margins\" style=\"font-size: 16pt;\">Pendências de Obras do PAR </span>";
            $html .= "<div class=\"pendencia-content\">";
            $html .= "<p></p>";
            $html .= "<h3>{$nomeEntidade}</h3>";
            $html .= "<p></p>";
            $html .= "<div class=\"list-group\">";
            $html .= "<a class=\"list-group-item active\" href=\"#\">";
            $html .= "<h4 class=\"list-group-item-heading\">Não existem pendências de Obras do PAR.</h4>";
            $html .= "</a>";
            $html .= "</div></div></div></div></div>";
        }
        return $html;
    }// end function montaSemPendenciaSemRestricao

    /**
     *
     */
    private function notIsPendencias(){
        return array(2056);
    }

    /**
     * @author Leo Kenzley<leo.oliveira@castgroup.com.br>
     * @param $obrid
     * @return boolean
     */
    private function isPendencia($obrid){
        if(isset($obrid) && !is_null($obrid) && $obrid != ''){
            $modelObra = new Par3_Model_Obra();

        }
    }

    /**
     * Função: consulta Pendencia Prestação de Contas na view do PAR 2011
     * - Retorna se a entidade está ou não adimplente.
     * @return array
     */
    public function consultaPendenciaPrestacaoContas($inuid){

        $instrumentoUnidade = new InstrumentoUnidade($inuid);

        if ($instrumentoUnidade->getEntidadeInadimplente($inuid)) {
            return 1;
        } else {
            return 0;
        }

    }
}