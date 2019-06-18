<?php
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of CumprimentoObjetoQuestionario
 *
 * @author lindalbertofilho
 */
class CumprimentoObjetoQuestionario extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.cumprimento_objeto_questionario";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("coqid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'coqid' => null,
        'coid' => null,
        'qrpid' => null,
        'coqstatus' => null,
        'usucpf' => null,
        'coqdatainclusao' => null
    );

    public $usunome;

    public function verificaExistencia($coid, $cria = false)
    {
        $sql = <<<DML
            SELECT coq.coqid, coq.coid, coq.qrpid, usu.usucpf, usu.usunome, to_char(coq.coqdatainclusao,'DD/MM/YYYY') AS coqdatainclusao
            FROM {$this->stNomeTabela} coq
            INNER JOIN seguranca.usuario usu ON coq.usucpf = usu.usucpf
            WHERE coid = $coid;
DML;
        $dados = $this->pegaLinha($sql);
        if(!$dados && $cria) {
            $arParam = array("queid" => QUEID_QUEST_CHKLST_CUMPRIMENTO, "titulo" => "Checklist do Cumprimento (" . $coid . " - Checklist do Cumprimento)");
            $qrpid = GerenciaQuestionario::insereQuestionario($arParam);
            $this->popularDadosObjeto(array('coid' => $coid, 'qrpid' => $qrpid, 'usucpf' => "'".$_SESSION['usucpf']."'"));
            $this->salvar();
            $this->commit();
            $this->verificaExistencia($coid);
            return;
        }
        $this->usunome = $dados['usunome'];
        $this->popularDadosObjeto($dados);
    }

    public function verificaRespostaItem5()
    {
        $queid = QUEID_QUEST_CHKLST_CUMPRIMENTO;
        $sql = <<<DML
            SELECT itpid FROM questionario.resposta WHERE qrpid = {$this->qrpid}
                AND perid = 4207 --(SELECT perid FROM questionario.pergunta WHERE queid = $queid AND perordem = 5 LIMIT 1)
DML;
        return $this->pegaUm($sql);
    }

    public function verificaPendencias()
    {
        if($this->qrpid) {
            $queid = QUEID_QUEST_CHKLST_CUMPRIMENTO;
            $sql = <<<DML
            SELECT
                (select CASE WHEN count((select perid from questionario.resposta where perid = p.perid and qrpid = {$this->qrpid} LIMIT 1)) = count(1) then 'S' else 'N' end from questionario.pergunta p where p.queid = $queid LIMIT 1) as respostas_zeradas,
                CASE (SELECT itpid FROM questionario.resposta WHERE qrpid = {$this->qrpid} AND perid = (SELECT perid FROM questionario.pergunta WHERE queid = $queid AND perordem = 5 LIMIT 1) LIMIT 1) WHEN 5577 THEN 'S' ELSE 'N' END AS resposta_fotos
DML;

            $pendencias = $this->pegaLinha($sql);

            if($pendencias) {
                $funcionamento = $this->pegaUm("SELECT
                                            itptitulo
                                        FROM questionario.resposta r
                                        LEFT JOIN questionario.itempergunta i ON i.itpid = r.itpid
                                        WHERE
                                            r.perid = 4163 -- 4.1 ) A edificação está em funcionamento?
                                            AND r.qrpid = {$this->qrpid}
                                           ");
                if ($funcionamento == 'Sim') {
//                    $COD_INEP                  = $this->pegaUm("SELECT
//                                                                        co_inep
//                                                                    FROM obras2.cumprimento_objeto_questionario  coq
//                                                                    JOIN obras2.cumprimento_objeto co ON co.coid = coq.coid AND co.status = 'A'
//                                                                    JOIN obras2.obras o ON o.obrid = co.obrid
//                                                                    WHERE coqstatus = 'A' AND qrpid = {$this->qrpid}");
//                    $pendencias['codigo_inep'] = (empty($COD_INEP)) ? false : true;
                    $pendencias['codigo_inep'] = true;
                } else {
                    $pendencias['codigo_inep'] = true;
                }
            }
            return $pendencias;
        }
    }
}