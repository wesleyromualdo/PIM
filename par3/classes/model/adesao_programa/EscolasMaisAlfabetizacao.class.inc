<?php

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';
require_once APPRAIZ . 'includes/workflow.php';

class Par3_Model_EscolasMaisAlfabetizacao extends Modelo
{
    protected $stNomeTabela = 'par3.escolasmaisalfabetizacao';

    protected $arChavePrimaria = array(
        'emaid'
    );

    protected $arChaveEstrangeira = array(
        'estuf' => array('tabela' => 'territorios.estado', 'pk' => 'estuf'),
        'muncod' => array('tabela' => 'territorios.municipio', 'pk' => 'muncod'),
    );

    protected $arAtributos = array(
        'emacodinep' => null,
        'estuf' => null,
        'muncod' => null,
        'emanome' => null,
        'emaano_censo' => null,
        'emacargahoraria' => null,
        'emaqtdturma' => null,
        'emanaoapta' => null,
        'emarede' => null,
        'emaano_ciclo' => null,
    );

    public function antesSalvar()
    {
        return parent::antesSalvar();
    }

    public function getListaEscolasElegiveis($arrParametros = array())
    {
        $objAdmEscolas = new Par3_Model_Adesaoalfabetizacao();
        $pfaid = 10; // mais alfabetizacao
        $arrEscolas = array();

        $itrid = $arrParametros['itrid'];
        $prgid = $arrParametros['prgid'];
        $inuid = $arrParametros['inuid'];
        $adpid = $arrParametros['adpid'];
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
            ($situacaoAtual['esdid'] == WF_ESDID_TERMONAOACEITO_MAISALFABETIZACAO || $situacaoAtual['esdid'] == WF_ESDID_ENVIADOPARAOMEC_MAISALFABETIZACAO)
            ) {
                $bloqueiaEdicao = true;
            }
        }

        /******************************************************/

        /******************************************************/

        //Filtar filtros

        if ($arrParametros['emamunicipio'] != '') {
            // Caso seja municipal, pode ignorar, já que só aparecera a nível de informação, pois já teremos o muncod
            if ($itrid == 1) {
                $arrWhere[] = "emamunicipio	ilike '%{$arrParametros['emamunicipio']}%' ";
            }
        }
        if ($arrParametros['emarede'] != '') {
            $arrWhere[] = "emarede	= '{$arrParametros['emarede']}' ";
        }
        if ($arrParametros['emacod'] != '') {
            $arrWhere[] = "emacodinep	= '{$arrParametros['emacod']}' ";
        }
        if ($arrParametros['emanome'] != '') {
            $arrWhere[] = "emanome	ilike '%{$arrParametros['emanome']}%' ";
        }
        if ($arrParametros['filtro_extra'] != '') {
            $arrWhere[] = $arrParametros['filtro_extra'];
        }

        $arrCodInepInseridos = $objAdmEscolas->retornaCodEntidadeMarcados($arrParametros);

        if ($arrParametros['checado'] != '') {
            $not = '';
            if ($arrParametros['checado'] == 'N') {
                $not = 'not';
            }
            if (count($arrCodInepInseridos) > 0) {
                $arrWhere[] = "emacodinep::integer {$not} in ( " . implode(', ', $arrCodInepInseridos) . " )";
            }
        }

        if ($arrParametros['ordenacao']) {
            $orderBy = "";
            switch ($arrParametros['ordenacao']) {
                case "municipio":
                    $orderBy = "ORDER BY municipio ASC";
                    break;
                case "emanome":
                    $orderBy = "ORDER BY emanome ASC";
                    break;
                case "emacodinep":
                    $orderBy = "ORDER BY emacodinep ASC";
                    break;
            }
        }

        /******************************************************/


        if ($itrid == 2) {
            $arrWhere[] = " emarede = 'Municipal' ";
            $join = <<<SQL
INNER JOIN par3.instrumentounidade iu ON iu.muncod = ema.muncod
SQL;
        } elseif ($itrid == 1 || $itrid == 3) {
            $arrWhere[] = " emarede = 'Estadual' ";
            $join = <<<SQL
INNER JOIN par3.instrumentounidade iu ON iu.estuf = ema.estuf
SQL;
        }

        $sqlWhere = <<<SQL
		  WHERE inuid = {$inuid} AND emacargahoraria = {$arrParametros['carga_horaria']}
