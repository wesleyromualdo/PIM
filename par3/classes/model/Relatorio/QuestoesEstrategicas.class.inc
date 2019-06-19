<?php

include_once APPRAIZ . "includes/library/simec/view/html_table.class.php";

/**
 * Par_Model_Questionario
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 21-09-2015
 * @link     no link
 */
class Par3_Model_Relatorio_QuestoesEstrategicas extends Modelo {

    private $perguntas = array();
    private $respostas = array();
    private $tabela;
    protected $arAtributos = array(
        'estuf' => null,
        'muncod' => null,
        'queid' => null,
    );

    /**
     * Função pegarSQLLista
     * - monta SQL para lista de intstrumentos unidade.
     *
     * @return retorna a query.
     *
     */
    public function pegarSQLLista() {
        $where = self::prepararFiltro();

        $sql = 'SELECT
    	           inuid,
    	           inudescricao
    	       FROM
    	           par3.instrumentounidade
    	       WHERE
    	           ' . implode(' AND ', $where) . '
    	       ORDER BY
    	           2';

        return $sql;
    }

//end pegarSQLLista()

    public function pegaQuestionario($inuid, $queid) {
        $sql = "SELECT qp.qrpid
	    		  FROM par.parquestionario qp
	    	INNER JOIN questionario.questionarioresposta qr ON qr.qrpid = qp.qrpid
	    		 WHERE qp.inuid = {$inuid}
	    		   AND qr.queid = {$queid}";

        return $this->pegaUm($sql);
    }

//end pegaQuestionario()

    public function buildDataGrid($xlsDownload = FALSE) {
        $tipoRelatorio = ($xlsDownload) ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;

        $tratamentoListaVazia = ($xlsDownload) ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $this->_listagem = new Simec_Listagem($tipoRelatorio);
        $this->_listagem->setCabecalho($this->getHeader());
        $this->_listagem->setQuery($this->buildQuery());
        $this->_listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $this->_listagem->turnOnOrdenacao('estuf');
        $this->_listagem->esconderColunas(['tenid']);
        if (!$xlsDownload && count($this->getFilters())) {
            $this->addInputFilters();
            $this->addCallbackDeCampo();
        }
        $this->_listagem->render($tratamentoListaVazia);
    }

    private function addCallbackDeCampo() {
        if (array_search('vTable.enttelefonecomercial', $this->getFilters())) {
            $this->_listagem->addCallbackDeCampo('enttelefonecomercial', 'formatar_telefone');
        }

        if (array_search('vTable.enttelefonecelular', $this->getFilters())) {
            $this->_listagem->addCallbackDeCampo('enttelefonecelular', 'formatar_telefone');
        }
    }

    public function getQueryQuestionario($queid) {
        $strSQL = <<<DML
SELECT
--NIVEL 01
  p.perid AS p_perid,
  p.queid AS p_queid,
  p.pertitulo AS p_pertitulo,

  --NIVEL 02
  ip.itpid AS ip_itpid,
  ip.itptitulo,

  --NIVEL 03
  p1.perid AS p1_perid,
  p1.pertitulo AS p1_pertitulo,

  --NIVEL 04
  ip2.itpid AS ip2_itpid,
  ip2.itptitulo AS ip2_itptitulo

FROM questionario.pergunta p
  LEFT JOIN questionario.itempergunta ip ON ip.perid = p.perid
  LEFT JOIN questionario.pergunta AS p1 ON p1.itpid = ip.itpid
  LEFT JOIN questionario.itempergunta ip2 ON ip2.perid = p1.perid


WHERE 1=1  AND p.queid = {$queid}
ORDER BY p.perordem, p1.perordem
DML;
        return $strSQL;
    }

