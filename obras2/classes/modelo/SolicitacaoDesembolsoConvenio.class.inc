<?php

class SolicitacaoDesembolsoConvenio extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.solicitacao_desembolso_convenio";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("sdcid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'sdcid' => null,
        'processo' => null,
        'convenio' => null,
        'docid' => null,
        'sdcjustificativa' => null,
        'sdcobs' => null,
        'sdcstatus' => null,
        'usucpf' => null,
        'sdcdatainclusao' => null,
        'sdcpercsolicitado' => null,
        'sdcpercpagamento' => null,
        'sdcobstec' => null,
        'sdcparcela' => null,
    );

    /**
     * Retorna todas as informações do processo
     *
     * @param $processo
     * @return array|bool|mixed|NULL
     */
    public function pegaDadosConvenioPorProcesso($processo)
    {
        $sql = "SELECT * FROM painel.dadosconvenios WHERE dcoprocesso = '$processo'";
        return $this->pegaLinha($sql);
    }

    /**
     * Retorna todas as obras do processo
     *
     * @param $processo
     * @return array|mixed|NULL
     */
    public function pegaObrasPorProcesso($processo)
    {
        $sql = "
            SELECT
              o.*,
              ed.*,
              ((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) || '%' as percentual_execucao
            FROM obras2.vm_termo_convenio_obras vm
            JOIN obras2.obras o ON o.obrid = vm.obrid AND o.obridpai IS NULL AND o.obrstatus = 'A'
            JOIN workflow.documento d ON d.docid = o.docid
            JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
            WHERE vm.pronumeroprocesso = '$processo'
        ";
        return $this->carregar($sql);
    }

    /**
     * Retorna os dados do processo por obra
     *
     * @param $processo
     * @return array|mixed|NULL
     */
    public function pegaProcessoConvPorObra($obrid)
    {
        $sql = "
            SELECT vm.* FROM obras2.vm_termo_convenio_obras vm
            WHERE vm.obrid = $obrid
        ";
        return $this->pegaLinha($sql);
    }

    /**
     * Retorna o percentual ponderado de execução do processo
     *
     * @param $processo
     * @return bool|mixed|NULL|string
     */
    public function pegaPercentualExecucao($processo)
    {
        $sql = "
            SELECT
              SUM(((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2)) / COUNT(*)
            FROM obras2.vm_termo_convenio_obras vm
            JOIN obras2.obras o ON o.obrid = vm.obrid AND o.obridpai IS NULL AND o.obrstatus = 'A'
            WHERE vm.pronumeroprocesso = '$processo'
        ";
        return $this->pegaUm($sql);
    }

    /**
     * Retorna o percentual atualmente pago do processo
     *
     * @param $processo
     */
    public function pegaPercentualPago($processo)
    {
        $dadosProcesso = $this->pegaDadosConvenioPorProcesso($processo);
        return ($dadosProcesso['dcovalorpago'] * 100) / $dadosProcesso['dcovalorconveniado'];
    }

    /**
     * Retorna o percentual que se encontra atualmente solicitado
     *
     * @param $processo
     */
    public function pegaPercentualSolicitado($processo)
    {
        return 0;
    }

    /**
     * Retorna o percentual total possível para ser solicitado
     *
     * @param $processo
     * @return bool|mixed|NULL|string
     */
    public function pegaPercentualSolicitacao($processo)
    {
        $execucao      = $this->pegaPercentualExecucao($processo);
        $pgo           = $this->pegaPercentualPago($processo);
        $pgoSolicitado = $this->pegaPercentualSolicitado($processo);

        return $execucao - ($pgo + $pgoSolicitado);
    }

    /**
     * Retorna true/false se a obra tem ou não pendencia no checklist
     * @param $obrid
     * @return array
     */
    public function verificaChecklistObra($obrid)
    {
        return false;
    }

    /**
     * Retorna todo o questionario da obra
     *
     * @param $obrid
     * @return array
     */
    public function pegaQuestionarioObra($obrid)
    {
        $obra = new Obras($obrid);
        $doc  = wf_pegarDocumento($obra->docid);
        $ops  = array('1.1', '1.2', '1.3', '1.4', '1.5', '1.6', '2.1'); // Questões opcionais para as obras concluídas

        $sql = "
            SELECT
              eq.etqdsc,
              itc.itcdsc,
              q.qstid,
              q.itcid,
              q.qstnumero,
              q.qstdsc,
              q.qstordem,
              q.qstctrlobs,
              q.qstobs,
              v.vdrid
            FROM
              obras2.etapaquestao eq
              JOIN obras2.questao q ON q.etqid = eq.etqid AND q.qststatus = 'A'
              JOIN obras2.itemcomposicaoquestao itc ON itc.itcid = q.itcid
              LEFT JOIN obras2.validador_questao vq ON vq.qstid = q.qstid AND vq.vdqstatus = 'A'
              LEFT JOIN obras2.validador v ON v.vdrid = vq.vdrid AND v.vdrstatus = 'A'
            WHERE eq.etqid = " . ETQID_DESEMBOLSO . "
            ORDER BY q.qstordem;
            ";

        $questionario = $this->carregar($sql);

        foreach ($questionario as $key => $questao) {

            if(!in_array($doc['esdid'], array(690, 693, 691)))
                continue;

            $validador = new Validador();
            $check     = $validador->check($obra, $questao['vdrid']);

            if (empty($questionario[$key]['qsvresposta']))
                $questionario[$key]['qsvresposta'] = ($check) ? 't' : 'f';

            if (!$check && $validador->vdrobrigatorio == 't') {
                if ($doc['esdid'] == 693 && in_array($questao['qstnumero'], $ops)) {
                    continue;
                }

                $habilitado                      = false;
                $habilita                        = 'N';
                $questionario[$key]['pendencia'] = $validador->getMessage();
                $questionario[$key]['vdrobrigatorio'] = $validador->vdrobrigatorio;
            }
        }

        return $questionario;
    }

}