SQL;
        if (is_array($arrWhere)) {
            $sqlWhere .= ' AND ' . implode(' AND ', $arrWhere);
        }

        $colunas = " emacodinep, ema.estuf, inudescricao, emanome, emacargahoraria, emaqtdturma ";
        $sql = <<<SQL
        SELECT {$colunas} FROM {$this->stNomeTabela} ema {$join} {$sqlWhere}  {$orderBy}
SQL;

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
            if ($esdid && $esdid == WF_ESDID_ENVIADOPARAOMEC_MAISALFABETIZACAO) {
                $disabledEdicao = '';
                $disabledJainseridas = 'disabled';
            }

            if (isset($arrParametros['filtro_extra'])) {
                $arrDisabledEdicao = array();
                $arrCodInepInseridos = null;
                $sql = <<<SQL
          SELECT * FROM par3.excecaoescolasprograma exc
          INNER JOIN par3.escolasexcecaoprograma esc ON esc.eepid = exc.eepid
          WHERE prgid = {$prgid} AND inuid = {$inuid};
SQL;

                $exc = $this->carregar($sql);
                $exc = (is_array($exc)) ? $exc : array();

                foreach ($exc as $k => $v) {
                    $arrCodInepInseridos[] = $v['codinep'];
                    $arrDisabledEdicao[$v['codinep']] = true;
                }
                $disabledEdicao = '';
                $disabledJainseridas = '';
            }

            foreach ($resultado as $k => $escola) {
                if (is_array($arrCodInepInseridos) && in_array($escola['emacodinep'], $arrCodInepInseridos)) {
                    $checkedJainseridas = 'checked';
                } else {
                    $checkedJainseridas = '';
                }

                if(is_array($arrDisabledEdicao)) {
                    $disabledEdicao = ($arrDisabledEdicao[$escola['emacodinep']] ? 'disabled' : '');
                }
                $arrEscolas[$k]['acao'] = <<<HTML
    <input {$disabledJainseridas} {$disabledEdicao} {$checkedJainseridas} class="checkbox-primary escola_input"  type=checkbox name="escola_checada[{$escola['emacodinep']}]" 
        id="{$escola['emacodinep']}" value="{$escola['emacodinep']}">
HTML;
                $arrEscolas[$k]['estuf'] = $escola['estuf'];
                $arrEscolas[$k]['inudescricao'] = $escola['inudescricao'];
                $arrEscolas[$k]['emacodinep'] = $escola['emacodinep'];
                $arrEscolas[$k]['emanome'] = $escola['emanome'];
                $arrEscolas[$k]['emanome'] .= "<input type=hidden name=nome_escola[{$escola['emacodinep']}] value=\"{$escola['emanome']}\" /> ";
                $arrEscolas[$k]['emacargahoraria'] .= $escola['emacargahoraria'];
                $arrEscolas[$k]['emaqtdturma'] .= $escola['emaqtdturma'];
            }
        }
        $todos = <<<HTML