    public function getQueryResposta($questoes, $cod, $tipo)
    {
        $where = $this->getWhere($questoes);

        $strSQLJoin = '';
        if ($tipo == 'item') {
            $strSQLJoin .= "INNER JOIN questionario.resposta r ON r.qrpid = qr.qrpid AND  r.itpid = {$cod}";
        }

        if ($tipo == 'pergunta') {
            $strSQLJoin .= "INNER JOIN questionario.resposta r ON r.qrpid = qr.qrpid AND  r.perid = {$cod}";
        }

        $strSQL = " SELECT
                        i.estuf,
                        i.muncod,
                        r.resid,
                        r.perid  AS r_perid,
                        r.qrpid AS r_qrpid,
                        r.usucpf,
                        r.resdsc,
                        r.itpid AS r_itpid,
                        p.queid,
                        (
                        SELECT array_agg(itptitulo)
                        FROM questionario.pergunta p2
                        LEFT JOIN questionario.itempergunta AS ip3 ON ip3.perid = p2.perid
                        WHERE
                            p2.pertipo = 'CK'
                            AND p2.itpid = r.itpid
                        ) AS chk,
                        (SELECT count(co_entidade) FROM par3.questoesestrategicas_escolas_ce     WHERE qrpid = qun.qrpid AND perid = r.perid) AS escolas_ce,
                        (SELECT count(co_entidade) FROM par3.questoesestrategicas_escolas_ept    WHERE qrpid = qun.qrpid AND perid = r.perid) AS escolas_ept,
                        (SELECT count(co_entidade) FROM par3.questoesestrategicas_escolas_estept WHERE qrpid = qun.qrpid AND perid = r.perid) AS escolas_estept,
                        (SELECT count(co_entidade) FROM par3.questoesestrategicas_escolas_gremio WHERE qrpid = qun.qrpid AND perid = r.perid) AS escolas_gremio,
                        (SELECT count(co_entidade) FROM par3.questoesestrategicas_escolas_proeja WHERE qrpid = qun.qrpid AND perid = r.perid) AS escolas_proeja
                    FROM questionario.questionarioresposta qr
                    INNER JOIN par3.questionario_unidade    qun ON qun.qrpid = qr.qrpid AND qun.queid = {$questoes['queid']}
                    INNER JOIN par3.instrumentounidade      i   ON i.inuid = qun.inuid
                    {$strSQLJoin}
                    INNER JOIN questionario.pergunta        p   ON p.perid = r.perid
                    {$where}";

        return $strSQL;
    }

    public function getQuery($where, $queid)
    {
        $strSQL = " SELECT
                      i.estuf,
                      i.muncod,
                      mun.mundescricao,
                      qr.qrpid,
                      qr.queid,
                      qr.qrptitulo,
                      qr.qrpdata
                    FROM questionario.questionarioresposta qr
                    INNER JOIN par3.questionario_unidade    qun ON qun.qrpid = qr.qrpid AND qun.queid = $queid
                    INNER JOIN par3.instrumentounidade      i   ON i.inuid = qun.inuid
                    INNER JOIN territorios.municipio        mun ON mun.muncod = i.muncod
                    {$where}";

        return $strSQL;
    }

    public function carregarQuestionario($filtro) {
        $queid = (int) $filtro['queid'];
        $sql = $this->getQueryQuestionario($queid);

        $dadosQuestionario = $this->carregar($sql);
        $questionario = ($dadosQuestionario ? $dadosQuestionario : array());
        $this->organizaDados($questionario);

        return $this->generateTableHtml($filtro);
    }

    private function organizaDados($dados) {
        foreach ($dados as $value) {

            $this->perguntas[$value['p_perid']]['nivel1']['titulo'] = $value['p_pertitulo'];
            $this->perguntas[$value['p_perid']]['nivel1']['perid'] = $value['p_perid'];

            if (!empty($value['itptitulo'])) {
                $this->getTituloQuestões($value['p_perid'], 2, $value['p_perid'], $value['ip_itpid'], $value['itptitulo']);
                $this->getTituloQuestões($value['p_perid'], 3, $value['ip_itpid'], $value['p1_perid'], $value['p1_pertitulo']);
                $this->getTituloQuestões($value['p_perid'], 4, $value['p1_perid'], $value['ip2_itpid'], $value['ip2_itptitulo']);
            }
        }
    }

    private function generateTableHtml($filtro) {
        if (is_array($this->perguntas)) {
            $this->tabela = new HTML_Table('relatorio_xls_questionario_estrategico', 'table table-condensed table-bordered  table-responsive conteudo', array('border' => 1));
            $this->setHeader();
            $this->setBody($filtro);
        }
        return $this->tabela->display();
    }

    private function addNivel1($keN1, $nivel) {
        if ($keN1 == 'nivel1') {
            $this->respostas[]['pergunta'] = $nivel['perid'];
            $this->tabela->addCell($nivel['titulo'], '', 'header', array('class' => 'text-center'));
        }
    }

    private function addNivel2($nivel, $campoN0) {
        foreach ($nivel as $cod => $titulo) {
            $this->respostas[]['item'] = $cod;
            $this->tabela->addCell($titulo, '', 'header', array('class' => 'text-center'));

            if (is_array($campoN0[3]) && key_exists($cod, $campoN0[3])) {
                $this->addNivel3($campoN0[3][$cod], $campoN0);
            }
        }
    }

    private function addNivel3($nivel, $campoN0) {
        foreach ($nivel as $cod => $titulo) {
            $this->respostas[]['pergunta'] = $cod;
            $this->tabela->addCell($titulo, '', 'header', array('class' => 'text-center'));

            if (is_array($campoN0[4]) && key_exists($cod, $campoN0[4])) {
                $this->addNivel4($campoN0[4][$cod], $campoN0);
            }
        }
    }

