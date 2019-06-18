<?php

/**
 * Class Par3_Controller_Pendencia
 */
class Par3_Controller_Pendencia extends Controle
{
    /** @var Par3_Model_Pendencias */
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_Pendencias();
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgrup.com.br>
     * @param $inuid
     * @return array|mixed|NULL
     * @description Este method retorna um array com as pendências da unidade.
     */
    public function getPendenciasObrasInstrumentoUnidadeById($inuid)
    {
        if (!empty($inuid)) {
            return $this->model->getPendenciasObrasIntrumentoUnidadeById($inuid);
        }
    }

    public function pegaPendenciaInstrumentoUnidade($inuid)
    {
        $sql = sprintf("
            SELECT
              count(pepid)
            FROM
              par3.pendecia_planejamento
            WHERE
              inuid = %d
              AND peddatalimite >= CAST(now() AS DATE)
              AND pepstatus = 'A'
        ", $inuid);

        $retorno = 'N';

        $total = $this->model->pegaUm($sql);

        if ((int)$total > 0) {
            $retorno = 'S';
        }

        return $retorno;
    }
    
    public function pegaPendenciaInstrumentoJuridico($inuid)
    {
        $sql = sprintf("
            SELECT
              count(dejid)
            FROM
              par3.desbloqueio_juridico
            WHERE
              inuid = %d
              AND dejdatalimite >= CAST(now() AS DATE)
              AND dejstatus = 'A'
        ", $inuid);

        $retorno = 'N';

        $total = $this->model->pegaUm($sql);

        if ((int)$total > 0) {
            $retorno = 'S';
        }

        return $retorno;
    }

    public function pegaValidaPendenciaPorInicitiva($iniid, $inuid = '')
    {
        $where = '';
        if (!empty($inuid)) {
            $where = " AND ip.inuid = $inuid ";
        }
        
        $sql = sprintf('
            SELECT
              (
                CASE
                  WHEN ip.total > 0
                    THEN \'N\'
                  ELSE coalesce(inivalidapendencia, \'S\')
                END
              ) AS inivalidapendencia
            FROM
              par3.iniciativa i
              LEFT JOIN (
                SELECT
                  count(ip.inpid) AS total,
                  ip.iniid
                FROM
                  par3.iniciativa_planejamento ip
                  INNER JOIN par3.iniciativa_emenda ie ON ie.inpid = ip.inpid
                WHERE
                  ip.inpstatus = \'A\'
                  AND ie.inestatus = \'A\'
                    '.$where.'
                GROUP BY
                  ip.iniid
              ) ip ON ip.iniid = i.iniid
            WHERE
              i.iniid = %d
        ', $iniid);

        return $this->model->pegaUm($sql);
    }

    public function pegaValidaPendenciaPorInicitivaProcesso($proid)
    {
        $sql = sprintf('
            SELECT
              count(ini.iniid)
            FROM
              par3.processo p
              INNER JOIN par3.processoparcomposicao pp ON pp.proid = p.proid AND pp.ppcstatus = \'A\'
              INNER JOIN par3.iniciativa_planejamento ip ON ip.inpid = pp.inpid
              INNER JOIN par3.iniciativa ini ON ini.iniid = ip.iniid
            WHERE
              ip.inpstatus = \'A\'
              AND p.proid = %d
              AND ini.inivalidapendencia = \'S\'
        ', $proid);

        $retorno = 'N';

        $total = $this->model->pegaUm($sql);

        if ((int)$total > 0) {
            $retorno = 'S';
        }

        return $retorno;
    }
    
    public function verificaPendenciaBloqueio($pagina, $inuid = '', $iniid = '', $proid = '', $retornoArray = false, $rgIniciativa = true, $rgEntidade = true, $rgJudicial = true, $inpid = '')
    {
        $sql = "SELECT b.rebid, p.tipdsc, p.tipid, b.rebplanejamento, b.rebempenho, b.rebtermo, b.rebpagamento FROM par3.regra_bloqueio b
					INNER JOIN par3.tipos_pendencias p ON p.tipid = b.tipid
				WHERE p.tipstatus = 'A'
					AND (b.rebplanejamento = TRUE OR b.rebempenho = TRUE OR b.rebtermo = TRUE OR b.rebpagamento = TRUE)
				ORDER BY b.rebid";
        $arrBloqueio = $this->model->carregar($sql);
        $arrBloqueio = $arrBloqueio ? $arrBloqueio : array();
        
        #Verifica se exite pendencia na view par3.vm_relatorio_quantitativo_pendencias
        $arrBloqueio = $this->pegaPendenciaView($inuid, $arrBloqueio);
        
        #Verifica se existe desbloqueio por inciativa, entidade ou judicial
        $arrDesbloqueio = $this->regraDesbloqueio($inuid, $iniid, $proid, $rgIniciativa, $rgEntidade, $rgJudicial);
        //ver($arrBloqueio, $arrDesbloqueio);
        /*
         *  tdeid = iniciativa, entidade, judicial
            tipid = CAE, CACS, Habilitacao....
         * */        
        foreach ($arrBloqueio as $key => $v) {
            $tipid = $v['tipid'];
            
            #Caso tenha algun desbloqueio, altera de true para false, isto é, retirando o bloqueio
            if ($v['rebplanejamento'] == 't') {
                if (is_array($arrDesbloqueio[$tipid])) {
                    foreach ($arrDesbloqueio[$tipid] as $des) {
                        if ($des['planejamento'] == 't') {
                            $arrBloqueio[$key]['rebplanejamento'] = 'f';
                            $arrBloqueio[$key]['desbloqueio'] = $arrDesbloqueio[$tipid];
                        }
                    }
                }
            }
            if ($v['rebempenho'] == 't') {
                if (is_array($arrDesbloqueio[$tipid])) {
                    foreach ($arrDesbloqueio[$tipid] as $des) {
                        if ($des['empenho'] == 't') {
                            $arrBloqueio[$key]['rebempenho'] = 'f';
                            $arrBloqueio[$key]['desbloqueio'] = $arrDesbloqueio[$tipid];
                        }
                    }
                }
            }
            if ($v['rebtermo'] == 't') {
                if (is_array($arrDesbloqueio[$tipid])) {
                    foreach ($arrDesbloqueio[$tipid] as $des) {
                        if ($des['termo'] == 't') {
                            $arrBloqueio[$key]['rebtermo'] = 'f';
                            $arrBloqueio[$key]['desbloqueio'] = $arrDesbloqueio[$tipid];
                        }
                    }
                }
            }
            if ($v['rebpagamento'] == 't') {
                if (is_array($arrDesbloqueio[$tipid])) {
                    foreach ($arrDesbloqueio[$tipid] as $des) {
                        if ($des['pagamento'] == 't') {
                            $arrBloqueio[$key]['rebpagamento'] = 'f';
                            $arrBloqueio[$key]['desbloqueio'] = $arrDesbloqueio[$tipid];
                        }
                    }
                }
            }
        }

        if( empty($inpid) && !empty($proid) ){
            $arInpid = $this->model->carregarColuna("SELECT inpid FROM par3.processoparcomposicao WHERE proid = $proid AND ppcstatus = 'A'
                                                    UNION ALL
                                                    SELECT o.inpid FROM par3.processoobracomposicao p
                                                    	INNER JOIN par3.obra o ON o.obrid = p.obrid AND o.obrstatus = 'A'
                                                    WHERE p.proid = $proid AND p.pocstatus = 'A'");
            $inpid = implode(', ', $arInpid);
        }
        
        $boEmenda = 0;
        if( $inpid ){
            $sql = "SELECT count(ie.ineid) FROM par3.iniciativa_emenda ie
                    	INNER JOIN workflow.documento d ON d.docid = ie.docid
                    WHERE ie.inestatus = 'A'
                    	AND inpid in ($inpid)
                    	AND d.esdid = ".PAR3_ESD_EMENDA_ACEITA;
            $boEmenda = $this->model->pegaUm($sql);
            
            $sql = "SELECT count(ie.ineid) FROM par3.iniciativa_emenda ie
                    	INNER JOIN workflow.documento d ON d.docid = ie.docid
                    WHERE ie.inestatus = 'A'
                    	AND inpid in ($inpid)";
            $boEmendaPlanej = $this->model->pegaUm($sql);
        }
        
        if (is_array($arrBloqueio)) {
            $temPendencia = false;
            $arrRetorno = array();
            foreach ($arrBloqueio as $v) {
                if ($pagina == 'planejamento') {
                    if ($v['rebplanejamento'] == 't') {
                        if( (int)$boEmendaPlanej > (int)0 ){
                            $temPendencia = false;
                        } else {
                            $temPendencia = true;
                        }
                        array_push($arrRetorno, array('tempendencia' => 'S', 'pendencia' => $v['tipdsc'], 'emenda' => (((int)$boEmendaPlanej > (int)0)?'S':'N')) );
                    }
                }
                if ($pagina == 'empenho') {
                    if ($v['rebempenho'] == 't') {
                        if( (int)$boEmenda > (int)0 ){
                            $temPendencia = false;
                        } else {
                            $temPendencia = true;
                        }
                        array_push($arrRetorno, array('tempendencia' => 'S', 'pendencia' => $v['tipdsc'], 'emenda' => (((int)$boEmenda > (int)0)?'S':'N')) );
                    }
                }
                if ($pagina == 'termo') {
                    if ($v['rebtermo'] == 't') {
                        if( (int)$boEmenda > (int)0 ){
                            $temPendencia = false;
                        } else {
                            $temPendencia = true;
                        }
                        array_push($arrRetorno, array('tempendencia' => 'S', 'pendencia' => $v['tipdsc'], 'emenda' => (((int)$boEmendaPlanej > (int)0)?'S':'N')) );
                    }
                }
                if ($pagina == 'pagamento') {
                    if ($v['rebpagamento'] == 't') {
                        $temPendencia = true;
                        array_push($arrRetorno, array('tempendencia' => 'S', 'pendencia' => $v['tipdsc'], 'emenda' => (((int)$boEmendaPlanej > (int)0)?'S':'N')) );
                    }
                }
            }
        }
        if ($retornoArray) {
            return $arrRetorno;
        } else {
            return $temPendencia;
        }
    }
    
    public function pegaPendenciaView($inuid, $arrBloqueio = array())
    {
        $sql = "SELECT inuid, habilitacao, cae, contas, monitoramento_par, obras_par, cacs, pne, siope
                FROM par3.vm_relatorio_quantitativo_pendencias WHERE inuid = $inuid";
        $arViewPendencia = $this->model->pegaLinha($sql);
        $arViewPendencia = $arViewPendencia ? $arViewPendencia : array();
        
        $arrRetorno = array();
        foreach ($arrBloqueio as $key => $v) {
            $tipid = $v['tipid'];
            switch ($tipid) {
                case 1:
                    $cae = $arViewPendencia['cae'];
                    $v['rebplanejamento']   = ($v['rebplanejamento']    == 't' ? $cae : $v['rebplanejamento']);
                    $v['rebempenho']        = ($v['rebempenho']         == 't' ? $cae : $v['rebempenho']);
                    $v['rebtermo']          = ($v['rebtermo']           == 't' ? $cae : $v['rebtermo']);
                    $v['rebpagamento']      = ($v['rebpagamento']       == 't' ? $cae : $v['rebpagamento']);
                    array_push($arrRetorno, $v);
                    break;
                case 2:
                    $cacs = $arViewPendencia['cacs'];
                    $v['rebplanejamento']   = ($v['rebplanejamento']    == 't' ? $cacs : $v['rebplanejamento']);
                    $v['rebempenho']        = ($v['rebempenho']         == 't' ? $cacs : $v['rebempenho']);
                    $v['rebtermo']          = ($v['rebtermo']           == 't' ? $cacs : $v['rebtermo']);
                    $v['rebpagamento']      = ($v['rebpagamento']       == 't' ? $cacs : $v['rebpagamento']);
                    array_push($arrRetorno, $v);
                    break;
                case 3:
                    $habilitacao = $arViewPendencia['habilitacao'];
                    $v['rebplanejamento']   = ($v['rebplanejamento']    == 't' ? $habilitacao : $v['rebplanejamento']);
                    $v['rebempenho']        = ($v['rebempenho']         == 't' ? $habilitacao : $v['rebempenho']);
                    $v['rebtermo']          = ($v['rebtermo']           == 't' ? $habilitacao : $v['rebtermo']);
                    $v['rebpagamento']      = ($v['rebpagamento']       == 't' ? $habilitacao : $v['rebpagamento']);
                    array_push($arrRetorno, $v);
                    break;
                case 4:
                    $monitoramento_par = $arViewPendencia['monitoramento_par'];
                    $v['rebplanejamento']   = ($v['rebplanejamento']    == 't' ? $monitoramento_par : $v['rebplanejamento']);
                    $v['rebempenho']        = ($v['rebempenho']         == 't' ? $monitoramento_par : $v['rebempenho']);
                    $v['rebtermo']          = ($v['rebtermo']           == 't' ? $monitoramento_par : $v['rebtermo']);
                    $v['rebpagamento']      = ($v['rebpagamento']       == 't' ? $monitoramento_par : $v['rebpagamento']);
                    array_push($arrRetorno, $v);
                    break;
                case 5:
                    $obras_par = $arViewPendencia['obras_par'];
                    $v['rebplanejamento']   = ($v['rebplanejamento']    == 't' ? $obras_par : $v['rebplanejamento']);
                    $v['rebempenho']        = ($v['rebempenho']         == 't' ? $obras_par : $v['rebempenho']);
                    $v['rebtermo']          = ($v['rebtermo']           == 't' ? $obras_par : $v['rebtermo']);
                    $v['rebpagamento']      = ($v['rebpagamento']       == 't' ? $obras_par : $v['rebpagamento']);
                    array_push($arrRetorno, $v);
                    break;
                case 6:
                    $pne = $arViewPendencia['pne'];
                    $v['rebplanejamento']   = ($v['rebplanejamento']    == 't' ? $pne : $v['rebplanejamento']);
                    $v['rebempenho']        = ($v['rebempenho']         == 't' ? $pne : $v['rebempenho']);
                    $v['rebtermo']          = ($v['rebtermo']           == 't' ? $pne : $v['rebtermo']);
                    $v['rebpagamento']      = ($v['rebpagamento']       == 't' ? $pne : $v['rebpagamento']);
                    array_push($arrRetorno, $v);
                    break;
                case 7:
                    $contas = $arViewPendencia['contas'];
                    $v['rebplanejamento']   = ($v['rebplanejamento']    == 't' ? $contas : $v['rebplanejamento']);
                    $v['rebempenho']        = ($v['rebempenho']         == 't' ? $contas : $v['rebempenho']);
                    $v['rebtermo']          = ($v['rebtermo']           == 't' ? $contas : $v['rebtermo']);
                    $v['rebpagamento']      = ($v['rebpagamento']       == 't' ? $contas : $v['rebpagamento']);
                    array_push($arrRetorno, $v);
                    break;
                case 8:
                    $siope = $arViewPendencia['siope'];
                    $v['rebplanejamento']   = ($v['rebplanejamento']    == 't' ? $siope : $v['rebplanejamento']);
                    $v['rebempenho']        = ($v['rebempenho']         == 't' ? $siope : $v['rebempenho']);
                    $v['rebtermo']          = ($v['rebtermo']           == 't' ? $siope : $v['rebtermo']);
                    $v['rebpagamento']      = ($v['rebpagamento']       == 't' ? $siope : $v['rebpagamento']);
                    array_push($arrRetorno, $v);
                    break;
            }
        }
        return $arrRetorno;
    }
    
    public function regraDesbloqueio($inuid = '', $iniid = '', $proid = '', $rgIniciativa = true, $rgEntidade = true, $rgJudicial = true)
    {
        $sql = "SELECT b.repid, p.tipid, t.tdeid, t.tdedsc, p.tipdsc, b.repplanejamento, b.repempenho, b.reptermo, b.reppagamento
                FROM par3.regra_pendecia b
                	INNER JOIN par3.tipos_pendencias p ON p.tipid = b.tipid
                	INNER JOIN par3.tipos_desbloqueio t ON t.tdeid = b.tdeid
                WHERE p.tipstatus = 'A'
                	AND (b.repplanejamento = TRUE OR b.repempenho = TRUE OR b.reptermo = TRUE OR b.reppagamento = TRUE)
                ORDER BY p.tipid, t.tdeid";
        $arrDesBloqueio = $this->model->carregar($sql);
        $arrDesBloqueio = $arrDesBloqueio ? $arrDesBloqueio : array();
        
        $boHabilitaPorIniciativa = 'N';
        $boHabilitaPorEntidade = 'N';
        $boHabilitaPorJuridico = 'N';
        if ($rgEntidade) {
            $boHabilitaPorEntidade = $this->pegaPendenciaInstrumentoUnidade($inuid);
        }
        if ($rgJudicial) {
            $boHabilitaPorJuridico = $this->pegaPendenciaInstrumentoJuridico($inuid);
        }
        if ($rgIniciativa) {
            if (!empty($proid)) {
                $boHabilitaPorIniciativa = $this->pegaValidaPendenciaPorInicitivaProcesso($proid);
            } else {
                $boHabilitaPorIniciativa = $this->pegaValidaPendenciaPorInicitiva($iniid, $inuid);
            }
        }
        
        #iniciativa, entidade ou juridico igual a "N", quer dizer que não tem desbloqueio
        
        $arrRegistro = array();
        foreach ($arrDesBloqueio as $v) {
            if ($v['repplanejamento'] == 't') {
                switch ($v['tdeid']) {
                    case 1: #Iniciativa
                        if ($boHabilitaPorIniciativa == 'N') {
                            $v['repplanejamento'] = 't';
                        } else {
                            $v['reptermo'] = 'f';
                        }
                        break;
                    case 2: #Entidade
                        if ($boHabilitaPorEntidade == 'N') {
                            $v['repplanejamento'] = 'f';
                        }
                        break;
                    case 3: #Juridico
                        if ($boHabilitaPorJuridico == 'N') {
                            $v['repplanejamento'] = 'f';
                        }
                        break;
                }
            }
            if ($v['repempenho'] == 't') {
                switch ($v['tdeid']) {
                    case 1: #Iniciativa
                        if ($boHabilitaPorIniciativa == 'N') {
                            $v['repempenho'] = 't';
                        } else {
                            $v['reptermo'] = 'f';
                        }
                        break;
                    case 2: #Entidade
                        if ($boHabilitaPorEntidade == 'N') {
                            $v['repempenho'] = 'f';
                        }
                        break;
                    case 3:
                        if ($boHabilitaPorJuridico == 'N') {
                            $v['repempenho'] = 'f';
                        }
                        break;
                }
            }
            if ($v['reptermo'] == 't') {
                switch ($v['tdeid']) {
                    case 1: #Iniciativa
                        if ($boHabilitaPorIniciativa == 'N') {
                            $v['reptermo'] = 't';
                        } else {
                            $v['reptermo'] = 'f';
                        }
                        break;
                    case 2: #Entidade
                        if ($boHabilitaPorEntidade == 'N') {
                            $v['reptermo'] = 'f';
                        }
                        break;
                    case 3:
                        if ($boHabilitaPorJuridico == 'N') {
                            $v['reptermo'] = 'f';
                        }
                        break;
                }
            }
            
            if ($v['reppagamento'] == 't') {
                switch ($v['tdeid']) {
                    case 1: #Iniciativa
                        if ($boHabilitaPorIniciativa == 'N') {
                            $v['reppagamento'] = 't';
                        } else {
                            $v['reptermo'] = 'f';
                        }
                        break;
                    case 2: #Entidade
                        if ($boHabilitaPorEntidade == 'N') {
                            $v['reppagamento'] = 'f';
                        }
                        break;
                    case 3:
                        if ($boHabilitaPorJuridico == 'N') {
                            $v['reppagamento'] = 'f';
                        }
                        break;
                }
            }
            /*
             *  tdeid = iniciativa, entidade, judicial
                tipid = CAE, CACS, Habilitacao....
             * */
            $arrRegistro[$v['tipid']][$v['tdeid']] = array('tdedsc' => $v['tdedsc'],
                                                            'tipdsc' => $v['tipdsc'],
                                                            'planejamento' => $v['repplanejamento'],
                                                            'empenho' => $v['repempenho'],
                                                            'termo' => $v['reptermo'],
                                                            'pagamento' => $v['reppagamento'],
            );
        }
        
        return $arrRegistro;
    }
}