marcar/desmarcar <br> <input {$disabledEdicao} {$disabledJainseridas} class="checkbox-primary" type=checkbox name="selecionar_todos" id="selecionar_todos" title="selecionar todos">
HTML;

        $cabecalho = [$todos, 'UF', 'Município', 'Código da Escola', 'Nome da Escola', 'Carga horária do programa', 'QTD de Turmas 1º/2º ano EF'];
        $arrayRetorno['cabecalho'] = $cabecalho;
        $arrayRetorno['arrayEscolas'] = $arrEscolas;
        $arrayRetorno['bloqueio'] = $bloqueiaEdicao;
        $arrayRetorno['status'] = 'success';
        return $arrayRetorno;
    }

    public function getListaEscolasNaoElegiveis($arrParametros = array())
    {
        $objAdmEscolas = new Par3_Model_Adesaoalfabetizacao();
        $arrEscolas = array();

        $itrid = $arrParametros['itrid'];
        $prgid = $arrParametros['prgid'];
        $inuid = $arrParametros['inuid'];
        $adpid = $arrParametros['adpid'];
        $adpano_ciclo = $_SESSION['par3']['adpano_ciclo'];


        /******************************************************/
        if ($adpid) {
            $docid = $this->pegaUm("SELECT docid FROM par3.prodesaoprograma 
                                    WHERE adpid = $adpid AND pfaid = (  SELECT pfaid FROM par3.proadesao 
                                                                        WHERE prgid = {$prgid} AND pfaano = $adpano_ciclo AND pfastatus = 'A')");
        }

        /******************************************************/

        //Filtar filtros

        if ($arrParametros['emamunicipio'] != '') {
            // Caso seja municipal, pode ignorar, já que só aparecera a nível de informação, pois já teremos o muncod
            if ($itrid == 1) {
                $arrWhere[] = "emamunicipio	ilike '%{$arrParametros['emamunicipio']}%' ";
            }
        }
        if ($arrParametros['emarede'] != '') {
            $arrWhere[] = "emarede	= '{$arrParametros['emarede']}' ";
        }
        if ($arrParametros['emacod'] != '') {
            $arrWhere[] = "emacodinep	= '{$arrParametros['emacod']}' ";
        }
        if ($arrParametros['emanome'] != '') {
            $arrWhere[] = "emanome	ilike '%{$arrParametros['emanome']}%' ";
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
                case "emanome":
                    $orderBy = "ORDER BY emanome ASC";
                    break;
                case "emacodinep":
                    $orderBy = "ORDER BY emacodinep ASC";
                    break;
            }
        }

        /******************************************************/

        if ($itrid == 2) {
            $arrWhere[] = " emarede = 'Municipal' ";
            $join = <<<SQL
INNER JOIN par3.instrumentounidade iu ON iu.muncod = ema.muncod
SQL;
        } elseif ($itrid == 1 || $itrid == 3) {
            $arrWhere[] = " emarede = 'Estadual' ";
            $join = <<<SQL
INNER JOIN par3.instrumentounidade iu ON iu.estuf = ema.estuf
SQL;
        }

        $sqlWhere = <<<SQL
		  WHERE inuid = {$inuid} AND emanaoapta = TRUE 
SQL;
        if (is_array($arrWhere)) {
            $sqlWhere .= ' AND ' . implode(' AND ', $arrWhere);
        }

        $colunas = " emacodinep, ema.estuf, inudescricao, emanome, emacargahoraria, emaqtdturma, CASE WHEN emanaoapta = TRUE THEN 'SIM' ELSE 'NÃO' END AS emanaoapta  ";
        $sql = <<<SQL
        SELECT {$colunas} FROM {$this->stNomeTabela} ema {$join} {$sqlWhere} {$orderBy}
SQL;

        $resultado = $this->carregar($sql);
        $resultado = (is_array($resultado)) ? $resultado : array();

        /******************************************************/

        if (count($resultado) > 0) {

            foreach ($resultado as $k => $escola) {
                $arrEscolas[$k]['estuf'] = $escola['estuf'];
                $arrEscolas[$k]['inudescricao'] = $escola['inudescricao'];
                $arrEscolas[$k]['emacodinep'] = $escola['emacodinep'];
                $arrEscolas[$k]['emanome'] = $escola['emanome'];
                $arrEscolas[$k]['emanome'] .= "<input type=hidden name=nome_escola[{$escola['emacodinep']}] value=\"{$escola['emanome']}\" /> ";
                if ($arrParametros['aeacargahoraria'] != 'N') {
                    $arrEscolas[$k]['emacargahoraria'] .= $escola['emacargahoraria'];
                    $arrEscolas[$k]['emaqtdturma'] .= $escola['emaqtdturma'];
                }
            }
        }

        $cabecalho = ['UF', 'Município', 'Código da Escola', 'Nome da Escola'];
        if ($arrParametros['aeacargahoraria'] != 'N') {
            $cabecalho[] = 'QTD Turmas 1º/2º ano EF';
            $cabecalho[] = 'Carga horária do programa';
        }
        $arrayRetorno['cabecalho'] = $cabecalho;
        $arrayRetorno['arrayEscolas'] = $arrEscolas;
        $arrayRetorno['status'] = 'success';
        return $arrayRetorno;
    }

    public function getListaEscolasElegiveisExcel($arrParametros = array())
    {

    }

    public function getListaEscolasEscolhidas($arrParametros = array())
    {
        $inuid = $arrParametros['inuid'];
        $adpano_ciclo = $arrParametros['adpano_ciclo'];
        $adpid = $arrParametros['adpid'];
        $where = '';
        if (isset($arrParametros['carga_horaria']) && !empty($arrParametros['carga_horaria'])) {
            $where = <<<SQL
    emacargahoraria = {$arrParametros['carga_horaria']}
SQL;
        }
        $sqlDados = <<<SQL

SELECT * FROM (
    SELECT DISTINCT
      ema.estuf,
      ema.muncod,
      aea.codinep :: INTEGER AS codinep,
      ema.emanome,
      ema.emacargahoraria,
      ema.emaqtdturma,
    
      CASE WHEN aeminseridaaposenvio = TRUE
        THEN
          'Sim'
      ELSE
        ''
      END                    AS enviado_apos_envio,
      adp.inuid,
      adp.adpid
    FROM
      par3.escolasmaisalfabetizacao ema
      INNER JOIN par3.adesaoescolamaisalfabetizacao aea ON aea.codinep = ema.emacodinep
      INNER JOIN par3.prodesaoprograma adp ON adp.adpid = aea.adpid AND adpano_ciclo = ema.emaano_ciclo
      WHERE
      aea.codinep::VARCHAR NOT IN ( SELECT codinep FROM par3.escolasexcecaoprograma exp
                            INNER JOIN par3.excecaoescolasprograma eep ON eep.eepid = exp.eepid
                            INNER JOIN par3.prodesaoprograma adp ON adp.adpid = eep.adpid AND adpano_ciclo = {$adpano_ciclo}
                            WHERE exp.expstatus = 'A' AND eep.eepstatus = 'A' AND eep.eepdecisao = 'S')
    
    UNION ALL
    
    SELECT DISTINCT
      eam.estuf,
      eam.muncod,
      exp.codinep :: INTEGER AS codinep,
      eam.emanome,
      eam.emacargahoraria,
      eam.emaqtdturma,
      'Sim'                  AS enviado_apos_envio,
      adp.inuid,
      adp.adpid
    FROM par3.prodesaoprograma adp
      INNER JOIN workflow.documento doc ON doc.docid = adp.docid
      INNER JOIN par3.excecaoescolasprograma eep ON eep.adpid = adp.adpid AND eep.eepstatus = 'A' AND eep.eepdecisao = 'S'
      INNER JOIN par3.escolasexcecaoprograma exp ON exp.eepid = eep.eepid AND expstatus = 'A'
      INNER JOIN par3.escolasmaisalfabetizacao eam ON exp.codinep :: INTEGER = eam.emacodinep :: INTEGER AND eam.emaano_ciclo = adp.adpano_ciclo
  ) AS foo
  WHERE
        inuid = {$inuid} AND
        adpid = {$adpid} AND
        {$where}
        

SQL;

//        ver($sqlDados);
        $cabecalho = array('UF', 'Município', 'Código da Escola', 'Escola', 'Carga Horária do programa', 'QTD Turmas 1º/2º ano EF', 'Inserido após o Envio ao MEC');
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $listagem->esconderColunas(array('inuid', 'adpid'));
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sqlDados);
        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
        return;
    }

    public function existeEscolaNoGrupo($arrParam = array())
    {
        $inuid = $arrParam['inuid'];
        $carga_horaria = $arrParam['carga_horaria'];
        $adpano_ciclo = $arrParam['adpano_ciclo'];

        $sql = <<<SQL
SELECT itrid, estuf, muncod
  FROM par3.instrumentounidade 
  WHERE inuid = {$inuid}
SQL;
        $entidade = $this->pegaLinha($sql);
        $itrid = $entidade['itrid'];

        if (($itrid == 1) || ($itrid == 3)) {
            $rede = 'Estadual';
            $clauseLocal = "estuf = '{$entidade['estuf']}'";
        } else {
            $rede = 'Municipal';
            $clauseLocal = "muncod = '{$entidade['muncod']}'";
        }

        $sql = <<<SQL
SELECT count(emaid) FROM {$this->stNomeTabela}
WHERE
    emacargahoraria = {$carga_horaria} AND
    emarede = '{$rede}' AND
    emaano_ciclo = $adpano_ciclo AND
    {$clauseLocal}
SQL;
        $total = $this->pegaUm($sql);
        return ($total > 0) ? true : false;
    }

    public function getEscolasContexto($arrParametros = array())
    {
        // CArrega variáveis
        $itrid = $arrParametros['itrid'];

        // Estadual
        if ($itrid == 1 || $itrid == 3) {
            $arrWhere[] = "estuf	= '{$arrParametros['estuf']}' ";
            $arrWhere[] = "emarede 	= 'Estadual' ";
            $campomunicipio = 'emamunicipio,';
        } else {
            $arrWhere[] = "muncod	= '{$arrParametros['muncod']}' ";
            $campomunicipio = '';
            $arrWhere[] = "emarede 	= 'Municipal' ";
        }

        //Filtar filtros
        if ($arrParametros['emamunicipio'] != '') {
            $arrWhere[] = "emamunicipio	ilike '%{$arrParametros['emamunicipio']}%' ";
        }
        if ($arrParametros['emarede'] != '') {
            $arrWhere[] = "emarede	= '{$arrParametros['emarede']}' ";
        }
        if ($arrParametros['emacodinep'] != '') {
            $arrWhere[] = "emacodinep	= '{$arrParametros['emacodinep']}' ";
        }
        if ($arrParametros['emanome'] != '') {
            $arrWhere[] = "emanome	ilike '%{$arrParametros['emanome']}%' ";
        }

        $objAdmEscolas = new Par3_Model_Adesaoalfabetizacao();
        $arrCodInepInseridos = $objAdmEscolas->retornaCodEntidadeMarcados($arrParametros);

        $arrCodInepInseridos = (is_array($arrCodInepInseridos)) ? $arrCodInepInseridos : Array();

        if ($arrParametros['checado'] != '') {
            $not = '';
            if ($arrParametros['checado'] == 'N') {
                $not = 'not';
            }
            if (count($arrCodInepInseridos) > 0) {
                $arrWhere[] = "emacodinep::integer {$not} in ( " . implode(', ', $arrCodInepInseridos) . " )";
            }
        }

        $colunas = " DISTINCT emacodinep ";

        if (is_array($arrWhere)) {
            $WHERE = (is_array($arrWhere) ? '(' . implode(' AND ', $arrWhere) . ')' : '');
        }
        $sql = " SELECT {$colunas} FROM {$this->stNomeTabela} WHERE {$WHERE} ";

        $resultado = $this->carregar($sql);
        $resultado = (is_array($resultado)) ? $resultado : array();
        foreach ($resultado as $k => $v) {
            $arrRetorno[] = $v['emacodinep'];
        }
        return $arrRetorno;
    }
}