    private function addNivel4($nivel, $campoN0) {
        foreach ($nivel as $cod => $titulo) {
            $this->respostas[]['item'] = $cod;
            $this->tabela->addCell($titulo, '', 'header', array('class' => 'text-center'));
        }
    }

    function setHeader() {
        $this->tabela->addTSection('thead');
        $this->tabela->addRow('thead');
        $this->tabela->addCell('Município', '', 'header', array('class' => 'text-center'));
        foreach ($this->perguntas as $keN0 => $campoN0) {

            foreach ($campoN0 as $keN1 => $campoN1) {
                $this->addNivel1($keN1, $campoN1);

                if (empty($campoN1['perid']) && is_array($campoN1[$perid])) {
                    $nivel2 = $campoN1[$perid];
                    $this->addNivel2($nivel2, $campoN0);
                }
                $perid = $campoN1['perid'];
            }
        }
    }

    function setBody($filtro) {
        $respostas = $this->getRespostas($filtro);

        foreach ($respostas as $res) {
            $this->tabela->addRow();
            $this->tabela->addCell($res['estuf'] . ' - ' . $res['mundescricao'], '', '', array('class' => 'text-center'));

            foreach ($this->respostas as $questoes) {
                if (is_array($questoes)) {
                    foreach ($questoes as $tipo => $cod) {
                        $respostaSelecionada = $this->getQueryRespostaItem($res, $cod, $tipo);
                        if (!empty($respostaSelecionada)) {
                            $valor =  $this->getDescricaoResposta($respostaSelecionada[0]);
                            $this->tabela->addCell($valor, '', 'body', array('class' => 'text-center'));
                        } else {
                            $this->tabela->addCell('', '', 'body', array('class' => 'text-center'));
                        }
                    }
                } else {
                    $this->tabela->addCell('', '', 'body', array('class' => 'text-center'));
                }
            }
        }
    }

    private function getDescricaoResposta($respostaSelecionada){
        if(!empty($respostaSelecionada['chk'])){
            $strCkeckbox = $respostaSelecionada['chk'];
            $val = str_replace('{', '',str_replace('}', '',str_replace('"', '',str_replace(',', '|',$strCkeckbox))));
        }
        if(empty($respostaSelecionada['queid'])){
            if(!empty($respostaSelecionada['escolas_ce'])) $val = $respostaSelecionada['escolas_ce'];
            elseif(!empty($respostaSelecionada['escolas_ce'])) $val = $respostaSelecionada['escolas_ept'];
            elseif(!empty($respostaSelecionada['escolas_ce'])) $val = $respostaSelecionada['escolas_estept'];
            elseif(!empty($respostaSelecionada['escolas_ce'])) $val = $respostaSelecionada['escolas_gremio'];
            elseif(!empty($respostaSelecionada['escolas_ce'])) $val = $respostaSelecionada['escolas_proeja'];
        }elseif(!empty($respostaSelecionada['resdsc'])) $val = $respostaSelecionada['resdsc'];
        else $val = 'X';
        return $val;
    }

    private function getTituloQuestões($p_perid, $nivel, $codPai, $codItem, $titulo) {
        if (!empty($titulo)) {
            $this->perguntas[$p_perid]['nivel' + $nivel][$codPai][$codItem] = $titulo;
        }
    }

    private function getRespostas($filtro) {
        $where = $this->getWhere($filtro);
        $sql = $this->getQuery($where, $filtro['queid']);
        $dados = $this->carregar($sql);
        $dadosResposta = ($dados ? $dados : array());
        return $dadosResposta;
    }

    private function getQueryRespostaItem($questoes, $cod, $tipo) {
        $sql = $this->getQueryResposta($questoes, $cod, $tipo);
        $dados = $this->carregar($sql);
        $dadosResposta = ($dados ? $dados : array());
        return $dadosResposta;
    }

    private function getWhere($filtro) {
        $where = '';
        $queid = (int) $filtro['queid'];
        $muncod = (int) $filtro['muncod'];
        $itrid = (int) $filtro['itrid'];

        if (!empty($queid)) {
            $where .= " AND qun.queid = {$queid} ";
        }

        if (!empty($itrid)) {
            $where .= " AND i.itrid = {$itrid} ";
        }

        if (!empty($filtro['estuf'])) {
            $where .= " AND i.estuf = '{$filtro['estuf']}' ";
        }

        if (!empty($muncod) ) {
            $where .= " AND i.muncod = '{$muncod}' ";
        }
        return $where;
    }

}

//end class
?>