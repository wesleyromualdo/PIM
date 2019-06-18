<?php

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';
require_once APPRAIZ . 'includes/workflow.php';

class Par3_Model_EscolasEscolaAcessivel extends Modelo
{
    protected $stNomeTabela = 'par3.escolas_ea';

    protected $arChavePrimaria = array(
        'eeaid'
    );

    protected $arChaveEstrangeira = array(
        'estuf' => array('tabela' => 'territorios.estado', 'pk' => 'estuf'),
        'muncod' => array('tabela' => 'territorios.municipio', 'pk' => 'muncod'),
    );
        
    protected $arAtributos = array(
        'eeacodinep' => null,
        'estuf' => null,
        'muncod' => null,
        'eeanome' => null,
        'eeaapta' => null,
        'eearede' => null,
        'eeaano_censo' => null,
        'eeaano_ciclo' => null,
        'eeaqtdmatbas' => null,
        'eeaqtdmatesp' => null,
        'eeaselecionada' => null,
        'eeavalordestinado' => null,
    );

    public function antesSalvar()
    {
        return parent::antesSalvar();
    }

    public function getListaEscolasElegiveis($arrParametros = array())
    {
        
//        ver($arrParametros);exit();
        $objAdmEscolas = new Par3_Model_Adesaoescolaacessivel();
        $pfaid = 12; // escola acessível
        $arrEscolas = array();

        $itrid = $arrParametros['itrid'];
        $prgid = $arrParametros['prgid'];
        $inuid = $arrParametros['inuid'];
        $adpid = $arrParametros['adpid'];
        $apta  = $arrParametros['apta'];
        $eeaselecionada  = $arrParametros['eeaselecionada'];
        $adpano_ciclo = $_SESSION['par3']['adpano_ciclo'];


        /******************************************************/
        if ($adpid) {
            $docid = $this->pegaUm("SELECT docid FROM par3.prodesaoprograma 
                                    WHERE adpid = $adpid AND pfaid = (  SELECT pfaid FROM par3.proadesao 
                                    WHERE prgid = {$prgid} AND pfaano = $adpano_ciclo AND pfastatus = 'A')");
        }
        $bloqueiaEdicao = FALSE;
        if ($docid) {
            $situacaoAtual = wf_pegarEstadoAtual($docid);
            if (
            ($situacaoAtual['esdid'] == WF_ESDID_TERMONAOACEITO_ESCOLAACESSIVEL || $situacaoAtual['esdid'] == WF_ESDID_ENVIADOPARAOMEC_ESCOLAACESSIVEL)
            ) {
                $bloqueiaEdicao = true;
            }
        }

        /******************************************************/

        /******************************************************/

        //Filtrar filtros

        if ($arrParametros['eeamunicipio'] != '') {
            // Caso seja municipal, pode ignorar, já que só aparecera a nível de informação, pois já teremos o muncod
            if ($itrid == 1) {
                $arrWhere[] = " mu.mundescricao ilike '%{$arrParametros['eeamunicipio']}%' ";
            }
        }
        if (!empty($eeaselecionada) && $eeaselecionada == 'S') {
            $arrWhere[] = "eeaselecionada = 'true' ";
        }
        if (!empty($eeaselecionada) && $eeaselecionada == 'N') {
            $arrWhere[] = "eeaselecionada = 'false' ";
        }
        if ($apta != '') {
            $arrWhere[] = "eeaapta = '{$apta}' ";
        }
        if ($arrParametros['eearede'] != '') {
            $arrWhere[] = "eearede = '{$arrParametros['eearede']}' ";
        }
        if ($arrParametros['eeacod'] != '') {
            $arrWhere[] = "eeacodinep = '{$arrParametros['eeacod']}' ";
        }
        if ($arrParametros['eeanome'] != '') {
            $arrWhere[] = "eeanome ilike '%{$arrParametros['eeanome']}%' ";
        }
        if ($arrParametros['filtro_extra'] != '') {
            $arrWhere[] = $arrParametros['filtro_extra'];
        }
        
        if($apta == 'true'){
            $arrCodInepInseridos = $objAdmEscolas->retornaCodEntidadeMarcados($arrParametros);
        } else {
            $arrCodInepInseridos = NULL;
        }

        if ($arrParametros['checado'] != '') {
            $not = '';
            if ($arrParametros['checado'] == 'N') {
                $not = 'not';
            }
            if (count($arrCodInepInseridos) > 0 && !empty($arrCodInepInseridos)) {
                $arrWhere[] = "eeacodinep::integer {$not} in ( " . implode(', ', $arrCodInepInseridos) . " )";
            }
        }
        
        if ($arrParametros['ordenacao']) {
            $orderBy = " ";
            switch ($arrParametros['ordenacao']) {
                case "inudescricao":
                    $orderBy = "ORDER BY inudescricao ASC";
                    break;
                case "eeanome":
                    $orderBy = "ORDER BY eeanome ASC";
                    break;
                case "eeacodinep":
                    $orderBy = "ORDER BY eeacodinep ASC";
                    break;
                case "eeaqtdmatbas":
                    $orderBy = "ORDER BY eeaqtdmatbas ASC";
                    break;
                case "eeaqtdmatesp":
                    $orderBy = "ORDER BY eeaqtdmatesp ASC";
                    break;
                case "eeavalordestinado":
                    $orderBy = "ORDER BY eeavalordestinado ASC";
                    break;
            }
        } else {
            $orderBy = " order by selecionada, eeaqtdmatbas DESC ";
        }

        /******************************************************/


        if ($itrid == 2) {
            $arrWhere[] = " eearede = 'Municipal' ";
            $join = " INNER JOIN par3.instrumentounidade iu ON iu.muncod = ema.muncod";
            $join .= " inner join territorios.municipio mu on ema.muncod = mu.muncod ";
        } elseif ($itrid == 1 || $itrid == 3) {
            $arrWhere[] = " eearede = 'Estadual' ";
            $join = " INNER JOIN par3.instrumentounidade iu ON iu.estuf = ema.estuf";
            $join .= " inner join territorios.municipio mu on ema.muncod = mu.muncod ";
        }

        $sqlWhere = " WHERE inuid = {$inuid} ";
        if (is_array($arrWhere)) {
            $sqlWhere .= ' AND ' . implode(' AND ', $arrWhere);
        }

        $colunas = " eeacodinep, ema.estuf, mu.mundescricao as inudescricao, eeanome, eeaqtdmatbas, eeaqtdmatesp, eeavalordestinado, eeaselecionada, eeamotivo, case when eeaselecionada = true then '1' else '2' end as selecionada ";
        $sql = " SELECT {$colunas} FROM {$this->stNomeTabela} ema {$join} {$sqlWhere}  {$orderBy}";
        
//        ver($sql);
        $resultado = $this->carregar($sql);
        $resultado = (is_array($resultado)) ? $resultado : array();

        /******************************************************/

        if (count($resultado) > 0) {

            $disabledJainseridas = '';
            $disabledEdicao = '';
            if ($bloqueiaEdicao) $disabledEdicao = 'disabled';

            $modelAdesaoPrograma = new Par3_Model_AdesaoPrograma();
            $result = $modelAdesaoPrograma->recuperaPorInuid($inuid, $pfaid);

            $docidInserido = $result['docid'];
            $situacaoAtual = wf_pegarEstadoAtual($docidInserido);
            $esdid = $situacaoAtual['esdid'];
            if ($esdid && $esdid == WF_ESDID_ENVIADOPARAOMEC_ESCOLAACESSIVEL) {
                $disabledEdicao = '';
                $disabledJainseridas = 'disabled';
            }

//            if (isset($arrParametros['filtro_extra'])) {
//                $arrDisabledEdicao = array();
//                $arrCodInepInseridos = null;
//                $sql = " SELECT * FROM par3.excecaoescolasprograma exc
//                INNER JOIN par3.escolasexcecaoprograma esc ON esc.eepid = exc.eepid
//                WHERE prgid = {$prgid} AND inuid = {$inuid} ";
//
//                $exc = $this->carregar($sql);
//                $exc = (is_array($exc)) ? $exc : array();
//
//                foreach ($exc as $k => $v) {
//                    $arrCodInepInseridos[] = $v['codinep'];
//                    $arrDisabledEdicao[$v['codinep']] = true;
//                }
//                $disabledEdicao = '';
//                $disabledJainseridas = '';
//            }
            // verificar se ja existe alguma escola inserida na adesao pelo codinep       
            if (count($arrCodInepInseridos) > 0) {
                $validaEscInseridaAdesao = $this->verificaEscolasInseridasAdesao($pfaid, $arrCodInepInseridos);
            }
            
            foreach ($resultado as $k => $escola) {
                //ver($escola);
                $resAdesaoEscola = $this->verificaAdesaoEscolaSalvarCodinep($pfaid, $escola['eeacodinep']);
//                ver($resAdesaoEscola);
                
//                if ($escola['eeaselecionada'] == 't') {
//                    $checkedJainseridas = 'checked';
//                    $valordestinadoescola = $escola['eeavalordestinado'];
//                    $somaVlrChecados = $somaVlrChecados + $valordestinadoescola;
//                } 
//                else {
//                    $checkedJainseridas = '';
//                }
                if($validaEscInseridaAdesao){
                    // $escola['eeaselecionada'] == 't' && 
                    if ($resAdesaoEscola) {
                        $checkedJainseridas = 'checked';
    //                    $escolaEstaSalva = '<span class="label label-success">Salva</span>';
                        $valordestinadoescola = $escola['eeavalordestinado'];
                        $somaVlrChecados = $somaVlrChecados + $valordestinadoescola;
                    } else {
                        $checkedJainseridas = '';
                    }
                } else {
                    // 
                    if ($escola['eeaselecionada'] == 't' && !$resAdesaoEscola) {
                            $checkedJainseridas = 'checked';
//                            $escolaEstaSalva = '<span class="label label-warning">Selecionada e Não Salva</span>';
                            $valordestinadoescola = $escola['eeavalordestinado'];
                            $somaVlrChecados = $somaVlrChecados + $valordestinadoescola;                    
                    } else {
                        $checkedJainseridas = '';
//                        $escolaEstaSalva = '<span class="label label-danger">N/A</span>';
                    }
                }
                
//                if (is_array($arrCodInepInseridos) && in_array($escola['eeacodinep'], $arrCodInepInseridos)) {
//                    $checkedJainseridas = 'checked';                    
//                    if($checkedJainseridas == 'checked'){
//                        $valordestinadoescola = $escola['eeavalordestinado'];
//                        $somaVlrChecados = $somaVlrChecados + $valordestinadoescola;
//                    }
//                } else {
//                    $checkedJainseridas = '';
//                }

                if(is_array($arrDisabledEdicao)) {
                    $disabledEdicao = ($arrDisabledEdicao[$escola['eeacodinep']] ? 'disabled' : '');
                }
                
                if($apta == 'true'){
                $arrEscolas[$k]['acao'] = <<<HTML
    <input {$disabledJainseridas} {$disabledEdicao} {$checkedJainseridas} class="checkbox-primary escola_input" onclick="pegarValorDestinado(this)"  type=checkbox name="escola_checada[{$escola['eeacodinep']}]" 
        id="{$escola['eeacodinep']}" value="{$escola['eeacodinep']}" data-valor-destinado="{$escola['eeavalordestinado']}">
HTML;
                } 
                
                $arrEscolas[$k]['estuf'] = $escola['estuf'].$escolaEstaSalva;
                $arrEscolas[$k]['inudescricao'] = $escola['inudescricao'];
                $arrEscolas[$k]['eeacodinep'] = $escola['eeacodinep'];
                $arrEscolas[$k]['eeanome'] = $escola['eeanome'];
                //$arrEscolas[$k]['eeanome'] .= "<input type=hidden name=nome_escola[{$escola['eeacodinep']}] value=\"{$escola['eeanome']}\" /> ";
                $arrEscolas[$k]['eeaqtdmatbas'] .= $escola['eeaqtdmatbas'];
                $arrEscolas[$k]['eeaqtdmatesp'] .= $escola['eeaqtdmatesp'];
                if($apta == 'true'){
                    $arrEscolas[$k]['eeavalordestinado'] .= simec_number_format($escola['eeavalordestinado'], 2, ',', '.' );
                } else {
                    $arrEscolas[$k]['eeamotivo'] .= $escola['eeamotivo'];
                }
//                $arrEscolas[$k]['escolasalva'] .= $resAdesaoEscola;
            }
        }
        if($apta == 'true'){
            $todos = "Marcar/Desmarcar";
//            $todos = <<<HTML
//Marcar/Desmarcar <br> 
//    <!--<input {$disabledEdicao} {$disabledJainseridas} class="checkbox-primary" type=checkbox name="selecionar_todos" id="selecionar_todos" title="selecionar todos">-->
//HTML;
        } else {
            $todos = '';
        }
        
        if($apta == 'true'){
            $cabecalho = [$todos, 'UF', 'Município', 'Código da Escola', 'Nome da Escola', 'Total de matrícula Educação Básica', 'Total de matrícula Educação Especial', 'Valor Destinado (R$)'];
        } else {
            $cabecalho = ['UF', 'Município', 'Código da Escola', 'Nome da Escola', 'Total de matrícula Educação Básica', 'Total de matrícula Educação Especial', 'Motivo'];
        }
        $arrayRetorno['cabecalho'] = $cabecalho;
        $arrayRetorno['arrayEscolas'] = $arrEscolas;
        $arrayRetorno['bloqueio'] = $bloqueiaEdicao;
        $arrayRetorno['status'] = 'success';
        $arrayRetorno['valorEscolasChecadas'] = $somaVlrChecados;
//        ver($arrayRetorno);
        return $arrayRetorno;
        
    }
    
    public function verificaEscolasInseridasAdesao($pfaid, $arrCodInepInseridos){
        $sqlES = "SELECT count(aea.codinep)
                FROM par3.adesaoescolaacessivel aea
                INNER JOIN par3.escolas_ea eea ON eea.eeacodinep = aea.codinep
                WHERE aea.codinep::integer IN ( " . implode(', ', $arrCodInepInseridos) . " ) AND eea.pfaid = {$pfaid} ";

        $escolaSalva = $this->pegaUm($sqlES);
                
        if($escolaSalva > 0){
            return true;
        } else {
            return false;
        }
    }
    
    public function recuperaTotalValorEscolasChecadas($dados){
        
        $pfaid = 12; // escola acessível        
        //ver($dados, d);
        if(is_array($dados['escola_checada'])){
            $codinep = implode(',', $dados['escola_checada']);
        } else {
            $codinep = null;
        }
        
        $valorUF = $dados['valor_disponivel_uf'];
        $valorMun = $dados['valor_disponivel_mun'];
        
        // se o valor disponivel por estado ou por municipio estiver vazio
        if((isset($valorUF) && $valorUF == 0)){            
            echo 'false';
            exit();
        }
        if((isset($valorMun) && $valorMun == 0)){            
            echo 'false';
            exit();
        }
        if (!is_null($codinep)) {
        
            $sql = "SELECT SUM(eeavalordestinado)
                    FROM par3.escolas_ea 
                    WHERE eeacodinep IN ({$codinep}) and pfaid = ($pfaid)";

            $valor = $this->pegaUm($sql);

            if($valor){
                if($valorUF){
                    $saldo = $valorUF - $valor;
                }
                else if ($valorMun){
                    $saldo = $valorMun - $valor;
                }

                $valorFinal = formata_valor($saldo);
            } else {
                $valorFinal = '0.00';
            }
            //ver($saldo);
            if($saldo && $saldo < 0){
                echo 'false';
                exit();
            }
        } else {
            if($valorUF){
                $valorFinal = formata_valor($valorUF);
            }
            else if ($valorMun){
                $valorFinal = formata_valor($valorMun);
            }
        }
        
        if($valorFinal < 0){
            echo 'false';
        } else {
            return $valorFinal;
        }
        
    }
    
    public function verificaAdesaoEscolaSalvarCodinep($pfaid, $codinep){
        
        $sqlES = "SELECT aea.codinep 
                FROM par3.adesaoescolaacessivel aea
                INNER JOIN par3.escolas_ea eea ON eea.eeacodinep = aea.codinep
                WHERE eea.pfaid = {$pfaid} and aea.codinep = {$codinep}";

        $escolaSalva = $this->pegaUm($sqlES);
                
        if(!empty($escolaSalva)){
//            echo '<br>Escola: '.$escolaSalva.' está salva ';
            return true;
        } else {
//            echo '<br>Escola: '.$escolaSalva.' não está salva ';
            return false;
        }
    }
    
    public function getListaEscolasNaoElegiveis($arrParametros = array())
    {
        $objAdmEscolas = new Par3_Model_Adesaoescolaacessivel();
        $arrEscolas = array();

        $itrid = $arrParametros['itrid'];
        $prgid = $arrParametros['prgid'];
        $inuid = $arrParametros['inuid'];
        $adpid = $arrParametros['adpid'];
        $adpano_ciclo = $_SESSION['par3']['adpano_ciclo'];


        /******************************************************/
        if ($adpid) {
            $docid = $this->pegaUm("SELECT docid FROM par3.prodesaoprograma 
                                    WHERE adpid = $adpid AND pfaid = 
                                    ( SELECT pfaid FROM par3.proadesao 
                                    WHERE prgid = {$prgid} AND pfaano = $adpano_ciclo AND pfastatus = 'A')");
        }

        /******************************************************/

        //Filtros

//        if ($arrParametros['eeamunicipio'] != '') {
//            // Caso seja municipal, pode ignorar, já que só aparecera a nível de informação, pois já teremos o muncod
//            if ($itrid == 1) {
//                $arrWhere[] = "eeamunicipio	ilike '%{$arrParametros['eeamunicipio']}%' ";
//            }
//        }
        if ($arrParametros['eearede'] != '') {
            $arrWhere[] = "eearede	= '{$arrParametros['eearede']}' ";
        }
        if ($arrParametros['eeacod'] != '') {
            $arrWhere[] = "eeacodinep	= '{$arrParametros['eeacod']}' ";
        }
        if ($arrParametros['eeanome'] != '') {
            $arrWhere[] = "eeanome	ilike '%{$arrParametros['eeanome']}%' ";
        }
        if ($arrParametros['filtro_extra'] != '') {
            $arrWhere[] = $arrParametros['filtro_extra'];
        }
        
        if ($arrParametros['ordenacao']) {
            $orderBy = "";
            switch ($arrParametros['ordenacao']) {
                case "municipio":
                    $orderBy = "ORDER BY municipio ASC";
                    break;
                case "eeanome":
                    $orderBy = "ORDER BY eeanome ASC";
                    break;
                case "eeacodinep":
                    $orderBy = "ORDER BY eeacodinep ASC";
                    break;
            }
        }

        /******************************************************/

        if ($itrid == 2) {
            $arrWhere[] = " eearede = 'Municipal' ";
            $join = " INNER JOIN par3.instrumentounidade iu ON iu.muncod = ema.muncod ";
        } elseif ($itrid == 1 || $itrid == 3) {
            $arrWhere[] = " eearede = 'Estadual' ";
            $join = " INNER JOIN par3.instrumentounidade iu ON iu.estuf = ema.estuf ";
        }

        $sqlWhere = " WHERE inuid = {$inuid} AND eeaapta = TRUE ";
        if (is_array($arrWhere)) {
            $sqlWhere .= ' AND ' . implode(' AND ', $arrWhere);
        }

        $colunas = " eeacodinep, ema.estuf, inudescricao, eeanome, CASE WHEN eeaapta = TRUE THEN 'SIM' ELSE 'NÃO' END AS eeaapta  ";
        $sql = " SELECT {$colunas} FROM {$this->stNomeTabela} ema {$join} {$sqlWhere} {$orderBy} ";
        
        $resultado = $this->carregar($sql);
        $resultado = (is_array($resultado)) ? $resultado : array();

        /******************************************************/

        if (count($resultado) > 0) {

            foreach ($resultado as $k => $escola) {
                $arrEscolas[$k]['estuf'] = $escola['estuf'];
                $arrEscolas[$k]['inudescricao'] = $escola['inudescricao'];
                $arrEscolas[$k]['eeacodinep'] = $escola['eeacodinep'];
                $arrEscolas[$k]['eeanome'] = $escola['eeanome'];
                $arrEscolas[$k]['eeanome'] .= "<input type=hidden name=nome_escola[{$escola['eeacodinep']}] value=\"{$escola['eeanome']}\" /> ";
            }
        }

        $cabecalho = ['UF', 'Município', 'Código da Escola', 'Nome da Escola'];
        $arrayRetorno['cabecalho'] = $cabecalho;
        $arrayRetorno['arrayEscolas'] = $arrEscolas;
        $arrayRetorno['status'] = 'success';
        return $arrayRetorno;
    }

    public function getListaEscolasAptasUF($pfaid)
    {
        $sql = "SELECT DISTINCT eea.estuf FROM {$this->stNomeTabela} eea
                WHERE eea.eeaapta = TRUE and eea.pfaid = {$pfaid} and eea.eearede = 'Estadual'
                GROUP BY eea.eeaid, eea.estuf order by eea.estuf ASC";
//        ver($sql);
        $res = $this->carregar($sql);
        return $res;
    }
    
    public function getListaEscolasAptasMu($pfaid)
    {
        $sql = "SELECT DISTINCT eea.muncod, iu.inudescricao FROM {$this->stNomeTabela} eea
                inner join par3.instrumentounidade iu on iu.muncod = eea.muncod
                WHERE eea.eeaapta = TRUE and iu.inustatus = 'A' and eea.pfaid = {$pfaid} and eea.eearede = 'Municipal'
                GROUP BY eea.muncod, iu.inudescricao order by iu.inudescricao ASC";
        
//        ver($sql);
        $res = $this->carregar($sql);
        return $res;
    }
       
    public function getSomaValorEscolasAptasMunUF($pfaid)
    {
        $sql = "               
            (select
                    eea.estuf, ' - ' as muncod, ' - ' as municipio, eea.eearede as esfera, sum( eea.eeavalordestinado ) as valordestinado
            from
                    {$this->stNomeTabela} eea
            left join par3.instrumentounidade iu on
                    iu.muncod = eea.muncod
            where
                    eea.eeaapta = true
                    and eea.eeaselecionada = true
                    and eea.pfaid = {$pfaid}
                    and eea.eearede = 'Estadual'
            group by
                    eea.estuf, eea.eearede)

            union all

            (select
                    eea.estuf, eea.muncod, iu.inudescricao as municipio, eea.eearede as esfera, sum( eea.eeavalordestinado ) as valordestinado
            from
                    {$this->stNomeTabela} eea
            inner join par3.instrumentounidade iu on
                    iu.muncod = eea.muncod
            where
                    eea.eeaapta = true
                    and eea.eeaselecionada = true
                    and iu.inustatus = 'A'
                    and eea.pfaid = {$pfaid}
                    and eea.eearede = 'Municipal'
            group by eea.estuf, eea.muncod, iu.inudescricao, eea.eearede)
            order by estuf asc ";
                        
//        ver($sql);die;
        $res = $this->carregar($sql);
        return $res;
    }

    public function getListaEscolasAptasPrograma($pfaid)
    {
        $cabecalho = array('UF','Município', 'Escola', 'Código INEP','Valor (R$)', 'Esfera', 'Selecionada');

        $sql = "SELECT eea.eeaid, eea.estuf, iu.inudescricao, eea.eeanome, eea.eeacodinep, eea.eeavalordestinado, eea.eearede, 
                CASE WHEN eea.eeaselecionada = 't' THEN '<span class=\"label label-success\">Sim</span>' ELSE '<span class=\"label label-danger\">Não</span>' END
                FROM {$this->stNomeTabela} eea
                left join par3.instrumentounidade iu on iu.muncod = eea.muncod
                WHERE eea.eeaapta = TRUE and eea.pfaid = {$pfaid} ORDER BY eea.estuf ASC";

        $listagem = new Simec_Listagem();
        $listagem->setAcoes(array('edit' => 'alterarEscola', 'delete' => 'excluirEscola'))
                 ->setCabecalho ($cabecalho)
                 ->addCallbackDeCampo(['eeavalordestinado'], 'formataNumeroMonetarioSemSimbolo')
                 ->setTamanhoPagina(10)
                 ->setId('listaAptas')
                 ->setQuery ( $sql );

        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }

    public function getListaEscolasInaptasPrograma($pfaid)
    {
        $cabecalho = array('UF','Município', 'Escola', 'Código INEP', 'Esfera', 'Motivo');

        $sql = "SELECT eea.eeaid, eea.estuf, iu.inudescricao, eea.eeanome, eea.eeacodinep, eea.eearede, eea.eeamotivo
                FROM {$this->stNomeTabela} eea
                left join par3.instrumentounidade iu on iu.muncod = eea.muncod
                WHERE eea.eeaapta = FALSE and eea.pfaid = {$pfaid} ORDER BY eea.estuf ASC";

        $listagem = new Simec_Listagem();
        $listagem->setAcoes(array('edit' => 'alterarEscola', 'delete' => 'excluirEscola'))
                 ->setCabecalho ($cabecalho)                                 
                 ->setTamanhoPagina(10)
                 ->setId('listaInaptas')
                 ->setQuery ( $sql );

        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }

    public function getListaEscolasEscolhidas($arrParametros = array())
    {
//        ver($arrParametros);
        $inuid = $arrParametros['inuid'];
        $adpano_ciclo = $arrParametros['adpano_ciclo'];
        $adpid = $arrParametros['adpid'];
        $where = '';
        $sqlDados = <<<SQL


    SELECT DISTINCT
        ema.estuf,
        te.mundescricao,
        aea.codinep :: INTEGER AS codinep,
        ema.eeanome,
        ema.eeavalordestinado,
        adp.inuid,
        adp.adpid
    FROM
        par3.escolas_ea ema
        INNER JOIN par3.adesaoescolaacessivel aea ON aea.codinep = ema.eeacodinep
        INNER JOIN par3.prodesaoprograma adp ON adp.adpid = aea.adpid AND adpano_ciclo = ema.eeaano_ciclo
        inner join territorios.municipio te on te.muncod = ema.muncod
    WHERE
        adp.inuid = {$inuid}
        AND adp.adpid = {$adpid} 
        AND ema.eeaapta = true
        {$where}
      
SQL;

//        ver($sqlDados);
        $cabecalho = array('UF', 'Município', 'Código da Escola', 'Escola', 'Valor Destinado (R$)');
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $listagem->esconderColunas(array('inuid', 'adpid'));
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sqlDados);
        $listagem->turnOffForm();
        $listagem->addCallbackDeCampo('eeavalordestinado', 'par3_mascaraMoeda');
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->render();
        return;
    }
    
    public function getTotalEscolasSelecionadasInuid($arrParametros = array())
    {
        $where = '';
        $inuid = $arrParametros['inuid'];
        $adpano_ciclo = $arrParametros['adpano_ciclo'];
        //$adpid = $arrParametros['adpid'];
        $itrid = $arrParametros['itrid'];
        
        if($inuid && $itrid && $adpano_ciclo){
        
            if ($itrid == 1 || $itrid == 3) {
                
            } else if ($itrid == 2) {
            }
            
            if ($itrid == 2) {
                $join = " INNER JOIN par3.instrumentounidade iu ON iu.muncod = eea.muncod";
                $join .= " inner join territorios.municipio mu on eea.muncod = mu.muncod ";
                $where = " AND eea.eearede = 'Municipal' ";
            } elseif ($itrid == 1 || $itrid == 3) {
                $join = " INNER JOIN par3.instrumentounidade iu ON iu.estuf = eea.estuf";
                $join .= " inner join territorios.municipio mu on eea.muncod = mu.muncod ";
                $where = " AND eea.eearede = 'Estadual' ";
            }

            $sqlDados = "select 
                                count(*)
                            from
                                par3.escolas_ea eea
                                {$join}
                            where 
                            iu.inuid = {$inuid}
                            and eea.eeaano_ciclo = '{$adpano_ciclo}'
                            and eea.eeaapta = 'true'
                            and eea.eeaselecionada = 'true' {$where} ";


            return $this->pegaUm($sqlDados);
        } else {
            return 0;
        }
        
    }
    public function getTotalEscolasEscolhidas($arrParametros = array())
    {
//        ver($arrParametros);
        $inuid = $arrParametros['inuid'];
        $adpano_ciclo = $arrParametros['adpano_ciclo'];
        $adpid = $arrParametros['adpid'];
        $where = '';
        $sqlDados = <<<SQL
            SELECT DISTINCT
                count(*)
            FROM
                par3.escolas_ea ema
                INNER JOIN par3.adesaoescolaacessivel aea ON aea.codinep = ema.eeacodinep
                INNER JOIN par3.prodesaoprograma adp ON adp.adpid = aea.adpid AND adpano_ciclo = ema.eeaano_ciclo
                inner join territorios.municipio te on te.muncod = ema.muncod
            WHERE
                aea.codinep::VARCHAR NOT IN ( SELECT codinep FROM par3.escolasexcecaoprograma exp
                                              INNER JOIN par3.excecaoescolasprograma eep ON eep.eepid = exp.eepid
                                              INNER JOIN par3.prodesaoprograma adp ON adp.adpid = eep.adpid AND adpano_ciclo = {$adpano_ciclo}
                                              WHERE exp.expstatus = 'A' AND eep.eepstatus = 'A' AND eep.eepdecisao = 'S'
                                             )    
            AND
                adp.inuid = {$inuid} AND
                adp.adpid = {$adpid} 
        {$where}
      
SQL;
        return $this->pegaUm($sqlDados);
    }
    
    function valorDisponivelUF($uf, $pfaid){
        global $db;  
        $sql = "select
                    sum( eea.eeavalordestinado ) as valordestinado
                from
                        par3.escolas_ea eea
                left join par3.instrumentounidade iu on
                        iu.muncod = eea.muncod
                where
                        eea.eeaapta = true
                        and eea.eeaselecionada = true
                        and eea.pfaid = {$pfaid}
                        and eea.eearede = 'Estadual'
                        and eea.estuf = '{$uf}' ";
        
        $res = $db->pegaUm($sql);
        
        return $res;
    }
    
    function valorDisponivelMun($muncod, $pfaid){
        global $db;        
        $sql = "select
                    sum( eea.eeavalordestinado ) as valordestinado
                from
                    par3.escolas_ea eea
                inner join par3.instrumentounidade iu on
                    iu.muncod = eea.muncod
                where
                    eea.eeaapta = true
                    and eea.eeaselecionada = true
                    and iu.inustatus = 'A'
                    and eea.pfaid = {$pfaid}
                    and eea.eearede = 'Municipal'
                    and eea.muncod = '{$muncod}' ";
                        
        $res = $db->pegaUm($sql);
        
        return $res;
    }
    
    function atualizaValorTotalPorPrograma($dados){
        global $db;
        $pfaid = $dados['pfaid'];
        
        $valorTotalPrograma = str_replace(',','.',str_replace('.','', $dados['pfavalortotalprograma']));
        $sql = "UPDATE par3.proadesao SET pfavalortotalprograma = '{$valorTotalPrograma}' WHERE pfaid = {$pfaid};";
        $db->executar($sql);
        
        $db->commit();
    }
        
    function importarCsvEscolas($arquivo, $pfaid, $pfaano){
        
        global $db;
        
        $fileAptas = $_FILES['csvapta']['tmp_name'];        
        $primeiraLinhaApta = false;
        
        if(!empty($fileAptas)){
            // escolas aptas
            if (($handle = fopen($fileAptas, "r")) !== FALSE) {            
                while (($data = fgetcsv($handle, 100000, ";")) !== FALSE) {
                    //pular cabeçalho
                    if(!$primeiraLinhaApta) {
                        $primeiraLinhaApta = true;
                        continue;
                    }
                    $codinep = $data[4];
                    $selecionada = $data[8];
                    //ver($data[2]);
                    //die;
                    //$tabela_educacenso = "educacenso_{$data[9]}";
                    // localizar muncod pelo codinep na tabela censo 2017 das escolas selecionadas
                    //if (strtoupper($data[2]) == 'MUNICIPAL') {
                        $muncod = $db->pegaUm("SELECT co_municipio FROM educacenso_2017.ts_censo_basico_escola WHERE co_entidade = '{$codinep}' ");
                    //} else {
                        //$muncod = null;
                    //}
                    // se encontrou muncod então insere
                    //if($muncod){
                        // insert de escolas na tabela escolas_ea
                        $data[10] = $muncod; // resultado muncod
                        $this->salvarEscolasAptasCSV($data, $pfaid, $pfaano);
                    //}
                    //$row++;
                }
                fclose($handle);
                $resApta = true;
            } else {
                $resApta = false;
            }
        }
        
        
        $fileInaptas = $_FILES['csvinapta']['tmp_name'];        
        $primeiraLinhaInapta = false;
        
        if(!empty($fileInaptas)){
            // escolas inaptas
            if (($handle = fopen($fileInaptas, "r")) !== FALSE) {            
                while (($data = fgetcsv($handle, 100000, ";")) !== FALSE) {
                    //pular cabeçalho
                    if(!$primeiraLinhaInapta) {
                        $primeiraLinhaInapta = true;
                        continue;
                    }
                    $codinep = $data[4];
                    $selecionada = $data[8];
                    //$tabela_educacenso = "educacenso_{$data[9]}";
                    // localizar muncod pelo codinep na tabela censo 2017 das escolas selecionadas
                    //if (strtoupper($data[2]) == 'MUNICIPAL') {
                        $muncod = $db->pegaUm("SELECT co_municipio FROM educacenso_2017.ts_censo_basico_escola WHERE co_entidade = '{$codinep}' ");
                    //} else {
                    //    $muncod = null;
                    //}
                    // se encontrou muncod então insere
                    //if($muncod){
                        // insert de escolas na tabela escolas_ea
                        $data[10] = $muncod; // resultado muncod
                        $this->salvarEscolasInaptasCSV($data, $pfaid, $pfaano);
                    //}
                    //$row++;
                }
                fclose($handle);
                $resInapta = true;
            } else {
                $resInapta = false;
            }
        }           
        $arrayRes = array('apta' => $resApta, 'inapta' => $resInapta);
        echo simec_json_encode($arrayRes);
    }
        
    function salvarEscolasAptasCSV($arrDadosSalvar, $pfaid, $pfaano)
    {        
        $eeacodinep = $arrDadosSalvar[4];
        $estuf = $arrDadosSalvar[0];
        $muncod = $arrDadosSalvar[10];
        $eeanome = addslashes($arrDadosSalvar[3]);
        $eeaano_censo = $arrDadosSalvar[9];
        $eeaano_ciclo = $pfaano;
        $eeaapta = 'true';
        $eearede = ucfirst(strtolower($arrDadosSalvar[2]));
        $eeaqtdmatbas = $arrDadosSalvar[5];
        $eeaqtdmatesp = $arrDadosSalvar[6];
        
        if (strtoupper($arrDadosSalvar[8]) == 'SIM') {
            $eeaselecionada = 'true';
        } else {
            $eeaselecionada = 'false';
        }
        $value = pg_escape_string($arrDadosSalvar[7]);
        $valorDestinado = str_replace(',', '.', str_replace('.', '', $value));
        
        $sqlInsert = "INSERT INTO par3.escolas_ea 
                     (eeacodinep, estuf, muncod, eeanome, eeaano_censo, eeaano_ciclo, eeaapta, eearede, eeaqtdmatbas, eeaqtdmatesp, eeavalordestinado, eeaselecionada, eeamotivo, pfaid )
                     VALUES 
                     ('{$eeacodinep}', '{$estuf}', '{$muncod}', '{$eeanome}', {$eeaano_censo}, {$eeaano_ciclo}, $eeaapta, '{$eearede}', '{$eeaqtdmatbas}', '{$eeaqtdmatesp}', '{$valorDestinado}', $eeaselecionada, '', $pfaid) ";
              
        //ver($sqlInsert);die;
        $this->executar($sqlInsert);

        if ($this->commit()) {
            return true;
        } else {
            return false;
        }
    }
    
    function salvarEscolasInaptasCSV($arrDadosSalvar, $pfaid, $pfaano)
    {        
        $eeacodinep = $arrDadosSalvar[4];
        $estuf = $arrDadosSalvar[0];
        $muncod = $arrDadosSalvar[10];
        $eeanome = addslashes($arrDadosSalvar[3]);
        $eeaano_censo = $arrDadosSalvar[8];
        $eeamotivo = addslashes($arrDadosSalvar[9]);
        $eeaano_ciclo = $pfaano;
        $eeaapta = 'false';
        $eearede = ucfirst(strtolower($arrDadosSalvar[2]));
        $eeaqtdmatbas = $arrDadosSalvar[5];
        $eeaqtdmatesp = $arrDadosSalvar[6];
        $eeaselecionada = 'false';
        $valorDestinado = '0.00';
                
        $sqlInsert = "INSERT INTO par3.escolas_ea 
                     (eeacodinep, estuf, muncod, eeanome, eeaano_censo, eeaano_ciclo, eeaapta, eearede, eeaqtdmatbas, eeaqtdmatesp, eeavalordestinado, eeaselecionada, eeamotivo, pfaid )
                     VALUES 
                     ('{$eeacodinep}', '{$estuf}', '{$muncod}', '{$eeanome}', {$eeaano_censo}, {$eeaano_ciclo}, $eeaapta, '{$eearede}', '{$eeaqtdmatbas}', '{$eeaqtdmatesp}', '{$valorDestinado}', $eeaselecionada, '$eeamotivo', $pfaid) ";
                
        //ver($sqlInsert);die;
        $this->executar($sqlInsert);

        if ($this->commit()) {
            return true;
        } else {
            return false;
        }
    }
    
    function salvarDadosEscolaCarga($dados)
    {                
        $valordestinado = str_replace(',', '.', str_replace('.', '', $dados['eeavalordestinado']));
        $sql = "UPDATE par3.escolas_ea SET eeavalordestinado = '{$valordestinado}', eeaapta = '{$dados['eeaapta']}', eeaselecionada = '{$dados['eeaselecionada']}' WHERE eeaid = {$dados['eeaid']};";
        
        $this->executar($sql);

        if ($this->commit()) {
            echo 'true';
        } else {
            echo 'false';
        }
    }
    
    function excluirEscolaCarga($dados)
    { 
        $sql = "SELECT count(*) FROM par3.escolas_ea ea                    
                WHERE ea.eeaapta = '{$dados['apta']}'
                AND ea.pfaid = {$dados['pfaid']}
                AND ea.eeaano_ciclo = '{$_SESSION['exercicio']}'
                AND ea.eeacodinep IN 
                (
                   SELECT aea.codinep FROM par3.adesaoescolaacessivel aea
                   INNER JOIN par3.prodesaoprograma pdp on pdp.adpid = aea.adpid
                   WHERE pdp.pfaid = {$dados['pfaid']}
                ) ";
              
        $resEscolas = $this->pegaUm($sql);
        
        if ($resEscolas > 0) {
            // se existir alguma escola que já foi para adesão então não deixa remover a carga
//            echo 'não pode excluir';
            return $erro = array('erro' => 1);
            
        } else {
            // senão então exclui toda a carga
//            echo 'pode excluir tudo';
            $sql = "DELETE FROM par3.escolas_ea ea
                    WHERE ea.eeaapta = '{$dados['apta']}'
                    AND ea.pfaid = {$dados['pfaid']}
                    AND ea.eeaano_ciclo = '{$_SESSION['exercicio']}' ";

            $this->executar($sql);
            $excluirEscolas = $this->commit();
            
            if($excluirEscolas){
                return $erro = array('erro' => 0);
            } 
        }
//        ver($resEscolas);
//        ver($dados);die;
    }
    
    function excluirEscola($eeaid, $pfaid)
    { 
        $sql = "SELECT count(*) FROM par3.escolas_ea ea                    
                WHERE ea.eeaid = {$eeaid}
                AND ea.eeacodinep IN 
                (
                   SELECT aea.codinep FROM par3.adesaoescolaacessivel aea
                   INNER JOIN par3.prodesaoprograma pdp on pdp.adpid = aea.adpid
                   WHERE pdp.pfaid = {$pfaid}
                ) ";
                                 
        $resEscola = $this->pegaUm($sql);
        
        
        if ($resEscola > 0) {
            // se existir a escola já foi para adesão então não deixa remover
            return $erro = array('erro' => 1);
            
        } else {
            // senão então exclui a escola
            $sql = "DELETE FROM par3.escolas_ea ea
                    WHERE ea.eeaid = {$eeaid}
                    AND ea.pfaid = {$pfaid}
                    AND ea.eeaano_ciclo = '{$_SESSION['exercicio']}' ";

            $this->executar($sql);
            $excluirEscola = $this->commit();
            
            if($excluirEscola){
                return $erro = array('erro' => 0);
            } 
        }
    }
    
    
    public function getDadosEscolasCarga($dados)
    {
        $sql = "SELECT eeaid, eeacodinep, eeanome, eeaapta, eeaselecionada, eeavalordestinado
                FROM {$this->stNomeTabela}
                WHERE eeaid = {$dados['eeaid']}";
        $res = $this->pegaLinha($sql);
        
        $resArray = array(
            'eeaid' => $res['eeaid'],
            'eeacodinep' => $res['eeacodinep'],
            'eeanome' => $res['eeanome'],
            'eeaapta' => $res['eeaapta'],
            'eeaselecionada' => $res['eeaselecionada'],
            'eeavalordestinado' => formata_valor($res['eeavalordestinado'])
        );        
//        ver($resArray);
        echo json_encode(simec_utf8_encode_recursive($resArray));
    }
}
